cmake_minimum_required(VERSION 3.16)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

PROJECT(Networking)

SET(Networking_VERSION_MAJOR 1)
SET(Networking_VERSION_MINOR 0)

#GNU setup
IF(CMAKE_COMPILER_IS_GNUCC)
  #add in warnings
  ADD_DEFINITIONS(-Wall)

  #shared lib support
  ADD_DEFINITIONS(-fPIC)

  #supported c++ std
  ADD_DEFINITIONS(-std=c++11)

ENDIF(CMAKE_COMPILER_IS_GNUCC)

find_package(Threads REQUIRED)



find_package(Boost COMPONENTS chrono)

#sanity check for proper boost find
if (NOT Boost_CHRONO_LIBRARY)
  message("Boost Chrono library not found, fix link issue!")
  set(Boost_CHRONO_LIBRARY)
endif()

find_package(Boost COMPONENTS program_options system REQUIRED)

find_package(ZMQ REQUIRED)

include_directories("./")

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_DEBUG_POSTFIX d)

#-------------------------------------------------------------------------------
# ZMQ
#-------------------------------------------------------------------------------
ADD_LIBRARY(
  NetZmq
  Zmq/PubSocket.cpp
  Zmq/Socket.cpp
  Zmq/SubSocket.cpp
  )

TARGET_LINK_LIBRARIES(
  NetZmq
  ${ZMQ_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )

#-------------------------------------------------------------------------------
# Package Config
#-------------------------------------------------------------------------------

SET(Net_VERSION_STRING "${Networking_VERSION_MAJOR}.${Networking_VERSION_MINOR}")
SET(Net_DIRNAME "${Networking_VERSION_MAJOR}.${Networking_VERSION_MINOR}")

SET(Net_DIRNAME_lib "lib/${NET_DIRNAME}")
SET(Net_DIRNAME_include "include/${NET_DIRNAME}")

# TO-DO: Do we need this
INCLUDE(CMakePackageConfigHelpers)

CONFIGURE_PACKAGE_CONFIG_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/NetConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/NetConfig.cmake"
  INSTALL_DESTINATION
    ${Net_DIRNAME_lib}
  PATH_VARS
    Net_DIRNAME_lib
    Net_DIRNAME_include
  )

WRITE_BASIC_PACKAGE_VERSION_FILE(
  "${CMAKE_CURRENT_BINARY_DIR}/NetConfigVersion.cmake"
  VERSION ${Net_VERSION_STRING}
  COMPATIBILITY AnyNewerVersion
  )

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------

INSTALL(
  TARGETS
    NetZmq
  DESTINATION
    ${Net_DIRNAME_lib}
  EXPORT
    NetLibExports
  )

INSTALL(
  EXPORT
    NetLibExports
  DESTINATION
    ${Net_DIRNAME_lib}
  )

INSTALL(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NetConfigVersion.cmake
  DESTINATION
    ${Net_DIRNAME_lib}
  )

INSTALL(
  FILES
    Zmq/PubSocket.hpp
    Zmq/Socket.hpp
    Zmq/SubSocket.hpp
  DESTINATION
    ${Net_DIRNAME_include}/Zmq
  )


#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
IF (ENABLE_TESTS)

#-------------------------------------------------------------------------------
# Basic Pub Socket
#-------------------------------------------------------------------------------

  ADD_EXECUTABLE(
    BasicPubSocket
    Tests/BasicPubSocket.cpp
    )

  TARGET_LINK_LIBRARIES(
    BasicPubSocket
    NetZmq
    )

#-------------------------------------------------------------------------------
# Basic Sub Socket
#-------------------------------------------------------------------------------
  ADD_EXECUTABLE(
    BasicSubSocket
    Tests/BasicSubSocket.cpp
    )

  TARGET_LINK_LIBRARIES(
    BasicSubSocket
    NetZmq
    )

ENDIF()
