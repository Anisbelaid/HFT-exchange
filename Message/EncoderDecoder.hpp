
#ifndef  _ENCODER_DECODER_HPP_
#define  _ENCODER_DECODER_HPP_

#include "Trading/Order.hpp"
#include "Feed/Order.hpp"
#include "Trading/Trade.hpp"
#include "Feed/Trade.hpp"
#include "Feed/Instrument.hpp"
#include "Trading/OrderReply.hpp"
#include "Trading/Reject.hpp"
#include "Intern/Header.hpp"
#include "Trading/Logon.hpp"
#include "Intern/Information.hpp"
#include <allocator/LinearAllocator.hpp>
#include <container/CustomArray_Primitive.hpp>
#include <server/Defines.hpp>
#include <thread/vDSO.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/core/noncopyable.hpp>
#include <time.h>
#include <cstdint>
#include <functional>


/**
* Implementation of a basic binairy Decoder
* @tparam Allocator Type of allocator for new object

*
*/
template <class Alocator = core::LinearAllocator>
class Decoder {
    protected :
        
        /// Callbacks array for supported messages
        core::CustomArray<std::function<void(void*)>,1,128,true>               mCallbacks;
        
        /// Messgaes size array 
        core::CustomArray<uint16_t,1,128,std::is_fundamental<uint16_t>::value> mMessageSizes;
        
        /// Decoding Buffer
        boost::circular_buffer<char>                                            mBuffer;
        
        ///Allocator
        Alocator *                                                              mEncoderAllocator{nullptr};
        
    public:
        
        Decoder () ;
       
        virtual ~Decoder() {};
        
        virtual void TreatUnkownMessgae (void*aMessage) {
        };
        
        virtual void TreatAddOrder (void*aMessage) {
        };
        
        virtual void TreatCancelOrder (void*aMessage) {
        };
        
        virtual void TreatTrade (void*aMessage) {
        };
        
        virtual void TreatNewOrder (void*aMessage) {
        };
        
        virtual void TreatNewCancel (void*aMessage) {
        };
        
        virtual void TreatExecution (void*aMessage) {
        };
        
        virtual void TreatOrderAccecpted (void*aMessage) {
        };
        
        virtual void TreatOrderRejected (void*aMessage) {
        };
        
        virtual void TreatOrderCanceled (void*aMessage) {
        };
        
        virtual void TreatInstrument (void*aMessage) {
        };

        virtual void TreatHeartBeat (void*aMessage){
        }

        virtual void TreatLogon (void*aMessage){
        }

        virtual void TreatLogonAccepeted (void*aMessage){
        }

        virtual void TreatLogonRejected (void*aMessage){
        }

        virtual void TreatInformation (void*aMessage){
        }

        virtual void TreatRequestApproval (void*aMessage){
        }

        virtual void TreatApproved (void*aMessage){
        }

        virtual void TreatRejected (void*aMessage){
        }

    protected :
        StructData * ensure (int aId,void* aMessage= nullptr,size_t aLength = 0);
        void Decode (void *,size_t);
        
    private:
        
        
        
    
};

