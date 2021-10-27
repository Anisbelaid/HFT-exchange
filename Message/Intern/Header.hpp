
#ifndef INTERN_HEADER_HPP
#define INTERN_HEADER_HPP

#include <Types.hpp>
#include <iostream>
#include <cstdint>

namespace intern {
      
    struct Header {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
        uint64_t RECEPTIONTIME;
        uint64_t MATCHINGSESSIONID;
        uint64_t PARENTSESSIONID;
        uint64_t SESSIONID;

    };

    struct HeartBeat : public Header {     
    };
    
}

#endif
