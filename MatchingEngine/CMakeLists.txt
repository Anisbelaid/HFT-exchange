cmake_minimum_required(VERSION 3.10.3)

project(MatchingEngine)          

set (PROJECT_VER "0")
set (PROJECT_VER_MAJOR "0")
set (PROJECT_VER_MINOR "0")
set (PTOJECT_VER_PATCH "0")

set (Matching_Include ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ )

add_compile_definitions(SBC LOG BOOST_LOG_DYN_LINK)

file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/Application/*.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -g -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -O3 -DNDEBUG")

add_executable(MatchingEngine ${sources} )

set_target_properties(MatchingEngine PROPERTIES LINK_FLAGS_RELEASE -s)

target_include_directories(MatchingEngine PUBLIC ${Matching_Include} ${GTKMM_INCLUDE_DIRS})

target_link_libraries(MatchingEngine PUBLIC
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

install(TARGETS MatchingEngine  CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/bin/)

install(TARGETS MatchingEngine  CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/MatchingEngine/bin/)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/startMatchingEngine.sh ${CMAKE_CURRENT_SOURCE_DIR}/stopMatchingEngine.sh ${CMAKE_CURRENT_SOURCE_DIR}/env.sh PERMISSIONS OWNER_EXECUTE CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/bin/)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/startMatchingEngine.sh ${CMAKE_CURRENT_SOURCE_DIR}/stopMatchingEngine.sh ${CMAKE_CURRENT_SOURCE_DIR}/env.sh PERMISSIONS OWNER_EXECUTE CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/MatchingEngine/bin/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/matching.ini ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/MatchingEngine/config/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/matching.ini  ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/MatchingEngine/config/)
