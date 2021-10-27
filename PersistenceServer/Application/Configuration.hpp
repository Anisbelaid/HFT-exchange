#ifndef _PERSISITENCE_CONFIGURATION_HPP
#define _PERSISITENCE_CONFIGURATION_HPP

#include <Application/Defines.hpp>
#include <Configuration.hpp>

namespace marketdata {
    
    /**
    *   A class helper to get envirement variable  
    *
    */
    class Configuration : public core::Configuration {
            std::string mFileName;
        public :
            /**
            * Default Contructor.
            */
            Configuration(std::string aConFolder,std::string aFileName) ;
            
            bool loadConfiguration() {
                std::string lConfile = core::Configuration::mConfFolder+"/"+mFileName;
                INFO(PERSISTANCE,"Loading Configuraion file at "<<lConfile);
                mConfReader =  new INIReader (lConfile);
                if (mConfReader->ParseError() < 0) {
                    ERRORR(PERSISTANCE,"Error reading configuration file "<<lConfile);
                    return false;
                }
                return true;
            }
           
            std::string getDataBasePath (){return mConfReader->Get("DATABASE", "PATH", "");} 
             
            std::string  getServerAddress (){return mConfReader->Get("SERVER", "Address", "127.0.0.1");} 
         
            int  getServerPort (){ return mConfReader->GetInteger("SERVER", "Port", -1);} 
            

            /**
            * Default Destructor.
            */
            virtual ~Configuration () = default;
            
    };
    
}

marketdata::Configuration::Configuration(std::string aConfFolder,std::string aFileName):core::Configuration(aConfFolder),mFileName(aFileName) {
}


#endif 



