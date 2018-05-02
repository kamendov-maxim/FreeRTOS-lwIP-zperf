include(GNUInstallDirs)

set(FREERTOS_DEFINES __GCC_POSIX=1 MAX_NUMBER_OF_TASKS=300)
set(FREERTOS_COMPILE_WARNING_FLAGS
  -W -Wall -Werror -Wmissing-braces -Wno-cast-align -Wparentheses -Wshadow
  -Wno-sign-compare -Wswitch -Wuninitialized -Wunknown-pragmas
  -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value
  -Wunused-variable -Wmissing-prototypes)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND FREERTOS_COMPILE_FLAGS -ggdb)
  list(APPEND FREERTOS_LINKER_FLAGS -ggdb)
endif()

set(FREERTOS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Sim")
set(
  FREERTOS_SOURCES
    "${FREERTOS_SOURCE_DIR}/Source/croutine.c"
    "${FREERTOS_SOURCE_DIR}/Source/event_groups.c"
    "${FREERTOS_SOURCE_DIR}/Source/list.c"
    "${FREERTOS_SOURCE_DIR}/Source/queue.c"
    "${FREERTOS_SOURCE_DIR}/Source/tasks.c"
    "${FREERTOS_SOURCE_DIR}/Source/timers.c"
    "${FREERTOS_SOURCE_DIR}/Source/portable/MemMang/heap_3.c"
    "${FREERTOS_SOURCE_DIR}/Source/portable/GCC/POSIX/port.c")

set(
  FREERTOS_HEADERS
    "${FREERTOS_SOURCE_DIR}/Project/FreeRTOSConfig.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/croutine.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/deprecated_definitions.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/event_groups.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/FreeRTOS.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/list.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/mpu_prototypes.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/mpu_wrappers.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/portable.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/projdefs.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/queue.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/semphr.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/StackMacros.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/task.h"
    "${FREERTOS_SOURCE_DIR}/Source/include/timers.h"
    "${FREERTOS_SOURCE_DIR}/Source/portable/GCC/POSIX/portmacro.h")

add_library(freertos ${FREERTOS_SOURCES})
target_compile_options(
  freertos
  PUBLIC "-m32"
  PRIVATE ${FREERTOS_COMPILE_WARNING_FLAGS})
target_compile_definitions(
  freertos
  PUBLIC ${FREERTOS_DEFINES})
target_include_directories(
  freertos
  PUBLIC "$<BUILD_INTERFACE:${FREERTOS_SOURCE_DIR}/Project>"
         "$<BUILD_INTERFACE:${FREERTOS_SOURCE_DIR}/Source/include>"
         "$<BUILD_INTERFACE:${FREERTOS_SOURCE_DIR}/Source/portable/GCC/POSIX>"
         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(freertos PUBLIC -m32 -pthread)

add_library(${CMAKE_PROJECT_NAME}::freertos ALIAS freertos)

install(
  FILES ${FREERTOS_HEADERS}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(
  TARGETS freertos
  EXPORT "${CMAKE_LC_PROJECT_NAME}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib")

