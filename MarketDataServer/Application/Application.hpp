#ifndef MARKET_DATA_APPLICATION_HHP
#define MARKET_DATA_APPLICATION_HHP

#include <skeleton/Application.hpp>

#include <Application/Defines.hpp>
#include <Application/Configuration.hpp>
#include <Application/Envirement.hpp>
#include <Session/InstrumentManager.hpp>
#include <Session/SessionManager.hpp>

#include <MarketOrder/MarketOrder.hpp>

#include <config.h.in>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/exception_ptr.hpp> 

namespace marketdata
{
    class MarketDataApp : public core::application {
        public :
            
            MarketDataApp () = default ;
            ~MarketDataApp() = default;
            
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
            
            /**
            * @brief Create a Session Manager object
            * 
            * @return core::IServer* 
            */
            core::SessionManager * createSessionManager () override {
                return new marketdata::SessionManager(mConfig);
            }

            /**
             * @brief Create a Configuration object
             * 
             * @return core::Configuration* 
             */
            core::Configuration * createConfiguration () {return new marketdata::Configuration (mEnvirement->getConfigFolder(),"marketdata.ini");}
            /**
             * @brief Create a Envirement object
             * 
             * @return core::Envirement* 
             */
            core::Envirement    * createEnvirement () {return new marketdata::Envirement ();}
            /**
             * @brief Create a Signal Handler object
             * 
             * @return core::SignalHandler* 
             */
            core::SignalHandler * createSignalHandler () {return new core::SignalHandler ();}

            /**
             * @brief Create a Market Order object
             * 
             * @return IMarketOrder* 
             */
            core::IOrderManager   * createOrderManager () override {return new marketdata::OrderManager ();}

            /**
            * Create a Server Object that wil be launched after startup
            * @return aPointer to a session server Handler implementation 
            */
            virtual core::InstrumentManager<>    * createInstrumentManager () {return new marketdata::InstrumentManager("instrument manager",500);}

             virtual void stopThreads () {}
            
    };
}
#endif