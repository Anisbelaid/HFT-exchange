#ifndef _MATCHING_ENGINE_CLIENT_HPP_
#define _MATCHING_ENGINE_CLIENT_HPP_


#include <Application/Defines.hpp>


#include <skeleton/Session.hpp>
#include <client/TCPClient.hpp>
#include <allocator/LinearAllocator.hpp>

namespace gateway {

    class DispatcherSession : public core::TCPClientSession<1024*1024,core::LinearAllocator>  {
            using Client = core::TCPClientSession<1024*1024,core::LinearAllocator>;
        public :

            DispatcherSession (const std::string& aAddress,const uint16_t aPort,const std::string& aInterface ,uint64_t aSessionName) :
            Client(aAddress,aPort,aInterface,aSessionName){
            }

            ~DispatcherSession () = default ;
            
            void TreatExecution (void *aTrade)
            {
                INFO(Gateway,"Dispatcher Session ::TreatExecution Session Id <"<<mSessionName<<">");
                StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
                lData->mData    = (void*) aTrade;
                lData->mLength  = sizeof(trading::Exec);
                notifyObservers(core::Event::FromGateway,lData);
            }

            void TreatRequestApproval (void*aMessage){
                INFO(Gateway,"Dispatcher Session ::TreatRequestApproval Session Id <"<<mSessionName<<">");
                StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
                lData->mData    = (void*) aMessage;
                lData->mLength  = sizeof(trading::RequestApproval);
                notifyObservers(core::Event::FromGateway,lData);
             }

            bool close () override {
                INFO(Gateway,"Dispatcher Session ::close Session Id <"<<mSessionName<<">");
                return Client::close ();
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
};



#endif

