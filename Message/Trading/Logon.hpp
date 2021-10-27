#ifndef __LOGON_HPP
#define __LOGON_HPP

#include <Intern/Header.hpp>

namespace trading {

    struct Logon  {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
        uint64_t RECPTIONTIME;
        char     USER[8];
        char     PASSWORD[8];
    };

    struct LogonAccepeted  {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
    };

    struct LogonRejected  {
        char     TYPE;
        char     PADDING[7];
        uint64_t SENDINGTIME;
    };


}

#endif