#ifndef _USER_MANAGER_HPP
#define _USER_MANAGER_HPP

#include <skeleton/Configuration.hpp>
#include <pattern/Singleton.hpp>
#include <boost/tokenizer.hpp>
#include <vector>

namespace core {
    /**
     * @brief 
     * 
     */
    struct User {
        std::string         mUserName       {""}; 
        std::string         mUserLogin      {""};
        std::string         mUserPassword   {""};
        std::string         mType           {""};
    };
    /**
     * @brief 
     * 
     */
    class UserManager {
        protected :
            typedef std::map <std::string,User*> Users_;
            ///
            const std::string cUserSection        = "USER_";
            ///
            const std::string cNameParameter      = "NAME";
            ///
            const std::string cLoginParameter     = "LOGIN";
            ///
            const std::string cPasswordParameter  = "PASSWORD";
            ///
            const std::string cType               = "TYPE";
            ///
            core::Configuration *mConfiguration {nullptr};
            ///
            Users_               mUserList;
        
        public :

            /**
             * @brief Construct a new user manager object
             * 
             */
            UserManager () = default;
            
            void Initialize (core::Configuration *aConfiguration){
                mConfiguration = aConfiguration;
                loadUsers ();
            }
            /**
             * @brief 
             * 
             * @return true 
             * @return false 
             */
            bool loadUsers (){
                
                uint16_t lUserIndex = 1;
                
                std::string lUserSection = cUserSection + std::to_string (lUserIndex);
                
                bool lResult = true;

                while (mConfiguration->HasSection (lUserSection))
                {
                    User * lUser = new User ();
                    lResult&=mConfiguration->readMandatoryParameter<> (lUserSection,cNameParameter,lUser->mUserName);
                    lResult&=mConfiguration->readMandatoryParameter<> (lUserSection,cLoginParameter,lUser->mUserLogin);
                    lResult&=mConfiguration->readMandatoryParameter<> (lUserSection,cPasswordParameter,lUser->mUserPassword);
                    lResult&=mConfiguration->readMandatoryParameter<> (lUserSection,cType,lUser->mType);
                    if (!lResult)
                        break;
                    else
                    {
                        mUserList.insert (std::make_pair(lUser->mUserName,lUser));
                    }
                    ++lUserIndex;
                    lUserSection = cUserSection + std::to_string (lUserIndex);
                }
                return lResult;
            }


            User * Authentificate (const std::string & aLogin,const std::string & aPassword){
                for (auto lUserIt : mUserList){
                    if (lUserIt.second->mUserLogin ==aLogin && lUserIt.second->mUserPassword == aPassword)
                        return lUserIt.second;
                }
                return nullptr;
            }
            /**
             * @brief Destroy the user manager object
             * 
             */
            virtual ~UserManager() {
                for (auto lUser : mUserList){
                    delete lUser.second;
                }
                mUserList.clear();
            } ;

    };
}

#endif 