#ifndef  BASKET_HPP_
#define  BASKET_HPP_

#include <rapidcsv.h>
#include <Trading/Order.hpp>
#include <functional>
#include <thread>
#include <chrono>
#include <cstring>

using namespace std::chrono;

/**
* Implementation of a generic async TCP server with multiple connection allowed 
* @tparam MAX_SIZE Indcate the maximum of order to load.
* @tparam POST The type of callback to process orders 
*
*/
template <uint64_t MAX_SIZE = 1000000 , class POST = std::function <void (void*,size_t)> >
class Basket  {
    protected :
        ///Index in the file
        uint64_t mIndex {0};
        
        ///CSV Document 
        rapidcsv::Document mDoc; 
        
        ///The callback to perform
        POST mCallback;
        
        trading::Order * mOrder = nullptr;
        
        trading::Cancel * mCancel = nullptr;
        
    public : 
        
        /**
        * @brief Create a Basket Server.
        *
        * @param aPath the Path to the CSV File
        * @param aCallback the callcack used to process orders
        * @return Constuctor
        */
        Basket (std::string aPath,POST aCallback);
        
        /**
        * Destroy a Basket.
        *
        * @return Destuctor
        */
        ~Basket () ;
        
        /**
        * Play the basket.
        *
        * @return void
        */
        void play (uint32_t aFrequency);
        
        /**
        * Build Order from aIndex row.
        *
        * @return void
        */
        virtual void buidOrder  (uint64_t aPrice,uint64_t aQty,std::string& aSide);
        
        /**
        * Build a cancel from aIndex row.
        *
        * @return void
        */
        virtual void buildCancel (size_t aIndex);
        
        /**
        * Get next Order.
        *
        * @return void
        */
        trading::Order* next ();
};



template <uint64_t MAX_SIZE,class POST>
Basket<MAX_SIZE,POST>::Basket (std::string aPath,POST aCallback):mDoc(aPath),mCallback(aCallback){    
   mOrder  = new trading::Order ();
   mCancel = new trading::Cancel();

}

template <uint64_t MAX_SIZE,class POST >
Basket<MAX_SIZE,POST>::~Basket (){
    delete mOrder;
}

template <uint64_t MAX_SIZE,class POST>
void Basket<MAX_SIZE,POST>::buidOrder (uint64_t aPrice,uint64_t aQty,std::string& aSide){

    mOrder->BUYSELL = (aSide=="buy" ? 'B':'S');
    mOrder->TYPE = 'O';
    mOrder->PRICE = aPrice;
    mOrder->SHARES = aQty;
    mOrder->STOCK = 500;
    mCallback((void*)mOrder,sizeof(trading::Order));

}

template <uint64_t MAX_SIZE,class POST >
void Basket<MAX_SIZE,POST>::buildCancel (size_t aIndex){
    std::string lClientId = mDoc.GetCell<std::string>(1,aIndex);
    std::string lStock = mDoc.GetCell<std::string>(3,aIndex);
    uint64_t lSize  = mDoc.GetCell<uint32_t>(1,aIndex);
    uint64_t lPrice = mDoc.GetCell<uint32_t>(6,aIndex);
    std::string lSide = mDoc.GetCell<std::string>(8,aIndex);
    std::string lTif = mDoc.GetCell<std::string>(1,aIndex);
    mOrder->BUYSELL= (lSide=="buy" ? 'B':'S');
    mOrder->TYPE = 'C';
    mOrder->PRICE = lPrice;
    mOrder->SHARES = lSize;
    mOrder->STOCK = 1234;
    std::memcpy (mOrder->CLIENTORDERID,lClientId.c_str(),lClientId.size());
}

template <uint64_t MAX_SIZE,class POST >
void Basket<MAX_SIZE,POST>::play (uint32_t aFrequency){
    
    std::vector<std::string> type = mDoc.GetColumn<std::string>("Event Type");
    std::vector<uint64_t> price = mDoc.GetColumn<uint64_t>("Limit Price (USD)");
    std::vector<uint64_t> qty = mDoc.GetColumn<uint64_t>("Original Quantity (BTC)");
    std::vector<std::string> side = mDoc.GetColumn<std::string>("Side");
    int lIndex = 0;
    while (lIndex<type.size())
    {
        if (type[lIndex]=="Place")
            buidOrder(price[lIndex],qty[lIndex],side[lIndex]);
        lIndex++;
    }
}

template <uint64_t MAX_SIZE,class POST >
trading::Order* Basket<MAX_SIZE,POST>::next (){
    buidOrder (mIndex);
    mIndex++;
    return mOrder;
}


#endif




