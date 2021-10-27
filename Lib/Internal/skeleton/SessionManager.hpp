#ifndef _SESSION_MANAGER_HPP
#define _SESSION_MANAGER_HPP

#include <server/IServer.hpp>
#include <logger/LoggerClient.hpp>
#include <skeleton/Session.hpp>
#include <skeleton/Defines.hpp>
#include <pattern/Singleton.hpp>
#include <Application/Configuration.hpp>
#include <Trading/Order.hpp>
#include <map>
#include <mutex>
#include <iostream>

namespace core{
    /**
     * @brief a class dedicated to manage sessions all along the server
     * 
     */
    class SessionManager  : public Observer , public Subject {

        protected :
            ///
            typedef std::map <uint32_t,Session*>  SESSIONMAP_;
            ///
            const char *           cTCPSESSIONCLIENTTAG;
            ///
            const char *           cUDPSESSIONCLIENTTAG;
            ///
            const char *           cTCPSESSIONSERVERTAG;

            /// a vector containing a list of all TCP Server Sessions in the server
            SESSIONMAP_             mTCPServerSession;

            SESSIONMAP_             mTCPClientSession ; 

            Session*                mGatewayClientSession{nullptr};

            Session*                mMarketDataClientSession{nullptr};

            Session*                mNodeClientSession{nullptr};

            Session*                mDispatcherClientSession{nullptr};

            Session*                mMatchingEngineSession{nullptr};

            core::LoggerSession*    mLoggerClientSession{nullptr};

            Session*                mFeedSession{nullptr};

            /// This Timer will holds hearbeat timeouts and async operations
            CPUTimerChecker         mInactivityTimer;

            /// a pointer to the general configuration
            Configuration *         mConfiguration{nullptr};
            
            /// a Mutex to protect concurrent access to the access to the main reactor 
            std::mutex              mMutexMessage;
        public :

            /**
             * @brief Construct a new Session Manager object
             * 
             */
            explicit SessionManager (Configuration *aConf) : cTCPSESSIONCLIENTTAG("TCP_CLIENT_"),cUDPSESSIONCLIENTTAG("UDP_CLIENT_"),cTCPSESSIONSERVERTAG("TCP_SERVER_"),mConfiguration(aConf) {
                mInactivityTimer.calibrate();
            };

            /** 
             * @brief Destroy the Session Manager object
             * 
             */
            ~SessionManager() {
                mInactivityTimer.shutdown();
                mTCPServerSession.clear ();
                mTCPClientSession.clear();
            } ;
            /**
             * @brief find a Client tcp session by its index
             * 
             * @param aId 
             * @return Session* 
             */
            Session* GetTCPClientSessionById (std::string aId)
            {
               std::string lSessionNameBroker = std::string(cTCPSESSIONCLIENTTAG) + aId;
               uint64_t lSessionName = std::hash<std::string>{}(lSessionNameBroker);
               auto lIt=mTCPClientSession.find(lSessionName) ;
               if (lIt!=mTCPClientSession.end())
                return lIt->second;
               else
                nullptr;
            }

            std::vector<Session*> GetActiveTCPClientSession()
            {
               std::vector<Session*> lTemp;
               for (auto lIt : mTCPClientSession)
               {
                   lTemp.push_back (lIt.second);
               }
               return lTemp;
            }
            /**
             * @brief initlize the singleton
             * 
             */
            virtual IServer* InitializeMainServer () = 0;

            /**
             * @brief initlize the singleton
             * 
             */
            virtual bool InitializeClientConnections () = 0;

             /**
             * @brief initlize the singleton
             * 
             */
            virtual LoggerSession * InitializeLoggerClient () = 0;

            /**
             * @brief create and  new TCP Session
             * 
             * @param aDescriptor 
             */
            virtual Session* createNewTCPServerSesion (int) = 0;
            
            void closeTCPServerSession(uint64_t aSessionId){
                SESSIONMAP_::iterator lIt = mTCPServerSession.find (aSessionId);
                if (lIt != mTCPServerSession.end())
                {
                    lIt->second->close();
                }
            }

            void closeTCPClientSession(uint64_t aSessionId){
                SESSIONMAP_::iterator lIt = mTCPClientSession.find (aSessionId);
                if (lIt != mTCPClientSession.end())
                {
                    lIt->second->close();
                }
            }
            
            /**
             * @brief Get the Feed Session object
             * 
             * @return Session* 
             */
            Session * getFeedSession ()
            {
                return mFeedSession;
            }
            /**
             * @brief 
             * 
             */
            void closeAllSession (){

                for (auto lSession : mTCPServerSession)
                    lSession.second->close();

                for (auto lSession : mTCPClientSession)
                    lSession.second->close();

                if (mGatewayClientSession)
                    mGatewayClientSession->close();

                if (mMarketDataClientSession)
                    mMarketDataClientSession->close();

                if(mNodeClientSession)
                    mNodeClientSession->close();

                if (mDispatcherClientSession)
                    mDispatcherClientSession->close();

                /*if (mLoggerClientSession)
                    mLoggerClientSession->close();*/

            }

