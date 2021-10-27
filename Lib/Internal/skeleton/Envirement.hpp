#ifndef ENVIREMENT_HPP
#define ENVIREMENT_HPP

#include <string>
#include <cstdlib>

namespace core {
    
    /**
    *   Aclass helper to get envirement variable  
    *
    */
    class Envirement   {
        protected :
            /// Path to the configuration file folder  
            std::string mConfigurationFolderPath;
            
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
            * Getter.
            * @return ConfigurationFolderPath
            */
            const std::string& getConfigFolder () const { return mConfigurationFolderPath; }
            
            
            /**
            * Sets exit signal to true.
            */
            virtual bool loadEnvirement(){
                if(const char* env_p = std::getenv("CONF_FOLDER")){
                    mConfigurationFolderPath=env_p;
                    return true;
                }
                else{
                    return false;
                }
                
            }; 
    };
    
}
#endif 

