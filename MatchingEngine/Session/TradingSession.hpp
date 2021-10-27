
#ifndef _TRADING_SESSION_HPP_
#define _TRADING_SESSION_HPP_

#include <allocator/LinearAllocator.hpp>
#include <skeleton/Session.hpp>
#include <thread/AsyncTask.hpp>

#include <sequencer/Sequencer.hpp>
#include <skeleton/Session.hpp>


namespace matching {

    /**
    * Implementation of the TCPConnection class to add functionnal layer  
    */
    class TradingSession : public core::TCPServerSession<> {

        using TCPSession = core::TCPServerSession<>;
        
        public :
            
            /**
            * @brief Create a new session.
            * @param aConnectionId a Connection Id.
            * @param aSockId 
            * @param aContext 
            * @return Constuctor
            */
            TradingSession (int aSockId,uint64_t aSessionName) : TCPSession(aSockId,aSessionName){
            };

            void AddSessionName (void*aMessage)
            {
                trading::Header *lHeader = ((trading::Header*)aMessage);
                lHeader->MATCHINGSESSIONID       = Session::mSessionName;
            }

            void TreatNewOrder (void*aMessage) {
               INFO(Matching,"Matching Server Session ::TreatNewOrder New order received ");
               AddSessionName(aMessage);
            };
            
            void TreatNewCancel (void*aMessage) {
                INFO(Matching,"Matching Server Session ::TreatNewCancel New cancel received ");
                AddSessionName(aMessage);
            };

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Matching,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                 DEBUGG(Matching,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                 WARN(Matching,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                 ERRORR(Matching,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                 TRACE(Matching,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Matching,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                 INFO(Matching,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Matching,"::onCloseThread "<<aMessage);
            };
            
    };

}

#endif


