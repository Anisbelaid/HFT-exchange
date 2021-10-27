#ifndef MATCHING_ENVIREMENT_HPP
#define MATCHING_ENVIREMENT_HPP

#include <cstdlib>
#include <skeleton/Envirement.hpp>

namespace dispatcher {
    
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
            bool loadEnvirement();
    };
    
}

bool dispatcher::Envirement::loadEnvirement(){
    bool lResult = core::Envirement::loadEnvirement();
    return lResult;  
}; 

#endif 


