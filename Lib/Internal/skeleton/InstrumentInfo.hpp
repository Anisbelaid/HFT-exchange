#ifndef  _INSTRUMENT_INFO_HPP_
#define  _INSTRUMENT_INFO_HPP_

#include <atomic>

namespace core {
    
    /**
     * This is to be used carefully it is optimized to be used by a single reader single writer with a sequential access
     */
    class InstrumentInfo {
        private :
            std::atomic_uint64_t         mKey{0};
            mutable std::mutex            mSymbolMutex;
            std::string                  mSymbol{0};
            std::atomic_uint16_t         mTradingPhase{0};
            std::atomic_uint64_t         mHigh{0};
            std::atomic_uint64_t         mLow{0};
            std::atomic_uint64_t         mLast{0};
            std::atomic_uint64_t         mOpen{0};
            std::atomic_uint64_t         mClose{0};
        public :

            const uint64_t getKey () const {
                return mKey.load (std::memory_order_release);
            }

            std::string getSymbol () const 
            {
                std::lock_guard<std::mutex> lock(mSymbolMutex);
                return mSymbol;
            }
            
            const uint16_t getTradingPhase () const {
                return mTradingPhase.load (std::memory_order_release);
            }

            const uint16_t getClose () const {
                return mClose.load (std::memory_order_release);
            }

            const uint16_t getOpen () const {
                return mOpen.load (std::memory_order_release);
            }
            
            const uint64_t getHigh () const {
                return mHigh.load (std::memory_order_release);
            }
            
            const uint64_t getLow () const {
                return mLow.load (std::memory_order_release);
            }
            
            const uint64_t getLast () const {
                return mLast.load (std::memory_order_release);
            }
            
            void setKey (const uint64_t &aKey)  {
                return mKey.store (aKey,std::memory_order_acquire);
            }
            
            void setLast (const uint64_t &aKey)  {
                return mLast.store (aKey,std::memory_order_acquire);
            }

            void setClose (const uint64_t &aKey)  {
                return mClose.store (aKey,std::memory_order_acquire);
            }
            
            void setOpen (const uint64_t &aKey)  {
                return mOpen.store (aKey,std::memory_order_acquire);
            }
            void setHigh (const uint64_t &aHigh)  {
                return mHigh.store (aHigh,std::memory_order_acquire);
            }
            
            void setLow (const uint64_t &aLow)  {
                return mLow.store (aLow,std::memory_order_acquire);
            }
            
            void setTradingPhase  (const uint16_t &aTradingPhase)  {
                return mTradingPhase.store (aTradingPhase,std::memory_order_acquire);
            }

            void setSymbol  (const std::string& aSymbol)  {
                std::lock_guard<std::mutex> lock(mSymbolMutex);
                mSymbol=aSymbol;
            }
             
            
            
    };
}

#endif 
