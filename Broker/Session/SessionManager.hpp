 #ifndef _BROKER_SESSION_MANAGER_HPP
 #define _BROKER_SESSION_MANAGER_HPP

#include <skeleton/SessionManager.hpp>

#include <Session/BrokerSession.hpp>

#include <Session/MarketDataServerSession.hpp>

namespace broker {

    class SessionManager : public core::SessionManager
    {
        private :        
            broker::Configuration *mConfiguration;
            
        public :
            /**
             * @brief Construct a new Session Manager object
             * 
             * @param aConf 
             */
            explicit SessionManager(core::Configuration * aConf):core::SessionManager (aConf){
                mConfiguration = dynamic_cast<broker::Configuration*>(aConf);
            }

            /**
             * @brief Destroy the Session Manager object
             * 
             */
            ~SessionManager() = default;
            
            /**
             * @brief initlize the singleton
             * 
             */
            core::IServer* InitializeMainServer (){
               return nullptr;
            }

            /**
             * @brief initlize the singleton
             * 
             */
            bool InitializeClientConnections (){
                std::vector <BrokerSection*> lConf= mConfiguration->getBrokersConfiguration ();
                for (auto lElement : lConf)
                    createBrokerSession(lElement);
                broker::Configuration *lConfig = dynamic_cast<broker::Configuration*>(mConfiguration);  
                std::string lAddress   = lConfig->getMarKetDataServerAddress() ;
                uint64_t    lPort      = lConfig->getMarketDataServerPort();
                std::string lInterface = lConfig->getMarKetDataServerInterface(); 
                createMarketDataClient(lAddress,lPort,lInterface); 

                return true;
            }

            void createBrokerSession (BrokerSection * aSection){
                std::string lSessionNameBroker = std::string(cTCPSESSIONCLIENTTAG) + std::to_string (aSection->mIndex);
                uint64_t lSessionName = std::hash<std::string>{}(lSessionNameBroker);
                INFO(Broker,"::SessionManagerEvent Broker session created Id <"<<lSessionName<<"> hash for <"<<lSessionNameBroker<<">");
                BrokerSession * lSession =  new BrokerSession (aSection->mAddress,aSection->mPort,aSection->mInterface,lSessionName,aSection->mBrokerName,"TEST");
                lSession->registerObserver(this);
                mInactivityTimer.pushRoutine(lSession);
                mTCPClientSession.insert (std::make_pair(lSessionName,lSession));
            }

            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual void createMarketDataClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                uint64_t lSessionName = std::hash<std::string>{}(std::string(cUDPSESSIONCLIENTTAG).append("MarketData"));
                mFeedSession =  new MarketDataClient (aInterface,aAddress,aPort,lSessionName);
                mFeedSession->registerObserver(this);
                if (mFeedSession->prepare())
                    mFeedSession->open("MarketData Client");
            } 

            virtual core::LoggerSession * InitializeLoggerClient () {return nullptr;}

            virtual core::Session* createNewTCPServerSesion (int) {return nullptr;};

    };
}

 #endif