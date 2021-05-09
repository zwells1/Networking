# - Try to find ZMQ
# Once done this will define
# ZMQ_FOUND - System has ZMQ
# ZMQ_INCLUDE_DIRS - The ZMQ include directories
# ZMQ_LIBRARIES - The libraries needed to use ZMQ
# ZMQ_DEFINITIONS - Compiler switches required for using ZMQ

if(WIN32)
  # windows cppzmq is installed via cmake
  FIND_PACKAGE(cppzmq REQUIRED)
  set (ZMQ_LIBRARIES cppzmq-static)
  set (ZMQ_INCLUDE_DIRS ${ZeroMQ_INCLUDE_DIR} ${cppzmq_INCLUDE_DIR})
else()
  find_library (ZMQ_LIBRARY NAMES zmq)
  find_path (ZMQ_INCLUDE_DIR  zmq.hpp)
  
  set (ZMQ_LIBRARIES ${ZMQ_LIBRARY})
  set (ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})
endif()

include ( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set ZMQ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args ( ZMQ DEFAULT_MSG ZMQ_LIBRARIES ZMQ_INCLUDE_DIRS )
