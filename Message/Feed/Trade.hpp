#ifndef  FEED_TRADE_HPP_
#define  FEED_TRADE_HPP_

#include <Feed/Header.hpp>

namespace feed {

        struct Trade : public Header {
            uint64_t ORDERID;
            uint64_t MATCHNUMBER;
            uint32_t SHARES;
            uint32_t PRICE;
            char     BUYSELL;
            char     PADDING[7];
            
            Trade (){
                static_assert((sizeof (Trade) <= 64) , "Trade Size exceeded maximum allowed 64");
            }
            
            friend std::ostream& operator<<(std::ostream& os, Trade const & aTrade) {
                return os <<"TRADE - TYPE="<< aTrade.TYPE<<" | ORDERID ="<<aTrade.ORDERID<<" | BUYSELL ="<<aTrade.BUYSELL<<" | SHARES ="<<aTrade.SHARES<<" | STOCK ="<<aTrade.STOCK<<" | PRICE ="<<aTrade.PRICE <<" | MATCHNUMBER ="<<aTrade.MATCHNUMBER <<'\n';
            }
    };
}

#endif
