 #ifndef _MATCHING_SESSION_MANAGER_HPP
 #define _MATCHING_SESSION_MANAGER_HPP

#include <sequencer/Sequencer.hpp>
#include <server/TCPServer.hpp>
#include <logger/LoggerClient.hpp>
#include <skeleton/SessionManager.hpp>
#include <skeleton/InstrumentManager.hpp>

#include <Session/DispatcherServer.hpp>
#include <Session/DispatcherSession.hpp>
#include <Session/MatchingEnginePublisher.hpp>

namespace dispatcher {

    class SessionManager : public core::SessionManager
    {
        private :
            
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
                dispatcher::Configuration *lConf = dynamic_cast<dispatcher::Configuration*>(mConfiguration);
                DispatcherServer* lServer = new DispatcherServer(0,lConf->getDispatcherAddress(),lConf->getDispatcherPort());
                return lServer;
            }

            /**
             * @brief initlize the singleton
             * 
             */
            bool InitializeClientConnections (){
                dispatcher::Configuration *lConf = dynamic_cast<dispatcher::Configuration*>(mConfiguration); 
                std::string lAddress     = lConf->getMatchingEngineAddress() ;
                uint16_t    lPort        = lConf->getMatchingEnginePort();
                std::string lInterface   = lConf->getMatchingEngineInterface(); 
                createDispatcherClient(lAddress,lPort,lInterface); 
                return true;   
            }

            /**
             * @brief initlize the singleton
             * 
             */
            core::LoggerSession * InitializeLoggerClient () override { 
                dispatcher::Configuration *lConf = dynamic_cast<dispatcher::Configuration*>(mConfiguration); 
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
                std::string lSessionName = std::to_string(aDescriptor)+cTCPSESSIONSERVERTAG+"Dispatcher" ;
                uint64_t    lSessionId = std::hash<std::string>{}(lSessionName);
                INFO(Dispatcher,"::SessionManagerEvent Server Session created Id <"<<lSessionId<<"> hash for <"<<lSessionName<<">");
                DispatcherSession * lSession =  new DispatcherSession (aDescriptor,lSessionId);
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
                std::string lSessionNameDispatcher = std::string(cTCPSESSIONCLIENTTAG) + "LOGGER";
                uint64_t    lSessionName = std::hash<std::string>{}(lSessionNameDispatcher);
                INFO(Dispatcher,"::SessionManagerEvent Logger session created Id <"<<lSessionName<<"> hash for <"<<lSessionNameDispatcher<<">");
                core::LoggerSession * lSession =  new core::LoggerSession (aAddress,aPort,aInterface,lSessionName);
                lSession->registerObserver (this);
                mTCPClientSession.insert (std::make_pair(lSessionName,lSession));
                return lSession;
            } 

            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual void createDispatcherClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                uint64_t lSessionName = std::hash<std::string>{}(std::string(cTCPSESSIONCLIENTTAG).append( "MatchingNegine"));
                mMatchingEngineSession =  new MatchingEnginePublisher (aAddress,aPort,aInterface,lSessionName);
                mMatchingEngineSession->registerObserver (this);
                if (mMatchingEngineSession->prepare())
                    mMatchingEngineSession->open("MarketData Client");
                mTCPClientSession.insert (std::make_pair(lSessionName,mMatchingEngineSession));
            } 


    };
}

 #endif