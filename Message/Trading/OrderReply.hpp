#ifndef  TRADING_ORDER_REPLY_HPP_
#define  TRADING_ORDER_REPLY_HPP_

#include <Trading/Header.hpp>

namespace trading {

    struct Accepted {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
        uint64_t RECPTIONTIME;
        uint64_t STOCK;
        uint64_t MATCHINGSESSIONID;
        uint64_t SESSIONID;
        uint64_t PARENTSESSIONID;
        uint64_t ORDERID;
        uint64_t CLIENTORDERID;
        
        friend std::ostream& operator<<(std::ostream& os, Accepted const & aOrder) {
            return os <<"ACCEPTED_ORDER - TYPE="<< aOrder.TYPE<<" | ORDERID ="<<aOrder.ORDERID<<" | STOCK ="<<aOrder.STOCK<<"| CLIENTORDERID ="<<aOrder.CLIENTORDERID<< '\n';
        }
    };
    
     struct Canceled {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
        uint64_t RECPTIONTIME;
        uint64_t STOCK;
        uint64_t MATCHINGSESSIONID;
        uint64_t SESSIONID;
        uint64_t PARENTSESSIONID;
        uint64_t ORDERID;
        uint64_t CLIENTORDERID;
        
        
    friend std::ostream& operator<<(std::ostream& os, Canceled const & aOrder) {
        return os <<"ACCEPTED_ORDER - TYPE="<< aOrder.TYPE<<" | CLIENTORDERID ="<<aOrder.CLIENTORDERID<<" | STOCK ="<<aOrder.STOCK<< '\n';
    }
      
    };

}
#endif



