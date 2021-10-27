#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>


enum class TimeInForce_ : uint8_t {
    DAY,
    GTC,
    ATP,
    IOC,
    FOK,
    GTX,
    GTD,
    ATC,
    TWAP,
    TWAPC,
    VWAP,
    VWAPC
};


enum class Side_ : uint8_t {
    B,
    S
};


enum class RejectReason_ :  uint8_t {
    NOREJECT,
    WRONGCREDENTIAL,
    INSTRUMENTNOTFOUND,
    PRICEOUTOFRANGE,
    BROKERNOTCONNECTED,
    ORDERNOTFOUND
};

enum class InternalEvent : uint8_t{
    clientConnection,
    clientDisconnection
};

#endif 
