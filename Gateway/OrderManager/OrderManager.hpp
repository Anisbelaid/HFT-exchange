#ifndef  GATEWAY_MARKET_ORDER_HPP_
#define  GATEWAY_MARKET_ORDER_HPP_

#include <Application/Defines.hpp>
#include <OrderManager/OrderScanner.hpp>

#include <EncoderDecoder.hpp>

#include <Trading/Order.hpp>
#include <Trading/OrderReply.hpp>

#include <server/Defines.hpp>
#include <functionnal/OrderManager.hpp>

#include <cstdint>
#include <mutex>




namespace gateway {

    /**
     * @brief Gateway interface for incoming orders 
     *        inherits from OrderReplyHandler that will handler replies messages generation
     */
    
    class OrderManager : public core::OrderManager<1000000,100,100,core::LinearAllocator> , Decoder<core::LinearAllocator> , public gateway::OrderScanner  {

        public:
            core::InstrumentManager<> * mIntsrumentManager{nullptr};

        public :

            OrderManager(core::InstrumentManager<> * aIntsrumentManager):core::OrderManager<1000000,100,100,core::LinearAllocator>(),Decoder<core::LinearAllocator>(),mIntsrumentManager(aIntsrumentManager),gateway::OrderScanner(aIntsrumentManager){
            }
            /**
             * @brief Treat new incomming order creation
             * 
             */
            void onNewOrder (trading::Order *);

            /**
             * @brief 
             * 
             * @param aOrder 
             */
            void onTwap (trading::Order *aOrder);

            /**
             * @brief 
             * 
             * @param aOrder 
             */
            void onTwapChild (trading::Order *aOrder);

            /**
             * @brief Treat new incomming order cancellation
             * 
             */
            void onCancelOrder (trading::Cancel *);

            /**
             * @brief 
             * 
             * @param[in] aOrder 
             * @param[out] aReply 
             * @return true 
             * @return false 
             */
            bool TreatOrder  (trading::Order *aOrder,StructData*& aReply) ;
            bool TreatCancel (trading::Cancel *aOrder,StructData*& aReply) ;


            virtual trading::Order * getOrderById (const uint64_t&aOrderId){ 
                auto lIt = mOrderList.find (aOrderId);
                if (lIt!=mOrderList.end())
                {
                    return lIt->second;
                }
                else
                {
                    return nullptr;
                }
            }

            void onMessage(void* aMessage, size_t aLength){
                Decoder<>::Decode (aMessage,aLength);
            } 

            void TreatNewOrder (void*aMessage) {
                INFO(Gateway,"Gateway Order Book ::TreatNewOrder "<<*(trading::Order *)aMessage);
                onNewOrder((trading::Order *)aMessage);
            };
            
            void TreatNewCancel (void*aMessage) {
                INFO(Gateway,"Gateway Order Book ::TreatNewCancel "<<*(trading::Cancel *)aMessage);
                onCancelOrder((trading::Cancel *)aMessage);
            };

            void TreatApproved (void* aMessage) {
                INFO(Gateway,"Gateway Order Book ::TreatApproved");
                StructData * lStructReply = (StructData*) mAllocator->Allocate (sizeof (StructData));
                lStructReply->mData = aMessage;
                lStructReply->mLength = sizeof(trading::OrderApproved);
                notifyObservers(core::Event::ToDispatcher,lStructReply);
            };

            virtual void onThreadAdded (const char *aMessage) {
                 INFO(Gateway,"Gateway Order Book ::onThreadId "<<aMessage);
            };
            virtual void onThreadError (const char *aMessage) {
                 ERRORR(Gateway,"Gateway Order Book ::onThreadError " <<aMessage);
            };
            virtual void onCloseThread (const char *aMessage) {
                 INFO(Gateway,"Gateway Order Book ::onCloseThread "<<aMessage);
            };
            

    };
    
    
    void OrderManager::onNewOrder (trading::Order *aOrder) 
    {

        //mOrderList.insert(std::make_pair(aOrder->ORDERID,aOrder));
        StructData * lData = nullptr;
        if (IsAlgoOrder(aOrder))
        {
            INFO(Gateway,"Gateway Order Book ::Algo Order Detected");
            return;
        }
        else
        {
            if (TreatOrder(aOrder,lData))
            {
               INFO(Gateway,"Gateway Order Book ::Order Accepted"); 
            }
            else
            {
                INFO(Gateway,"Gateway Order Book ::Order Rejected"); 
            }

        }
    }

    void OrderManager::onCancelOrder (trading::Cancel *aCancel)
    {
        StructData * lData = nullptr;
        if (TreatCancel(aCancel,lData))
        {
            INFO(Gateway,"Gateway Order Book ::Cancel Accepted");
            mOrderList.erase(aCancel->ORDERID);
        }
        else
        {
            INFO(Gateway,"Gateway Order Book ::Cancel Rejected"); 
        }

        }

