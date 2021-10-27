#ifndef _ALGO_ORDER_CONTAINER_HPP
#define _ALGO_ORDER_CONTAINER_HPP


#include <Trading/Order.hpp>
#include <functionnal/AlgoOrder.hpp>
#include <thread/CPUTimer.hpp>

namespace core {

    class AlgoOrderContainer {         
        public :
            
            /**
             * @brief Construct a new Algo Order Container object
             * 
             */
            AlgoOrderContainer();

            /**
             * @brief Destroy the Algo Order Container object
             * 
             */
            virtual ~AlgoOrderContainer() = default ;

            /**
             * @brief 
             * 
             * @return true 
             * @return false 
             */
            bool IsAlgoOrder (trading::Order *);

            /**
             * @brief 
             * 
             */
            virtual void onTwap (trading::Order *){};

            /**
             * @brief 
             * 
             */
            virtual void onVwap (trading::Order *){};

            /**
             * @brief 
             * 
             */
            virtual void onTwapChild (trading::Order *){};

            /**
             * @brief 
             * 
             */
            virtual void onVwapChild (trading::Order *){};


    };

    AlgoOrderContainer::AlgoOrderContainer(){
    }

    bool AlgoOrderContainer::IsAlgoOrder (trading::Order *aOrder)
    {
        if (aOrder->TIMEINFORCE == TimeInForce_::TWAP ){
            onTwap(aOrder);
            return true;
        }
        else if (aOrder->TIMEINFORCE == TimeInForce_::VWAP)
        {
            onVwap(aOrder);
            return false;
        }
        else if (aOrder->TIMEINFORCE == TimeInForce_::TWAPC ){
            onTwapChild(aOrder);
            return true;
        }
        else if (aOrder->TIMEINFORCE == TimeInForce_::VWAPC)
        {
            onVwapChild(aOrder);
            return false;
        }
        else
        {
            return false;
        }
        
    }
 
}
#endif
