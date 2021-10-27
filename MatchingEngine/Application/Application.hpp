#ifndef MATCHING_ENGINE_APPLICATION_HPP_
#define MATCHING_ENGINE_APPLICATION_HPP_


#include <Session/SessionManager.hpp>
#include <skeleton/Application.hpp>

#include <Application/Defines.hpp>
#include <Application/Configuration.hpp>
#include <Application/Envirement.hpp>

#include <MatchingEngine/OrderBook.hpp>

#include <config.h.in>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/exception_ptr.hpp> 

namespace matching
{
    /**
     * @brief main application class fromm which all thread wiil be launched
     * 
     */
    class MatchingApp : public core::application {

        public :
            MatchingApp (){};

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
                    return new matching::SessionManager(mConfig);
            }

            /**
             * @brief Create a Configuration object
             * 
             * @return core::Configuration* gateway pointer
             */
            core::Configuration * createConfiguration () {return new matching::Configuration (mEnvirement->getConfigFolder(),"matching.ini");}
            
                /**
                 * @brief Create a Envirement object
                 * 
                 * @return core::Envirement* gateway pointer
                 */
            core::Envirement    * createEnvirement () override {return new matching::Envirement ();}

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
            core::IOrderManager   * createOrderManager () override {return new matching::OrderManager ();}

            core::InstrumentManager<> * createInstrumentManager () {return nullptr;};

    };
}
 #endif