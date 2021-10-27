#ifndef  FEED_ORDER_HPP_
#define  FEED_ORDER_HPP_

#include <iostream>
#include <Types.hpp>
#include <Feed/Header.hpp>


namespace feed {
    
    struct Order  : public Header {
        uint64_t ORDERID;
        uint32_t SHARES;
        uint32_t PRICE;
        char     BUYSELL;
        char     PADDING[15];
        uint64_t RATIO;
        uint64_t STARTTIME;
        uint64_t STOPTIME;
        char     PADDING1[40];
        
        
        
        friend std::ostream& operator<<(std::ostream& os, Order const & aOrder) {
            return os <<"ADD_ORDER - TYPE="<< aOrder.TYPE<<" | BUYSELL ="<<aOrder.BUYSELL<<"| SHARES ="<<aOrder.SHARES<<" | STOCK ="<<aOrder.STOCK<<" | PRICE ="<<aOrder.PRICE << '\n';
        }   
        
        Order (){
            static_assert((sizeof (Order) % 64 == 0) , "Header Size exceeded maximum allowed 64");
        }
    };
    
    
    struct Cancel  : public Header {   
        uint64_t ORDERID;
        uint32_t SHARES;
        char     BUYSELL;
        char     PADDING[15];
    
        friend std::ostream& operator<<(std::ostream& os, Cancel const & aOrder) {
            return os <<"ADD_ORDER - TYPE="<< aOrder.TYPE<<" | BUYSELL ="<<aOrder.BUYSELL<<" | SHARES ="<<aOrder.SHARES<<" | STOCK ="<<aOrder.STOCK<< '\n';
        } 
        
        Cancel (){
            static_assert((sizeof (Cancel) <= 64) , "Header Size exceeded maximum allowed 32");
        }
    
    };

}

#endif



