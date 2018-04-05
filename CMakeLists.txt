#
# Simple CMakeLists.txt to build the tcpecho example on FreeRTOS and lwIP.
#
cmake_minimum_required(VERSION 3.5)
project(foobar VERSION 0.1.0)


set(COMMON_FLAGS -ggdb -m32 -pthread -O0)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
list(APPEND COMMON_FLAGS -ggdb)
endif()

##
## FreeRTOS
##
set(FREERTOS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freertos)
set(FREERTOS_SOURCES ${FREERTOS_SOURCE_DIR}/Source/croutine.c
                     ${FREERTOS_SOURCE_DIR}/Source/event_groups.c
                     ${FREERTOS_SOURCE_DIR}/Source/list.c
                     ${FREERTOS_SOURCE_DIR}/Source/queue.c
                     ${FREERTOS_SOURCE_DIR}/Source/tasks.c
                     ${FREERTOS_SOURCE_DIR}/Source/timers.c
                     ${FREERTOS_SOURCE_DIR}/Source/portable/MemMang/heap_3.c
                     ${FREERTOS_SOURCE_DIR}/Source/portable/GCC/POSIX/port.c)

set(FREERTOS_DEFINES __GCC_POSIX=1)

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port. 
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
list(APPEND FREERTOS_DEFINES DMAX_NUMBER_OF_TASKS=300)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
list(APPEND FREERTOS_DEFINES DEBUG=1 USE_STDIO)
endif()

# Warnings
set(FREERTOS_WARNING_FLAGS -W
                           -Wall
                           -Wformat
                           -Wmissing-braces
                           -Wno-cast-align
                           -Wparentheses
                           -Wshadow
                           -Wno-sign-compare
                           -Wswitch
                           -Wuninitialized
                           -Wunknown-pragmas
                           -Wunused-function
                           -Wunused-label
                           -Wunused-parameter
                           -Wunused-value
                           -Wunused-variable
                           -Wmissing-prototypes)

add_library(freertos ${FREERTOS_SOURCES})
target_compile_options(freertos PUBLIC ${COMMON_FLAGS} ${FREERTOS_FLAGS} ${FREERTOS_WARNING_FLAGS})
target_compile_definitions(freertos PUBLIC ${FREERTOS_DEFINES})
target_include_directories(freertos PUBLIC "${FREERTOS_SOURCE_DIR}/Project"
                                           "${FREERTOS_SOURCE_DIR}/Source/include"
                                           "${FREERTOS_SOURCE_DIR}/Source/portable/GCC/POSIX")

##
## lwIP
##
set(LWIP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwip)


# lwIP list of files was copied from lwip/lwip/src/Filelists.mk.
# COREFILES, CORE4FILES: The minimum set of files needed for lwIP.
set(LWIP_COREFILES ${LWIP_SOURCE_DIR}/lwip/src/core/init.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/def.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/dns.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/inet_chksum.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/ip.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/mem.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/memp.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/netif.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/pbuf.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/raw.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/stats.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/sys.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/altcp.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/altcp_alloc.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/altcp_tcp.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/tcp.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/tcp_in.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/tcp_out.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/timeouts.c
                   ${LWIP_SOURCE_DIR}/lwip/src/core/udp.c)

set(LWIP_CORE4FILES ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/autoip.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/dhcp.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/etharp.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/icmp.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/igmp.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/ip4_frag.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/ip4.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv4/ip4_addr.c)

set(LWIP_CORE6FILES ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/dhcp6.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/ethip6.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/icmp6.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/inet6.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/ip6.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/ip6_addr.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/ip6_frag.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/mld6.c
                    ${LWIP_SOURCE_DIR}/lwip/src/core/ipv6/nd6.c)

# APIFILES: The files which implement the sequential and socket APIs.
set(LWIP_APIFILES ${LWIP_SOURCE_DIR}/lwip/src/api/api_lib.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/api_msg.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/err.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/if_api.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/netbuf.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/netdb.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/netifapi.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/sockets.c
                  ${LWIP_SOURCE_DIR}/lwip/src/api/tcpip.c)

# NETIFFILES: Files implementing various generic network interface functions
set(LWIP_NETIFFILES ${LWIP_SOURCE_DIR}/lwip/src/netif/ethernet.c
${LWIP_SOURCE_DIR}/lwip/src/netif/bridgeif.c
${LWIP_SOURCE_DIR}/lwip/src/netif/bridgeif_fdb.c
${LWIP_SOURCE_DIR}/lwip/src/netif/slipif.c)

