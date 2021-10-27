#ifndef GATEWAY_ENVIREMENT_HPP
#define GATEWAY_ENVIREMENT_HPP

#include <skeleton/Envirement.hpp>

namespace gateway {
    
    /**
     * @brief Enriement vaiable container for Gateway
     *        inherit from API enrirement
     */
    class Envirement  : public core::Envirement  {
            
        public :
            /**
             * @brief Construct a new Envirement object
             * 
             */
            Envirement() = default;
            
            /**
             * @brief Destroy the Envirement object
             * 
             */
            ~Envirement () = default;
            
            /**
             * @brief load envirement variables
             * 
             * @return true if all envirment variable were fully loaded
             * @return false if ot
             */
            bool loadEnvirement(){
                bool lResult = core::Envirement::loadEnvirement();
                return lResult;  
            }; 
    };
    
}
#endif 


