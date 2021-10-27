#ifndef  TRADING_REJECT_HPP_
#define  TRADING_REJECT_HPP_

#include <Types.hpp>
#include <Trading/OrderReply.hpp>    
    
namespace trading {  
    struct Reject {
            char     TYPE;
            char     PADDING[7];
            uint64_t SENDINGTIME;
            uint64_t RECPTIONTIME;
            uint64_t STOCK;
            uint64_t MATCHINGSESSIONID;
            uint64_t SESSIONID;
            uint64_t PARENTSESSIONID;
            uint64_t ORDERID;
            uint64_t      CLIENTORDERID;
            uint8_t       REASON;
            
            friend std::ostream& operator<<(std::ostream& os, Reject const & aOrder) {
                return os <<"REJECT - TYPE="<< aOrder.TYPE<<" | ORDERID ="<<aOrder.ORDERID<<" | REASON ="<<(uint8_t)aOrder.REASON<< '\n';
            }
    };
}
#endif


