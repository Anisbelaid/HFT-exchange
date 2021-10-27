cmake_minimum_required(VERSION 3.10.3)

project(Persistence)

set (Persistence_Include ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/  )

file(GLOB_RECURSE sources_Persistence ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )

add_compile_definitions(SBC TEST BOOST_LOG_DYN_LINK)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -g -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -O3 -DNDEBUG")

add_executable(Persistence ${sources_Persistence} )

target_include_directories(Persistence PUBLIC ${Persistence_Include} )

target_link_libraries(Persistence PUBLIC
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
   boost_log    
   boost_log_setup
   explain
   dl
)

install(TARGETS Persistence  CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Persistence/bin/)

install(TARGETS Persistence  CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Persistence/bin/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini  CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Persistence/config/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${CMAKE_CURRENT_SOURCE_DIR}/logger.ini  CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Persistence/config/)
