#ifndef  _ORDER_MANAGER_HPP_
#define  _ORDER_MANAGER_HPP_

#include <functionnal/OrderManager.hpp>
#include <container/CustomArray.hpp>
#include <Trading/Order.hpp>

#include <unordered_map>


namespace dispatcher {
    
    struct UserRoutingData 
    {
        uint64_t mSessionId{0};
        uint64_t mParentSessionId{0};
    };

    class OrderManager : public core::OrderManager <> , Decoder<> 
    {   
        using BaseOrderManager = core::OrderManager <>;
        std::unordered_map <std::string,UserRoutingData> mRetailRoutingData;
        std::unordered_map <std::string,UserRoutingData> mBrokerRoutingData;
        public :
            
            OrderManager () {}

            virtual void onMessage (void *aMessage,size_t aSize=0) {
                Decoder<>::Decode (aMessage,aSize);
            };


            void TreatApproved (void*aMessage)
            {
                INFO(Dispatcher,"Order Book ::TreatApproved ");
                trading::OrderApproved* lApproved = (trading::OrderApproved*)aMessage;
                auto lIt =BaseOrderManager::mOrderList.find (lApproved->ORDERID);
                
                if (lIt != BaseOrderManager::mOrderList.end ())
                {
                    StructData * lStructOrder = (StructData*) mAllocator->Allocate (sizeof (StructData));
                    lStructOrder->mData = lIt->second;
                    lStructOrder->mLength = sizeof(trading::Order);
                    notifyObservers(core::Event::ToMatchingEngine,lStructOrder);
                }
                else
                {
                    StructData * lStructOrder = (StructData*) mAllocator->Allocate (sizeof (StructData));
                    trading::OrderRejected * lRequestRejected = (trading::OrderRejected*) mAllocator->Allocate (sizeof (trading::OrderRejected));
                    lRequestRejected->TYPE = 'Z';
                    lRequestRejected->ORDERID= lRequestRejected->ORDERID;
                    lRequestRejected->SESSIONID= lRequestRejected->SESSIONID;
                    lRequestRejected->PARENTSESSIONID= lRequestRejected->PARENTSESSIONID;
                    lStructOrder->mData = lRequestRejected;
                    lStructOrder->mLength = sizeof( trading::OrderRejected);
                    notifyObservers(core::Event::FromDispatcher,lStructOrder);
                }
            }
            void TreatNewOrder (void*aMessage) {
                INFO(Dispatcher,"Order Book ::TreatNewOrder ");
                trading::Order *lOrder = (trading::Order *) aMessage;
                StructData * lStructOrder = (StructData*) mAllocator->Allocate (sizeof (StructData));
                
                if (lOrder->ORDERTYPE == trading::OrderType::Retail)
                {   
                    std::string lUser(lOrder->BROKER,8);
                    auto lIt = mBrokerRoutingData.find (lUser);
                    if (lIt!=mBrokerRoutingData.end())
                    {
                        BaseOrderManager::mOrderList.insert (std::make_pair(lOrder->ORDERID,lOrder));
                        trading::RequestApproval * lRequestApproval = (trading::RequestApproval*) mAllocator->Allocate (sizeof (trading::RequestApproval));
                        lRequestApproval->TYPE = 'W';
                        lRequestApproval->ORDERID= lOrder->ORDERID;
                        lRequestApproval->STOCK = lOrder->STOCK;
                        lRequestApproval->SHARES = lOrder->SHARES;
                        lRequestApproval->PRICE = lOrder->PRICE;
                        lRequestApproval->SESSIONID= lIt->second.mSessionId;
                        lRequestApproval->PARENTSESSIONID= lIt->second.mParentSessionId;
                        lStructOrder->mData = lRequestApproval;
                        lStructOrder->mLength = sizeof( trading::RequestApproval);
                        notifyObservers(core::Event::FromDispatcher,lStructOrder);
                    }
                    else
                    {
                        trading::OrderRejected * lRequestRejected = (trading::OrderRejected*) mAllocator->Allocate (sizeof (trading::OrderRejected));
                        lRequestRejected->TYPE = 'Z';
                        lRequestRejected->ORDERID= lOrder->ORDERID;
                        lRequestRejected->SESSIONID= lOrder->SESSIONID;
                        lRequestRejected->PARENTSESSIONID= lOrder->PARENTSESSIONID;
                        lStructOrder->mData = lRequestRejected;
                        lStructOrder->mLength = sizeof( trading::OrderRejected);
                        notifyObservers(core::Event::FromDispatcher,lStructOrder);
                    }
                } 
                else
                {
                    lStructOrder->mData = aMessage;
                    lStructOrder->mLength = sizeof(trading::Order);
                    notifyObservers(core::Event::ToMatchingEngine,lStructOrder);
                }
            };
            
