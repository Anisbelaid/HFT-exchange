#ifndef _ARRAY_PRIMITIVE_HPP_
#define _ARRAY_PRIMITIVE_HPP_

#include <type_traits>
#include <memory>
# include <type_traits>
#include <container/CustomArray.hpp>
namespace core {
    
    /**
    * Implementation of a sequencer that will ensure that only the last element is treated from the all clients
    * @tparam element the type of data to handle in this array 
    * @tparam Low The lower bound index included
    * @tparam High The higher bound index included
    *
    */
    template <typename element,uint64_t LOW,uint64_t HIGH> 
    class CustomArray <element,LOW,HIGH,true> : private boost::noncopyable {
        static_assert(LOW >= 1, "Lower bound should be 1>=" );
        static_assert(LOW < HIGH, "Lower bound should be 1>=" );
        private:
            /// The min index holding some value
            uint64_t mMinIndex{HIGH-LOW};
            /// The max index holding some value
            uint64_t mMaxIndex{LOW};
            /// No value to be used to test existing of aan element at an index
            element  mNoValue;
            /// data of the array
            ///TODO think of using static storage
            //element mTab[HIGH-LOW];
            typename std::aligned_storage<sizeof(element), alignof(element)>::type mTab[HIGH-LOW+1];
            
        public :
            
            /**
            * Create array and set all the element to Novalue.
            * @return Constuctor
            */
            CustomArray(element aNoValue);
            
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
            element & operator[](int aIndex) ;
            
            /**
            * add an element at an index
            * @param aIndex the index in which we want to se aValue
            * @param aValue the element we want to set
            * @return true if the element is inserted false otherwise
            */
            bool add (uint64_t aIndex, element aValue);
            
            /**
            * add an element at the end of the array  
            * @param aValue the element we want to set
            * @return true if the element is inserted false otherwise
            */
            bool add (element aValue);
            
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
            element  begin() { return mTab[mMinIndex-LOW]; }
            
            //TODO get the mast element without get out of bounds
            /**
            * Get the last element of the container
            * @return a pointer to the last  element
            */
            element  end() { return mTab[mMaxIndex-LOW]; }
            /**
            *
            * @return a reference to the first Index
            */
            uint64_t firstIndex() { return  mMinIndex; }
            
            /**
            * 
            * @return a reference to the last Index
            */
            uint64_t lastIndex() { return  mMaxIndex; }
            
            /**
            * @param aIndex starting parameter
            * @return the next full element
            */
            element& next(uint64_t& aIndex); 
            
    };
}

template <class element,uint64_t LOW,uint64_t HIGH>
core::CustomArray<element,LOW,HIGH,true>::CustomArray(element aNoValue){
      mNoValue = aNoValue;
      for (int i=0 ;i<HIGH-LOW+1;i++){
           new (&mTab[i]) element(mNoValue) ; 
      }
}

template <class element,uint64_t LOW,uint64_t HIGH>
element & core::CustomArray<element,LOW,HIGH,true>::operator[](int aIndex){ 
    if (aIndex < LOW || aIndex > HIGH){
            return mNoValue;
    }
   // return mTab[aIndex-LOW];
    return *reinterpret_cast<element*> (&mTab[aIndex-LOW]);
}

template <class element,uint64_t LOW,uint64_t HIGH>
bool core::CustomArray<element,LOW,HIGH,true>::add(uint64_t aIndex, element aValue){
    if (aIndex < LOW || aIndex > HIGH){
            return false;
    }
    if (empty ()){
        mMinIndex =aIndex;
        mMaxIndex =aIndex;
    }
    else{
        if (aIndex< mMinIndex) mMinIndex = aIndex;
        if (aIndex> mMaxIndex) mMaxIndex = aIndex;
    }
    //*(reinterpret_cast<element*>(&mTab[aIndex-LOW]))=std::move(aValue);
    new (&mTab[aIndex-LOW]) element(aValue) ; 
    return true;
}

template <class element,uint64_t LOW,uint64_t HIGH>
bool core::CustomArray<element,LOW,HIGH,true>::add(element aValue){
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

template <class element,uint64_t LOW,uint64_t HIGH>
void core::CustomArray<element,LOW,HIGH,true>::remove(uint64_t aIndex){
    if (aIndex < LOW || aIndex > HIGH){
            return ;
    }
    
   // *(reinterpret_cast<element*>())=std::move(mNoValue);
    new (&mTab[aIndex-LOW]) element(mNoValue) ; 
    if (aIndex == mMinIndex) while (!operator[](mMinIndex)&&mMinIndex<=mMaxIndex) mMinIndex ++;
    if (aIndex == mMaxIndex) while (!operator[](mMaxIndex)&&mMaxIndex>=mMinIndex) mMaxIndex --;
}

template <class element,uint64_t LOW,uint64_t HIGH>
element &core::CustomArray<element,LOW,HIGH,true>::next(uint64_t &aIndex){
    if (aIndex  < LOW || aIndex+1 > HIGH){
            return mNoValue ;
    }
    while (*reinterpret_cast<element*> (&mTab[++aIndex-LOW])==mNoValue) {;}
    return (*reinterpret_cast<element*> (&mTab[aIndex-LOW]));
}

#endif

