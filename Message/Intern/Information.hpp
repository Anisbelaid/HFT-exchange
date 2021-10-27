#ifndef _INFORMATION_HPP
#define _INFORMATION_HPP

#include <Intern/Header.hpp>

#include <Types.hpp>

namespace intern {

    enum class EVENTS : uint8_t
    {
       ClientConnection =  1,
       BrokerConnection =  2,
       ClientDisonnection =  3,
       BrokerDisconnection = 4
    };

    struct Information : public Header  {
        EVENTS           EVENTID;
        char             PADDING[7];
        char             USER[8];

        friend std::ostream& operator<<(std::ostream& os, Information const & aOrder) {
            return os <<"INFORMATION - TYPE="<< aOrder.TYPE << '\n';
        }  
    };


}

#endif