template <class Alocator>
Decoder<Alocator>::Decoder():mCallbacks([&](void* aMessage){this->TreatUnkownMessgae(aMessage);}),mMessageSizes(0),mBuffer(64*1024*1024)
{
        mEncoderAllocator = new Alocator ();
        
        mEncoderAllocator->Init (1024*1024,64);
        
        mCallbacks.add('O',[&](void* aMessage ){this->TreatNewOrder(aMessage);});     
        mCallbacks.add('C',[&](void* aMessage ){this->TreatNewCancel(aMessage);});
        mCallbacks.add('E',[&](void* aMessage ){this->TreatExecution(aMessage);});
        mCallbacks.add('J',[&](void* aMessage ){this->TreatOrderRejected(aMessage);});
        mCallbacks.add('A',[&](void* aMessage ){this->TreatOrderAccecpted(aMessage);});
        mCallbacks.add('D',[&](void* aMessage ){this->TreatOrderCanceled(aMessage);});
        mCallbacks.add('H',[&](void* aMessage ){this->TreatHeartBeat(aMessage);});
        mCallbacks.add('L',[&](void* aMessage ){this->TreatLogon(aMessage);});
        mCallbacks.add('Q',[&](void* aMessage ){this->TreatLogonAccepeted(aMessage);});
        mCallbacks.add('R',[&](void* aMessage ){this->TreatLogonRejected(aMessage);});
        mCallbacks.add('I',[&](void* aMessage ){this->TreatInstrument(aMessage);});
        mCallbacks.add('P',[&](void* aMessage ){this->TreatTrade(aMessage);});
        mCallbacks.add('X',[&](void* aMessage ){this->TreatAddOrder(aMessage);});
        mCallbacks.add('Y',[&](void* aMessage ){this->TreatCancelOrder(aMessage);});
        mCallbacks.add('F',[&](void* aMessage ){this->TreatInformation(aMessage);});
        mCallbacks.add('W',[&](void* aMessage ){this->TreatRequestApproval(aMessage);});
        mCallbacks.add('Y',[&](void* aMessage ){this->TreatApproved(aMessage);});
        mCallbacks.add('Z',[&](void* aMessage ){this->TreatRejected(aMessage);});
        
        mMessageSizes.add('O',sizeof(trading::Order));
        mMessageSizes.add('C',sizeof(trading::Cancel));
        mMessageSizes.add('E',sizeof(trading::Exec));
        mMessageSizes.add('J',sizeof(trading::Reject));
        mMessageSizes.add('A',sizeof(trading::Accepted));
        mMessageSizes.add('D',sizeof(trading::Canceled));
        mMessageSizes.add('I',sizeof(feed::Instrument));
        mMessageSizes.add('X',sizeof(feed::Order));
        mMessageSizes.add('Y',sizeof(feed::Cancel));
        mMessageSizes.add('P',sizeof(feed::Trade));
        mMessageSizes.add('H',sizeof(intern::HeartBeat));
        mMessageSizes.add('L',sizeof(trading::Logon));
        mMessageSizes.add('Q',sizeof(trading::LogonAccepeted));
        mMessageSizes.add('R',sizeof(trading::LogonRejected));
        mMessageSizes.add('F',sizeof(intern::Information));
        mMessageSizes.add('W',sizeof(trading::RequestApproval));
        mMessageSizes.add('Y',sizeof(trading::OrderApproved));
        mMessageSizes.add('Z',sizeof(trading::OrderRejected));

 
}

template <class Alocator>
StructData * Decoder<Alocator>::ensure(int aId,void* aMessage , size_t aSize)
{
        if (aSize){
            int i = 0;
            while (i<aSize){
                mBuffer.push_back(((char*)aMessage)[i]);
                i++;
            }
        }
        if (mBuffer.size() == 0) 
        {
            return nullptr;
        }
        char *lBuff = mBuffer.array_one().first;
        size_t lSize = mBuffer.size();
        size_t lMsgSize =0;
        do 
        {
            char MessageId = *lBuff;
            lMsgSize = mMessageSizes[MessageId];
            if (!lMsgSize) 
                mBuffer.pop_front();
            lBuff = mBuffer.array_one().first;
            lSize = mBuffer.size();
        }
        while (!lMsgSize || mBuffer.empty());

        if (mBuffer.size()<lMsgSize) return nullptr;
        
        if (!mBuffer.empty()) 
        {
            StructData * lData = (StructData*)mEncoderAllocator->Allocate (sizeof(StructData));
            lData->mClientId = aId;
            lData->mData = (void*)mEncoderAllocator->Allocate (lMsgSize);
            memcpy(lData->mData,lBuff,lMsgSize);
            lData->mLength = lMsgSize;
            int i = 0;
            while (i<lMsgSize && mBuffer.size()>0) 
            {
                mBuffer.pop_front();
                i++;
            }
            return lData;
        }
        return nullptr;
}

template <class Alocator>
void Decoder<Alocator>::Decode(void* aMessage, size_t aSize)
{
        char MessageId= 0;
        MessageId = (*(char*)aMessage);
        size_t lSize = mMessageSizes[MessageId];
        if (!lSize)
        {
            std::cout <<"Drop Packet: Packet Size|Msg Size ["<<aSize<<"|"<<lSize<< "]Messages Header "<<MessageId<<std::endl;
        }
        else if (lSize<=aSize) {
            mCallbacks[MessageId](aMessage);
        }
        else
        {
            std::cout <<"lost Packet: Packet Size|Msg Size ["<<aSize<<"|"<<lSize<< "]Messages Header"<<MessageId<<std::endl;
        }
}


#endif

