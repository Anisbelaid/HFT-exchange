#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <type_traits>
#include <memory>
#include <type_traits>
#include <bitset>
#include <boost/core/noncopyable.hpp>
#include <cmath>
#include <iostream>
#include <limits>
namespace core {
    
    constexpr long mask0 = 1;
    constexpr long mask1 = 2;
    constexpr long mask2 = 4;
    constexpr long mask3 = 8;
    constexpr long mask4 = 16;
    constexpr long mask5 = 32;
    constexpr long mask6 = 64;
    constexpr long mask7 = 128;
    constexpr long mask8 = 256;
    constexpr long mask9 = 512;
    constexpr long mask10 = 1024;
    constexpr long mask11 = 2048;
    constexpr long mask12 = 4096;
    constexpr long mask13 = 8192;
    constexpr long mask14 = 16384;
    constexpr long mask15 = 32768;
    constexpr long mask16 = 65536;
    constexpr long mask17 = 131072;
    constexpr long mask18 = 262144;
    constexpr long mask19 = 524288;
    constexpr long mask20 = 1048576;
    constexpr long mask21 = 2097152;
    constexpr long mask22 = 4194304;
    constexpr long mask23 = 8388608;
    constexpr long mask24 = 16777216;
    constexpr long mask25 = 33554432;
    constexpr long mask26 = 67108864;
    constexpr long mask27 = 134217728;
    constexpr long mask28 = 268435456;
    constexpr long mask29 = 536870912;
    constexpr long mask30 = 1073741824;
    constexpr long mask31 = 2147483648;
    constexpr long mask32 = 4294967296;
    
    constexpr long nmask0 = ~mask0;
    constexpr long nmask1 = ~mask1;
    constexpr long nmask2 = ~mask2;
    constexpr long nmask3 = ~mask3;
    constexpr long nmask4 = ~mask4;
    constexpr long nmask5 = ~mask5;
    constexpr long nmask6 = ~mask6;
    constexpr long nmask7 = ~mask7;
    constexpr long nmask8 = ~mask8;
    constexpr long nmask9 = ~mask9;
    constexpr long nmask10 = ~mask10;
    constexpr long nmask11 = ~mask11;
    constexpr long nmask12 = ~mask12;
    constexpr long nmask13 = ~mask13;
    constexpr long nmask14 = ~mask14;
    constexpr long nmask15 = ~mask15;
    constexpr long nmask16 = ~mask16;
    constexpr long nmask17 = ~mask17;
    constexpr long nmask18 = ~mask18;
    constexpr long nmask19 = ~mask19;
    constexpr long nmask20 = ~mask20;
    constexpr long nmask21 = ~mask21;
    constexpr long nmask22 = ~mask22;
    constexpr long nmask23 = ~mask23;
    constexpr long nmask24 = ~mask24;
    constexpr long nmask25 = ~mask25;
    constexpr long nmask26 = ~mask26;
    constexpr long nmask27 = ~mask27;
    constexpr long nmask28 = ~mask28;
    constexpr long nmask29 = ~mask29;
    constexpr long nmask30 = ~mask30;
    constexpr long nmask31 = ~mask31;
    constexpr long nmask32 = ~mask32;
    
    constexpr long  getMask (int aPos){
        switch (aPos){
            case 0  : return mask0; break;
            case 1  : return mask1; break;
            case 2  : return mask2; break;
            case 3  : return mask3; break;
            case 4  : return mask4; break;
            case 5  : return mask5; break;
            case 6  : return mask6; break;
            case 7  : return mask7; break;
            case 8  : return mask8; break;
            case 9  : return mask9; break;
            case 10 : return mask10; break;
            case 11 : return mask11; break;
            case 12 : return mask12; break;
            case 13 : return mask13; break;
            case 14 : return mask14; break;
            case 15 : return mask15; break;
            case 16 : return mask16; break;
            case 17 : return mask17; break;
            case 18 : return mask18; break;
            case 19 : return mask19; break;
            case 20 : return mask20; break;
            case 21 : return mask21; break;
            case 22 : return mask22; break;
            case 23 : return mask23; break;
            case 24 : return mask24; break;
            case 25 : return mask25; break;
            case 26 : return mask26; break;
            case 27 : return mask27; break;
            case 28 : return mask28; break;
            case 29 : return mask29; break;
            case 30 : return mask30; break;
            case 31 : return mask31; break;
            case 32 : return mask32; break;
            default : return 0; break;
        }
    }
    
