#ifndef SERVER_DEFINES_HPP_
#define SERVER_DEFINES_HPP_


#include <cstdio>
#include <cstdint>

#define LOCALHOST "127.0.0.1"
constexpr uint16_t gMaxCnnection  = 300;
constexpr uint64_t gMaxOrder      = 10000000;
constexpr uint64_t gMaxInstrument = 10000000;

typedef struct StructData {
    void * mData {nullptr};
    size_t mLength {0};
    size_t mClientId {0};
} StructData;
        
#endif
