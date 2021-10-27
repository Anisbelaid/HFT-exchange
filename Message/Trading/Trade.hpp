#ifndef TRADING_EXEC_HPP
#define TRADING_EXEC_HPP

#include <Trading/Header.hpp>

namespace trading {
    
    class  Exec : public Header {
        public:
            uint32_t EXECUTEDSHARES;
            uint32_t EXECUTEDPRICE;
            uint64_t MATCHNUMBER;
            char     CLIENTID[8];
            char     PADDING[2];
            
            
        friend std::ostream& operator<<(std::ostream& os, Exec const & aOrder) {
            return os <<"Exec - TYPE="<< aOrder.TYPE<<" | CLIENTID = "<<aOrder.CLIENTID<<" | EXECUTEDSHARES ="<<aOrder.EXECUTEDSHARES<<" | EXECUTEDPRICE ="<<aOrder.EXECUTEDPRICE<<" | MATCHNUMBER ="<<aOrder.MATCHNUMBER <<" | SESSIONID ="<<aOrder.SESSIONID << '\n';
        }
        
        Exec (){
            //static_assert((sizeof (Exec) <= 64) , "EXEC Size exceeded maximum allowed 64");
        }
    };
    
}
#endif
