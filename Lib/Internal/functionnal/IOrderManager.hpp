#ifndef _IMARKET_ORDER_HPP
#define _IMARKET_ORDER_HPP

namespace core{

    class IOrderManager: public Subject , public Observer{
        public:
            IOrderManager() = default;
            virtual void start () =0;
            virtual void stop () =0;
            virtual ~IOrderManager () = default;
    };
}
#endif