#ifndef GEMINI_BASKET_HPP
#define GEMINI_BASKET_HPP

#include "Basket.hpp"

class gemini : Basket<>{

    public :
        gemini (std::string aPath,std::function <void (void*,size_t)> aCallback):Basket<>(aPath,aCallback){};
        
        /**
        * Build Order from aIndex row.
        *
        * @return void
        */
        virtual void buidOrder (size_t aIndex);
        
        /**
        * Build a cancel from aIndex row.
        *
        * @return void
        */
        virtual void buildCancel (size_t aIndex);
        
        /**
        * Play the basket.
        *
        * @return void
        */
        virtual void play (uint32_t aFrequency);
    
};



void gemini::buidOrder (size_t aIndex){
    
    std::string lTif = mDoc.GetCell<std::string>(-1,aIndex);
    
    TimeInForce_ lTif_int ;
    if (lTif == "fill-or-kill")
        lTif_int = TimeInForce_::FOK;
    else if (lTif == "immediate-or-cancel")
        lTif_int = TimeInForce_::IOC;
    else
        lTif_int =TimeInForce_::DAY;
        

    double lSize = mDoc.GetCell<double>(9,aIndex);
    
    const double multiplier = std::pow(10.0, 7);
    
    uint64_t lSize_int =  std::ceil(lSize * multiplier) ;
    
    std::string lStock = mDoc.GetCell<std::string>(3,aIndex);
    
    //uint64_t lClientId  = mDoc.GetCell<uint64_t>(1,aIndex);
    
    std::string lSide = mDoc.GetCell<std::string>(8,aIndex);
    
    double lPrice = mDoc.GetCell<double>(6,aIndex);
    
    std::string lAccount = mDoc.GetCell<std::string>(5,aIndex);
    
    uint32_t lPrice_int = lPrice * 100;
    
    //std::string lOrderType = mDoc.GetCell<std::string>(11,i);
    //std::cout <<"lOrderType "<< i <<" = "<< lOrderType <<std::endl;
    
    mOrder->BUYSELL = (lSide=="buy" ? 'B':'S');
    mOrder->TYPE = 'O';
    mOrder->PRICE = lPrice_int;
    mOrder->SHARES = lSize_int;
    mOrder->STOCK = 500;//std::hash<std::string>{}(lStock);
    //mOrder->CLIENTID = lClientId ;
    mOrder->TIMEINFORCE = (uint8_t)lTif_int;
    mOrder->SESSIONID   = std::hash<std::string>{}(lAccount);
}


void gemini::buildCancel (size_t aIndex){
        
    double lSize = mDoc.GetCell<double>(9,aIndex);
    
    const double multiplier = std::pow(10.0, 7);
    
    uint64_t lSize_int =  std::ceil(lSize * multiplier) ;
    
    std::string lStock = mDoc.GetCell<std::string>(3,aIndex);
    
    std::string lAccount = mDoc.GetCell<std::string>(5,aIndex);
    
    //uint64_t lClientId  = mDoc.GetCell<uint64_t>(1,aIndex);
    
    std::string lSide = mDoc.GetCell<std::string>(8,aIndex);
    
    double lPrice = mDoc.GetCell<double>(6,aIndex);
    
    uint32_t lPrice_int = lPrice * 100;
    
    mCancel->BUYSELL = (lSide=="buy" ? 'B':'S');
    mCancel->TYPE = 'C';
    mCancel->SHARES = lSize_int;
    mCancel->STOCK = 500;//std::hash<std::string>{}(lStock);
    //std::memcpy (mCancel->CLIENTID,lClientId,7);
    mCancel->SESSIONID = std::hash<std::string>{}(lAccount);
}

void gemini::play (uint32_t aFrequency){
    
    std::string lAction = "";
    for (int i = 0; i < mDoc.GetRowCount(); ++i){
      try{
        lAction = mDoc.GetCell<std::string>(7,i);
      }
      catch (...)
      {
          std::cerr <<"Error loading gemini row"<<i<<std::endl;
      }
    
      if (lAction == "Place"){
        buidOrder (i);
        mCallback ((void*)mOrder,sizeof(trading::Order));
      }
      else if (lAction == "Cancel"){
        buildCancel (i);
        mCallback ((void*)mCancel,sizeof(trading::Cancel));
      }
      
      std::this_thread::sleep_for(std::chrono::nanoseconds(aFrequency));
    }
}

#endif



