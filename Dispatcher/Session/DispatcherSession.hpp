
#ifndef _TRADING_SESSION_HPP_
#define _TRADING_SESSION_HPP_

#include <allocator/LinearAllocator.hpp>
#include <skeleton/Session.hpp>
#include <thread/AsyncTask.hpp>

#include <sequencer/Sequencer.hpp>
#include <skeleton/Session.hpp>


namespace dispatcher {

    /**
    * Implementation of the TCPConnection class to add functionnal layer  
    */
    class DispatcherSession : public core::TCPServerSession<> {

        using TCPSession = core::TCPServerSession<>;
        
        public :
            
            /**
            * @brief Create a new session.
            * @param aConnectionId a Connection Id.
            * @param aSockId 
            * @param aContext 
            * @return Constuctor
            */
            DispatcherSession (int aSockId,uint64_t aSessionName) : TCPSession(aSockId,aSessionName){
            };

            void AddSessionName (void*aMessage)
            {
                trading::Header *lHeader = ((trading::Header*)aMessage);
                lHeader->PARENTSESSIONID       = Session::mSessionName;
            }

            void TreatNewOrder (void*aMessage) {
               INFO(Dispatcher,"Dispatcher Session ::TreatNewOrder New order received ");
               AddSessionName(aMessage);
            };

            void TreatNewCancel (void*aMessage) {
               INFO(Dispatcher,"Dispatcher Session ::TreatNewCancel New cancel received ");
               AddSessionName(aMessage);
            };
            
            void TreatInformation (void*aMessage) {
                INFO(Dispatcher,"Dispatcher Session ::TreatInformation New information received ");
                intern::Information *lHeader = ((intern::Information*)aMessage);
                lHeader->PARENTSESSIONID       = Session::mSessionName;
            }

            void TreatApproved (void*aMessage)
            {
                INFO(Dispatcher,"Dispatcher Session ::TreatApproved New cancel received ");
                AddSessionName(aMessage);
            }

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Dispatcher,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(Dispatcher,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(Dispatcher,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(Dispatcher,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(Dispatcher,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Dispatcher,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Dispatcher,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Dispatcher,"::onCloseThread "<<aMessage);
            };
            
    };

}

#endif


