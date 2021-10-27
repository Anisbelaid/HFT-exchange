cmake_minimum_required(VERSION 3.10.3)

project(Broker)          

set (Client_Includes ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/ )

file(GLOB_RECURSE sources_broker ${CMAKE_CURRENT_SOURCE_DIR}/Application/*.cpp)

add_compile_definitions(SBC LOG BOOST_LOG_DYN_LINK)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -g -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -std=c++2a -O3 -DNDEBUG")

add_executable(Broker ${sources_broker} )

target_include_directories(Broker PUBLIC ${Client_Includes} ${GTKMM_INCLUDE_DIRS})

target_link_libraries(Broker PUBLIC
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

install(TARGETS Broker CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Broker/bin/)

install(TARGETS Broker CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Broker/bin/)

install(FILES Broker.ini 
              logger.ini  
              UserInterface/css/Connect.css
              UserInterface/css/MainWindow.css 
              UserInterface/css/Approval.css
              UserInterface/css/Basket.css 
              UserInterface/css/EntryBox.css 
              UserInterface/css/MarketWatch.css 
              UserInterface/css/OrderBottle.css 
              UserInterface/css/Trades.css  CONFIGURATIONS Debug RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Broker/config/)

install(FILES Broker.ini 
              logger.ini
              UserInterface/css/Connect.css  
              UserInterface/css/MainWindow.css 
              UserInterface/css/Approval.css
              UserInterface/css/Basket.css 
              UserInterface/css/EntryBox.css 
              UserInterface/css/MarketWatch.css 
              UserInterface/css/OrderBottle.css 
              UserInterface/css/Trades.css  CONFIGURATIONS Release RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Broker/config/)

install(FILES startBroker.sh stopBroker.sh env.sh CONFIGURATIONS Debug PERMISSIONS OWNER_EXECUTE DESTINATION ${CMAKE_INSTALL_PREFIX}/Debug/Broker/bin/)

install(FILES startBroker.sh stopBroker.sh env.sh CONFIGURATIONS Release PERMISSIONS OWNER_EXECUTE DESTINATION ${CMAKE_INSTALL_PREFIX}/Release/Broker/bin/)