            void TreatNewCancel (void*aMessage) {
                INFO(Dispatcher,"Market Order ::TreatNewCancel ");
                StructData * lStructOrder = (StructData*) mAllocator->Allocate (sizeof (StructData));
                lStructOrder->mData = aMessage;
                lStructOrder->mLength = sizeof(trading::Cancel);
                notifyObservers(core::Event::ToMatchingEngine,lStructOrder);
            };

            void TreatInformation (void*aMessage) {
                INFO(Dispatcher,"Market Order ::TreatInformation ");
                intern::Information *lEvent = (intern::Information*)aMessage;
                switch (lEvent->EVENTID)
                {
                    case intern::EVENTS::ClientConnection:
                        onClientConnection (lEvent);
                        break;
                    case intern::EVENTS::BrokerConnection:
                        onBrokerConnection (lEvent);
                        break;
                    case intern::EVENTS::ClientDisonnection:
                        onClientDisconnection (lEvent);
                        break;
                    case intern::EVENTS::BrokerDisconnection:
                        onBrokerDisconnection (lEvent);
                        break;
                    default :
                        break;

                }
            };
            
            void onClientConnection(intern::Information *aEvent)
            {
                std::string lUser (aEvent->USER,8);
                auto lIt = mRetailRoutingData.find (lUser);
                if (lIt == mRetailRoutingData.end())
                {
                    UserRoutingData lData;
                    lData.mSessionId=aEvent->SESSIONID;
                    lData.mParentSessionId=aEvent->PARENTSESSIONID;
                    mRetailRoutingData.insert (std::make_pair(lUser,lData));
                    INFO(Dispatcher,"Order Manager :: User added");
                }
                else
                {
                  INFO(Dispatcher,"Order Manager :: User already logged");  
                }
            }
            
            void onBrokerConnection(intern::Information *aEvent)
            {
                std::string lUser (aEvent->USER,8);
                auto lIt = mBrokerRoutingData.find (lUser);
                if (lIt == mBrokerRoutingData.end())
                {
                    UserRoutingData lData;
                    lData.mSessionId=aEvent->SESSIONID;
                    lData.mParentSessionId=aEvent->PARENTSESSIONID;
                    mBrokerRoutingData.insert (std::make_pair(lUser,lData));
                    INFO(Dispatcher,"Order Manager :: Broker added");
                }
                else
                {
                  INFO(Dispatcher,"Order Manager :: User already logged");  
                }
            }

            void onClientDisconnection(intern::Information *aEvent)
            {
                std::string lUser (aEvent->USER,8);
                auto lIt = mBrokerRoutingData.find (lUser);
                if (lIt != mBrokerRoutingData.end())
                {
                    mBrokerRoutingData.erase (lIt);
                    INFO(Dispatcher,"Order Manager :: Broker deleted");
                }
                else
                {
                  INFO(Dispatcher,"Order Manager :: Broker not logged");  
                }
            }

            void onBrokerDisconnection(intern::Information *aEvent)
            {
                std::string lUser (aEvent->USER,8);
                auto lIt = mBrokerRoutingData.find (lUser);
                if (lIt != mBrokerRoutingData.end())
                {
                    mBrokerRoutingData.erase (lIt);
                    INFO(Dispatcher,"Order Manager :: User deleted");
                }
                else
                {
                  INFO(Dispatcher,"Order Manager :: User not logged");  
                }
            }

            void onThreadAdded (const char *aMessage) {
                 INFO(Dispatcher,"Market Order ::onThreadId "<<aMessage);
            };
            void onThreadError (const char *aMessage) {
                 ERRORR(Dispatcher,"Market Order  ::onThreadError " <<aMessage);
            };
            void onCloseThread (const char *aMessage) {
                 INFO(Dispatcher,"Market Order  ::onCloseThread "<<aMessage);
            };
            
    };


}
#endif


