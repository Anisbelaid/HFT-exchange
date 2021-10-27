cmake_minimum_required(VERSION 3.10.3)

project(MatchingEngine_Bench)          

include(CTest)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

find_package(GoogleBenchmark  REQUIRED)

set (Bench_Include /usr/include/ ${CMAKE_CURRENT_SOURCE_DIR}/MatchingEngine_Bench/ ${CMAKE_CURRENT_SOURCE_DIR}/Sequencer_Bench/ ${CMAKE_CURRENT_SOURCE_DIR}/MatchingEngine_Bench/ ${INCLUDES} ${MATCHING_ENGINE_INCLUDES})

link_directories("/usr/lib/x86_64-linux-gnu/")

file(GLOB_RECURSE sources_Bench ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_compile_definitions(SBC  BOOST_LOG_DYN_LINK)

add_executable(MatchingEngine_Bench ${sources_Bench} )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -O3  -DNDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++2a -O3 -DNDEBUG")

target_include_directories(MatchingEngine_Bench PUBLIC ${Bench_Include} )

set_target_properties(MatchingEngine_Bench PROPERTIES LINK_FLAGS_RELEASE -s)

target_link_libraries(MatchingEngine_Bench PUBLIC
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
   ${benchmark_LIBRARIES} 
   pthread
   boost_log
   boost_log_setup
   explain   
)

install(TARGETS MatchingEngine_Bench  CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/benchmark/)

install(TARGETS MatchingEngine_Bench  CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/MatchingEngine/benchmark/)
