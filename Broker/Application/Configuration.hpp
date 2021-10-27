
#ifndef BROKER_CONFIGURATION_HPP
#define BROKER_CONFIGURATION_HPP

#include <skeleton/Configuration.hpp>

namespace broker {
    

    struct BrokerSection {
        uint16_t        mIndex;
        std::string     mInterface{""};
        std::string     mAddress{""};
        uint16_t        mPort{0};
        std::string     mBrokerName{""};
        std::string     mBasket{""};
        std::string     mType{""};
        
    };
    /**
    *   a class helper to get envirement variable  
    *
    */
    class Configuration : public core::Configuration {
            using BaseConf = core::Configuration; 
            typedef std::vector < BrokerSection*>   BrokerConf_;
            const std::string                       cBrokerSection; 
            BrokerConf_                             mBrokersConfig;
            /// Market Data Server Address
            std::string mMarKetDataServerAddress;
            /// Market Data Server Address
            std::string mMarKetDataServerInterface;
            ///Market Data Server Port
            uint16_t    mMarketDataServerPort;
        public :
            /**
            * Default Contructor.
            */
            explicit Configuration(const std::string& aConFolder,const std::string& aFileName):BaseConf(aConFolder,aFileName),cBrokerSection("Broker."){
            }
            
            ~Configuration () = default ;
            /**
             * @brief 
             * 
             * @return true if configuration loading success 
             * @return false if configuration loading fails
             */
            bool load() {
                int lIndex = 1;
                std::string lBrokerSection = cBrokerSection+std::to_string(lIndex);
                bool lResult = true;
                lResult &= readMandatoryParameter<std::string >("MARKET_DATA", "UDPADDRESS", mMarKetDataServerAddress);
                lResult &= readMandatoryParameter<uint16_t>("MARKET_DATA","UDPPORT", mMarketDataServerPort);
                lResult &= readMandatoryParameter<std::string >("MARKET_DATA","INTERFACE", mMarKetDataServerInterface);
                while (HasSection (lBrokerSection))
                {
                    BrokerSection * lSection = new BrokerSection();
                    lResult &=readMandatoryParameter<std::string >(lBrokerSection,"BASKET", lSection->mBasket);
                    lResult &=readMandatoryParameter<std::string >(lBrokerSection,"TCPADDRESS", lSection->mAddress);
                    lResult &=readMandatoryParameter<uint16_t>(lBrokerSection,"TCPPORT", lSection->mPort);
                    lResult &=readMandatoryParameter<std::string >(lBrokerSection,"INTERFACE", lSection->mInterface);
                    lResult &=readMandatoryParameter<std::string >(lBrokerSection,"BROKERNAME", lSection->mBrokerName);
                    lResult &=readMandatoryParameter<std::string >(lBrokerSection,"TYPE", lSection->mType);
                    lSection->mIndex = lIndex;
                    mBrokersConfig.push_back(lSection);
                    lIndex++;
                    lBrokerSection = cBrokerSection+std::to_string(lIndex);
                    if (!lResult) 
                        break;
                }

                return lResult;
            }

            const BrokerConf_& getBrokersConfiguration (){return mBrokersConfig;}

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

            
            
    };
    
    
}


#endif 



