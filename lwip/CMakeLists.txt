
# grab all the files from the FilesList.mk files...
# don't want to have to maintain that manually...

set(LWIP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lwip")
set(LWIP_CONTRIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lwip-contrib")

# lwIP list of files was copied from lwip/lwip/src/Filelists.mk.
# COREFILES, CORE4FILES: The minimum set of files needed for lwIP.
set(LWIP_COREFILES "${LWIP_SOURCE_DIR}/src/core/init.c"
                   "${LWIP_SOURCE_DIR}/src/core/def.c"
                   "${LWIP_SOURCE_DIR}/src/core/dns.c"
                   "${LWIP_SOURCE_DIR}/src/core/inet_chksum.c"
                   "${LWIP_SOURCE_DIR}/src/core/ip.c"
                   "${LWIP_SOURCE_DIR}/src/core/mem.c"
                   "${LWIP_SOURCE_DIR}/src/core/memp.c"
                   "${LWIP_SOURCE_DIR}/src/core/netif.c"
                   "${LWIP_SOURCE_DIR}/src/core/pbuf.c"
                   "${LWIP_SOURCE_DIR}/src/core/raw.c"
                   "${LWIP_SOURCE_DIR}/src/core/stats.c"
                   "${LWIP_SOURCE_DIR}/src/core/sys.c"
                   "${LWIP_SOURCE_DIR}/src/core/altcp.c"
                   "${LWIP_SOURCE_DIR}/src/core/altcp_alloc.c"
                   "${LWIP_SOURCE_DIR}/src/core/altcp_tcp.c"
                   "${LWIP_SOURCE_DIR}/src/core/tcp.c"
                   "${LWIP_SOURCE_DIR}/src/core/tcp_in.c"
                   "${LWIP_SOURCE_DIR}/src/core/tcp_out.c"
                   "${LWIP_SOURCE_DIR}/src/core/timeouts.c"
                   "${LWIP_SOURCE_DIR}/src/core/udp.c")

set(LWIP_COREHEADERS "${LWIP_SOURCE_DIR}/src/include/lwip/init.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/def.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/dns.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/inet_chksum.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/ip.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/netif.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/pbuf.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/stats.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/sys.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/altcp.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/altcp_tcp.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/tcp.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/timeouts.h"
                     "${LWIP_SOURCE_DIR}/src/include/lwip/udp.h")

set(LWIP_CORE4FILES "${LWIP_SOURCE_DIR}/src/core/ipv4/autoip.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/dhcp.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/etharp.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/icmp.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/igmp.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/ip4_frag.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/ip4.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv4/ip4_addr.c")

set(LWIP_CORE4HEADERS "${LWIP_SOURCE_DIR}/src/include/lwip/autoip.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/dhcp.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/etharp.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/icmp.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/igmp.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ip4_frag.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ip4.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ip4_addr.h")

set(LWIP_CORE6FILES "${LWIP_SOURCE_DIR}/src/core/ipv6/dhcp6.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/ethip6.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/icmp6.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/inet6.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/ip6.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/ip6_addr.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/ip6_frag.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/mld6.c"
                    "${LWIP_SOURCE_DIR}/src/core/ipv6/nd6.c")

set(LWIP_CORE6HEADERS "${LWIP_SOURCE_DIR}/src/include/lwip/dhcp6.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ethip6.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/icmp6.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ip6.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ip6_addr.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/ip6_frag.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/mld6.h"
                      "${LWIP_SOURCE_DIR}/src/include/lwip/nd6.h")

# APIFILES: The files which implement the sequential and socket APIs.
set(LWIP_APIFILES "${LWIP_SOURCE_DIR}/src/api/api_lib.c"
                  "${LWIP_SOURCE_DIR}/src/api/api_msg.c"
                  "${LWIP_SOURCE_DIR}/src/api/err.c"
                  "${LWIP_SOURCE_DIR}/src/api/if_api.c"
                  "${LWIP_SOURCE_DIR}/src/api/netbuf.c"
                  "${LWIP_SOURCE_DIR}/src/api/netdb.c"
                  "${LWIP_SOURCE_DIR}/src/api/netifapi.c"
                  "${LWIP_SOURCE_DIR}/src/api/sockets.c"
                  "${LWIP_SOURCE_DIR}/src/api/tcpip.c")

set(LWIP_APIHEADERS "${LWIP_SOURCE_DIR}/src/include/lwip/api.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/err.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/if_api.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/netbuf.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/netdb.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/netifapi.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/sockets.h"
                    "${LWIP_SOURCE_DIR}/src/include/lwip/tcpip.h")

# NETIFFILES: Files implementing various generic network interface functions
set(LWIP_NETIFFILES "${LWIP_SOURCE_DIR}/src/netif/ethernet.c"
                    "${LWIP_SOURCE_DIR}/src/netif/bridgeif.c"
                    "${LWIP_SOURCE_DIR}/src/netif/bridgeif_fdb.c"
                    "${LWIP_SOURCE_DIR}/src/netif/slipif.c"
                    # Extra source file include in order to be able to run
                    # lwIP in raw mode on a tap device. Other interfaces may be
                    # added in the future.
                    "${LWIP_CONTRIB_SOURCE_DIR}/ports/unix/port/netif/tapif.c")

set(LWIP_NETIFHEADERS "${LWIP_SOURCE_DIR}/src/include/ethernet.h"
                      "${LWIP_SOURCE_DIR}/src/include/bridgeif.h"
                      "${LWIP_SOURCE_DIR}/src/include/bridgeif_opts.h"
                      "${LWIP_SOURCE_DIR}/src/include/slipif.h"
                      "${LWIP_CONTRIB_SOURCE_DIR}/ports/unix/port/include/netif/tapif.h")

# SIXLOWPAN: 6LoWPAN
set(LWIP_SIXLOWPAN "${LWIP_SOURCE_DIR}/src/netif/lowpan6_common.c"
                   "${LWIP_SOURCE_DIR}/src/netif/lowpan6.c"
                   "${LWIP_SOURCE_DIR}/src/netif/lowpan6_ble.c"
                   "${LWIP_SOURCE_DIR}/src/netif/zepif.c")

set(LWIP_SIXLOWPANHEADERS "${LWIP_SOURCE_DIR}/src/include/netif/lowpan6_common.h"
                          "${LWIP_SOURCE_DIR}/src/include/netif/lowpan6.h"
                          "${LWIP_SOURCE_DIR}/src/include/netif/lowpan6_ble.h"
                          "${LWIP_SOURCE_DIR}/src/include/netif/lowpan6_opts.h"
                          "${LWIP_SOURCE_DIR}/src/include/netif/zepif.h")

# PPPFILES: PPP
set(LWIP_PPPFILES "${LWIP_SOURCE_DIR}/src/netif/ppp/auth.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/ccp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/chap-md5.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/chap_ms.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/chap-new.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/demand.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/eap.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/ecp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/eui64.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/fsm.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/ipcp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/ipv6cp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/lcp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/magic.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/mppe.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/multilink.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/ppp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/pppapi.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/pppcrypt.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/pppoe.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/pppol2tp.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/pppos.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/upap.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/utils.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/vj.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/polarssl/arc4.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/polarssl/des.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/polarssl/md4.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/polarssl/md5.c"
                  "${LWIP_SOURCE_DIR}/src/netif/ppp/polarssl/sha1.c")

set(LWIP_PPPHEADERS "${LWIP_SOURCE_DIR}/src/include/netif/ccp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/chap-md5.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/chap_ms.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/chap-new.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/eap.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/ecp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/eui64.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/fsm.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/ipcp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/ipv6cp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/lcp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/magic.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/mppe.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/ppp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/pppapi.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/pppcrypt.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/pppoe.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/pppol2tp.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/pppos.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/upap.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/ppp_opts.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/vj.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/polarssl/arc4.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/polarssl/des.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/polarssl/md4.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/polarssl/md5.h"
                    "${LWIP_SOURCE_DIR}/src/include/netif/polarssl/sha1.h")

set(LWIP_SOURCES "${LWIP_CONTRIB_SOURCE_DIR}/ports/freertos/sys_arch.c"
                 ${LWIP_COREFILES}
                 ${LWIP_CORE4FILES}
                 ${LWIP_CORE6FILES}
                 ${LWIP_APIFILES}
                 ${LWIP_NETIFFILES}
                 ${LWIP_PPPFILES}
                 ${LWIP_SIXLOWPAN})

set(LWIP_HEADERS ${LWIP_COREHEADERS}
                 ${LWIP_CORE4HEADERS}
                 ${LWIP_CORE6HEADERS}
                 ${LWIP_APIHEADERS}
                 ${LWIP_NETIFHEADERS}
                 ${LWIP_CORE4HEADERS}
                 ${LWIP_PPPHEADERS})

set(LWIP_PRIVATE_DEFINES "configSTACK_DEPTH_TYPE=UBaseType_t")
set(LWIP_PUBLIC_DEFINES "LWIP_COMPAT_MUTEX=0"
                        "DEFAULT_THREAD_STACKSIZE=1024"
                        "TCPIP_THREAD_STACKSIZE=1024")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND LWIP_PUBLIC_DEFINES "LWIP_DEBUG")
endif()

set(LWIP_COMPILE_WARNING_FLAGS
  -Wall -Wparentheses -Wsequence-point -Wswitch-default -Wextra -Wundef
  -Wshadow -Wpointer-arith -Wcast-qual -Wc++-compat -Wwrite-strings
  -Wold-style-definition -Wcast-align -Wmissing-prototypes -Wredundant-decls
  -Wnested-externs -Wno-address -Wunreachable-code -Wuninitialized -Wlogical-op
  -Wno-unused-parameter -Wno-unused-variable)

add_library(lwip ${LWIP_SOURCES})
target_compile_options(
  lwip PRIVATE ${LWIP_COMPILE_WARNING_FLAGS})
target_compile_definitions(
  lwip PUBLIC ${LWIP_PUBLIC_DEFINES} PRIVATE ${LWIP_PRIVATE_DEFINES})
target_link_libraries(lwip PUBLIC freertos)
target_include_directories(
  lwip
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${LWIP_SOURCE_DIR}/src/include>"
    "$<BUILD_INTERFACE:${LWIP_CONTRIB_SOURCE_DIR}/ports/freertos/include>"
    # The order matters! The UNIX port must be included for tapif.h, but the
    # FreeRTOS port must be included first.
    "$<BUILD_INTERFACE:${LWIP_CONTRIB_SOURCE_DIR}/ports/unix/port/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

add_library(lwip_tcpecho_raw INTERFACE)
target_compile_options(lwip_tcpecho_raw INTERFACE ${LWIP_COMPILE_WARNING_FLAGS})
target_compile_definitions(lwip_tcpecho_raw INTERFACE ${LWIP_PUBLIC_DEFINES})
target_link_libraries(lwip_tcpecho_raw INTERFACE lwip)
target_include_directories(
  lwip_tcpecho_raw
  INTERFACE
    "$<BUILD_INTERFACE:${LWIP_CONTRIB_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_DATADIR}/lwip>")
target_sources(
  lwip_tcpecho_raw
  INTERFACE
    "$<BUILD_INTERFACE:${LWIP_CONTRIB_SOURCE_DIR}/apps/tcpecho_raw/tcpecho_raw.c>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_DATADIR}/lwip/apps/tcpecho_raw/tcpecho_raw.c>")

add_library(lwip_udpecho_raw INTERFACE)
target_compile_options(lwip_udpecho_raw INTERFACE ${LWIP_COMPILE_WARNING_FLAGS})
target_compile_definitions(lwip_udpecho_raw INTERFACE ${LWIP_PUBLIC_DEFINES})
target_link_libraries(lwip_udpecho_raw INTERFACE lwip)
target_include_directories(
  lwip_udpecho_raw
  INTERFACE
    "$<BUILD_INTERFACE:${LWIP_CONTRIB_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${LWIP_INSTALL_DATADIR}/lwip>")
target_sources(
  lwip_udpecho_raw
  INTERFACE
    "$<BUILD_INTERFACE:${LWIP_CONTRIB_SOURCE_DIR}/apps/udpecho_raw/udpecho_raw.c>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_DATADIR}/lwip/apps/udpecho_raw/udpecho_raw.c>")

install(
  FILES ${LWIP_HEADERS}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/lwip")

install(
  FILES "${LWIP_CONTRIB_SOURCE_DIR}/apps/tcpecho_raw/tcpecho_raw.h"
        "${LWIP_CONTRIB_SOURCE_DIR}/apps/tcpecho_raw/tcpecho_raw.c"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/lwip/apps/tcpecho_raw")

install(
  FILES "${LWIP_CONTRIB_SOURCE_DIR}/apps/udpecho_raw/udpecho_raw.h"
        "${LWIP_CONTRIB_SOURCE_DIR}/apps/udpecho_raw/udpecho_raw.c"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/lwip/apps/udpecho_raw")

