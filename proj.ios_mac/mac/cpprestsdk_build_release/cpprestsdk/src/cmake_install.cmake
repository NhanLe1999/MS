# Install script for directory: /Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pplx" TYPE FILE FILES "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/threadpool.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cpprest" TYPE FILE FILES
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/astreambuf.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/asyncrt_utils.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/base_uri.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/containerstream.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/filestream.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/http_client.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/http_compression.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/http_headers.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/http_listener.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/http_msg.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/interopstream.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/json.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/oauth1.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/oauth2.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/producerconsumerstream.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/rawptrstream.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/streams.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/uri.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/uri_builder.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/version.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/ws_client.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/ws_msg.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pplx" TYPE FILE FILES
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplx.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplxcancellation_token.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplxconv.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplxinterface.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplxlinux.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplxtasks.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/pplx/pplxwin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cpprest/details" TYPE FILE FILES
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/SafeInt3.hpp"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/basic_types.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/cpprest_compat.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/fileio.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/http_constants.dat"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/http_helpers.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/http_server.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/http_server_api.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/nosal.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/resource.h"
    "/Users/apple/MJ_ToolTest/proj.ios_mac/mac/cpprestsdk/src/../include/cpprest/details/web_utilities.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/apple/MJ_ToolTest/build_release_mac/proj.ios_mac/mac/cpprestsdk/Binaries/Release/libcpprest.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcpprest.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcpprest.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcpprest.a")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk" TYPE FILE FILES "/Users/apple/MJ_ToolTest/build_release_mac/proj.ios_mac/mac/cpprestsdk/src/cpprestsdk-config.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk/cpprestsdk-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk/cpprestsdk-targets.cmake"
         "/Users/apple/MJ_ToolTest/build_release_mac/proj.ios_mac/mac/cpprestsdk/src/CMakeFiles/Export/lib/cpprestsdk/cpprestsdk-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk/cpprestsdk-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk/cpprestsdk-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk" TYPE FILE FILES "/Users/apple/MJ_ToolTest/build_release_mac/proj.ios_mac/mac/cpprestsdk/src/CMakeFiles/Export/lib/cpprestsdk/cpprestsdk-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cpprestsdk" TYPE FILE FILES "/Users/apple/MJ_ToolTest/build_release_mac/proj.ios_mac/mac/cpprestsdk/src/CMakeFiles/Export/lib/cpprestsdk/cpprestsdk-targets-release.cmake")
  endif()
endif()

