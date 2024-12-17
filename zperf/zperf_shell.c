/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lwip/sockets.h"
#include <string.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(net_zperf, CONFIG_NET_ZPERF_LOG_LEVEL);

/* #include <ctype.h> */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <zephyr/kernel.h>
/* #include <zephyr/shell/shell.h> */

#include <zephyr/net/net_core.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zperf.h>

#include "zperf_internal.h"

#include "zperf_session.h"
/* #include "fsl_debug_console.h" */

#include "defines.h"

/**
 * @brief Define shell handler for shell operations
 * **/
static SHELL_HANDLE_DEFINE(_shell_handle_buff);

extern serial_handle_t g_serialHandle;

/* Get some useful debug routings from net_private.h, requires
 * that NET_LOG_ENABLED is set.
 */
#define NET_LOG_ENABLED 1
#include "net/net_private.h"

#include <zephyr/shell/shell.h>
/* typedef void * 	shell_handle_t; */
/**/
/* typedef enum  {  */
/*   kStatus_SHELL_Success = 0,  */
/*   kStatus_SHELL_Error = 1, */
/*   kStatus_SHELL_OpenWriteHandleFailed = 2, */
/*   kStatus_SHELL_OpenReadHandleFailed = 3 */
/* } shell_status_t; */

// #include "ipv6.h" /* to get infinite lifetime */
#include "lwip/ip6.h" /* to get infinite lifetime */

/**
 * @brief When CONFIG_NET_CONTEXT_PRIORITY is disabled in config.h then un-defined it
 *        for this transilation unit.
 * **/

static const char *CONFIG = "unified"
#if defined(CONFIG_WIFI)
                            " wifi"
#endif
#if defined(CONFIG_NET_L2_ETHERNET)
                            " ethernet"
#endif
#if defined(CONFIG_NET_IPV4)
                            " ipv4"
#endif
#if defined(CONFIG_NET_IPV6)
                            " ipv6"
#endif
                            "";

static struct sockaddr_in6 in6_addr_my = {
    .sin6_family = AF_INET6,
    .sin6_port = PP_HTONS(MY_SRC_PORT),
};

static struct sockaddr_in6 in6_addr_dst = {
    .sin6_family = AF_INET6,
    .sin6_port = PP_HTONS(DEF_PORT),
};

static struct sockaddr_in in4_addr_dst = {
    .sin_family = AF_INET,
    .sin_port = PP_HTONS(DEF_PORT),
};

static struct sockaddr_in in4_addr_my = {
    .sin_family = AF_INET,
    .sin_port = PP_HTONS(MY_SRC_PORT),
};

static struct in6_addr shell_ipv6;

static struct in_addr shell_ipv4;

#define DEVICE_NAME "zperf shell"

const uint32_t TIME_US[] = {60 * 1000 * 1000, 1000 * 1000, 1000, 0};
const char *TIME_US_UNIT[] = {"m", "s", "ms", "us"};
const uint32_t KBPS[] = {1024, 0};
const char *KBPS_UNIT[] = {"Mbps", "Kbps"};
const uint32_t K[] = {1024 * 1024, 1024, 0};
const char *K_UNIT[] = {"M", "K", ""};

static void print_number(const shell_handle_t sh, uint32_t value, const uint32_t *divisor_arr, const char **units)
{
    const char **unit;
    const uint32_t *divisor;
    uint32_t dec, radix;

    unit = units;
    divisor = divisor_arr;

    while (value < *divisor)
    {
        divisor++;
        unit++;
    }

    if (*divisor != 0U)
    {
        radix = value / *divisor;
        dec = (value % *divisor) * 100U / *divisor;
        printf("%u.%s%u %s", radix, (dec < 10) ? "0" : "", dec, *unit);
    }
    else
    {
        printf("%u %s", value, *unit);
    }
}

static long parse_number(const char *string, const uint32_t *divisor_arr, const char **units)
{
    const char **unit;
    const uint32_t *divisor;
    char *suffix;
    long dec;
    int cmp;

    dec = strtoul(string, &suffix, 10);
    unit = units;
    divisor = divisor_arr;

    do
    {
        cmp = strncasecmp(suffix, *unit++, 1);
    } while (cmp != 0 && *++divisor != 0U);

    return (*divisor == 0U) ? dec : dec * *divisor;
}

