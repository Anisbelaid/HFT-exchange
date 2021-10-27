#ifndef GATEWAY_CONFIGURATION_HPP
#define GATEWAY_CONFIGURATION_HPP

#include <cstdint>
#include <Application/Defines.hpp>
#include <skeleton/Configuration.hpp>

namespace gateway {
    
    /**
     * @brief An implmentation for configuration reader
     * 
     */
    class Configuration : public core::Configuration {

        using BaseConf = core::Configuration;
        private :
            /// GatewayId
            uint64_t mGatewayId;
            /// Gateway Port
            uint16_t mGatewayPort;
            ///Gateway Address
            std::string mGatewayAddress;
            /// Market Data Server Address
            std::string mMarKetDataServerAddress;
            /// Market Data Server Address
            std::string mMarKetDataServerInterface;
            ///Market Data Server Port
            uint16_t    mMarketDataServerPort;
            ///Referential file path
            std::string mReferentialFilePath;
            ///Matching Engine connection Interface
            std::string mDispatcherInterface;
            ///Matching Engine Address
            std::string mDispatcherAddress;
            ///Matching Engine Port
            std::uint16_t mDispatcherPort;
            ///Logging Server Address
            std::string mLogginServerAddress;
            ///Logging Server Port
            std::uint16_t mLogginServerPort;
            ///Logging Server Interface
            std::string mLogginServerInterface;
            ///Login to the server
            std::string mNodeAddress;
            ///Password to the server
            std::uint16_t mNodePort;
            ///
            std::string mNodeInterface;

        public :
            /**
            * @brief build a custom contructor.
            * @param aConfFolder a configuration folder path
            * @param aFileName a configuration filename
            */
            Configuration(const std::string& aConFolder,const std::string& aFileName) ;
            
            /**
            * @brief Load the parameters.
            */
            bool load () ;

            /**
             * @brief Get the gateway port 
             * 
             * @return uint16_t 
             */
            uint64_t& getGatewayId (){
                return mGatewayId;
            } 

            /**
             * @brief Get the gateway port 
             * 
             * @return uint16_t 
             */
            std::string& getGatewayAddress (){
                return mGatewayAddress;
            }

            /**
             * @brief Get the gateway port 
             * 
             * @return uint16_t 
             */
            uint16_t& getGatewayPort (){
                return mGatewayPort;
            } 
            
            /**
             * @brief get the market data server
             *
             * @return std::string 
             */ 
            std::string& getMarKetDataServerAddress (){
                return mMarKetDataServerAddress;
            }
            
            /**
             * @brief Get the market data server port 
             * 
             * @return uint16_t 
             */
            uint16_t& getMarketDataServerPort (){
                return mMarketDataServerPort;
            }

            std::string & getMarKetDataServerInterface ()
            {
                return mMarKetDataServerInterface;
            }
            /**
             * @brief Get the Referential file path 
             * 
             * @return std::string 
             */
            std::string& getReferentialFilePath (){
                return mReferentialFilePath;
            }
            
            
            /**
             * @brief Get the Matching Engine Address
             * 
             * @return std::string 
             */
            std::string& getDispatcherAddress (){
                return mDispatcherAddress;
            }
            
            /**
             * @brief Get the matching engine port 
             * 
             * @return uint16_t 
             */
            uint16_t& getDispatcherPort (){
                return mDispatcherPort;
            }
            std::string& getDispatcherInterface(){
                return mDispatcherInterface;
            }
            /**
             * @brief Get the logging server address
             * 
             * @return std::string 
             */
            std::string & getLoggerServerAddress (){
                return mLogginServerAddress;
            }
            std::string & getLoggerServerInterface (){
                return mLogginServerInterface;
            }
            /**
             * @brief Get the logging server port
             * 
             * @return uint16_t 
             */
            uint16_t getLoggerServerPort (){
                return mLogginServerPort;
            }

            /**
             * @brief 
             * 
             * @return uint16_t 
             */
            std::string& getNodeAddress (){
                return mNodeAddress;
            }

            /**
             * @brief 
             * 
             * @return uint16_t 
             */
            std::string& getNodeInterface (){
                return mNodeInterface;
            }

            /**
             * @brief 
             * 
             * @return uint16_t 
             */
            uint16_t getNodePort (){
                return mNodePort;
            }
            
            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessage) {
                INFO(Gateway,aMessage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessage) {
                DEBUGG(Gateway,aMessage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWaning (const char *aMessage) {
                WARN(Gateway,aMessage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessage) {
                ERRORR(Gateway,aMessage);
            };

            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessage) {
                TRACE(Gateway,aMessage);
            };

            /**
            * Default Destructor.
            */
            virtual ~Configuration () = default;
            
    };
    
    
}

gateway::Configuration::Configuration(const std::string& aConfFolder,const std::string& aFileName):BaseConf(aConfFolder,aFileName){
}

bool gateway::Configuration::load (){

    bool lResult = true;

    lResult &= readMandatoryParameter<uint64_t>("GATEWAY", "GATEWAY_ID", mGatewayId);
    lResult &= readMandatoryParameter<std::string>("GATEWAY", "TCPADDRESS", mGatewayAddress);
    lResult &= readMandatoryParameter<uint16_t>("GATEWAY", "TCPPORT", mGatewayPort);
    lResult &= readMandatoryParameter<std::string >("GATEWAY", "REFERENTIAL", mReferentialFilePath);
    lResult &= readMandatoryParameter<std::string >("MARKET_DATA", "UDPADDRESS", mMarKetDataServerAddress);
    lResult &= readMandatoryParameter<uint16_t>("MARKET_DATA","UDPPORT", mMarketDataServerPort);
    lResult &= readMandatoryParameter<std::string >("MARKET_DATA","INTERFACE", mMarKetDataServerInterface);
    lResult &= readMandatoryParameter<std::string >("DISPATCHER","TCPADDRESS", mDispatcherAddress);
    lResult &= readMandatoryParameter<uint16_t>("DISPATCHER","TCPPORT", mDispatcherPort);
    lResult &= readMandatoryParameter<std::string >("DISPATCHER","INTERFACE", mDispatcherInterface);
    lResult &= readMandatoryParameter<std::string >("NODE","TCPADDRESS", mNodeAddress);
    lResult &= readMandatoryParameter<uint16_t>("NODE","TCPPORT", mNodePort);
    lResult &= readMandatoryParameter<std::string >("NODE","INTERFACE", mNodeInterface);
    readMandatoryParameter<std::string >("LOGGER","TCPADDRESS", mLogginServerAddress);
    readMandatoryParameter<uint16_t>("LOGGER","TCPPORT", mLogginServerPort);
    readMandatoryParameter<std::string >("LOGGER","INTERFACE", mLogginServerInterface);

    return lResult;
}


#endif 