    constexpr long  getNMask (int aPos){
        switch (aPos){
            case 0  : return nmask0; break;
            case 1  : return nmask1; break;
            case 2  : return nmask2; break;
            case 3  : return nmask3; break;
            case 4  : return nmask4; break;
            case 5  : return nmask5; break;
            case 6  : return nmask6; break;
            case 7  : return nmask7; break;
            case 8  : return nmask8; break;
            case 9  : return nmask9; break;
            case 10 : return nmask10; break;
            case 11 : return nmask11; break;
            case 12 : return nmask12; break;
            case 13 : return nmask13; break;
            case 14 : return nmask14; break;
            case 15 : return nmask15; break;
            case 16 : return nmask16; break;
            case 17 : return nmask17; break;
            case 18 : return nmask18; break;
            case 19 : return nmask19; break;
            case 20 : return nmask20; break;
            case 21 : return nmask21; break;
            case 22 : return nmask22; break;
            case 23 : return nmask23; break;
            case 24 : return nmask24; break;
            case 25 : return nmask25; break;
            case 26 : return nmask26; break;
            case 27 : return nmask27; break;
            case 28 : return nmask28; break;
            case 29 : return nmask29; break;
            case 30 : return nmask30; break;
            case 31 : return nmask31; break;
            case 32 : return nmask32; break;
            default : return 0; break;
        }
    }
    
    
    
    template <uint64_t NB_ELEMENT>
    class bitmap {
        std::array <uint32_t,NB_ELEMENT> mTab;
        public :

          void set (int pos){
            int x= (pos/32);
            int y= (pos%32);
            mTab[x] |= getMask(y);
        }
        
        void reset (int pos){
            int x= (pos/32);
            int y = (pos%32);
            mTab[x] &= getNMask(y);
        }

        bool get (int pos){
            int x = (pos)/32;
            int y = pos%32;
            return mTab[x]&getMask(y);
        } 

	int next (int pos){

	   int x = (pos)/32;
	   
	   int y = pos%32;
	  
	   mTab[x] &= getNMask(y);
       
       int lResult = x*32;
       
       while ( (x<= NB_ELEMENT/32) && (!mTab[x]))
	   {
         lResult +=32;
         x++;
	   };
        
        if (mTab[x]){
            lResult += __builtin_ctz(mTab[x]);
        }
        return lResult;
    }

	int previous (int pos){
	   int x = (pos)/32;
	   int y = pos%32;
       mTab[x] &= getNMask(y);
       int dist = 0;
       
	   	while ( (!mTab[x]) && (x >= 0) )
		{
		     dist += 32;
		     x--;
		}
		if (mTab[x]) {
			dist = x*32;
			dist+= __builtin_ctz(mTab[x]);
		}
	   return dist;
	}
		
		
};

    /**
    * Implementation of a Custom Array  that will ensure that store Data from LOW to HIGH INDEX and have MIN/MAX bounds that varies on chnage
    * @tparam element the type of data to handle in this array 
    * @tparam Low The lower bound index included
    * @tparam High The higher bound index included
    *
    */
    template <typename element,uint64_t LOW,uint64_t HIGH ,bool ISPRIMITIVE = std::is_fundamental<element>::value> 
    class CustomArray : private boost::noncopyable {
        static_assert(LOW >= 1, "Lower bound should be 1>=" );
        static_assert(LOW < HIGH, "Lower bound should be 1>=" );
        private:
            /// The min index holding some value
            uint64_t mMinIndex{HIGH};
            /// The max index holding some value
            uint64_t mMaxIndex{LOW};
            /// No value to be used to test existing of aan element at an index
            element * mNoValue{0};
            /// data of the array
            ///TODO think of using static storage
            element * mTab[HIGH-LOW+1];
            //typename std::aligned_storage<sizeof(element*), alignof(element*)>::type mTab[HIGH-LOW];
            bitmap <HIGH-LOW> mBitmap;
        public :
            
            /**
            * Create array and set all the element to Novalue.
            * @return Constuctor
            */
            CustomArray(element*  aNoValue);
            
            /**
            * Destroy all the element : element should be freed before destroying Container
            * @return Destructor
            */
            ~CustomArray()= default;
            
            /**
            * Destroy all the element : element should be freed before destroying container
            * @param aIndex to the elemnt we want to get
            * @return a reference to element at index
            */
            element * operator[](int aIndex) ;
            
