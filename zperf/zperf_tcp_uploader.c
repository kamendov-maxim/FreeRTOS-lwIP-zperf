/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lwip/tcpip.h"
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(net_zperf, CONFIG_NET_ZPERF_LOG_LEVEL);

#include <zephyr/kernel.h>

#include <errno.h>

#include <zephyr/net/socket.h>
#include <zperf.h>

#include "zperf_internal.h"

static char sample_packet[PACKET_SIZE_MAX];

static struct zperf_async_upload_context tcp_async_upload_ctx;

static ssize_t sendall(int sock, const void *buf, size_t len)
{
	while (len) {
        /* LOCK_TCPIP_CORE(); */
		ssize_t out_len = zsock_send(sock, buf, len, 0);
        /* UNLOCK_TCPIP_CORE(); */

		if (out_len < 0) {
			return out_len;
		}

		buf = (const char *)buf + out_len;
		len -= out_len;
	}

	return 0;
}

static int tcp_upload(int sock,
		      unsigned int duration_in_ms,
		      unsigned int packet_size,
		      struct zperf_results *results)
{
	k_timepoint_t end = sys_timepoint_calc(K_MSEC(duration_in_ms));
	int64_t start_time, end_time;
	uint32_t nb_packets = 0U, nb_errors = 0U;
	uint32_t alloc_errors = 0U;
	int ret = 0;

	if (packet_size > PACKET_SIZE_MAX) {
		NET_WARN("Packet size too large! max size: %u\n",
			PACKET_SIZE_MAX);
		packet_size = PACKET_SIZE_MAX;
	}

	/* Start the loop */
	start_time = k_uptime_ticks();

	(void)memset(sample_packet, 'z', sizeof(sample_packet));

	/* Set the "flags" field in start of the packet to be 0.
	 * As the protocol is not properly described anywhere, it is
	 * not certain if this is a proper thing to do.
	 */
	(void)memset(sample_packet, 0, sizeof(uint32_t));

	do {
		/* Send the packet */
		ret = sendall(sock, sample_packet, packet_size);
		if (ret < 0) {
			if (nb_errors == 0 && ret != -ENOMEM) {
				NET_ERR("Failed to send the packet (%d)", errno);
			}

			nb_errors++;

			if (errno == -ENOMEM) {
				/* Ignore memory errors as we just run out of
				 * buffers which is kind of expected if the
				 * buffer count is not optimized for the test
				 * and device.
				 */
				alloc_errors++;
			} else {
				ret = -errno;
				break;
			}
		} else {
			nb_packets++;
		}

#if defined(CONFIG_ARCH_POSIX)
		k_busy_wait(100 * USEC_PER_MSEC);
#else
		k_yield();
#endif

	} while (!sys_timepoint_expired(end));

	end_time = k_uptime_ticks();

	/* Add result coming from the client */
	results->nb_packets_sent = nb_packets;
	results->client_time_in_us =
				k_ticks_to_us_ceil32(end_time - start_time);
	results->packet_size = packet_size;
	results->nb_packets_errors = nb_errors;

	if (alloc_errors > 0) {
		NET_WARN("There was %u network buffer allocation "
			 "errors during send.\nConsider increasing the "
			 "value of CONFIG_NET_BUF_TX_COUNT and\n"
			 "optionally CONFIG_NET_PKT_TX_COUNT Kconfig "
			 "options.",
			 alloc_errors);
	}

	if (ret < 0) {
		return ret;
	}

	return 0;
}

int zperf_tcp_upload(const struct zperf_upload_params *param,
		     struct zperf_results *result)
{

	int sock;
	int ret;

	if (param == NULL || result == NULL) {
		return -EINVAL;
	}

	sock = zperf_prepare_upload_sock((struct sockaddr*)(&param->peer_addr), param->options.tos,
					 param->options.priority, IPPROTO_TCP);
	if (sock < 0) {
		return sock;
	}

	if (param->options.tcp_nodelay &&
	    zsock_setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
			     &param->options.tcp_nodelay,
			     sizeof(param->options.tcp_nodelay)) != 0) {
		NET_WARN("Failed to set IPPROTO_TCP - TCP_NODELAY socket option.");
		return -EINVAL;
	}

	ret = tcp_upload(sock, param->duration_ms, param->packet_size, result);

	zsock_close(sock);

	return ret;
}

static void tcp_upload_async_work(struct k_work *work)
{
	struct zperf_async_upload_context *upload_ctx =
		CONTAINER_OF(work, struct zperf_async_upload_context, work);
	struct zperf_results result;
	int ret;

	upload_ctx->callback(ZPERF_SESSION_STARTED, NULL,
			     upload_ctx->user_data);

	ret = zperf_tcp_upload(&upload_ctx->param, &result);
	if (ret < 0) {
		upload_ctx->callback(ZPERF_SESSION_ERROR, NULL,
				     upload_ctx->user_data);
	} else {
		upload_ctx->callback(ZPERF_SESSION_FINISHED, &result,
				     upload_ctx->user_data);
	}
}

int zperf_tcp_upload_async(const struct zperf_upload_params *param,
			   zperf_callback callback, void *user_data)
{
	if (param == NULL || callback == NULL) {
		return -EINVAL;
	}

	if (k_work_is_pending(&tcp_async_upload_ctx.work)) {
		return -EBUSY;
	}

	memcpy(&tcp_async_upload_ctx.param, param, sizeof(*param));
	tcp_async_upload_ctx.callback = callback;
	tcp_async_upload_ctx.user_data = user_data;

	zperf_async_work_submit(&tcp_async_upload_ctx.work);

	return 0;
}

void zperf_tcp_uploader_init(void)
{
	k_work_init(&tcp_async_upload_ctx.work, tcp_upload_async_work);
}
