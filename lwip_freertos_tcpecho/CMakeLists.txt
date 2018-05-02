cmake_minimum_required(VERSION 3.5)

if(NOT TARGET FreeRTOS-lwIP-Sim::freertos)
  find_package(FreeRTOS-lwIP-Sim)
endif()

add_executable(
  tcpecho
  "${CMAKE_CURRENT_SOURCE_DIR}/src/freertos_tcpecho.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/freertos/FreeRTOSCommonHooks.c")

target_link_libraries(
  tcpecho
  FreeRTOS-lwIP-Sim::freertos
  FreeRTOS-lwIP-Sim::lwip
  FreeRTOS-lwIP-Sim::lwip_tcpecho_raw
  FreeRTOS-lwIP-Sim::lwip_udpecho_raw)