            /**
            * add an element at an index
            * @param aIndex the ind
            * ex in which we want to se aValue
            * @param aValue the element we want to set
            * @return true if the element is inserted false otherwise
            */
            bool add (uint64_t aIndex, element* aValue);
            
            /**
            * add an element at the end of the array  
            * @param aValue the element we want to set
            * @return true if the element is inserted false otherwise
            */
            bool add (element* aValue);
            
            /**
            * remove an element at aIndex
            * @param aIndex to the elemnt we want to delete
            * @return a reference to the last Index
            */
            void remove(uint64_t aIndex) ;
            
            /**
            * Chech if the container is empty
            * @return true if the Container is empty false otherwise
            */
            bool empty () {return mMinIndex>mMaxIndex;}
            
            /**
            * Chech if the container is full
            * @return true if the Container is full false otherwise
            */
            bool full () {return (mMinIndex<LOW || mMaxIndex>HIGH);}
            
            
            /**
            * Get the first element of the container
            * @return a pointer to the first  element
            */
            element * begin() { return mTab[mMinIndex]; }
            
            //TODO get the mast element without get out of bounds
            /**
            * Get the last element of the container
            * @return a pointer to the last  element
            */
            element * end() { return mTab[mMaxIndex]; }
            /**
            *
            * @return a reference to the first Index
            */
            uint64_t firstIndex() { return  mMinIndex+LOW; }
            
            /**
            * 
            * @return a reference to the last Index
            */
            uint64_t lastIndex() { return  mMaxIndex+LOW; }
            
            /**
            * @return the next full element
            */
            uint64_t next();
            
            /**
            * @return the next full element
            */
            uint64_t previous(); 
            
    };
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::CustomArray(element *aNoValue){
      mNoValue = aNoValue;
      for (int i=0 ;i<HIGH-LOW;i++){
            mTab[i] = aNoValue;
      }
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
element * core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::operator[](int aIndex){ 
    if (aIndex < LOW || aIndex > HIGH){
            return mNoValue;
    }
    if (mBitmap.get(aIndex-LOW))
        return mTab[aIndex-LOW];
    else
    {
        return mNoValue;
    }
    //return reinterpret_cast<element*> (&mTab[aIndex-LOW]);
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
bool core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::add(uint64_t aIndex, element* aValue){
    if (aIndex < LOW || aIndex > HIGH){
            return false;
    }
    
    if (empty ()){
        mMinIndex =aIndex-LOW;
        mMaxIndex =aIndex-LOW;
    }
    else{
        if (aIndex< mMinIndex) mMinIndex = aIndex-LOW;
        if (aIndex> mMaxIndex) mMaxIndex = aIndex-LOW;
    }
    mTab[aIndex-LOW] = aValue;
    mBitmap.set(aIndex-LOW);

    return true;
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
bool core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::add(element * aValue){
    if (mMaxIndex > HIGH){
        return false;
    }
    
    if (empty()){
        add(LOW,aValue);
        return true;
    }

    add(mMaxIndex+1,aValue);
    return true;
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
void core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::remove(uint64_t aIndex){
    if (aIndex < LOW || aIndex > HIGH){
            return ;
    }
    mTab[aIndex-LOW] = mNoValue;
    mBitmap.reset(aIndex-LOW);
    if (aIndex == mMinIndex) while (!operator[](mMinIndex)&&mMinIndex<=mMaxIndex) mMinIndex ++;
    if (aIndex == mMaxIndex) while (!operator[](mMaxIndex)&&mMaxIndex>=mMinIndex) mMaxIndex --;
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
uint64_t core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::next(){
    if (empty()) return std::numeric_limits<uint64_t>::max();
    if (mMinIndex == mMaxIndex) {
        mBitmap.reset (mMinIndex);
        mMinIndex++;
        return std::numeric_limits<uint64_t>::max();
    }

    mMinIndex = mBitmap.next(mMinIndex);
    return mMinIndex+LOW;
}

template <class element,uint64_t LOW,uint64_t HIGH,bool ISPRIMITIVE>
uint64_t core::CustomArray<element,LOW,HIGH,ISPRIMITIVE>::previous(){
    if (empty()) return 0;
    if (mMinIndex == mMaxIndex) {
        mBitmap.reset (mMaxIndex);
        mMaxIndex--;
        return 0;
    }
    mMaxIndex = mBitmap.previous(mMaxIndex);
    return mMaxIndex+LOW;
}

#endif
