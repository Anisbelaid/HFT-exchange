#ifndef _ALGO_ORDER_HPP
#define _ALGO_ORDER_HPP

#include <thread/CPUTimer.hpp>

namespace core {

    /**
     * @brief 
     * 
     */
    class AlgoOrderListner{
        public:
            /**
             * @brief Create a Order object
             * 
             * @return trading::Order* 
             */
            virtual trading::Order * createOrder(){return nullptr;}

            /**
             * @brief 
             * 
             * @param aOrder 
             */
            virtual void TriggerTwapChild (trading::Order *aOrder){
            }
            
            /**
             * @brief Destroy the Container Listner object
             * 
             */
            virtual ~AlgoOrderListner (){};
    };

    class AlgoOrder {//: public core::Routine {
        protected :
            AlgoOrderListner * mConatinerListner{nullptr};
        public :
            
            explicit AlgoOrder (AlgoOrderListner * aContainerListner) :mConatinerListner(aContainerListner){
            };
            
            ~AlgoOrder () = default;

            virtual void splitOrder () = 0;
    };

    class Twap : public AlgoOrder   {
        private:
             
            typedef std::vector <trading::Order*> ChildOrders_;
            trading::Order * mOrder {nullptr};
            double mRatio {0.0};
            ChildOrders_ mChildOrder ;

        public :

            Twap (AlgoOrderListner * aContainerListner,trading::Order *aOrder,double aRatio):AlgoOrder(aContainerListner),
                                                                                                      mOrder(aOrder),mRatio(aRatio){
                splitOrder ();
            }

            virtual bool Task () {
                if (mChildOrder.empty())
                {
                    return true; //Task finished
                }
                else
                {
                    trading::Order * lOrder =  mChildOrder.back();
                    //gateway::InstrumentInfo * lInstrument = InstrumentManager<>::getInstance ()->getInstrument (lOrder->STOCK);
                    uint64_t lPrice = 0;//(lInstrument->getClose()+lInstrument->getHigh()+lInstrument->getLow()+lInstrument->getOpen())/4;
                    lOrder->PRICE=lPrice;
                    mConatinerListner->TriggerTwapChild (lOrder);
                    //INFO(Gateway,"Twap Child Order triggered "<<lOrder->CLIENTID);
                    mChildOrder.pop_back();
                    return false;
                }
            };

            void splitOrder (){
                /*uint64_t lQty  = mOrder->SHARES ;
                uint64_t lUnit = lQty * mRatio;
                while (lQty){
                    trading::Order * lOrder = mConatinerListner->createOrder ();
                    lOrder->TYPE = mOrder->TYPE;
                    lOrder->STOCK = mOrder->STOCK;
                    lOrder->ORDERID = mOrder->ORDERID;
                    lOrder->REMAININGSHARES = mOrder->REMAININGSHARES;
                    lOrder->TIMEINFORCE = (uint8_t)TimeInForce_::TWAPC;
                    lOrder->SESSIONID = mOrder->SESSIONID;
                    lOrder->BUYSELL = mOrder->BUYSELL;
                    strcpy (lOrder->CLIENTORDERID,mOrder->CLIENTORDERID);
                    if (lQty>=lUnit) {
                        lQty-=lUnit;
                        lOrder->SHARES = lUnit ;
                    }
                    else {
                        lOrder->SHARES = lQty ;
                        lQty  = 0 ;
                    }
                    //INFO(Gateway,"Twap Child Order created "<<lOrder->CLIENTID);
                    mChildOrder.push_back (lOrder);
                }*/
            }
    };
}
#endif