# SIXLOWPAN: 6LoWPAN
set(LWIP_SIXLOWPAN ${LWIP_SOURCE_DIR}/lwip/src/netif/lowpan6_common.c
${LWIP_SOURCE_DIR}/lwip/src/netif/lowpan6.c
${LWIP_SOURCE_DIR}/lwip/src/netif/lowpan6_ble.c
${LWIP_SOURCE_DIR}/lwip/src/netif/zepif.c)

# PPPFILES: PPP
set(LWIP_PPPFILES ${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/auth.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/ccp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/chap-md5.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/chap_ms.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/chap-new.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/demand.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/eap.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/ecp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/eui64.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/fsm.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/ipcp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/ipv6cp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/lcp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/magic.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/mppe.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/multilink.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/ppp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/pppapi.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/pppcrypt.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/pppoe.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/pppol2tp.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/pppos.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/upap.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/utils.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/vj.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/polarssl/arc4.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/polarssl/des.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/polarssl/md4.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/polarssl/md5.c
${LWIP_SOURCE_DIR}/lwip/src/netif/ppp/polarssl/sha1.c)

set(LWIP_SOURCES ${LWIP_SOURCE_DIR}/lwip-contrib/ports/freertos/sys_arch.c
                 ${LWIP_COREFILES}
                 ${LWIP_CORE4FILES}
                 ${LWIP_CORE6FILES}
                 ${LWIP_APIFILES}
                 ${LWIP_NETIFFILES}
                 ${LWIP_PPPFILES}
                 ${LWIP_SIXLOWPAN})

set(LWIP_WARNING_FLAGS -Wall
                       -Wparentheses
                       -Wsequence-point
                       -Wswitch-default
                       -Wextra
                       -Wundef
                       -Wshadow
                       -Wpointer-arith
                       -Wcast-qual
                       -Wc++-compat
                       -Wwrite-strings
                       -Wold-style-definition
                       -Wcast-align
                       -Wmissing-prototypes
                       -Wredundant-decls
                       -Wnested-externs
                       -Wno-address
                       -Wunreachable-code
                       -Wuninitialized
                       -Wlogical-op
                       -Wno-unused-parameter
                       -Wno-unused-variable)

set(LWIP_DEFINES configSTACK_DEPTH_TYPE=UBaseType_t LWIP_COMPAT_MUTEX=0 DEFAULT_THREAD_STACKSIZE=1024 TCPIP_THREAD_STACKSIZE=1024)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
list(APPEND LWIP_DEFINES LWIP_DEBUG)
endif()

add_library(lwip ${LWIP_SOURCES})
target_compile_options(lwip PUBLIC ${COMMON_FLAGS} ${LWIP_FLAGS} ${LWIP_WARNING_FLAGS})
target_compile_definitions(lwip PUBLIC ${LWIP_DEFINES})
target_link_libraries(lwip PUBLIC freertos)
target_include_directories(lwip PUBLIC "${LWIP_SOURCE_DIR}/include"
                                       "${LWIP_SOURCE_DIR}/lwip/src/include"
                                       "${LWIP_SOURCE_DIR}/lwip-contrib/ports/freertos/include"
                                       "${LWIP_SOURCE_DIR}/lwip-contrib/ports/unix/port/include")

set(TCPECHO_SOURCES "lwip_freertos_tcpecho/src/freertos_tcpecho.c"
                    "lwip_freertos_tcpecho/freertos/FreeRTOSCommonHooks.c"
                    "${LWIP_SOURCE_DIR}/lwip-contrib/ports/unix/port/netif/tapif.c"
                    "${LWIP_SOURCE_DIR}/lwip-contrib/apps/tcpecho/tcpecho.c"
                    "${LWIP_SOURCE_DIR}/lwip-contrib/apps/tcpecho_raw/tcpecho_raw.c")

add_executable(tcpecho ${TCPECHO_SOURCES})
#target_compile_options(tcpecho PRIVATE ${COMMON_FLAGS} ${LWIP_FLAGS} ${LWIP_WARNING_FLAGS})
target_compile_options(tcpecho PRIVATE ${COMMON_FLAGS} ${LWIP_FLAGS})
target_compile_definitions(tcpecho PUBLIC ${LWIP_DEFINES})
target_include_directories(tcpecho PRIVATE "${LWIP_SOURCE_DIR}/lwip-contrib")
target_link_libraries(tcpecho freertos lwip)
set_target_properties(tcpecho PROPERTIES LINK_FLAGS "-m32 -pthread")

