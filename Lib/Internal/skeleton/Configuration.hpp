#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP


#include <logger/OutputListner.hpp>
#include <INIReader.h>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <type_traits>

namespace core {
    
    /**
    *   @brief A class helper to get configuration variable  
    *   @details This class will hold generic metho that will read parameters .
    *            example : read a string , read an integer , read a mandatory fields 
    */
    class Configuration  : public core::OutputListner {
        private :
            /// Configuration file reader
            INIReader *mConfReader;
        protected :
            /// Configuration file directory
            std::string mConfFolder;
            /// Configuration file name
            std::string mFileName;

        public :           
            /**
             * @brief read an optionnal name from the configurtion file
             * 
             * @tparam type of parammeter
             * @param[in] Section section in the ini file 
             * @param[in] aParamName parameter name 
             * @param[in] aDefaultValue default value of the parameter
             * @param[out] aOutputValue result value
             * @return true if parameter is found 
             *         false if parameter is not fund
             */
            template <typename type>
            bool readOptionalParameter (std::string aSection,std::string aParamName,type aDefaultValue,type& aOutputValue)
            {
                if (!mConfReader)
                    onError ("no configuration file");
                
                if (!mConfReader->HasValue (aSection,aParamName))
                {
                    std::stringstream lLog;
                    lLog<<aSection<<":"<<aParamName<<" not found .Default parameter ("<<aDefaultValue<<") will be set";
                    onInfo (lLog.str().c_str());
                    aOutputValue = aDefaultValue;
                    return false;
                }
                else 
                {
                    try
                    {
                        std::string lOutput ="";
                        lOutput = mConfReader->Get (aSection,aParamName,lOutput);
                        aOutputValue = boost::lexical_cast<type> (lOutput);
                        onInfo ((aSection+":"+aParamName+"  found .Parameter value ("+lOutput+") will be set").c_str());
                        return true;
                    }
                    catch (const boost::bad_lexical_cast &e)
                    {
                         std::stringstream lLog;
                         lLog<<aSection<<":"<<aParamName<<" unexpected value .Default parameter ("<<aDefaultValue<<") will be set";
                         onWarning (lLog.str().c_str());
                         aOutputValue = aDefaultValue;
                        return false;
                    }
                }
                
            }

            /**
             * @brief read an optionnal name from the configurtion file
             * 
             * @tparam type of parammeter
             * @param[in] Section section in the ini file 
             * @param[in] aParamName parameter name 
             * @param[out] aOutputValue result value
             * @return true if parameter is found 
             *         false if parameter is not fund
             */
            template <typename type>
            bool readMandatoryParameter (std::string aSection,std::string aParamName,type& aOutputValue){
                if (!mConfReader)
                    onError ("no configuration file");
                
                if (!mConfReader->HasValue (aSection,aParamName))
                {
                    onError ((aSection+":"+aParamName+" not found ").c_str());
                    return false;
                }
                else 
                {
                    try
                    {
                        std::string lOutput ="";
                        lOutput = mConfReader->Get (aSection,aParamName,lOutput);
                        aOutputValue = boost::lexical_cast<type> (lOutput);
                        onInfo ((aSection+":"+aParamName+"  found .parameter value ("+lOutput+") will be set").c_str());
                        return true;
                    }
                    catch (const boost::bad_lexical_cast &e)
                    {
                        onError ((aSection+":"+aParamName+" unexpected value ").c_str());
                        return false;
                    }
                } 
            }
            
            /**
             * @brief 
             * 
             * @param aSection 
             * @return true 
             * @return false 
             */
            bool HasSection (const std::string& aSection){
                return mConfReader->HasSection(aSection);
            }
            /**
            * @brief Default Contructor.
            * @param[in] aConfFolder path to the confifuration folder 
            */
            Configuration(std::string aConFolder,std::string aFileNme) ;

            
            /**
            * @brief Load the configuration file at the indicated Path.
            */
            bool loadConfigurationFile ();

            /**
            * @brief Load the parameters.
            */
            virtual bool load () = 0;

            
            /* Default Destructor.
            */
            virtual ~Configuration () = default ;
            
    };
    
    
}

core::Configuration::Configuration(std::string aConFolder,std::string aFileNme) :mConfFolder(aConFolder),mFileName(aFileNme) {
}

bool core::Configuration::loadConfigurationFile() {
        std::string lConfilConfiguratione = core::Configuration::mConfFolder+"/"+mFileName;
        onInfo (("Loading Configuraion file at "+lConfilConfiguratione).c_str());
        mConfReader =  new INIReader (lConfilConfiguratione);
        if (mConfReader->ParseError() < 0) {
            onError("Error reading configuration file ");
            return false;
        }
        else
        {
            onInfo("Configuration file Loaded");
            return true;
        }
}


#endif 


