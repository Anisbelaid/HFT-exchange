#ifndef OBJECT_FACTORY_HPP_
#define OBJECT_FACTORY_HPP_

#include <server/Defines.hpp>
#include <allocator/LinearAllocator.hpp>
#include <Trading/Order.hpp>
#include <Trading/OrderReply.hpp>
#include <Trading/Reject.hpp>
#include <Trading/Trade.hpp>
#include <Feed/Order.hpp>
#include <Feed/Trade.hpp>
#include <Feed/Instrument.hpp>

namespace core
{
   enum class object : uint8_t
   {
       Order,
       Twap,
       Vwap,
       Accepted,
       Reject
   };

    template<class Allocator = core::LinearAllocator>
    class ObjectFactory {
        private:
            static Allocator    mAllocator;
        public:
            static StructData * getObject (object aObject){
                StructData * lStruct = new (mAllocator->Allocate(sizeof (StructData))) StructData;
                switch (aObject)
                {
                    case object::Order :
                    {
                        trading::Order * lOrder = new (mAllocator->Allocate(sizeof (StructData))) trading::Order;
                        lOrder->TIMEINFORCE = TimeInForce_::DAY;
                        lStruct->mData = (void *)lOrder;
                        lStruct->mLength =  sizeof (trading::Order);
                    }
                    break;
                    case object::Twap : 
                    {
                        trading::Order * lOrder = new (mAllocator->Allocate(sizeof (trading::Order))) trading::Order;
                        lOrder->TIMEINFORCE = TimeInForce_::TWAP;
                        lStruct->mData = (void *)lOrder;
                        lStruct->mLength =  sizeof (trading::Order);
                    }
                    break;
                    case object::Vwap:
                    {
                        trading::Order * lOrder = new (mAllocator->Allocate(sizeof (trading::Order))) trading::Order;
                        lOrder->TIMEINFORCE = TimeInForce_::VWAP;
                        lStruct->mData = (void *)lOrder;
                        lStruct->mLength =  sizeof (trading::Order);
                    }
                    break;
                    case object::Accepted:
                    {
                        trading::Accepted * lReply = new (mAllocator->Allocate(sizeof (trading::Accepted))) trading::Accepted;
                        lStruct->mData       = (void *) lReply;
                        lStruct->mLength    = sizeof (trading::Accepted);
                    }
                    break;
                    case object::Reject:
                    {
                        trading::Reject * lReply = new (mAllocator->Allocate(sizeof (trading::Reject))) trading::Reject;
                        lStruct->mData       = (void *) lReply;
                        lStruct->mLength    = sizeof (trading::Reject);
                    }
                    break;
                }
                return lStruct;

            }
   };
}

#endif