            /**
             * @brief 
             * 
             */
            void removeAllSessions () {
                for (auto lSession : mTCPServerSession)
                    delete lSession.second;

                if (mGatewayClientSession)
                    delete  mGatewayClientSession;

                if (mMarketDataClientSession)
                    delete mMarketDataClientSession;

                if(mNodeClientSession)
                    delete  mNodeClientSession;

                if (mDispatcherClientSession)
                    delete  mDispatcherClientSession;

                if (mLoggerClientSession)
                    delete mLoggerClientSession;
            }

            /**
             * @brief 
             * 
             * @param aName 
             */
            bool pushReplyToTCPSeverClient (uint64_t aClientId, void * NewClientaMessage , size_t aLength) {
               SESSIONMAP_::iterator lIt =  mTCPServerSession.find (aClientId);
               if (lIt !=  mTCPServerSession.end())
               {
                    TCPServerSession<> *lSession = dynamic_cast<TCPServerSession<>*>(lIt->second);
                    lSession->push (NewClientaMessage,aLength);
                    return true;
               }
               return false;
            }

            /**
             * @brief 
             * 
             * @param aName 
             */
            void  pushToNode (void * NewClientaMessage , size_t aLength) {
                if (mNodeClientSession->IsConnected())
                    mNodeClientSession->submit (NewClientaMessage,aLength);
            }

            /**
             * @brief 
             * 
             * @param aName 
             */
            void  pushToGateway (void * NewClientaMessage , size_t aLength) {
                if (mGatewayClientSession->IsConnected())
                    mGatewayClientSession->submit (NewClientaMessage,aLength);
            }
            /**
             * @brief 
             * 
             */
            virtual void pushToMarketDataServer (void * NewClientaMessage , size_t aLength)
            {

            }

            /**
             * @brief 
             * 
             * @param aName 
             */
            void  pushToMarketData(void * NewClientaMessage , size_t aLength) {
                if (mMarketDataClientSession->IsConnected())
                    mMarketDataClientSession->submit (NewClientaMessage,aLength);
            }
            
            
            /**
             * @brief 
             * 
             * @param aName 
             */
            void  pushToMatchingEngine(void * NewClientaMessage , size_t aLength) {
                if (mMatchingEngineSession->IsConnected())
                    mMatchingEngineSession->submit (NewClientaMessage,aLength);
            }
            /**
             * @brief 
             * 
             * @param aName 
             */
            void  pushToDispatcher(void * NewClientaMessage , size_t aLength) {
                if (mDispatcherClientSession->IsConnected())
                    mDispatcherClientSession->submit (NewClientaMessage,aLength);
            }

            /**
             * @brief 
             * 
             * @param aName 
             */
            void  pushToLogger(void * NewClientaMessage , size_t aLength) {
            }
            
            /**
            * Update the state of this observer
            */
            virtual void update(core::Event aEvent,  StructData * aData = nullptr) {
                switch (aEvent)
                {
                    case (core::Event::FromServerSession): 
                    {
                        /// multithreaded call to the reactor
                        mMutexMessage.lock();
                            notifyObservers (core::Event::FromServerSession,aData);
                        mMutexMessage.unlock();
                    }
                    break;
                    case (core::Event::FromMatchingEngine):
                    {
                        trading::Header* lHeader = (trading::Header *)aData->mData;
                        pushReplyToTCPSeverClient(lHeader->MATCHINGSESSIONID,aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::FromDispatcher):
                    {
                        trading::Header* lHeader = (trading::Header *)aData->mData;
                        pushReplyToTCPSeverClient(lHeader->PARENTSESSIONID,aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::FromGateway):
                    {
                        trading::Header* lHeader = (trading::Header *)aData->mData;
                        pushReplyToTCPSeverClient(lHeader->SESSIONID,aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::ToMatchingEngine):
                    {
                        pushToMatchingEngine(aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::ToDispatcher):
                    {
                        pushToDispatcher(aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::ToGateway):
                    {
                        pushToGateway(aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::ToMarketData):
                    {
                        pushToMarketData(aData->mData,aData->mLength);;
                    }
                    break;
                    case (core::Event::ToLogger):
                    {
                        pushToLogger (aData->mData,aData->mLength);
                    }
                    break;
                    case (core::Event::Broadcast):
                    {
                        pushToMarketDataServer (aData->mData,aData->mLength);
                    }
                    break;
                    default:
                    break;
                }
            };

            virtual void update(Event,void* aData = 0) {};
            /**
            * Update the state of this observer
            */
            virtual void update(core::Event aEvent,std::size_t  aData = 0 ) {
                if (aEvent == core::Event::ServerShutdown){
                    closeAllSession();
                    //removeServerSessions();
                    //removeClientSessions();
                }
                else if (aEvent == core::Event::StartServer){
                    //startAllClientSession ();
                }
                else if (aEvent == core::Event::NewClient)
                {
                    Session * lSession = createNewTCPServerSesion(aData);
                    mInactivityTimer.pushRoutine(lSession);
                }
                else if (aEvent == core::Event::ClientDisconnection)
                {
                    closeTCPServerSession(aData);
                    //removeTCPServerSession(aData);
                }
                else if (aEvent == core::Event::ServerDisconnection){
                    closeTCPClientSession(aData);
                    //removeTCPClientSession(aData);
                }
            };


    };
}

#endif // !_SESSION_MANAGER_HPP