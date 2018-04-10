set(TCPECHO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/freertos_tcpecho.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/freertos/FreeRTOSCommonHooks.c")

add_executable(tcpecho ${TCPECHO_SOURCES})
target_link_libraries(tcpecho freertos lwip lwip_tcpecho_raw lwip_udpecho_raw)

