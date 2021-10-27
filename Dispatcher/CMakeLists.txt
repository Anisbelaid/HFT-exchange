cmake_minimum_required(VERSION 3.10.3)

project(Dispatcher)          

set (PROJECT_VER "0")
set (PROJECT_VER_MAJOR "0")
set (PROJECT_VER_MINOR "0")
set (PTOJECT_VER_PATCH "0")

set (Dispatcher_Include ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ )

add_compile_definitions(SBC LOG BOOST_LOG_DYN_LINK)

file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/Application/*.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -g -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -O3 -DNDEBUG")

add_executable(Dispatcher ${sources} )

set_target_properties(Dispatcher PROPERTIES LINK_FLAGS_RELEASE -s)

target_include_directories(Dispatcher PUBLIC ${Dispatcher_Include} ${GTKMM_INCLUDE_DIRS})

target_link_libraries(Dispatcher PUBLIC
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

install(TARGETS Dispatcher  CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Dispatcher/bin/)

install(TARGETS Dispatcher  CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Dispatcher/bin/)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/startDispatcher.sh ${CMAKE_CURRENT_SOURCE_DIR}/stopDispatcher.sh ${CMAKE_CURRENT_SOURCE_DIR}/env.sh PERMISSIONS OWNER_EXECUTE CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Dispatcher/bin/)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/startDispatcher.sh ${CMAKE_CURRENT_SOURCE_DIR}/stopDispatcher.sh ${CMAKE_CURRENT_SOURCE_DIR}/env.sh PERMISSIONS OWNER_EXECUTE CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Dispatcher/bin/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dispatcher.ini ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Dispatcher/config/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dispatcher.ini  ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Dispatcher/config/)
