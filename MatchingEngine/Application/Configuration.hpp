#ifndef MATCHING_CONFIGURATION_HPP
#define MATCHING_CONFIGURATION_HPP

#include <Application/Defines.hpp>
#include <skeleton/Configuration.hpp>

namespace matching {
    
    /**
    *   @brief A class helper to get envirement variable  
    *
    */
    class Configuration : public core::Configuration {

        using BaseConf = core::Configuration;
        private :
            /// Market Data Server Address
            std::string mMarKetDataServerAddress;
            /// Market Data Server Address
            std::string mMarKetDataServerInterface;
            ///Market Data Server Port
            uint16_t    mMarketDataServerPort;
            ///Referential file path
            std::string mReferentialFilePath;
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
            Configuration(const std::string& aConFolder,const std::string& aFileName) ;
             
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

            /**
             * @brief Get the Mar Ket Data Server Interface object
             * 
             * @return std::string& 
             */
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
             * @brief Get the logging server address
             * 
             * @return std::string 
             */
            std::string & getLoggerServerAddress (){
                return mLogginServerAddress;
            }

            /**
             * @brief Get the Logger Server Interface object
             * 
             * @return std::string& 
             */
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
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessage) {
                INFO(Matching,aMessage);
            };
            
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessage) {
                DEBUGG(Matching,aMessage);
            };
            
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWaning (const char *aMessage) {
                WARN(Matching,aMessage);
            };
            
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessage) {
                ERRORR(Matching,aMessage);
            };

            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessage) {
                TRACE(Matching,aMessage);
            };

            /**
            * Default Destructor.
            */
            virtual ~Configuration () = default;
            
    };
    
    
}

matching::Configuration::Configuration(const std::string& aConfFolder,const std::string& aFileName):BaseConf(aConfFolder,aFileName){
}

bool matching::Configuration::load (){

    bool lResult = true;

    lResult &= readMandatoryParameter<std::string > ("MARKET_DATA", "TCPADDRESS", mMarKetDataServerAddress);
    lResult &= readMandatoryParameter<uint16_t>     ("MARKET_DATA","TCPPORT", mMarketDataServerPort);
    lResult &= readMandatoryParameter<std::string > ("MARKET_DATA","INTERFACE", mMarKetDataServerInterface);
    lResult &= readMandatoryParameter<std::string > ("MATCHING_ENGINE","TCPADDRESS", mMatchingEngineAddress);
    lResult &= readMandatoryParameter<uint16_t>     ("MATCHING_ENGINE","TCPPORT", mMatchingEnginePort);
    lResult &= readMandatoryParameter<std::string > ("MATCHING_ENGINE","INTERFACE", mMatchingEngineInterface);
    lResult &= readMandatoryParameter<std::string > ("MATCHING_ENGINE", "REFERENTIAL", mReferentialFilePath);
    readMandatoryParameter<std::string >            ("LOGGER","TCPADDRESS", mLogginServerAddress);
    readMandatoryParameter<uint16_t>                ("LOGGER","TCPPORT", mLogginServerPort);
    readMandatoryParameter<std::string >            ("LOGGER","INTERFACE", mLogginServerInterface);

    return lResult;
}



#endif 



