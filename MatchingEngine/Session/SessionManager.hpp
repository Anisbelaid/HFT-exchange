 #ifndef _MATCHING_SESSION_MANAGER_HPP
 #define _MATCHING_SESSION_MANAGER_HPP

#include <sequencer/Sequencer.hpp>
#include <server/TCPServer.hpp>
#include <logger/LoggerClient.hpp>
#include <skeleton/SessionManager.hpp>
#include <skeleton/InstrumentManager.hpp>

#include <Session/TradingServer.hpp>
#include <Session/TradingSession.hpp>
#include <Session/MulticastPublisher.hpp>

namespace matching {

    class SessionManager : public core::SessionManager
    {
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
                matching::Configuration *lConf = dynamic_cast<matching::Configuration*>(mConfiguration);
                TradingServer* lServer = new TradingServer(0,lConf->getMatchingEngineAddress(),lConf->getMatchingEnginePort());
                return lServer;
            }

            /**
             * @brief initlize the singleton
             * 
             */
            bool InitializeClientConnections (){
                matching::Configuration *lConf = dynamic_cast<matching::Configuration*>(mConfiguration); 
                std::string lAddress     = lConf->getMarKetDataServerAddress() ;
                uint16_t    lPort        = lConf->getMarketDataServerPort();
                std::string lInterface   = lConf->getMarKetDataServerInterface(); 
                createMarketDataClient(lAddress,lPort,lInterface); 
                return true;   
            }

            /**
             * @brief initlize the singleton
             * 
             */
            core::LoggerSession * InitializeLoggerClient () override { 
                matching::Configuration *lConf = dynamic_cast<matching::Configuration*>(mConfiguration); 
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
                INFO(Matching,"::SessionManagerEvent Server Session created Id <"<<lSessionId<<"> hash for <"<<lSessionName<<">");
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
                INFO(Matching,"::SessionManagerEvent Logger session created Id <"<<lSessionName<<"> hash for <"<<lSessionNameMatching<<">");
                core::LoggerSession * lSession =  new core::LoggerSession (aAddress,aPort,aInterface,lSessionName);
                lSession->registerObserver (this);
                mTCPClientSession.insert (std::make_pair(lSessionName,lSession));
                return lSession;
            } 

            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual void createMarketDataClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                uint64_t lSessionName = std::hash<std::string>{}(std::string(cTCPSESSIONCLIENTTAG).append( "MarketData"));
                mMarketDataClientSession =  new MarketDataPublisher (aAddress,aPort,aInterface,lSessionName);
                mMarketDataClientSession->registerObserver (this);
                if (mMarketDataClientSession->prepare())
                    mMarketDataClientSession->open("MarketData Client");
                mTCPClientSession.insert (std::make_pair(lSessionName,mMarketDataClientSession));
            } 


    };
}

 #endif