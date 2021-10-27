
#ifndef _PERSISTANCE_SESSION_HPP_
#define _PERSISTANCE_SESSION_HPP_

#include <TCPConnection.hpp>
#include <Application/Defines.hpp>
#include <EncoderDecoder.hpp>
#include <Trading/Order.hpp>
#include <Trading/Trade.hpp>
#include <Trading/Reject.hpp>
#include <Trading/OrderReply.hpp>
#include <Feed/Order.hpp>
#include <Feed/Trade.hpp>
#include <Feed/Instrument.hpp>

namespace server {
    
   
    /**
    * Implementation of the TCPConnection class to add functionnal layer  
    */
    class PersistenceSession : public core::TCPConnection<> , public Decoder<>
    {
            
            ///  Connection an alias to the TCPConnection
            using Connection = core::TCPConnection<>;
        
            ///  Connection an alias to the ThreadPool context
            using Pool = core::ThreadPool;
            
            
        public :
            
            /**
            * Create a new session.
            * @param aConnectionId a Connection Id.
            * @param aSock    
            * @param aContext  
            */
            PersistenceSession (int aConnectionId,int aSock,IContext& aContext) ;
            
        
            /**
            * @brief Ensure that we received the full message to process.
            *
            * @param aConnectionId The ConnectionId.
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return StructData * pointer to a full message
            */
            StructData * ensure (int aConnectionId,void * aData,size_t aLength) override {
                return Decoder<>::ensure(aConnectionId,aData,aLength);
            }; 
            
            
            /**
            * This method should process the new message to a peer.
            *
            * @param aId
            * @param aData The final data to process.
            */
            void processMessage (int aId,StructData * aData) override{
                 Decoder<>::Decode(aData->mData,aData->mLength);
            }; 
            
            void TreatAddOrder(void * aMessage) override;
            
            void TreatTrade(void * aMessage) override;
            
            void TreatExecution(void * aMessage) override;
            
            void TreatNewOrder(void * aMessage) override;
            
            /**
            * destroy the PersistenceSession.
            * @return Constuctor
            */
            ~PersistenceSession () = default;
    };
    
    PersistenceSession::PersistenceSession(int aConnectionid,int aSock,IContext& aContext) : Connection(aConnectionid,aSock,aContext){
    };
    
    void PersistenceSession::TreatAddOrder(void * aMessage) {
        feed::Order * lOrder = (feed::Order *)aMessage;
        INFO(PERSISTANCE,"New Message "<<*lOrder );
    };
    
    void PersistenceSession::TreatExecution(void * aMessage) {
        trading::Exec * lOrder = (trading::Exec*)aMessage;
        INFO(PERSISTANCE,"New Message "<<*lOrder );
    };
    
    void PersistenceSession::TreatTrade(void * aMessage) {
        feed::Trade * lOrder = (feed::Trade*)aMessage;
        INFO(PERSISTANCE,"New Message "<<*lOrder );
    };
    
    void PersistenceSession::TreatNewOrder(void * aMessage) {
        trading::Order * lOrder = (trading::Order*)aMessage;
        INFO(PERSISTANCE,"New Message "<<*lOrder );
    };
    

}

#endif


