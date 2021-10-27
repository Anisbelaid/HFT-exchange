#ifndef _MARKET_ORDER_HPP
#define _MARKET_ORDER_HPP

#include <sequencer/Sequencer.hpp>
#include <functionnal/AlgoOrderContainer.hpp>
#include <functionnal/TradeHandler.hpp>
#include <unordered_map>

namespace core {
    
    /**
    * @brief Interface of a market Order that will be used to push transfer back and treat messager  
    */
   template <uint32_t Capacity = 1000000, uint16_t MAX_ALGO_ORDER = 10,uint16_t MAX_USER = 100,class Allocator = core::LinearAllocator>
    class OrderManager : public IOrderManager  , public Sequencer<MAX_USER> , public AlgoOrderContainer , public AlgoOrderListner
    {
        using sequencer = Sequencer<MAX_USER>;
        protected:
        typedef  std::unordered_map<uint64_t,trading::Reject*> RejetcList_;
        ///
        typedef  std::unordered_map<uint64_t,trading::Accepted*> ReplyList_;
        /// Type of Conatiner that will hold pointers to incomming orders 
        typedef  std::unordered_map<uint64_t,trading::Order*> OrderList_;
        /// Type of Conatiner that will hold pointers to incomming orders 
        typedef  std::unordered_map<uint64_t, TradeHandler<Allocator>*> TradeHandlerList_;

            /// Orders Container
            OrderList_          mOrderList ;
            
            ///
            ReplyList_          mReplyList ;


            ///
            RejetcList_          mRejectList ;
            
            ///
            TradeHandlerList_   mTradeHandlerList;

            ///
            Allocator           *mAllocator{nullptr};

        public:
            
            /**
            * @brief Market Implementation .
            * @param aSessionManager The adress of the destination.
            * @param aPort athe port of the destinaation.
            * @return Constuctor
            */
            OrderManager()  {
                mAllocator = new Allocator();
                mAllocator->Init (1024*1024,64);
            };
            
            /**
            * Destroy the Connection.
            * @return Destructor
            */
            virtual ~OrderManager() {
                delete mAllocator;
            }; 
             
            /**
             * @brief 
             * 
             */
            void start (){
                sequencer::launch ();
            }

            void stop (){
                sequencer::shutdown();
            }

            /**
             * Update the state of this observer
             */
            virtual void update(core::Event aEvent,  StructData * aData = nullptr) {
                if (aEvent == core::Event::FromServerSession){
                    sequencer::push(aData);
                }
                else if (aEvent == core::Event::FromMatchingEngine)
                {
                    notifyObservers(core::Event::FromMatchingEngine,aData);
                }
                else if (aEvent == core::Event::ToMarketData)
                {
                    notifyObservers(core::Event::ToMarketData,aData);
                }
            }

            /**
             * Update the state of this observer
             */
            virtual void update(Event aEvent ,std::size_t aData = 0) {
                Event lSub = Event::Subscription ;
                Event lUnSub = Event::UnSubscription ;
                if (aEvent == lSub)
                {
                    sequencer::subscribe(aData,1);
                }
                else if(aEvent == lUnSub)
                {
                    sequencer::unSubscribe(aData);
                }
            }

            void update(Event,void* aData = 0){};
  

    };
}
#endif
