#ifndef _LINEAR_ALLOCATOR_HPP
#define _LINEAR_ALLOCATOR_HPP

#include <boost/core/noncopyable.hpp>
#include <list>
#include <cstdint>

namespace core{
      
    class LinearAllocator : private boost::noncopyable
    {
        private:
            std::list<char*> mStart;
            std::size_t mOffset{0};
            std::size_t mTotalSize{0};
            uint64_t mAllignement{0};
            std::size_t mUsed{0};
            std::size_t mNbAllocation{0} ;
        public:
            LinearAllocator() = default ;
             ~LinearAllocator();

             char* Allocate(const std::size_t) ;
            
             void Init(const std::size_t,const uint16_t);

             const std::size_t CalculatePadding() ;
            
            void Dump ();
        
    }; 
    


LinearAllocator::~LinearAllocator() { 
    for (char * ptr : mStart)
    {   
        delete [] ptr;
        ptr=nullptr;
    } 
} 

void LinearAllocator::Init(const std::size_t aTotalSize, const uint16_t aAllignment) {
    mTotalSize = aTotalSize;
    mAllignement = aAllignment;
    char *lPtr = new char[mTotalSize];
    mStart.push_back(lPtr);
    mOffset = 0;
}

char* LinearAllocator::Allocate(const std::size_t size) 
{   
   mNbAllocation ++;
   std::size_t padding = 0;
   void * lPtr =mStart.back();
    if ( mOffset % 64 != 0) {
        padding = CalculatePadding();
    }
    if (mOffset + padding + size > mTotalSize) {
        char * ptr= new char[mTotalSize];
        mStart.push_back(ptr);
        mOffset = 0;
    }
    mOffset += padding;
    const std::size_t nextAddress = (std::size_t)mStart.back() + mOffset;
    mUsed += size;
    mOffset += size;    
    return (char*) nextAddress+1;
}

const std::size_t LinearAllocator::CalculatePadding() 
{
    std::size_t base  = reinterpret_cast<std::size_t>(mStart.back())+ mOffset;
    const std::size_t multiplier = (base/mAllignement) + 1;
    const std::size_t alignedAddress = multiplier * mAllignement;
    const std::size_t padding = alignedAddress - base;
    return padding;
}

void LinearAllocator::Dump() 
{   
    //ServerLog->info ("Number of object allocated {0:d} bytes",mNbAllocation);
    //ServerLog->info ("Total size of memory allocated {0:d} bytes",mTotalSize);
    //ServerLog->info ("Total size of used memory {0:d} bytes",mUsed);
    //ServerLog->info ("Total size of lost memory {0:d} bytes",mOffset-mUsed);
}
 
    
}
#endif
