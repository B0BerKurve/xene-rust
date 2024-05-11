# Install script for directory: C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/blds/zlib-ng/src/2.0.7-e3901fc4d8.clean

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static")
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
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/lib/zlibstatic-ng.lib")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/lib" TYPE STATIC_LIBRARY FILES "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/blds/zlib-ng/x64-windows-static-rel/zlibstatic-ng.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/include/zlib-ng.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/include" TYPE FILE RENAME "zlib-ng.h" FILES "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/blds/zlib-ng/src/2.0.7-e3901fc4d8.clean/zlib-ng.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/include/zconf-ng.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/include" TYPE FILE RENAME "zconf-ng.h" FILES "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/blds/zlib-ng/x64-windows-static-rel/zconf-ng.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/lib/pkgconfig/zlib-ng.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/pkgs/zlib-ng_x64-windows-static/lib/pkgconfig" TYPE FILE FILES "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/blds/zlib-ng/x64-windows-static-rel/zlib-ng.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/zach/Documents/Websites/_GITHUB/xenesense-rust/vcpkg_installed/vcpkg/blds/zlib-ng/x64-windows-static-rel/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
