#ifndef  _Limit_HPP_
#define  _Limit_HPP_

#include <Types.hpp>
#include <Trading/Order.hpp>
#include <Intern/Header.hpp>
#include <allocator/LinearAllocator.hpp>

namespace intern {
    
    struct OrderListElement{
        trading::Order * elem {0};
        struct OrderListElement * Next{0};
    };
    
    struct OrderList
    {
        OrderListElement * Head;
        OrderListElement * Tail;
    };
    
    class Limit : public Header {
        public :
            uint64_t STOCK;
            OrderList * mOrderList;
            uint32_t INSTRUMENTID;
            uint32_t PRICE;
            uint32_t QUANTITY;
            char SIDE;
            char PADDING[11];
            
            Limit (){
                //static_assert((sizeof (Limit) <= 64) , "Limit Size exceeded maximum allowed 64");
            }
            
            template <class T = core::LinearAllocator>
            void push (trading::Order * aOrder , T * allocator, bool aNew  = false){
                if (aNew)
                {
                    mOrderList = (OrderList *)allocator->Allocate (sizeof (OrderList));
                    mOrderList->Head =nullptr;
                    mOrderList->Tail = nullptr;
                }
                OrderListElement * a =(OrderListElement *)allocator->Allocate (sizeof (OrderListElement));
                a->Next = nullptr;
                a->elem = aOrder;
                if (mOrderList->Head == nullptr){
                    mOrderList->Head=a ;
                }
                else
                {
                    mOrderList->Tail->Next=a;
                }
                
                mOrderList->Tail = a;
                
            }
            
    };
    
}
#endif


