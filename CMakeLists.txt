cmake_minimum_required(VERSION 3.20)

set (DVERSION "0.3.0" CACHE STRING "define project version")
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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(POSITION_INDEPENDENT_CODE True)
if(NOT MSVC)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif ()
endif()

include(GNUInstallDirs)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

option(UES_VENDORED_FAKEVIM "Set OFF to find FakeVim by pkg-config" ON)
option(UES_VENDORED_QMARKDOWNTEXTEDIT "Set OFF to find QmarkdownTextedit by pkg-config" ON)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "UES_VENDORED_FAKEVIM: ${UES_VENDORED_FAKEVIM}")
message(STATUS "UES_VENDORED_QMARKDOWNTEXTEDIT: ${UES_VENDORED_QMARKDOWNTEXTEDIT}")

add_subdirectory(3rdparty)
add_subdirectory(dmarked)
