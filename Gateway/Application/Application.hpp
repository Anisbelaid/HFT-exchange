#ifndef GATEWAY_APPLICATION_HPP_
#define GATEWAY_APPLICATION_HPP_


#include <Session/SessionManager.hpp>
#include <Session/InstrumentManager.hpp>
#include <skeleton/Application.hpp>

#include <Application/Defines.hpp>
#include <Application/Configuration.hpp>
#include <Application/Envirement.hpp>

#include <OrderManager/OrderManager.hpp>

#include <config.h.in>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/exception_ptr.hpp> 

namespace gateway
{
    /**
     * @brief main application class fromm which all thread wiil be launched
     * 
     */
    class GatewayApp : public core::application {

        public :
            GatewayApp (){

            };

            bool Initialize () override  {
                bool lResult = true;

                try {
                    Logger::initFromConfig("../config/logger.ini");
                }
                catch (const boost::exception& ex) {
                    std::cerr <<"Error loading logger configuration"<< boost::diagnostic_information(ex);
                    return false;
                }

                lResult &=  core::application::Initialize();

                return lResult;
            };
            
            void stopThreads () {
                mOrderManager->stop();
            };


            /**
            * @brief Create a Session Manager object
            * 
            * @return core::IServer* 
            */
            core::SessionManager * createSessionManager () override {
                    return new gateway::SessionManager(mConfig);
            }

            /**
             * @brief Create a Configuration object
             * 
             * @return core::Configuration* gateway pointer
             */
            core::Configuration * createConfiguration () {return new gateway::Configuration (mEnvirement->getConfigFolder(),"gateway.ini");}
            
                /**
                 * @brief Create a Envirement object
                 * 
                 * @return core::Envirement* gateway pointer
                 */
            core::Envirement    * createEnvirement () override {return new gateway::Envirement ();}

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
            core::IOrderManager   * createOrderManager () override {return new gateway::OrderManager (core::application::mInstrumentManager);}

            core::InstrumentManager<>   * createInstrumentManager () {return new gateway::InstrumentManager();};

    };
}
 #endif