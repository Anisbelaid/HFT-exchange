cmake_minimum_required(VERSION 3.12.4) 

project(vOne)
##########################################################PARAMETERS#############################################################
if (NOT DEFINED ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE Debug)
endif ()

#if (NOT DEFINED CMAKE_MODULE_PATH)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/)
#endif()

#if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../Laverage)
#endif ()
#################################################################################################################################  
########################################################Cmake includes###########################################################
include(CTest)
include(CPack)
include(CodeCoverage)
include(FindGoogleBenchmark)
#################################################################################################################################
########################################################External package#########################################################
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(GTest)
FIND_PACKAGE(Boost 1.67.0 COMPONENTS filesystem system thread stacktrace_backtrace log log_setup REQUIRED)
FIND_PACKAGE(Python COMPONENTS Interpreter Development)
FIND_PACKAGE(benchmark)
FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
#################################################################################################################################
########################################################Project setups###########################################################
if (GTEST_FOUND)
    enable_testing()
endif()

if (PYTHON_FOUND)
    set (PYTHON_EXECUTABLE /usr/bin/python3.7)
endif()

if (BOOST_FOUND)
    SET(Boost_USE_MULTITHREADED ON)
endif ()

#if (DOXYGEN_FOUND)
#    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    # note the option ALL which allows to build the docs together with the application
#    add_custom_target( doc_doxygen ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#        COMMENT "Generating API documentation with Doxygen"
#        VERBATIM )
    
#    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc_doxygen DESTINATION ${CMAKE_INSTALL_PREFIX})
#endif (DOXYGEN_FOUND)
#################################################################################################################################
#######################################################Envirement and options####################################################
set (INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/Lib/External/ ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Internal/ ${CMAKE_CURRENT_SOURCE_DIR}/Message/ /usr/include/ )
set (MATCHING_ENGINE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/MainServer/ ${CMAKE_CURRENT_SOURCE_DIR}/MainServer/MatchingEngine/ ${CMAKE_CURRENT_SOURCE_DIR}/MainServer/Sequencer ${CMAKE_CURRENT_SOURCE_DIR}/MainServer/TradingServer/ ${CMAKE_CURRENT_SOURCE_DIR}/Message/ )
option(BENCHMARK_ENABLE_TESTING "Enable testing of the benchmark library." ON)
option(BENCHMARK_ENABLE_EXCEPTIONS "Enable the use of exceptions in the benchmark library." ON)
option(BENCHMARK_ENABLE_LTO "Enable link time optimisation of the benchmark library." OFF)
option(BENCHMARK_USE_LIBCXX "Build and test using libc++ as the standard library." OFF)
################################################################################################################################

message(STATUS "")
message(STATUS "BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID          : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install configuration: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Module path          : ${CMAKE_MODULE_PATH}")
message(STATUS "  Python               : ${PYTHON_EXECUTABLE}")
message(STATUS "  Build benchmarks     : ${BENCH}")
message(STATUS "  Boost include dirs   : ${Boost_INCLUDE_DIRS}")
message(STATUS "  GTK include dir      : ${GTK2_INCLUDE_DIRS}")
message(STATUS "") 

#add_subdirectory(MatchingEngine)
#add_subdirectory(Dispatcher)
add_subdirectory(Broker)
#add_subdirectory(Gateway)
#add_subdirectory(MarketDataServer)
#add_subdirectory(PersistenceServer)
#add_subdirectory(MonitoringServer)
#add_subdirectory(Lib)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION "Eaumex matching engine")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VENDOR "Eumex")
set(CPACK_PACKAGE_CONTACT "anis@eaumex.com mondher@eaumex.com")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-1.0.0")

