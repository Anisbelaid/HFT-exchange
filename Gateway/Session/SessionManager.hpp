 #ifndef _GATEWAY_SESSION_MANAGER_HPP
 #define _GATEWAY_SESSION_MANAGER_HPP

#include <sequencer/Sequencer.hpp>
#include <server/TCPServer.hpp>
#include <logger/LoggerClient.hpp>
#include <skeleton/SessionManager.hpp>
#include <logger/LoggerClient.hpp>


#include <Session/GatewayServer.hpp>
#include <Session/GatewayServerSession.hpp>
#include <Session/DispatcherSession.hpp>
#include <Session/MarketDataServerSession.hpp>


namespace gateway {

    class SessionManager : public core::SessionManager
    {
        private :
            uint64_t                      mServerId{0};
            core::UserManager             mUserManager;       
        public :
            /**
             * @brief Construct a new Session Manager object
             * 
             * @param aConf 
             */
            explicit SessionManager(core::Configuration * aConf):core::SessionManager(aConf){
                mUserManager.Initialize(aConf);
            }

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
                gateway::Configuration *lConf = dynamic_cast<gateway::Configuration*>(mConfiguration);
                GatewayServer* lServer = new GatewayServer(lConf->getGatewayId(),lConf->getGatewayAddress(),lConf->getGatewayPort());
                mServerId              = lServer->getServerId();
                return lServer;
            }

            /**
             * @brief initlize the singleton
             * 
             */
            bool InitializeClientConnections (){
                gateway::Configuration *lConf = dynamic_cast<gateway::Configuration*>(mConfiguration); 
                std::string lAddress     = lConf->getDispatcherAddress() ;
                uint16_t    lPort        = lConf->getDispatcherPort();
                std::string lInterface   = lConf->getDispatcherInterface(); 
                createDispatcherClient(lAddress,lPort,lInterface);
                lAddress   = lConf->getMarKetDataServerAddress() ;
                lPort      = lConf->getMarketDataServerPort();
                lInterface = lConf->getMarKetDataServerInterface(); 
                createMarketDataClient(lAddress,lPort,lInterface); 
                return true;   
            }

            /**
             * @brief initlize the singleton
             * 
             */
            core::LoggerSession * InitializeLoggerClient () override { 
                gateway::Configuration *lConf = dynamic_cast<gateway::Configuration*>(mConfiguration); 
                std::string lAddress     = lConf->getLoggerServerAddress() ;
                uint16_t     lPort        = lConf->getLoggerServerPort();
                std::string lInterface   = lConf->getLoggerServerInterface(); 
                return createLoggerClient(lAddress,lPort,lInterface);
            }

            /**
             * @brief create and add new TCP Session
             * 
             * @param aDescriptor 
             */
            core::Session* createNewTCPServerSesion (int aDescriptor){
                std::string lSessionName = std::to_string(mServerId)+std::to_string(aDescriptor)+cTCPSESSIONSERVERTAG ;
                uint64_t    lSessionId = std::hash<std::string>{}(lSessionName);
                INFO(Gateway,"::SessionManagerEvent Server Session created Id <"<<lSessionId<<"> hash for <"<<lSessionName<<">");
                GatewaySession * lSession =  new GatewaySession (aDescriptor,lSessionId,mServerId,&mUserManager);
                lSession->registerObserver (this);
                notifyObservers (core::Event::Subscription,(size_t)lSessionId);
                lSession->open(lSessionName);
                mTCPServerSession.insert (std::make_pair(lSessionId,lSession));
                return lSession;
            }
            
            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual core::LoggerSession* createLoggerClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                uint64_t    lSessionName = std::hash<std::string>{}(std::string(cTCPSESSIONCLIENTTAG).append("LOGGER"));
                INFO(Gateway,"::SessionManagerEvent Logger session created Id <"<<lSessionName<<">");
                mLoggerClientSession =  new core::LoggerSession (aAddress,aPort,aInterface,lSessionName);
                mLoggerClientSession->registerObserver(this);
                mTCPClientSession.insert (std::make_pair(lSessionName,mLoggerClientSession));
                return mLoggerClientSession;
            } 

            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual void createDispatcherClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                uint64_t lSessionName = std::hash<std::string>{}(std::string(cTCPSESSIONCLIENTTAG).append("Dispatcher"));
                INFO(Gateway,"::SessionManagerEvent DispatcherSessionsession created Id <"<<lSessionName<<">");
                mDispatcherClientSession =  new DispatcherSession (aAddress,aPort,aInterface,lSessionName);
                mDispatcherClientSession->registerObserver(this);
                if (mDispatcherClientSession->prepare())
                    mDispatcherClientSession->open("Dispatcher Client");
                mTCPClientSession.insert (std::make_pair(lSessionName,mDispatcherClientSession));
            }


            /**
             * @brief create and add new Matching engine  Session
             * 
             */
            virtual void createMarketDataClient (const std::string &aAddress,uint16_t aPort,const std::string &aInterface){
                uint64_t lSessionName = std::hash<std::string>{}(std::string(cUDPSESSIONCLIENTTAG).append("MarketData"));
                mFeedSession =  new MarketDataClient (aInterface,aAddress,aPort,lSessionName);
                mFeedSession->registerObserver(this);
                //if (mFeedSession->prepare())
                //    mFeedSession->open("MarketData Client");
            } 
    };
}

 #endif