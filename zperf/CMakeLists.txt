cmake_minimum_required(VERSION 3.5)

if(NOT TARGET FreeRTOS-lwIP-Sim::freertos)
  find_package(FreeRTOS-lwIP-Sim)
endif()

add_executable(
  zperf
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_session.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_udp_receiver.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_tcp_receiver.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/kernel.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/net/net_core.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/net/net_ip.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/net/net_if.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/net/net_private.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/thread.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr/work.c"
 "${CMAKE_CURRENT_SOURCE_DIR}/zperf_shell.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_udp_uploader.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_common.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_tcp_uploader.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/zperf_main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/freertos/FreeRTOSCommonHooks.c")

target_include_directories(
  zperf
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/zephyr"
  "${CMAKE_CURRENT_SOURCE_DIR}/"
)
target_link_libraries(
  zperf
  FreeRTOS-lwIP-Sim::freertos
  FreeRTOS-lwIP-Sim::lwip
  FreeRTOS-lwIP-Sim::lwip_tcpecho_raw
  FreeRTOS-lwIP-Sim::lwip_udpecho_raw)
