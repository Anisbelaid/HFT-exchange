 #ifndef _MATCHING_SESSION_MANAGER_HPP
 #define _MATCHING_SESSION_MANAGER_HPP

#include <sequencer/Sequencer.hpp>
#include <server/TCPServer.hpp>
#include <logger/LoggerClient.hpp>
#include <skeleton/SessionManager.hpp>
#include <skeleton/InstrumentManager.hpp>

#include <Application/Configuration.hpp>
#include <Session/MatchingEngineClient.hpp>
#include <Session/MatchingEngineSession.hpp>
#include <Session/MarketDataServer.hpp>

namespace marketdata {

    class SessionManager : public core::SessionManager
    {
        TradingServer       * mTradingServer {nullptr};
        MarketDataServer    * mMarketDataServer {nullptr};
        public :
            /**
             * @brief Construct a new Session Manager object
             * 
             * @param aConf 
             */
            explicit SessionManager(core::Configuration * aConf):core::SessionManager(aConf){}

            /**
             * @brief Destroy the Session Manager object
             * 
             */
            ~SessionManager(){};
            
            /**
             * @brief initlize the singleton
             * 
             */
            core::IServer* InitializeMainServer (){
                marketdata::Configuration *lConf = dynamic_cast<marketdata::Configuration*>(mConfiguration);
                mMarketDataServer =new MarketDataServer(lConf->getMarketDataServerPort());
                mInactivityTimer.pushRoutine(mMarketDataServer);
                return mMarketDataServer;
            }
            
            virtual void pushToMarketDataServer (void * NewClientaMessage , size_t aLength)
            {
                if (mMarketDataServer)
                    mMarketDataServer->push(NewClientaMessage,aLength);
            }
            /**
             * @brief initlize the singleton
             * 
             */
            bool InitializeClientConnections (){
                marketdata::Configuration *lConf = dynamic_cast<marketdata::Configuration*>(mConfiguration);
                mTradingServer =  new TradingServer(0,lConf->getMatchingEngineAddress(),lConf->getMatchingEnginePort());
                if (mTradingServer->setup())
                {
                    mTradingServer->start();
                    mTradingServer->registerObserver(this);
                    return true;
                }
                else
                {
                    return false;
                }   
            }

            /**
             * @brief initlize the singleton
             * 
             */
            core::LoggerSession * InitializeLoggerClient () override { 
                marketdata::Configuration *lConf = dynamic_cast<marketdata::Configuration*>(mConfiguration); 
                std::string lAddress     = lConf->getLoggerServerAddress() ;
                uint16_t     lPort       = lConf->getLoggerServerPort();
                std::string lInterface   = lConf->getLoggerServerInterface(); 
                return createLoggerClient(lAddress,lPort,lInterface);
            }

            /**
             * @brief create and add new TCP Session
             * 
             * @param aDescriptor 
             */
            core::Session* createNewTCPServerSesion (int aDescriptor){
                std::string lSessionName = std::to_string(aDescriptor)+cTCPSESSIONSERVERTAG ;
                uint64_t    lSessionId = std::hash<std::string>{}(lSessionName);
                INFO(MARKETDATA,"::SessionManager Event Server Session created Id <"<<lSessionId<<"> hash for <"<<lSessionName<<">");
                TradingSession * lSession =  new TradingSession (aDescriptor,lSessionId);
                lSession->registerObserver (this);
                notifyObservers (core::Event::Subscription,(size_t)lSessionId);
                lSession->start();
                mTCPServerSession.insert (std::make_pair(lSessionId,lSession));
                return lSession;
            }
            
            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual core::LoggerSession* createLoggerClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                std::string lSessionNameMatching = std::string(cTCPSESSIONCLIENTTAG) + "LOGGER";
                uint64_t    lSessionName = std::hash<std::string>{}(lSessionNameMatching);
                INFO(MARKETDATA,"::SessionManagerEvent Logger session created Id <"<<lSessionName<<"> hash for <"<<lSessionNameMatching<<">");
                core::LoggerSession * lSession =  new core::LoggerSession (aAddress,aPort,aInterface,lSessionName);
                mTCPClientSession.insert (std::make_pair(lSessionName,lSession));
                return lSession;
            } 

    };
}

 #endif