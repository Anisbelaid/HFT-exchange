cmake_minimum_required(VERSION 3.10.3)

project(Lib_Test)          

set (LIB_Include ${INCLUDES} /usr/include/)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_compile_definitions(SBC TEST BOOST_LOG_DYN_LINK)

option(CodeCoverage "CodeCoverage" OFF)

set(CMAKE_CXX_FLAGS "-Wno-deprecated-register ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-Wno-deprecated-register -O0 -g -fprofile-arcs -ftest-coverage ${CMAKE_CXX_FLAGS_DEBUG}")

add_executable(Lib_Test ${CMAKE_CURRENT_SOURCE_DIR}/Entry.cpp )

target_include_directories(Lib_Test PUBLIC ${LIB_Include})

target_link_libraries(Lib_Test PUBLIC
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
   gtest
   pthread
   gcov
   boost_log
   boost_log_setup
   explain
)


