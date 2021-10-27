#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <server/IServer.hpp>
#include <logger/LoggerClient.hpp>  
#include <skeleton/SignalManager.hpp>
#include <skeleton/Envirement.hpp>
#include <skeleton/UserInterface.hpp>
#include <skeleton/InstrumentManager.hpp>
#include <skeleton/Configuration.hpp>
#include <skeleton/SessionManager.hpp>
#include <pattern/Observer.hpp>
#include <sequencer/Sequencer.hpp>
#include <functionnal/IOrderManager.hpp>


namespace core {
    
    /**
    *   Represent the main program 
    *
    */
    class application  : public Observer  
    {
        protected :
             UserInterface<>    * mUserInterface{nullptr};
             Configuration      * mConfig{nullptr};
             Envirement         * mEnvirement{nullptr};
             SignalHandler      * mSignalHandler{nullptr};
             IServer            * mServer{nullptr};
             SessionManager     * mSessionManager{nullptr};
             LoggerSession      * mLogClient{nullptr};
             IOrderManager      * mOrderManager{nullptr};
             InstrumentManager<>* mInstrumentManager{nullptr};
             
        protected :
            /**
            * Default Contructor.
            */
            application() = default ;
            
            /**
            * Default Destructor.
            */
            virtual ~application () 
            {
                if (mUserInterface) delete mUserInterface;
                if (mConfig) delete mConfig;
                if (mEnvirement) delete mEnvirement;
                if (mSignalHandler) delete mSignalHandler;
                if (mServer) delete mServer;
                if (mSessionManager) delete mSessionManager;
                if (mLogClient) delete mLogClient;
                if (mOrderManager) delete mOrderManager;
                if (mInstrumentManager) delete mInstrumentManager;
            }
            
            /**
            * Update the state of this observer
            */
             virtual void update(Event,  StructData * aData = nullptr) {};

             virtual void update(Event,void* aData = 0) {};

            /**
            * Update the state of this observer
            */
            virtual void update(Event aEvent ,std::size_t aData = 0) {
                if (aEvent == Event::StartServer){
                    mServer->start();
                }
                else if (aEvent == Event::ServerShutdown)
                {
                    mServer->stop();
                }
            };

            virtual void stopThreads () = 0;
            /**
            * Create a user Configuration handler implementation.
            * @return aPointer to an Configuration Handler implementation 
            */
            virtual Configuration * createConfiguration () = 0;
            
            /**
            * Create a user Envirement handler implementation.
            * @return aPointer to an Envirement Handler implementation 
            */
            virtual Envirement    * createEnvirement () = 0;
            
            /**
            * Create a user Signal handler implementation.
            * @return aPointer to an SignalHandler Handler implementation 
            */
            virtual SignalHandler    * createSignalHandler () = 0;

            /**
            * Create a Server Object that wil be launched after startup
            * @return aPointer to a session server Handler implementation 
            */
            virtual SessionManager   * createSessionManager () = 0;

             /**
            * Create a Server Object that wil be launched after startup
            * @return aPointer to a session server Handler implementation 
            */
            virtual IOrderManager    * createOrderManager () = 0;

            /**
            * Create a Server Object that wil be launched after startup
            * @return aPointer to a session server Handler implementation 
            */
            virtual InstrumentManager<>   * createInstrumentManager () = 0;

            /**
            * Create a Server Object that wil be launched after startup
            * @return aPointer to a session server Handler implementation 
            */
            virtual UserInterface<>    * createUserInterface (SessionManager*,Configuration*) {return nullptr;};

            /**
            * Initilize the program 
            */
            virtual bool Initialize (){
                bool lResult =true;
                /***********Step0************************/
                mEnvirement = createEnvirement();
                lResult &= mEnvirement->loadEnvirement ();
                /************Step1**********************/
                mConfig = createConfiguration();
                lResult = (mConfig ? true:false);
                lResult &= mConfig->loadConfigurationFile ();
                lResult &= mConfig->load ();
                /************Step2**********************/
                mSessionManager = createSessionManager();
                mServer = mSessionManager->InitializeMainServer ();
                if (mServer && mServer->setup())
                {
                    mServer->start();
                    mServer->registerObserver(mSessionManager);
                }
                lResult &= mSessionManager->InitializeClientConnections (); 
                /*************Step3*********************/
                mSignalHandler = createSignalHandler();
                lResult = (mSignalHandler ? true:false);
                mSignalHandler->setupSignalCallbacks ();
                mSignalHandler->registerObserver(mSessionManager)   ;
                /************Step5***********************/
                mLogClient = mSessionManager->InitializeLoggerClient ();
                if (mLogClient && mLogClient->setup())
                    mLogClient->start("Logger CLient");
                /***************final step*******************/
                mInstrumentManager = createInstrumentManager ();
                if (mInstrumentManager)
                {
                   mInstrumentManager->registerObserver(mSessionManager);
                   mInstrumentManager->setConfiguration(mConfig);
                   mInstrumentManager->initialize();
                }
                 mOrderManager = createOrderManager ();
                if (mOrderManager){
                    mOrderManager->registerObserver(mSessionManager);
                    mSessionManager->registerObserver(mOrderManager);
                    mOrderManager->start();
                }
                return  lResult ;  
            }
            
        public:
            /**
            * a main entry to the application.
            * @param[in] argc  the  number of arguments passed to the main program.
            * @param[in] argv**  an array of the arguments passed to the rpogram.
            */
            virtual int main (int argc ,char **argv,bool gUI = false){
                if (!Initialize()) return false;
                if (gUI)
                {
                    mUserInterface = createUserInterface (mSessionManager,mConfig);
                    return mUserInterface->InitUI(argc,argv);
                }
                else
                {
                    while (!SignalHandler::mbGotExitSignal);
                    return 0;
                }
            };
        
    };
    
}
#endif 