static shell_status_t parse_ipv6_addr(const shell_handle_t sh, char *host, char *port, struct sockaddr_in6 *addr)
{
    int ret;
    if (!host)
    {
        return -kStatus_SHELL_Error;
    }

    ret = net_addr_pton(AF_INET6, host, &addr->sin6_addr);
    if (ret < 0)
    {
        printf("Invalid IPv6 address %s\n", host);
        return -kStatus_SHELL_Error;
    }

    addr->sin6_port = htons(strtoul(port, NULL, 10));
    if (!addr->sin6_port)
    {
        printf("Invalid port %s\n", port);
        return -kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t parse_ipv4_addr(const shell_handle_t sh, char *host, char *port, struct sockaddr_in *addr)
{
    int ret;

    if (!host)
    {
        return -kStatus_SHELL_Error;
    }

    ret = net_addr_pton(AF_INET, host, &addr->sin_addr);
    if (ret < 0)
    {
        printf("Invalid IPv4 address %s\n", host);
        return -kStatus_SHELL_Error;
    }

    addr->sin_port = htons(strtoul(port, NULL, 10));
    if (!addr->sin_port)
    {
        printf("Invalid port %s\n", port);
        return -kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t zperf_bind_host(const shell_handle_t sh, size_t argc, char *argv[],
                                      struct zperf_download_params *param)
{
    int ret;

    /* Parse options */
    if (argc >= 2)
    {
        param->port = strtoul(argv[1], NULL, 10);
    }
    else
    {
        param->port = DEF_PORT;
    }

    if (argc >= 3)
    {
        char *addr_str = argv[2];
        struct sockaddr addr;

        memset(&addr, 0, sizeof(addr));

        ret = net_ipaddr_parse(addr_str, strlen(addr_str), &addr);
        if (ret < 0)
        {
            printf("Cannot parse address \"%s\"\n", addr_str);
            return ret;
        }

        memcpy(&param->addr, &addr, sizeof(struct sockaddr));
    }

    return kStatus_SHELL_Success;
}

static shell_status_t cmd_setip(const shell_handle_t sh, size_t argc, char *argv[])
{
    int start = 0;

    if (IS_ENABLED(CONFIG_NET_IPV6) && !IS_ENABLED(CONFIG_NET_IPV4))
    {
        if (argc != 3)
        {
            /* shell_help(sh); */
            printf("help");
            return -kStatus_SHELL_Error;
        }

        if (zperf_get_ipv6_addr(argv[start + 1], argv[start + 2], &shell_ipv6) < 0)
        {
            printf("Unable to set %s address (%s disabled)\n", "IPv6", "IPv4");
            return kStatus_SHELL_Success;
        }

        printf("Setting IP address %s\n", net_sprint_ipv6_addr(&shell_ipv6));
    }

    if (IS_ENABLED(CONFIG_NET_IPV4) && !IS_ENABLED(CONFIG_NET_IPV6))
    {
        if (argc != 2)
        {
            /* shell_help(sh); */
            printf("help");
            return -kStatus_SHELL_Error;
        }

        if (zperf_get_ipv4_addr(argv[start + 1], &shell_ipv4) < 0)
        {
            printf("Unable to set %s address (%s disabled)\n", "IPv4", "IPv6");
            return -kStatus_SHELL_Error;
        }

        printf("Setting IP address %s\n", net_sprint_ipv4_addr(&shell_ipv4));
    }

    if (IS_ENABLED(CONFIG_NET_IPV6) && IS_ENABLED(CONFIG_NET_IPV4))
    {
        if (net_addr_pton(AF_INET6, argv[start + 1], &shell_ipv6) < 0)
        {
            if (argc != 2)
            {
                /* shell_help(sh); */
                printf("help");
                return -kStatus_SHELL_Error;
            }

            if (zperf_get_ipv4_addr(argv[start + 1], &shell_ipv4) < 0)
            {
                printf("Unable to set %s address\n", "IPv4");
                return -kStatus_SHELL_Error;
            }

            printf("Setting IP address %s\n", net_sprint_ipv4_addr(&shell_ipv4));
        }
        else
        {
            if (argc != 3)
            {
                /* shell_help(sh); */
                printf("help");
                return -kStatus_SHELL_Error;
            }

            if (zperf_get_ipv6_addr(argv[start + 1], argv[start + 2], &shell_ipv6) < 0)
            {
                printf("Unable to set %s address\n", "IPv6");
                return -kStatus_SHELL_Error;
            }

            printf("Setting IP address %s\n", net_sprint_ipv6_addr(&shell_ipv6));
        }
    }

    return kStatus_SHELL_Success;
}

static void udp_session_cb(enum zperf_status status, struct zperf_results *result, void *user_data)
{
    const shell_handle_t sh = user_data;

    switch (status)
    {
    case ZPERF_SESSION_STARTED:
        printf("New session started.\n");
        break;

    case ZPERF_SESSION_FINISHED: {
        uint32_t rate_in_kbps;

        /* Compute baud rate */
        if (result->time_in_us != 0U)
        {
            rate_in_kbps = (uint32_t)(((uint64_t)result->total_len * 8ULL * (uint64_t)USEC_PER_SEC) /
                                      ((uint64_t)result->time_in_us * 1024ULL));
        }
        else
        {
            rate_in_kbps = 0U;
        }

        printf("End of session!\n");

        printf(" duration:\t\t");
        print_number(sh, result->time_in_us, TIME_US, TIME_US_UNIT);
        printf("\n");

        printf(" received packets:\t%u\n", result->nb_packets_rcvd);
        printf(" nb packets lost:\t%u\n", result->nb_packets_lost);
        printf(" nb packets outorder:\t%u\n", result->nb_packets_outorder);

        printf(" jitter:\t\t\t");
        print_number(sh, result->jitter_in_us, TIME_US, TIME_US_UNIT);
        printf("\n");

        printf(" rate:\t\t\t");
        print_number(sh, rate_in_kbps, KBPS, KBPS_UNIT);
        printf("\n");

        break;
    }

    case ZPERF_SESSION_ERROR:
        printf("UDP session error.\n");
        break;
    }
}

static shell_status_t cmd_udp_download_stop(const shell_handle_t sh, size_t argc, char *argv[])
{
    int ret;

    ret = zperf_udp_download_stop();
    if (ret < 0)
    {
        printf("UDP server not running!\n");
        return -kStatus_SHELL_Error;
    }

    printf("UDP server stopped\n");

    return kStatus_SHELL_Success;
}

static shell_status_t cmd_udp_download(const shell_handle_t sh, size_t argc, char *argv[])
{
    if (IS_ENABLED(CONFIG_NET_UDP))
    {
        struct zperf_download_params param = {0};
        int ret;

        ret = zperf_bind_host(sh, argc, argv, &param);
        if (ret < 0)
        {
            printf("Unable to bind host.\n");
            /* shell_help(sh); */
            printf("help");
            return -kStatus_SHELL_Error;
        }

        ret = zperf_udp_download(&param, udp_session_cb, (void *)sh);
        if (ret == -EALREADY)
        {
            printf("UDP server already started!\n");
            return -kStatus_SHELL_Error;
        }
        else if (ret < 0)
        {
            printf("Failed to start UDP server!\n");
            return -kStatus_SHELL_Error;
        }

        k_yield();

        printf("UDP server started on port %u\n", param.port);

        return kStatus_SHELL_Success;
    }
    else
    {
        return -kStatus_SHELL_Error;
    }
}

static void shell_udp_upload_print_stats(const shell_handle_t sh, struct zperf_results *results)
{
    if (IS_ENABLED(CONFIG_NET_UDP))
    {
        unsigned int rate_in_kbps, client_rate_in_kbps;

        printf("-\nUpload completed!\n");

        if (results->time_in_us != 0U)
        {
            rate_in_kbps = (uint32_t)(((uint64_t)results->total_len * (uint64_t)8 * (uint64_t)USEC_PER_SEC) /
                                      ((uint64_t)results->time_in_us * 1024U));
        }
        else
        {
            rate_in_kbps = 0U;
        }

        if (results->client_time_in_us != 0U)
        {
            client_rate_in_kbps = (uint32_t)(((uint64_t)results->nb_packets_sent * (uint64_t)results->packet_size *
                                              (uint64_t)8 * (uint64_t)USEC_PER_SEC) /
                                             ((uint64_t)results->client_time_in_us * 1024U));
        }
        else
        {
            client_rate_in_kbps = 0U;
        }

        if (!rate_in_kbps)
        {
            printf("LAST PACKET NOT RECEIVED!!!\n");
        }

        printf("Statistics:\t\tserver\t(client)\n");
        printf("Duration:\t\t");
        print_number(sh, results->time_in_us, TIME_US, TIME_US_UNIT);
        printf("\t(");
        print_number(sh, results->client_time_in_us, TIME_US, TIME_US_UNIT);
        printf(")\n");

        printf("Num packets:\t\t%u\t(%u)\n", results->nb_packets_rcvd, results->nb_packets_sent);

        printf("Num packets out order:\t%u\n", results->nb_packets_outorder);
        printf("Num packets lost:\t%u\n", results->nb_packets_lost);

        printf("Jitter:\t\t\t");
        print_number(sh, results->jitter_in_us, TIME_US, TIME_US_UNIT);
        printf("\n");

        printf("Rate:\t\t\t");
        print_number(sh, rate_in_kbps, KBPS, KBPS_UNIT);
        printf("\t(");
        print_number(sh, client_rate_in_kbps, KBPS, KBPS_UNIT);
        printf(")\n");
    }
}

static void shell_tcp_upload_print_stats(const shell_handle_t sh, struct zperf_results *results)
{
    if (IS_ENABLED(CONFIG_NET_TCP))
    {
        unsigned int client_rate_in_kbps;

        printf("-\nUpload completed!\n");

        if (results->client_time_in_us != 0U)
        {
            client_rate_in_kbps = (uint32_t)(((uint64_t)results->nb_packets_sent * (uint64_t)results->packet_size *
                                              (uint64_t)8 * (uint64_t)USEC_PER_SEC) /
                                             ((uint64_t)results->client_time_in_us * 1024U));
        }
        else
        {
            client_rate_in_kbps = 0U;
        }

        printf("Duration:\t");
        print_number(sh, results->client_time_in_us, TIME_US, TIME_US_UNIT);
        printf("\n");
        printf("Num packets:\t%u\n", results->nb_packets_sent);
        printf("Num errors:\t%u (retry or fail)\n", results->nb_packets_errors);
        printf("Rate:\t\t");
        print_number(sh, client_rate_in_kbps, KBPS, KBPS_UNIT);
        printf("\n");
    }
}

static void udp_upload_cb(enum zperf_status status, struct zperf_results *result, void *user_data)
{
    const shell_handle_t sh = user_data;

    switch (status)
    {
    case ZPERF_SESSION_STARTED:
        break;

    case ZPERF_SESSION_FINISHED: {
        shell_udp_upload_print_stats(sh, result);
        break;
    }

    case ZPERF_SESSION_ERROR:
        printf("UDP upload failed\n");
        break;
    }
}

static void tcp_upload_cb(enum zperf_status status, struct zperf_results *result, void *user_data)
{
    const shell_handle_t sh = user_data;

    switch (status)
    {
    case ZPERF_SESSION_STARTED:
        break;

    case ZPERF_SESSION_FINISHED: {
        shell_tcp_upload_print_stats(sh, result);
        break;
    }

    case ZPERF_SESSION_ERROR:
        printf("TCP upload failed\n");
        break;
    }
}

 /* static int ping_handler(struct net_icmp_ctx *ctx, */
 /* 			struct net_pkt *pkt, */
 /* 			struct net_icmp_ip_hdr *ip_hdr, */
 /* 			struct net_icmp_hdr *icmp_hdr, */
 /* 			void *user_data) */
 /* { */
 /* 	struct k_sem *sem_wait = user_data; */
 /**/
 /* 	ARG_UNUSED(ctx); */
 /* 	ARG_UNUSED(pkt); */
 /* 	ARG_UNUSED(ip_hdr); */
 /* 	ARG_UNUSED(icmp_hdr); */
 /**/
 /* 	k_sem_give(sem_wait); */
 /**/
 /* 	return kStatus_SHELL_Success; */
 /* } */
 /**/
 /* static void send_ping(const shell_handle_t sh, */
 /* 		      struct in6_addr *addr, */
 /* 		      int timeout_ms) */
 /* { */
 /* 	static struct k_sem sem_wait; */
 /* 	struct sockaddr_in6 dest_addr = { 0 }; */
 /* 	struct net_icmp_ctx ctx; */
 /* 	int ret; */
 /**/
 /* 	ret = net_icmp_init_ctx(&ctx, NET_ICMPV6_ECHO_REPLY, 0, ping_handler); */
 /* 	if (ret < 0) { */
 /* 		shell_fprintf(sh, SHELL_WARNING, "Cannot send ping (%d)\n", ret); */
 /* 		return; */
 /* 	} */
 /**/
 /* 	memcpy(&dest_addr.sin6_addr, addr, sizeof(struct in6_addr)); */
 /**/
 /* 	k_sem_init(&sem_wait, 0, 1); */
 /**/
 /* 	(void)net_icmp_send_echo_request(&ctx, */
 /* 					 net_if_get_default(), */
 /* 					 (struct sockaddr *)&dest_addr, */
 /* 					 NULL, &sem_wait); */
 /**/
 /* 	ret = k_sem_take(&sem_wait, K_MSEC(timeout_ms)); */
 /* 	if (ret == -EAGAIN) { */
 /* 		shell_fprintf(sh, SHELL_WARNING, "ping %s timeout\n", */
 /* 			      net_sprint_ipv6_addr(addr)); */
 /* 	} */
 /**/
 /* 	(void)net_icmp_cleanup_ctx(&ctx); */
 /* } */

static shell_status_t execute_upload(const shell_handle_t sh, const struct zperf_upload_params *param, bool is_udp,
                                     bool async)
{

    struct zperf_results results = {0};
    int ret;
    printf("Duration:\t");
    print_number(sh, param->duration_ms * USEC_PER_MSEC, TIME_US, TIME_US_UNIT);
    printf("\n");
    printf("Packet size:\t%u bytes\n", param->packet_size);
    printf("Rate:\t\t%u kbps\n", param->rate_kbps);
    printf("Starting...\n");

    if (IS_ENABLED(CONFIG_NET_IPV6) && param->peer_addr.ss_family == AF_INET6)
    {
        /* struct sockaddr_in6 *ipv6 = */
         		/* (struct sockaddr_in6 *)&param->peer_addr; */
        /* For IPv6, we should make sure that neighbor discovery
         * has been done for the peer. So send ping here, wait
         * some time and start the test after that.
         */
         /* send_ping(sh, &ipv6->sin6_addr, MSEC_PER_SEC); */
    }

    if (is_udp && IS_ENABLED(CONFIG_NET_UDP))
    {
        uint32_t packet_duration = zperf_packet_duration(param->packet_size, param->rate_kbps);

        printf("Rate:\t\t");
        print_number(sh, param->rate_kbps, KBPS, KBPS_UNIT);
        printf("\n");

        if (packet_duration > 1000U)
        {
            printf("Packet duration %u ms\n", (unsigned int)(packet_duration / 1000U));
        }
        else
        {
            printf("Packet duration %u us\n", (unsigned int)packet_duration);
        }

        if (async)
        {
            ret = zperf_udp_upload_async(param, udp_upload_cb, (void *)sh);
            if (ret < 0)
            {
                printf("Failed to start UDP async upload (%d)\n", ret);
                return ret;
            }
        }
        else
        {
            ret = zperf_udp_upload(param, &results);
            if (ret < 0)
            {
                printf("UDP upload failed (%d)\n", ret);
                return ret;
            }

            shell_udp_upload_print_stats(sh, &results);
        }
    }
    else
    {
        if (!IS_ENABLED(CONFIG_NET_UDP))
        {
            printf("UDP not supported\n");
        }
    }

    if (!is_udp && IS_ENABLED(CONFIG_NET_TCP))
    {
        if (async)
        {
            ret = zperf_tcp_upload_async(param, tcp_upload_cb, (void *)sh);
            if (ret < 0)
            {
                printf("Failed to start TCP async upload (%d)\n", ret);
                return ret;
            }
        }
        else
        {
            ret = zperf_tcp_upload(param, &results);
            if (ret < 0)
            {
                printf("TCP upload failed (%d)\n", ret);
                return ret;
            }

            shell_tcp_upload_print_stats(sh, &results);
        }
    }
    else
    {
        if (!IS_ENABLED(CONFIG_NET_TCP))
        {
            printf("TCP not supported\n");
        }
    }

    return kStatus_SHELL_Success;
}

static shell_status_t parse_arg(size_t *i, size_t argc, char *argv[])
{
    int res = -1;
    const char *str = argv[*i] + 2;
    char *endptr;

    if (*str == 0)
    {
        if (*i + 1 >= argc)
        {
            return -1;
        }

        *i += 1;
        str = argv[*i];
    }

    errno = 0;
    if (strncmp(str, "0x", 2) == 0)
    {
        res = strtol(str, &endptr, 16);
    }
    else
    {
        res = strtol(str, &endptr, 10);
    }

    if (errno || (endptr == str))
    {
        return -kStatus_SHELL_Error;
    }

    return res;
}

static shell_status_t shell_cmd_upload(const shell_handle_t sh, size_t argc, char *argv[], enum net_ip_protocol proto)
{
    struct zperf_upload_params param = {0};
    struct sockaddr_in6 ipv6 = {.sin6_family = AF_INET6};
    struct sockaddr_in ipv4 = {.sin_family = AF_INET};
    char *port_str;
    bool async = false;
    bool is_udp;
    int start = 0;
    size_t opt_cnt = 0;

    param.options.priority = -1;
    is_udp = proto == nip_IPPROTO_UDP;

    /* Parse options */
    for (size_t i = 1; i < argc; ++i)
    {
        if (*argv[i] != '-')
        {
            break;
        }

        switch (argv[i][1])
        {
        case 'S': {
            int tos = parse_arg(&i, argc, argv);

            if (tos < 0 || tos > UINT8_MAX)
            {
                printf("Parse error: %s\n", argv[i]);
                return -kStatus_SHELL_Error;
            }

            param.options.tos = tos;
            opt_cnt += 2;
            break;
        }

        case 'a':
            async = true;
            opt_cnt += 1;
            break;

        case 'n':
            if (is_udp)
            {
                printf("UDP does not support -n option\n");
                return -kStatus_SHELL_Error;
            }
            param.options.tcp_nodelay = 1;
            opt_cnt += 1;
            break;

#ifdef CONFIG_NET_CONTEXT_PRIORITY
        case 'p':
            param.options.priority = parse_arg(&i, argc, argv);
            if (param.options.priority < 0 || param.options.priority > UINT8_MAX)
            {
                printf("Parse error: %s\n", argv[i]);
                return -kStatus_SHELL_Error;
            }
            opt_cnt += 2;
            break;
#endif /* CONFIG_NET_CONTEXT_PRIORITY */

        default:
            printf("Unrecognized argument: %s\n", argv[i]);
            return -kStatus_SHELL_Error;
        }
    }

    start += opt_cnt;
    argc -= opt_cnt;

    if (argc < 2)
    {
        printf("Not enough parameters.\n");

        if (is_udp)
        {
            if (IS_ENABLED(CONFIG_NET_UDP))
            {
                /* shell_help(sh); */
                printf("help");
                return -kStatus_SHELL_Error;
            }
        }
        else
        {
            if (IS_ENABLED(CONFIG_NET_TCP))
            {
                /* shell_help(sh); */
                printf("help");
                return -kStatus_SHELL_Error;
            }
        }

        return -kStatus_SHELL_Error;
    }

    if (argc > 2)
    {
        port_str = argv[start + 2];
        printf("Remote port is %s\n", port_str);
    }
    else
    {
        port_str = DEF_PORT_STR;
    }

    if (IS_ENABLED(CONFIG_NET_IPV6) && !IS_ENABLED(CONFIG_NET_IPV4))
    {
        if (parse_ipv6_addr(sh, argv[start + 1], port_str, &ipv6) < 0)
        {
            printf("Please specify the IP address of the "
                   "remote server.\n");
            return -kStatus_SHELL_Error;
        }

        printf("Connecting to %s\n", net_sprint_ipv6_addr(&ipv6.sin6_addr));
        copy_sockaddr_in6_to_sockaddr_storage(&ipv6, &param.peer_addr);
    }

    if (IS_ENABLED(CONFIG_NET_IPV4) && !IS_ENABLED(CONFIG_NET_IPV6))
    {
        if (parse_ipv4_addr(sh, argv[start + 1], port_str, &ipv4) < 0)
        {
            printf("Please specify the IP address of the "
                   "remote server.\n");
            return -kStatus_SHELL_Error;
        }

        printf("Connecting to %s\n", net_sprint_ipv4_addr(&ipv4.sin_addr));
        copy_sockaddr_in_to_sockaddr_storage(&ipv4, &param.peer_addr);
    }

    if (IS_ENABLED(CONFIG_NET_IPV6) && IS_ENABLED(CONFIG_NET_IPV4))
    {
        if (parse_ipv6_addr(sh, argv[start + 1], port_str, &ipv6) != kStatus_SHELL_Success)
        {
            if (parse_ipv4_addr(sh, argv[start + 1], port_str, &ipv4) < 0)
            {
                printf("Please specify the IP address "
                       "of the remote server.\n");
                return -kStatus_SHELL_Error;
            }

            printf("Connecting to ipv4 %s\n", net_sprint_ipv4_addr(&ipv4.sin_addr));
            copy_sockaddr_in_to_sockaddr_storage(&ipv4, &param.peer_addr);
        }
        else
        {
            printf("Connecting to ipv6 %s\n", net_sprint_ipv6_addr(&ipv6.sin6_addr));
            copy_sockaddr_in6_to_sockaddr_storage(&ipv6, &param.peer_addr);
        }
    }

    if (argc > 3)
    {
        param.duration_ms = MSEC_PER_SEC * strtoul(argv[start + 3], NULL, 10);
    }
    else
    {
        param.duration_ms = MSEC_PER_SEC * 1;
    }

    if (argc > 4)
    {
        param.packet_size = parse_number(argv[start + 4], K, K_UNIT);
    }
    else
    {
        param.packet_size = 256U;
    }

    if (argc > 5)
    {
        param.rate_kbps = (parse_number(argv[start + 5], K, K_UNIT) + 1023) / 1024;
    }
    else
    {
        param.rate_kbps = 10U;
    }

    return execute_upload(sh, &param, is_udp, async);
}

static shell_status_t cmd_tcp_upload(const shell_handle_t sh, size_t argc, char *argv[])
{
    return shell_cmd_upload(sh, argc, argv, nip_IPPROTO_TCP);
}

static shell_status_t cmd_udp_upload(const shell_handle_t sh, size_t argc, char *argv[])
{
    return shell_cmd_upload(sh, argc, argv, nip_IPPROTO_UDP);
}

static shell_status_t shell_cmd_upload2(const shell_handle_t sh, size_t argc, char *argv[], enum net_ip_protocol proto)
{
    struct zperf_upload_params param = {0};
    sa_family_t family;
    uint8_t is_udp;
    bool async = false;
    int start = 0;
    size_t opt_cnt = 0;

    is_udp = proto == nip_IPPROTO_UDP;

    /* Parse options */
    for (size_t i = 1; i < argc; ++i)
    {
        if (*argv[i] != '-')
        {
            break;
        }

        switch (argv[i][1])
        {
        case 'S': {
            int tos = parse_arg(&i, argc, argv);

            if (tos < 0 || tos > UINT8_MAX)
            {
                printf("Parse error: %s\n", argv[i]);
                return -kStatus_SHELL_Error;
            }

            param.options.tos = tos;
            opt_cnt += 2;
            break;
        }

        case 'a':
            async = true;
            opt_cnt += 1;
            break;

        case 'n':
            if (is_udp)
            {
                printf("UDP does not support -n option\n");
                return -kStatus_SHELL_Error;
            }
            param.options.tcp_nodelay = 1;
            opt_cnt += 1;
            break;

#ifdef CONFIG_NET_CONTEXT_PRIORITY
        case 'p':
            param.options.priority = parse_arg(&i, argc, argv);
            if (param.options.priority == -1 || param.options.priority > UINT8_MAX)
            {
                printf("Parse error: %s\n", argv[i]);
                return -kStatus_SHELL_Error;
            }
            opt_cnt += 2;
            break;
#endif /* CONFIG_NET_CONTEXT_PRIORITY */

        default:
            printf("Unrecognized argument: %s\n", argv[i]);
            return -kStatus_SHELL_Error;
        }
    }

    start += opt_cnt;
    argc -= opt_cnt;
    printf("argc is %zu\n", argc);

    if (argc < 2)
    {
        printf("Not enough parameters.\n");

        if (is_udp)
        {
            if (IS_ENABLED(CONFIG_NET_UDP))
            {
                /* shell_help(sh); */
                printf("help");
                return -kStatus_SHELL_Error;
            }
        }
        else
        {
            if (IS_ENABLED(CONFIG_NET_TCP))
            {
                /* shell_help(sh); */
                printf("help");
                return -kStatus_SHELL_Error;
            }
        }

        return -kStatus_SHELL_Error;
    }

    family = !strcmp(argv[start + 1], "v4") ? AF_INET : AF_INET6;

    if (family == AF_INET6)
    {
        if (net_ipv6_is_addr_unspecified(&in6_addr_dst.sin6_addr))
        {
            printf("Invalid destination IPv6 address.\n");
            return -kStatus_SHELL_Error;
        }

        printf("Connecting to %s\n", net_sprint_ipv6_addr(&in6_addr_dst.sin6_addr));
        copy_sockaddr_in6_to_sockaddr_storage(&in6_addr_dst, &param.peer_addr);
    }
    else
    {
        if (net_ipv4_is_addr_unspecified(&in4_addr_dst.sin_addr))
        {
            printf("Invalid destination IPv4 address.\n");
            return -kStatus_SHELL_Error;
        }

        printf("Connecting to %s\n", net_sprint_ipv4_addr(&in4_addr_dst.sin_addr));
        copy_sockaddr_in_to_sockaddr_storage(&in4_addr_dst, &param.peer_addr);
    }

    if (argc > 2)
    {
        param.duration_ms = MSEC_PER_SEC * strtoul(argv[start + 2], NULL, 10);
    }
    else
    {
        param.duration_ms = MSEC_PER_SEC * 1;
    }

    if (argc > 3)
    {
        param.packet_size = parse_number(argv[start + 3], K, K_UNIT);
    }
    else
    {
        param.packet_size = 256U;
    }
    printf("packet size %hu\n", param.packet_size);

    if (argc > 4)
    {
        param.rate_kbps = (parse_number(argv[start + 4], K, K_UNIT) + 1023) / 1024;
    }
    else
    {
        param.rate_kbps = 10U;
    }

    return execute_upload(sh, &param, is_udp, async);
}

static shell_status_t cmd_tcp_upload2(const shell_handle_t sh, size_t argc, char *argv[])
{
    return shell_cmd_upload2(sh, argc, argv, nip_IPPROTO_TCP);
}

static shell_status_t cmd_udp_upload2(const shell_handle_t sh, size_t argc, char *argv[])
{
    return shell_cmd_upload2(sh, argc, argv, nip_IPPROTO_UDP);
}

static shell_status_t cmd_tcp(const shell_handle_t sh, size_t argc, char *argv[])
{
    if (IS_ENABLED(CONFIG_NET_TCP))
    {
        /* shell_help(sh); */
        printf("help");
        return -kStatus_SHELL_Error;
    }

    printf("TCP support is not enabled. "
           "Set CONFIG_NET_TCP=y in your config file.\n");

    return -kStatus_SHELL_Error;
}

static shell_status_t cmd_udp(const shell_handle_t sh, size_t argc, char *argv[])
{
    if (IS_ENABLED(CONFIG_NET_UDP))
    {
        /* shell_help(sh); */
        printf("help");
        return -kStatus_SHELL_Error;
    }

    printf("UDP support is not enabled. "
           "Set CONFIG_NET_UDP=y in your config file.\n");

    return -kStatus_SHELL_Error;
}

static shell_status_t cmd_connectap(const shell_handle_t sh, size_t argc, char *argv[])
{
    printf("Zephyr has not been built with Wi-Fi support.\n");

    return kStatus_SHELL_Success;
}

static void tcp_session_cb(enum zperf_status status, struct zperf_results *result, void *user_data)
{
    const shell_handle_t sh = user_data;

    switch (status)
    {
    case ZPERF_SESSION_STARTED:
        printf("New TCP session started.\n");
        break;

    case ZPERF_SESSION_FINISHED: {
        uint32_t rate_in_kbps;

        /* Compute baud rate */
        if (result->time_in_us != 0U)
        {
            rate_in_kbps = (uint32_t)(((uint64_t)result->total_len * 8ULL * (uint64_t)USEC_PER_SEC) /
                                      ((uint64_t)result->time_in_us * 1024ULL));
        }
        else
        {
            rate_in_kbps = 0U;
        }

        printf("TCP session ended\n");

        printf(" Duration:\t\t");
        print_number(sh, result->time_in_us, TIME_US, TIME_US_UNIT);
        printf("\n");

        printf(" rate:\t\t\t");
        print_number(sh, rate_in_kbps, KBPS, KBPS_UNIT);
        printf("\n");

        break;
    }

    case ZPERF_SESSION_ERROR:
        printf("TCP session error.\n");
        break;
    }
}

static shell_status_t cmd_tcp_download_stop(const shell_handle_t sh, size_t argc, char *argv[])
{
    int ret;

    ret = zperf_tcp_download_stop();
    if (ret < 0)
    {
        printf("TCP server not running!\n");
        return -ret;
    }

    printf("TCP server stopped\n");

    return kStatus_SHELL_Success;
}

static shell_status_t cmd_tcp_download(const shell_handle_t sh, size_t argc, char *argv[])
{
    if (IS_ENABLED(CONFIG_NET_TCP))
    {
        struct zperf_download_params param = {0};
        int ret;

        ret = zperf_bind_host(sh, argc, argv, &param);
        if (ret < 0)
        {
            printf("Unable to bind host.\n");
            /* shell_help(sh); */
            printf("help");
            return -kStatus_SHELL_Error;
        }

        ret = zperf_tcp_download(&param, tcp_session_cb, (void *)sh);
        if (ret == -EALREADY)
        {
            printf("TCP server already started!\n");
            return -kStatus_SHELL_Error;
        }
        else if (ret < 0)
        {
            printf("Failed to start TCP server!\n");
            return -kStatus_SHELL_Error;
        }

        printf("TCP server started on port %u\n", param.port);

        return kStatus_SHELL_Success;
    }
    else
    {
        return -kStatus_SHELL_Error;
    }
}

/* static shell_status_t cmd_version(const shell_handle_t sh, size_t argc, char *argv[]) */
static shell_status_t cmd_version(void)
{
    printf("Version: %s\nConfig: %s\n", ZPERF_VERSION, CONFIG);

    return kStatus_SHELL_Success;
}

void zperf_shell_init(void)
{
    int ret;

    if (IS_ENABLED(MY_IP6ADDR_SET))
    {
        ret = net_addr_pton(AF_INET6, MY_IP6ADDR, &in6_addr_my.sin6_addr);
        if (ret < 0)
        {
            NET_WARN("Unable to set %s address\n", "IPv6");
        }
        else
        {
            NET_INFO("Setting IP address %s", net_sprint_ipv6_addr(&in6_addr_my.sin6_addr));
        }

        ret = net_addr_pton(AF_INET6, DST_IP6ADDR, &in6_addr_dst.sin6_addr);
        if (ret < 0)
        {
            NET_WARN("Unable to set destination %s address %s", "IPv6", DST_IP6ADDR ? DST_IP6ADDR : "(not set)");
        }
        else
        {
            NET_INFO("Setting destination IP address %s", net_sprint_ipv6_addr(&in6_addr_dst.sin6_addr));
        }
    }
    else
    {
        NET_WARN("Not init ! IPv4");
    }

    if (IS_ENABLED(MY_IP4ADDR_SET))
    {
        ret = net_addr_pton(AF_INET, MY_IP4ADDR, &in4_addr_my.sin_addr);
        if (ret < 0)
        {
            NET_WARN("Unable to set %s address\n", "IPv4");
        }
        else
        {
            NET_INFO("Setting IP address %s", net_sprint_ipv4_addr(&in4_addr_my.sin_addr));
        }

        ret = net_addr_pton(AF_INET, DST_IP4ADDR, &in4_addr_dst.sin_addr);
        if (ret < 0)
        {
            NET_WARN("Unable to set destination %s address %s", "IPv4", DST_IP4ADDR ? DST_IP4ADDR : "(not set)");
        }
        else
        {
            NET_INFO("Setting destination IP address %s", net_sprint_ipv4_addr(&in4_addr_dst.sin_addr));
        }
    }
    else
    {
        NET_WARN("Not init ! IPv6");
    }
}

/* SHELL_STATIC_SUBCMD_SET_CREATE(zperf_cmd_tcp_download, */
/*                                SHELL_CMD(stop, NULL, "Stop TCP server\n", cmd_tcp_download_stop),
 * SHELL_SUBCMD_SET_END); */
/**/
/* SHELL_STATIC_SUBCMD_SET_CREATE(zperf_cmd_tcp, */
/*                                SHELL_CMD(upload, NULL, */
/*                                          "[<options>] <dest ip> <dest port> <duration> <packet size>[K]\n" */
/*                                          "<options>     command options (optional): [-S tos -a]\n" */
/*                                          "<dest ip>     IP destination\n" */
/*                                          "<dest port>   port destination\n" */
/*                                          "<duration>    of the test in seconds\n" */
/*                                          "<packet size> Size of the packet in byte or kilobyte " */
/*                                          "(with suffix K)\n" */
/*                                          "Available options:\n" */
/*                                          "-S tos: Specify IPv4/6 type of service\n" */
/*                                          "-a: Asynchronous call (shell will not block for the upload)\n" */
/*                                          "-n: Disable Nagle's algorithm\n" */
/* #ifdef CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "-p: Specify custom packet priority\n" */
/* #endif // CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "Example: tcp upload 192.0.2.2 1111 1 1K\n" */
/*                                          "Example: tcp upload 2001:db8::2\n", */
/*                                          cmd_tcp_upload), */
/*                                SHELL_CMD(upload2, NULL, */
/*                                          "[<options>] v6|v4 <duration> <packet size>[K] <baud rate>[K|M]\n" */
/*                                          "<options>     command options (optional): [-S tos -a]\n" */
/*                                          "<v6|v4>:      Use either IPv6 or IPv4\n" */
/*                                          "<duration>    Duration of the test in seconds\n" */
/*                                          "<packet size> Size of the packet in byte or kilobyte " */
/*                                          "(with suffix K)\n" */
/*                                          "Available options:\n" */
/*                                          "-S tos: Specify IPv4/6 type of service\n" */
/*                                          "-a: Asynchronous call (shell will not block for the upload)\n" */
/* #ifdef CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "-p: Specify custom packet priority\n" */
/* #endif // CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "Example: tcp upload2 v6 1 1K\n" */
/*                                          "Example: tcp upload2 v4\n" */
/*                                          "-n: Disable Nagle's algorithm\n" */
/* #if defined(CONFIG_NET_IPV6) && defined(MY_IP6ADDR_SET) */
/*                                          "Default IPv6 address is " MY_IP6ADDR ", destination [" DST_IP6ADDR */
/*                                          "]:" DEF_PORT_STR "\n" */
/* #endif */
/* #if defined(CONFIG_NET_IPV4) && defined(MY_IP4ADDR_SET) */
/*                                          "Default IPv4 address is " MY_IP4ADDR ", destination " DST_IP4ADDR */
/*                                          ":" DEF_PORT_STR "\n" */
/* #endif */
/*                                          , */
/*                                          cmd_tcp_upload2), */
/*                                SHELL_CMD(download, zperf_cmd_tcp_download, */
/*                                          "[<port>]:  Server port to listen on/connect to\n" */
/*                                          "[<host>]:  Bind to <host>, an interface address\n" */
/*                                          "Example: tcp download 5001 192.168.0.1\n", */
/*                                          cmd_tcp_download), */
/*                                SHELL_SUBCMD_SET_END); */
/**/
/* SHELL_STATIC_SUBCMD_SET_CREATE(zperf_cmd_udp_download, */
/*                                SHELL_CMD(stop, NULL, "Stop UDP server\n", cmd_udp_download_stop),
 * SHELL_SUBCMD_SET_END); */
/**/
/* SHELL_STATIC_SUBCMD_SET_CREATE(zperf_cmd_udp, */
/*                                SHELL_CMD(upload, NULL, */
/*                                          "[<options>] <dest ip> [<dest port> <duration> <packet size>[K] " */
/*                                          "<baud rate>[K|M]]\n" */
/*                                          "<options>     command options (optional): [-S tos -a]\n" */
/*                                          "<dest ip>     IP destination\n" */
/*                                          "<dest port>   port destination\n" */
/*                                          "<duration>    of the test in seconds\n" */
/*                                          "<packet size> Size of the packet in byte or kilobyte " */
/*                                          "(with suffix K)\n" */
/*                                          "<baud rate>   Baudrate in kilobyte or megabyte\n" */
/*                                          "Available options:\n" */
/*                                          "-S tos: Specify IPv4/6 type of service\n" */
/*                                          "-a: Asynchronous call (shell will not block for the upload)\n" */
/* #ifdef CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "-p: Specify custom packet priority\n" */
/* #endif // CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "Example: udp upload 192.0.2.2 1111 1 1K 1M\n" */
/*                                          "Example: udp upload 2001:db8::2\n", */
/*                                          cmd_udp_upload), */
/*                                SHELL_CMD(upload2, NULL, */
/*                                          "[<options>] v6|v4 [<duration> <packet size>[K] <baud rate>[K|M]]\n" */
/*                                          "<options>     command options (optional): [-S tos -a]\n" */
/*                                          "<v6|v4>:      Use either IPv6 or IPv4\n" */
/*                                          "<duration>    Duration of the test in seconds\n" */
/*                                          "<packet size> Size of the packet in byte or kilobyte " */
/*                                          "(with suffix K)\n" */
/*                                          "<baud rate>   Baudrate in kilobyte or megabyte\n" */
/*                                          "Available options:\n" */
/*                                          "-S tos: Specify IPv4/6 type of service\n" */
/*                                          "-a: Asynchronous call (shell will not block for the upload)\n" */
/* #ifdef CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "-p: Specify custom packet priority\n" */
/* #endif // CONFIG_NET_CONTEXT_PRIORITY */
/*                                          "Example: udp upload2 v4 1 1K 1M\n" */
/*                                          "Example: udp upload2 v6\n" */
/* #if defined(CONFIG_NET_IPV6) && defined(MY_IP6ADDR_SET) */
/*                                          "Default IPv6 address is " MY_IP6ADDR ", destination [" DST_IP6ADDR */
/*                                          "]:" DEF_PORT_STR "\n" */
/* #endif */
/* #if defined(CONFIG_NET_IPV4) && defined(MY_IP4ADDR_SET) */
/*                                          "Default IPv4 address is " MY_IP4ADDR ", destination " DST_IP4ADDR */
/*                                          ":" DEF_PORT_STR "\n" */
/* #endif */
/*                                          , */
/*                                          cmd_udp_upload2), */
/*                                SHELL_CMD(download, zperf_cmd_udp_download, */
/*                                          "[<port>]:  Server port to listen on/connect to\n" */
/*                                          "[<host>]:  Bind to <host>, an interface address\n" */
/*                                          "Example: udp download 5001 192.168.0.1\n", */
/*                                          cmd_udp_download), */
/*                                SHELL_SUBCMD_SET_END); */
/**/
/* SHELL_STATIC_SUBCMD_SET_CREATE(zperf_commands, SHELL_CMD(connectap, NULL, "Connect to AP", cmd_connectap), */
/*                                SHELL_CMD(setip, NULL, */
/*                                          "Set IP address\n" */
/*                                          "<my ip> <prefix len>\n" */
/*                                          "Example setip 2001:db8::2 64\n" */
/*                                          "Example setip 192.0.2.2\n", */
/*                                          cmd_setip), */
/*                                SHELL_CMD(tcp, zperf_cmd_tcp, "Upload/Download TCP data", cmd_tcp), */
/*                                SHELL_CMD(udp, zperf_cmd_udp, "Upload/Download UDP data", cmd_udp), */
/*                                SHELL_CMD(version, NULL, "Zperf version", cmd_version), SHELL_SUBCMD_SET_END); */
/**/
/* SHELL_CMD_REGISTER(zperf, zperf_commands, "Zperf commands", NULL, 0, 0); */

/* void shell_task(size_t argc, char ** argv) */
void shell_task()
{
    shell_handle_t shell_handle = _shell_handle_buff;
    /* char *argv[10] = {"cmd_udp_upload", "fda8:06c3:ce53:a890:0000:0000:0000:0008", "5001", "60", "1K", "100M"}; */
    char *argv[10] = {"cmd_udp_upload", "192.168.0.1", "5001", "60", "1K", "100M"};
    //, "1K", "1M"};
    int argc = 6;

    /*    SHELL_Init(shell_handle, g_serialHandle, "shell : "); */
    /* SHELL_RegisterCommand(shell_handle, SHELL_COMMAND(zperf)); */
    shell_cmd_upload(NULL, argc, argv, nip_IPPROTO_UDP);
    while (1)
    {
    	continue;
    }
}
