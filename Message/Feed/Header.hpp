#ifndef FEED_HEADER_HPP
#define FEED_HEADER_HPP

#include <Types.hpp>
#include <iostream>
#include <cstdint>

namespace feed {
      
    struct Header {
        char     TYPE;
        char     PADDING[7];
        uint64_t SEQUENCENUMBER;
        uint64_t SENDINGTIME;
        uint64_t STOCK;
        
        Header (){
            static_assert((sizeof (Header) <= 32) , "Header Size exceeded maximum allowed 32");
        }
    };
    
}

#endif
