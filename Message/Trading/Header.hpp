#ifndef HEADER_HPP
#define HEADER_HPP

#include <Types.hpp>
#include <iostream>
#include <cstdint>

namespace trading {
      
       struct Header {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
        uint64_t RECPTIONTIME;
        uint64_t STOCK;
        uint64_t MATCHINGSESSIONID;
        uint64_t SESSIONID;
        uint64_t PARENTSESSIONID;
        uint64_t ORDERID;

    };
    
}

#endif