    bool OrderManager::TreatCancel (trading::Cancel *aCancel,StructData*& aReply)  
    {
         RejectReason_ lReject = scanOrderCancellation (aCancel);
        if (lReject == RejectReason_::NOREJECT)
        {
            trading::Canceled* lReply = (trading::Canceled*)mAllocator->Allocate(sizeof (trading::Canceled));
            lReply->TYPE='D';
            StructData * lStructReply = (StructData*) mAllocator->Allocate (sizeof (StructData));
            lStructReply->mClientId = aCancel->SESSIONID;
            lStructReply->mData =(void *) lReply;
            ((trading::Canceled*)(lStructReply->mData))->STOCK           = aCancel->STOCK;
            ((trading::Canceled*)(lStructReply->mData))->SESSIONID       = aCancel->SESSIONID;
            ((trading::Canceled*)(lStructReply->mData))->ORDERID         = aCancel->ORDERID;
            ((trading::Canceled*)(lStructReply->mData))->CLIENTORDERID   = aCancel->CLIENTORDERID;
            lStructReply->mLength = sizeof(trading::Canceled);
            notifyObservers(core::Event::FromGateway,lStructReply);
            aReply = lStructReply;
            StructData * lStructOrder = (StructData*) mAllocator->Allocate (sizeof (StructData));
            lStructOrder->mClientId = aCancel->SESSIONID;
            lStructOrder->mData = aCancel;
            lStructOrder->mLength = sizeof(trading::Cancel);
            notifyObservers(core::Event::ToDispatcher,lStructOrder);
            return true;
        }
        else
        {
            trading::Reject *lReply = (trading::Reject*)mAllocator->Allocate(sizeof (trading::Reject));
            lReply->TYPE ='J';
            lReply->REASON = (uint8_t)lReject;
            StructData * lStructReject = (StructData*) mAllocator->Allocate (sizeof (StructData));
            lStructReject->mClientId = aCancel->SESSIONID;
            lStructReject->mData =(void *) lReply;
            ((trading::Reject*)(lStructReject->mData))->SESSIONID       = aCancel->SESSIONID;
            ((trading::Reject*)(lStructReject->mData))->STOCK           = aCancel->STOCK;
            ((trading::Reject*)(lStructReject->mData))->ORDERID         = aCancel->ORDERID;
            ((trading::Reject*)(lStructReject->mData))->CLIENTORDERID   = aCancel->CLIENTORDERID;
            lStructReject->mLength = sizeof(trading::Reject);
            aReply = lStructReject;
            notifyObservers(core::Event::FromGateway,lStructReject);
            return false;
        } 
    }
    
    void OrderManager::onTwap (trading::Order *aOrder){
        StructData * lData = (StructData*) mAllocator->Allocate(sizeof(StructData));
        lData->mData    = (void *) aOrder;
        lData->mLength  = sizeof(trading::Order);
        notifyObservers(core::Event::ToNode,lData);
    }

    void OrderManager::onTwapChild (trading::Order *aOrder){
        StructData * lData = nullptr;
        if (TreatOrder(aOrder,lData))
        {
            INFO(Gateway,"Gateway Order Book ::Order Accepted"); 
        }
        else
        {
            INFO(Gateway,"Gateway Order Book ::Order Rejected"); 
        }
    }

    bool OrderManager::TreatOrder (trading::Order *aOrder,StructData*& aReply)
    {         
        RejectReason_ lReject = scanOrderCreation (aOrder);
        if (lReject == RejectReason_::NOREJECT)
        {
            trading::Accepted* lReply = (trading::Accepted*)mAllocator->Allocate(sizeof (trading::Accepted));
            lReply->TYPE='A';
            StructData * lStructReply = (StructData*) mAllocator->Allocate (sizeof (StructData));
            lStructReply->mClientId = aOrder->SESSIONID;
            lStructReply->mData =(void *) lReply;
            lReply->STOCK         = aOrder->STOCK;
            lReply->SESSIONID     = aOrder->SESSIONID;
            lReply->ORDERID       = aOrder->ORDERID;
            lReply->CLIENTORDERID = aOrder->CLIENTORDERID;
            INFO(Gateway,"Gateway Order Book ::Order Reply "<<*(lReply)); 
            lStructReply->mLength = sizeof(trading::Accepted);
            notifyObservers(core::Event::FromGateway,lStructReply);
            aReply = lStructReply;
            StructData * lStructOrder = (StructData*) mAllocator->Allocate (sizeof (StructData));
            lStructOrder->mClientId = aOrder->SESSIONID;
            lStructOrder->mData = aOrder;
            lStructOrder->mLength = sizeof(trading::Order);
            notifyObservers(core::Event::ToDispatcher,lStructOrder);
            //mReplyList.insert (std::make_pair(aOrder->ORDERID,lReply));
            return true;
        }
        else
        {
            trading::Reject *lReply = (trading::Reject*)mAllocator->Allocate(sizeof (trading::Reject));
            lReply->TYPE ='J';
            lReply->REASON = (uint8_t)lReject;
            StructData * lStructReject = (StructData*) mAllocator->Allocate (sizeof (StructData));
            lStructReject->mClientId = aOrder->SESSIONID;
            lStructReject->mData = (void *)lReply;
            lReply->SESSIONID     = aOrder->SESSIONID;
            lReply->STOCK         = aOrder->STOCK;
            lReply->SESSIONID     = aOrder->SESSIONID;
            lReply->ORDERID       = aOrder->ORDERID;
            lReply->CLIENTORDERID = aOrder->CLIENTORDERID;
            lStructReject->mLength = sizeof(trading::Reject);
            aReply = lStructReject;
            notifyObservers(core::Event::FromGateway,lStructReject);
           // mRejectList.insert(std::make_pair(aOrder->ORDERID,lReply));
            return false;
        } 
    }

}

#endif


