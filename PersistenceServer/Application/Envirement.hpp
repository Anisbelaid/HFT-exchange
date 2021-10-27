#ifndef _PERSISTENCE_ENVIREMENT_HPP
#define _PERSISTENCE_ENVIREMENT_HPP

#include <cstdlib>
#include <Envirement.hpp>

namespace marketdata {
    
    /**
    *   A class helper to get envirement variable  
    *
    */
    class Envirement  : public core::Envirement  {
            
        public :
            /**
            * Default Contructor.
            */
            Envirement() = default;
            
            /**
            * Default Destructor.
            */
            virtual ~Envirement () = default;
            
            /**
            * load gateway envirement variable.
            */
            bool loadEnvirement(){
                bool lResult = core::Envirement::loadEnvirement();
                return lResult;  
            }; 
    };
    
}
#endif 


