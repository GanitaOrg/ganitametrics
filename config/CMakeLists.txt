##=========================================================================
## This is a configuration file for building GanitaMetrics. 
## 
##=========================================================================

project(GanitaMetrics CXX C)

## Version
set(GanitaMetrics_VERSION_MAJOR "0")
set(GanitaMetrics_VERSION_MINOR "9")
set(GanitaMetrics_VERSION_PATCH "2")

cmake_minimum_required(VERSION 2.8)

enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CC")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++0x")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lbsd" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lbsd" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++0x")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lbsd" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lbsd" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-stdlib=libc++")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lGanitaGraph" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lGanitaGraph" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang++")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "clang++")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-stdlib=libc++")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lGanitaGraph" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lGanitaGraph" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "c++")
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++0x")
  set( CMAKE_SHARED_LINKER_FLAGS  "-lbsd" )
  set( CMAKE_STATIC_LINKER_FLAGS  "-lbsd" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(warnings "/W4 /WX /EHsc")
else()
  set(warnings "-Wall -Wextra")
  set(cmake_cxx_extra "-std=c++11")
endif()

if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${warnings} ${cmake_cxx_extra}"
    CACHE STRING "Flags used by the compiler during build." FORCE)
  set(CMAKE_C_FLAGS "${warnings} ${cmake_cxx_extra}"
    CACHE STRING "Flags used by the compiler during build." FORCE)
endif()

  IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_64)
    set(CPACK_TOPLEVEL_TAG x86_64)
  ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(CPACK_SYSTEM_NAME x86_32)
    set(CPACK_TOPLEVEL_TAG x86_32)
  ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")

# configure_file(config.h.in config.h)

include_directories(/usr/local/include ../src/include)
link_directories (/usr/local/lib)

add_library(GanitaMetrics SHARED ../src/cpp/GanitaMetrics.cpp ../src/cpp/GanitaMetricsVersion.cpp ../src/cpp/GanitaMetricsMat.cpp ../src/cpp/GanitaMetricsMotDetection.cpp  ../src/cpp/GanitaMetricsTopDetection.cpp ../src/cpp/GanitaMetricsTrack.cpp ../src/cpp/GanitaMetricsTrackSet.cpp ../src/cpp/GanitaMetricsOptions.cpp)

target_include_directories (GanitaMetrics PUBLIC /usr/local/include ../src/include ../src/cpp)

add_executable(gmetrics ../src/drivers/gmetrics.cc)
# add_test(gztest gmetrics ../sample/Chacon3.gztran)

target_link_libraries (gmetrics GanitaMetrics GanitaGraph m ${CMAKE_SHARED_LINKER_FLAGS})

install(TARGETS GanitaMetrics DESTINATION lib)
install(TARGETS gmetrics DESTINATION bin)
FILE(GLOB files "../src/include/ganita/metrics/*.hpp")
install(FILES ${files} DESTINATION include/ganita/metrics)
install(DIRECTORY DESTINATION include/ganita/metrics)

# if(OpenMP_CXX_FLAGS)
#   set_property(
#     SOURCE hmm_test.cpp
#     PROPERTY COMPILE_DEFINITIONS WITH_OMP
#     )
# ENDIF(OpenMP_CXX_FLAGS)
