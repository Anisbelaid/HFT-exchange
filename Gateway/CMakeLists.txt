cmake_minimum_required(VERSION 3.10.3)

project(Gateway)          

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)
set (VERSION_PATCH 1)
set (GATEWAY_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file( ./config.h.in ${CMAKE_CURRENT_SOURCE_DIR})

set (Gateway_Include ${CMAKE_CURRENT_SOURCE_DIR}/  ${INCLUDES})

file(GLOB_RECURSE sources_gateway ${CMAKE_CURRENT_SOURCE_DIR}/Application/*.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -g -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -O3 -DNDEBUG")

add_compile_definitions(SBC LOG BOOST_LOG_DYN_LINK)

add_executable(Gateway ${sources_gateway} )

set_target_properties(Gateway PROPERTIES LINK_FLAGS_RELEASE -s)


target_include_directories(Gateway PUBLIC ${Gateway_Include} ${GTKMM_INCLUDE_DIRS})

target_link_libraries(Gateway PUBLIC
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${GTKMM_LIBRARIES}
  boost_log    
  boost_log_setup
  boost_stacktrace_backtrace
  dl 
  backtrace
  explain
)

#enable_testing()
#add_subdirectory(Test)
#add_subdirectory(Benchmark)

install(TARGETS Gateway CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Gateway/bin/)

install(TARGETS Gateway CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Gateway/bin/)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/startGateway.sh ${CMAKE_CURRENT_SOURCE_DIR}/stopGateway.sh ${CMAKE_CURRENT_SOURCE_DIR}/env.sh  PERMISSIONS OWNER_EXECUTE CONFIGURATIONS Debug  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Gateway/bin/)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/startGateway.sh ${CMAKE_CURRENT_SOURCE_DIR}/stopGateway.sh ${CMAKE_CURRENT_SOURCE_DIR}/env.sh   PERMISSIONS OWNER_EXECUTE CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Gateway/bin/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/gateway.ini ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini   CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Gateway/config/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/gateway.ini  ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Gateway/config/)

