#ifndef  _ORDER_SCANNER_HPP_
#define  _ORDER_SCANNER_HPP_

#include <Trading/Reject.hpp>
#include <skeleton/InstrumentInfo.hpp>
#include <skeleton/InstrumentManager.hpp>
#include <Trading/Order.hpp>

namespace gateway {
    
    
    class OrderScanner {
         core::InstrumentManager<> * mIntsrumentManager{nullptr};    
        public :
            OrderScanner (core::InstrumentManager<> * aIntsrumentManager):mIntsrumentManager(aIntsrumentManager){

            } 
            ~OrderScanner () = default ;
            virtual trading::Order * getOrderById (const uint64_t&){ }
            RejectReason_ scanOrderCreation (trading::Order *aOrder) ;
            RejectReason_ scanOrderCancellation (trading::Cancel *aOrder) ;
                
    };
}


RejectReason_ gateway::OrderScanner::scanOrderCreation(trading::Order *aOrder){
    core::InstrumentInfo * lInst = mIntsrumentManager->getInstrument(aOrder->STOCK);
    if (!lInst) return RejectReason_::INSTRUMENTNOTFOUND;
    return RejectReason_::NOREJECT;
}

RejectReason_ gateway::OrderScanner::scanOrderCancellation(trading::Cancel *aOrder){
    core::InstrumentInfo * lInst = mIntsrumentManager->getInstrument(aOrder->STOCK);
    if (!lInst) return RejectReason_::INSTRUMENTNOTFOUND;
    //trading::Order * lOrder = getOrderById(aOrder->ORDERID);
    //if (!lOrder) return RejectReason_::ORDERNOTFOUND;
    return RejectReason_::NOREJECT;
}
#endif



