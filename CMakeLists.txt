cmake_minimum_required(VERSION 3.5)
project(FreeRTOS-lwIP-Sim VERSION 0.1.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LC_PROJECT_NAME)

# Conflicts may be introduced when placing the libraries or headers in the
# default system locations, i.e. /usr/lib and /usr/include on *NIX platforms.
# The default installation prefix will therefore be /opt/FreeRTOS-lwIP-Sim.
# find_package can still be used as /opt/<name> is in CMAKE_SYSTEM_PREFIX_PATH
# by default on *NIX platforms.
if(UNIX)
  set(CMAKE_INSTALL_PREFIX /opt/${CMAKE_LC_PROJECT_NAME})
endif()

# Simple convenience function to install a file including directory structure.
function(install_file file_ dest_ basedir_)
  get_filename_component(dir1_ "${file_}" DIRECTORY)
  string(REPLACE "${basedir_}" "" dir_ "${dir1_}")
  install(
    FILES ${file_}
    DESTINATION "${dest_}/${dir_}/")
endfunction()

add_subdirectory(freertos)
add_subdirectory(lwip)
add_subdirectory(lwip_freertos_tcpecho)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_LC_PROJECT_NAME}-config.cmake.in"
  "${CMAKE_LC_PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}")

write_basic_package_version_file(
  "${CMAKE_LC_PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_LC_PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_LC_PROJECT_NAME}-config-version.cmake"
  DESTINATION
    "${CMAKE_INSTALL_PREFIX}")

install(
  EXPORT "${CMAKE_LC_PROJECT_NAME}"
  FILE "${CMAKE_LC_PROJECT_NAME}-targets.cmake"
  NAMESPACE "${CMAKE_PROJECT_NAME}::"
  DESTINATION "${CMAKE_INSTALL_PREFIX}")

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lwip_freertos_tcpecho"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/examples")

