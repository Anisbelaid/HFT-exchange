cmake_minimum_required(VERSION 3.10.3)

project(Gateway_Test)          

set (Test_Include /usr/include/ ${CMAKE_CURRENT_SOURCE_DIR}/ ${Gateway_Include})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_compile_definitions(SBC TEST BOOST_LOG_DYN_LINK)

option(CodeCoverage "CodeCoverage" OFF)

set(CMAKE_CXX_FLAGS "-Wno-deprecated-register ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-Wno-deprecated-register -O0 -g -fprofile-arcs -ftest-coverage ${CMAKE_CXX_FLAGS_DEBUG}")

add_executable(Gateway_Test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp )

target_include_directories(Gateway_Test PUBLIC ${Test_Include} )

target_link_libraries(Gateway_Test PUBLIC
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
   gtest
   pthread
   gcov
   boost_log
   boost_log_setup
   explain
)

#add_test(ConfigurationTest Gateway_Test)

#add_test(EnvirementTest Gateway_Test)

#set_tests_properties(ConfigurationTest PROPERTIES ENVIRONMENT "GATEWAY_TEST=/home/anis/Desktop/workSpace/repo/core/Gateway/Test")

#set_tests_properties(EnvirementTest PROPERTIES ENVIRONMENT "CONF_FOLDER=/home/anis/Desktop/workSpace/repo/core/Gateway/Test")

#if (CMAKE_BUILD_TYPE MATCHES Debug)
#        setup_target_for_coverage(${PROJECT_NAME}_coverage Gateway_Test coverage)
#        setup_target_for_coverage_cobertura(${PROJECT_NAME}_cobertura Gateway_Test coverage --gtest_output=xml:coverage.junit.xml)
#endif()

#install(TARGETS Gateway_Test CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Gateway/test/)

#install(TARGETS Gateway_Test CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Gateway/test/)

#install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage CONFIGURATIONS Debug DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Gateway/test/Coverage)

#install(DIRECTORY ressource CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Gateway/test/ressource)

#install(DIRECTORY ressource CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Gateway/test/ressource)
