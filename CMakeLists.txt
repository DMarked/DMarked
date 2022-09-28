cmake_minimum_required(VERSION 3.20)

set (DVERSION "0.3.0" CACHE STRING "define project version" FORCE)
project (dmarked
  VERSION ${DVERSION}
  DESCRIPTION "dtk based markdown editor"
  HOMEPAGE_URL "https://github.com/DMarked/DMarked"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

add_subdirectory(dmarked)
