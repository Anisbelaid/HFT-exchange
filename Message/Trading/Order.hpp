#ifndef  TRADING_ORDER_HPP_
#define  TRADING_ORDER_HPP_

#include <Types.hpp>
#include <Trading/Header.hpp>


namespace trading {


    enum class OrderType: uint8_t
    {
        Retail = 0,
        Broker = 1 
    };
    
    struct Order  {
        char         TYPE;
        char         PADDINGHEADER[7];
        uint64_t     SENDINGTIME;
        uint64_t     RECPTIONTIME;
        uint64_t     STOCK;
        uint64_t     MATCHINGSESSIONID;
        uint64_t     SESSIONID;
        uint64_t     PARENTSESSIONID;
        uint64_t     ORDERID;
        uint64_t     SHARES ;
        uint64_t     REMAININGSHARES;
        uint64_t     PRICE ;
        uint64_t     CLIENTORDERID ;
        char         BROKER [8];
        char         USER [8];
        TimeInForce_ TIMEINFORCE ;
        char         BUYSELL; 
        OrderType    ORDERTYPE;  
        char         PADDING [5];         
            
        friend std::ostream& operator<<(std::ostream& os, Order const & aOrder) {
            return os <<"ENTER_ORDER - TYPE="<< aOrder.TYPE<<"| RECEPTIONTIME = "<<aOrder.RECPTIONTIME<<" | STOCK ="<<aOrder.STOCK<<" | BUYSELL ="<<aOrder.BUYSELL<<" | SHARES ="<<aOrder.SHARES<<" | STOCK ="<<aOrder.STOCK<<" | PRICE ="<<aOrder.PRICE<<" | TIMEINFORCE ="<<(uint8_t)aOrder.TIMEINFORCE<<" | REMAININGSHARES ="<<aOrder.REMAININGSHARES <<" | SESSIONID ="<<aOrder.SESSIONID <<" | ORDERID ="<<aOrder.ORDERID<< " | CLIENTORDERID ="<<aOrder.CLIENTORDERID<<'\n';
        }   
    };
    
    struct RequestApproval : public Header
    {
        uint64_t     SHARES ;
        uint64_t     PRICE ;
        char         USERID [8];
    };

    struct OrderApproved : public Header
    {
        uint64_t     SHARES ;
        uint64_t     PRICE ;
        char         USERID [8];
    };

    struct OrderRejected : public Header
    {
        uint64_t     SHARES ;
        uint64_t     PRICE ;
        char         USERID [8];
    };
    
    struct Cancel  {
            char         TYPE;
            char         PADDINGHEADER[7];
            uint64_t     SENDINGTIME;
            uint64_t     RECPTIONTIME;
            uint64_t     STOCK;
            uint64_t     MATCHINGSESSIONID;
            uint64_t     SESSIONID;
            uint64_t     PARENTSESSIONID;
            uint64_t     ORDERID;
            uint64_t     CLIENTORDERID;
            char         BUYSELL;
            char         PADDING [7];
            
            
       friend std::ostream& operator<<(std::ostream& os, Cancel const & aOrder) {
            return os <<"CANCEL_ORDER - TYPE="<< aOrder.TYPE<<"| RECEPTIONTIME = "<<aOrder.RECPTIONTIME<<" | CLIENTORDERID ="<<aOrder.CLIENTORDERID<<" | STOCK ="<<aOrder.STOCK<<" | SESSIONID ="<<aOrder.SESSIONID <<" | ORDERID ="<<aOrder.ORDERID<< '\n';
        }  
    };


}

#endif



