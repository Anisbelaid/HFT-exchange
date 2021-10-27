cmake_minimum_required(VERSION 3.10.3)

project(MatchingEngine_Test)          

set (Test_Include /usr/include/ ${CMAKE_CURRENT_SOURCE_DIR}/MatchingEngine_Test/ ${CMAKE_CURRENT_SOURCE_DIR}/Sequencer/ ${CMAKE_CURRENT_SOURCE_DIR}/ServerTest/ ${INCLUDES} ${MATCHING_ENGINE_INCLUDES})

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../CMakeModules)

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

set(CMAKE_BUILD_TYPE Debug)

include(CodeCoverage)

APPEND_COVERAGE_COMPILER_FLAGS()



link_directories("/usr/lib/x86_64-linux-gnu/")

set(COVERAGE_LCOV_EXCLUDES '/usr/include/*' '/usr/local/*' '8/*')

file(GLOB_RECURSE sources_test ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

add_compile_definitions(SBC TEST BOOST_LOG_DYN_LINK)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -g -DDEBUG")

add_executable(MatchingEngine_Test ${sources_test} )

SETUP_TARGET_FOR_COVERAGE_LCOV(NAME MatchingEngine_Test_COV
                              EXECUTABLE MatchingEngine_Test
                              DEPENDENCIES MatchingEngine
                            )

target_include_directories(MatchingEngine_Test PUBLIC ${Test_Include} )

target_link_libraries(MatchingEngine_Test PUBLIC
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
   gtest
   gcov 
   boost_log
   boost_log_setup
   explain
)

set(COVERAGE_EXCLUDES
   '/usr/*'
    '${CMAKE_BINARY_DIR}/googletest-src/googletest/include/gtest/*'
    '${CMAKE_BINARY_DIR}/googletest-src/googletest/src/*'
    '${CMAKE_BINARY_DIR}/googletest-src/googlemock/include/gmock/*'
    '${CMAKE_BINARY_DIR}/googletest-src/googlemock/src/*'
)

add_test(
  NAME MatchingEngine_Tests
  COMMAND MatchingEngine_Test
)


install(TARGETS MatchingEngine_Test CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/test/)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/MatchingEngine_Test_COV DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/test/Coverage)

install(DIRECTORY MatchingEngine_Test/ressource CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/test/ressource)

