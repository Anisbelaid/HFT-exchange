#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP

#include <boost/core/noncopyable.hpp>
#include <mutex>

namespace core { 
    /**
     * @brief a generic singleton
     * 
     * @tparam Instance type 
     */
    template <typename Instance>
    class Singleton : private boost::noncopyable {
        
        public:
            /**
             * @brief Get the unique Instance object
             * 
             * @return Instance* 
             */
            static Instance *  getInstance (){
                if (!mInstance){
                    std::lock_guard <std::mutex>lLock(mMutex);
                    if (!mInstance)
                        mInstance = new Instance ();
                }
                return mInstance;
            }
            
            /**
             * @brief Destroy the singleton Instance
             * 
             */
            static void  DestroyInstance (){
                if (mInstance){
                    std::lock_guard<std::mutex> lLock(mMutex);
                    if (mInstance)
                        delete mInstance ;
                }
            }
            
        protected:
            /**
             * @brief Construct a new Singleton object
             * 
             */
            Singleton()  = default;

            /**
             * @brief Destroy the Singleton object
             * 
             */
            virtual ~Singleton () = default ;
            
        private:
            /// A mutex to protect singleton creation
            static std::mutex mMutex ;
            /// Private pointer to singleton Instance
            static Instance *mInstance ;
    };
}


template <typename Instance>
Instance * core::Singleton<Instance>::mInstance = nullptr;

template <typename Instance>
std::mutex core::Singleton<Instance>::mMutex ;

#endif
