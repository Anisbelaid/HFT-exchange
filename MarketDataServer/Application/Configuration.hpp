#ifndef MARKETDATA_CONFIGURATION_HPP
#define MARKETDATA_CONFIGURATION_HPP

#include <Application/Defines.hpp>
#include <skeleton/Configuration.hpp>

namespace marketdata {
    
    /**
    *   a class helper to get envirement variable  
    *
    */
    class Configuration : public core::Configuration {
        private :
            /// Market Data Server Address
            std::string mReferentialPath;
            /// Market Data Server Address
            std::string mMarKetDataServerAddress;
            /// Market Data Server Address
            std::string mMarKetDataServerInterface;
            ///Market Data Server Port
            uint16_t    mMarketDataServerPort;

            ///Matching Engine connection Interface
            std::string mMatchingEngineInterface;
            ///Matching Engine Address
            std::string mMatchingEngineAddress;
            ///Matching Engine Port
            std::uint16_t mMatchingEnginePort;

            ///Logging Server Address
            std::string mLogginServerAddress;
            ///Logging Server Port
            std::uint16_t mLogginServerPort;
            ///Logging Server Interface
            std::string mLogginServerInterface;

        public :
            /**
            * Default Contructor.
            */
            Configuration(std::string aConFolder,std::string aFileName) ;
            
            /**
            * @brief Load the parameters.
            */
            bool load () ;

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
             * @brief Get the Matching Engine Address
             * 
             * @return std::string 
             */
            std::string& getMatchingEngineAddress (){
                return mMatchingEngineAddress;
            }
            
            /**
             * @brief Get the matching engine port 
             * 
             * @return uint16_t 
             */
            uint16_t& getMatchingEnginePort (){
                return mMatchingEnginePort;
            }
            std::string& getMatchingEngineInterface(){
                return mMatchingEngineInterface;
            }
            
            /**
             * @brief Get the Referential Path object
             * 
             * @return std::string& 
             */
            std::string & getReferentialPath (){
                return mReferentialPath;
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
            * Default Destructor.
            */
            virtual ~Configuration () = default;
            
    };
    
    
}

marketdata::Configuration::Configuration(std::string aConfFolder,std::string aFileName):core::Configuration(aConfFolder,aFileName){
}

bool marketdata::Configuration::load (){

    bool lResult = true;

    lResult &= readMandatoryParameter<std::string >("MARKET_DATA", "UDPADDRESS", mMarKetDataServerAddress);
    lResult &= readMandatoryParameter<uint16_t>("MARKET_DATA","UDPPORT", mMarketDataServerPort);
    lResult &= readMandatoryParameter<std::string >("MARKET_DATA","INTERFACE", mMarKetDataServerInterface);
    lResult &= readMandatoryParameter<std::string >("MARKET_DATA","REFERENTIAL", mReferentialPath);
    lResult &= readMandatoryParameter<std::string >("MATCHING_ENGINE","TCPADDRESS", mMatchingEngineAddress);
    lResult &= readMandatoryParameter<uint16_t>("MATCHING_ENGINE","TCPPORT", mMatchingEnginePort);
    lResult &= readMandatoryParameter<std::string >("MATCHING_ENGINE","INTERFACE", mMatchingEngineInterface);
    readMandatoryParameter<std::string >("LOGGER","TCPADDRESS", mLogginServerAddress);
    readMandatoryParameter<uint16_t>("LOGGER","TCPPORT", mLogginServerPort);
    readMandatoryParameter<std::string >("LOGGER","INTERFACE", mLogginServerInterface);

    return lResult;
}
#endif 



