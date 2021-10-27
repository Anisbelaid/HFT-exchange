#ifndef BROKER_SESSION_HPP_
#define BROKER_SESSION_HPP_

#include <Application/Defines.hpp>
#include <skeleton/Session.hpp>
#include <Application/Defines.hpp>
#include <EncoderDecoder.hpp>
#include <Trading/Logon.hpp> 

namespace broker
{
    class BrokerSession : public core::TCPClientSession<>  {
        using Client = core::TCPClientSession<>;
        std::string     mLogin {""};
        std::string     mPassword {""};
        trading::Logon  mLogon;
        public :

            explicit BrokerSession (const std::string& aAddress,const uint16_t aPort,const std::string& aInterface ,uint64_t aSessionName,const std::string &aLogin = "",const std::string& aPassword="") :
            Client(aAddress,aPort,aInterface,aSessionName),mLogin(aLogin),mPassword(aPassword){
            }

            ~BrokerSession () {};
            
            void Sendlogon ()
            {
                struct timespec lTime;
                core::gClockTime(CLOCK_REALTIME, &lTime);
                mLogon.SENDINGTIME = lTime.tv_nsec;
                mLogon.TYPE = 'L';
                strcpy(mLogon.USER,mLogin.c_str());
                strcpy(mLogon.PASSWORD,mPassword.c_str());
                submit ((void*)&mLogon,sizeof (trading::Logon));
            }

            /**
             * @brief 
             * 
             */
            virtual void TreatLogonAccepeted (void* aMessage)
            {
                INFO(Broker,"broker Session ::TreatLogonAccepeted ");
                mLogged =true;
                mMessageReceived = true;
            }

            void TreatRequestApproval (void*aMessage){
                INFO(Broker,"broker Session ::TreatRequestApproval ");
                for (auto lIt : mOrderApprovalCallback)
                    lIt(aMessage);
                
             }

            /**
             * @brief 
             * 
             */
            virtual void TreatLogonRejected (void* aMessage)
            {
                INFO(Broker,"broker Session ::TreatLogonRejected ");
                notifyObservers(core::Event::ServerDisconnection,Session::mSessionName);
            }
            
            void TreatOrderAccecpted (void* aMessage){
                INFO(Broker,"Broker Session ::TreatOrderAccecpted ... ");
                INFO(Broker,"Broker Session "<<*(trading::Accepted*)aMessage);
                for (auto lIt : mOrderAccecptedCallback)
                    lIt(aMessage);
            }

            void TreatOrderRejected (void* aMessage){
                INFO(Broker,"Broker Session ::TreatOrderRejected ...");
                INFO(Broker,"Broker Session "<<*(trading::Reject*)aMessage);
                for (auto lIt : mOrderRejectedCallback)
                    lIt(aMessage);
            }

            void TreatExecution (void *aTrade)
            {
                INFO(Broker,"Broker Session ::TreatExecution");
            }

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            void onDisconnect (){
                INFO(Broker,"Broker Session ::onDisconnect Session Id <"<<mSessionName<<">");
                notifyObservers(core::Event::ServerDisconnection,mSessionName);
            };


            const std::string& getUser () const 
            {
                return mLogin;
            }
            bool close () override {
                INFO(Broker,"broker Session ::close Session Id <"<<mSessionName<<">");
                return Client::close ();
            }

            /**
             * @brief enter an info message 
             * @param[in] input message event
             */
            virtual void onInfo (const char *aMessaage) {
                INFO(Broker,aMessaage);
            };
            /**
             * @brief enter an debug message
             * @param[in] input message event 
             */
            virtual void onDebug (const char *aMessaage) {
                DEBUGG(Broker,aMessaage);
            };
            /**
             * @brief enter a warning message 
             * @param[in] input message event 
             */
            virtual void onWarning (const char *aMessaage) {
                    WARN(Broker,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onError (const char *aMessaage) {
                ERRORR(Broker,aMessaage);
            };
            /**
             * @brief enter an error message 
             * @param[in] input message event
             */
            virtual void onTrace (const char *aMessaage) {
                TRACE(Broker,aMessaage);
            };

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadAdded (const char *aMessage) {
                INFO(Broker,"::onThreadAdded "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onThreadError (const char *aMessage) {
                    INFO(Broker,"::onThreadError "<<aMessage);
            };
            /**
             * @brief 
             * 
             * @param aMessage 
             */
            virtual void onCloseThread (const char *aMessage) {
                INFO(Broker,"::onCloseThread "<<aMessage);
            };
            
    }; 
}



#endif

