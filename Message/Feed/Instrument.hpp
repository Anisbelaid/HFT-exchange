#ifndef  _INSTRUMENT_HPP_
#define  _INSTRUMENT_HPP_

#include <Types.hpp>
#include <Feed/Header.hpp>

namespace feed {
    
    class Instrument : public Header  {
        public:
            char     SYMBOL[8];
            uint32_t QUANTITYDECIMAL;
            uint32_t PRICEDECIMAL;
            char     PADDING [8];
            
            Instrument (){
                static_assert((sizeof (Instrument) <= 64) , "Instrument size exceeded maximum allowed 64");
            }
    };

}


#endif
