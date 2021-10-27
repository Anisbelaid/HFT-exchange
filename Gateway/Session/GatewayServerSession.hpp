#ifndef _GATEWAY_SESSION_HPP_
#define _GATEWAY_SESSION_HPP_


#include <allocator/LinearAllocator.hpp>
#include <skeleton/Session.hpp>
#include <skeleton/UserManager.hpp>
#include <thread/AsyncTask.hpp>

#include <sequencer/Sequencer.hpp>

#include <pattern/ObjectFactory.hpp>
#include <Trading/Logon.hpp>
#include <Trading/Reject.hpp>
#include <atomic>

namespace gateway {

    class GatewaySession : public core::TCPServerSession<1024*1024,core::LinearAllocator > {
        
        using TCPSession = core::TCPServerSession<1024*1024,core::LinearAllocator>;
        ///
        static std::atomic_uint64_t       mOrderId; 
        ///
        core::UserManager                 *mUserManager{nullptr};
        trading::LogonAccepeted            mLogonAccepted;
        trading::LogonRejected             mLogonRejected;
        public :

            /**
             * @brief Construct a new Gateway Session object
             * 
             * @param aSock 
             * @param aSessionName 
             */
            GatewaySession (int aSock,uint64_t aSessionName,uint64_t aServerId,core::UserManager * aUserManager) : TCPSession(aSock,aSessionName,aServerId),mUserManager(aUserManager)  {
                mLogonAccepted.TYPE='Q';
                mLogonRejected.TYPE='R';  
            };

            void AddSessionName (void*aMessage)
            {
                trading::Header *lHeader = ((trading::Header*)aMessage);
                lHeader->SESSIONID       = Session::mSessionName;
            }
             
            void AddOrderId (void *aMessage){
                mOrderId.fetch_add(1, std::memory_order_relaxed);
                trading::Header *lHeader = ((trading::Header*)aMessage);
                lHeader->ORDERID = mServerId+mOrderId.load();
            } 

            void AddReceptionTime (void *aMessage)
            {
                struct timespec lTime;
                core::gClockTime(CLOCK_REALTIME, &lTime);
                trading::Header *lHeader = ((trading::Header*)aMessage);
                lHeader->RECPTIONTIME=lTime.tv_sec*1000000000+lTime.tv_nsec;
            }

            void TreatApproved (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatApproved");
                AddSessionName(aMessage);
            };

            void TreatNewOrder (void* aMessage) {
                AddOrderId(aMessage);
                AddSessionName(aMessage);
                AddReceptionTime(aMessage);
                uint64_t lSTock =((trading::Order*)aMessage)->STOCK;
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatNewOrder");
            };
            
            void TreatNewCancel (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatNewCancel");
                AddSessionName(aMessage);
            };

            void TreatOrderRejected (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatOrderRejected");
            };

            void TreatExecution (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatExecution");           
            };

            void TreatOrderAccecpted (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatOrderAccecpted");
            };

            void TreatOrderCanceled (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatOrderCanceled");
            };

            void TreatInformation (void* aMessage) {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatInformation");
            };

            
            void TreatLogon (void*aMessage)
            {
                INFO(Gateway,"Gateway Server Session "<<mSessionName<<" ::TreatLogon");
                trading::Logon* lMessage = (trading::Logon*)aMessage;
                std::string lLogin   (lMessage->USER,strlen(lMessage->USER));
                std::string lPassword(lMessage->PASSWORD,strlen(lMessage->PASSWORD));
                core::User *lUser = mUserManager->Authentificate (lLogin,lPassword);
                if (lUser != nullptr)
                {
                    mUserId  = std::hash<std::string>{}(lLogin);
                    mLogged = true;
                    mMessageReceived = true;
                    struct timespec lTime;
                    core::gClockTime(CLOCK_REALTIME, &lTime);
                    mLogonAccepted.SENDINGTIME = lTime.tv_nsec;
                    submit ((void*)&mLogonAccepted,sizeof(trading::LogonAccepeted));
                    intern::Information* lEvent = (intern::Information*)mAllocator->Allocate(sizeof (intern::Information));
                    lEvent->TYPE='F';
                    lEvent->SENDINGTIME=lTime.tv_nsec;
                    strcpy (lEvent->USER,lLogin.c_str());
                    if (lUser->mType=="Retail")
                        lEvent->EVENTID=intern::EVENTS::ClientConnection;
                    else
                        lEvent->EVENTID=intern::EVENTS::BrokerConnection;
                    StructData * lStructReply = (StructData*) mAllocator->Allocate (sizeof (StructData));
                    lStructReply->mClientId = mSessionName;
                    lStructReply->mData = lEvent;
                    lEvent->SESSIONID = mSessionName;
                    lStructReply->mLength = sizeof(intern::Information);
                    notifyObservers(core::Event::ToDispatcher,lStructReply);
                }
                else
                {
                    struct timespec lTime;
                    core::gClockTime(CLOCK_REALTIME, &lTime);
                    mLogonRejected.SENDINGTIME = lTime.tv_nsec;
                    submit ((void*)&mLogonRejected,sizeof(trading::LogonRejected));
                    notifyObservers(core::Event::ClientDisconnection,Session::mSessionName);
                }
             }

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Gateway,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                DEBUGG(Gateway,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                WARN(Gateway,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                ERRORR(Gateway,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                TRACE(Gateway,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Gateway,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Gateway,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Gateway,"::onCloseThread "<<aMessage);
            };

    };

    std::atomic_uint64_t GatewaySession::mOrderId(0);
    
}



#endif


