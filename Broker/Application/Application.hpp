#ifndef _BROKER_APPLICATION_HPP
#define _BROKER_APPLICATION_HPP

#include <Session/SessionManager.hpp>
#include <skeleton/Application.hpp>
#include <UserInterface/UserInterface.hpp>

namespace broker{

    class BrokerApp : public core::application {
        public :
            
            BrokerApp () = default ;
            ~BrokerApp() = default;
            
            
            bool Initialize () {
                
                bool lResult = true;
                try {
                    Logger::initFromConfig("../config/logger.ini");
                    INFO(Broker,"Logger Configuration Loaded ...");
                }
                catch (...) {
                    std::cerr <<"Error loading logger configuration";
                    return false;
                }

                lResult &= core::application::Initialize();

                return lResult;
            };
            
        
            /**
            * @brief Create a Session Manager object
            * 
            * @return core::IServer* 
            */
            core::SessionManager * createSessionManager () override {
                    return new broker::SessionManager(mConfig);
            }

            /**
             * @brief Create a Configuration object
             * 
             * @return core::Configuration* gateway pointer
             */
            core::Configuration * createConfiguration () {return new broker::Configuration (mEnvirement->getConfigFolder(),"Broker.ini");}
            
                /**
                 * @brief Create a Envirement object
                 * 
                 * @return core::Envirement* gateway pointer
                 */
            core::Envirement    * createEnvirement () override {return new core::Envirement ();}

            /**
             * @brief Create a Signal Handler object
             * 
             * @return core::SignalHandler* pointer 
             */
            core::SignalHandler * createSignalHandler () override {return new core::SignalHandler ();}

            /**
             * @brief Create a Market Order object
             * 
             * @return IMarketOrder* 
             */
            core::IOrderManager   * createOrderManager () override {return nullptr;}

            core::UserInterface <>  * createUserInterface (core::SessionManager*aSessionManager,core::Configuration *aConfigurationManager) override { return new UserInterface(aSessionManager,aConfigurationManager);}
            
            virtual core::InstrumentManager<>   * createInstrumentManager () {return nullptr;};
            /**
             * @brief 
             * 
             */
            void stopThreads (){

            }
            

    };
}
#endif