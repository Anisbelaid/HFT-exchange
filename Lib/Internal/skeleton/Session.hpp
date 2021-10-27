#ifndef _SESSION_HPP_
#define _SESSION_HPP_

#include <pattern/Observer.hpp>
#include <server/TCPConnection.hpp>
#include <thread/CPUTimer.hpp>
#include <client/TCPClient.hpp>
#include <client/UDPClient.hpp>
#include <Intern/Header.hpp>
#include <EncoderDecoder.hpp>

#include <string>
#include <vector>

namespace core {
    /**
     * @brief this class is listner interface on message that will be received and sent 
     * that will allow an object to register and hear any event
     * 
     */
    class MessageListener 
    {
        protected:
            typedef std::vector<std::function<void(void*)> > Callbacks_;
            /// Order Accepted creation callback
            Callbacks_ mOrderAccecptedCallback ;
            /// Order Rejected creation callback
            Callbacks_ mOrderRejectedCallback ;
            /// Order Cancelled callback
            Callbacks_ mOrderCancelledCallback ;
            /// Instrument creation callback
            Callbacks_ mInstrumentCallback ;
            /// Instrument creation callback
            Callbacks_ mTradesCallback ;
            /// Instrument creation callback
            Callbacks_ mOrderApprovalCallback ;
        public :
            /**
             * @brief add a listner on order accepted 
             * 
             */
            virtual void RegisterOrderAccecpted (std::function<void(void*)> aFunction)
            {
                mOrderAccecptedCallback.push_back(aFunction); 
            }
            /**
             * @brief add a listner on order rejected 
             * 
             */
            virtual void RegisterOrdeRejected (std::function<void(void*)> aFunction)
            {
                mOrderRejectedCallback.push_back(aFunction); 
            }

             /**
             * @brief add a listner on order cancelled 
             * 
             */
            virtual void RegisterInstrument (std::function<void(void*)> aFunction)
            {
                mInstrumentCallback.push_back(aFunction); 
            }

            /**
             * @brief add a listner on Trades
             * 
             */
            virtual void RegisterTrades (std::function<void(void*)> aFunction)
            {
                mTradesCallback.push_back(aFunction); 
            }

             /**
             * @brief add a listner on order cancelled 
             * 
             */
            virtual void RegisterOrdeCancelled (std::function<void(void*)> aFunction)
            {
                mOrderCancelledCallback.push_back(aFunction); 
            }

            /**
             * @brief add a listner on order approval 
             * 
             */
            virtual void RegisterApproval (std::function<void(void*)> aFunction)
            {
                mOrderApprovalCallback.push_back(aFunction); 
            }


            /**
             * @brief Destroy the Message Listener object
             * 
             */
            virtual ~MessageListener() {}
    }; 

    /**
     * @brief 
     * 
     */
    class Session : public Subject , public Routine , public MessageListener {
        protected  :
            
            /// The session name 
            uint64_t                  mSessionName {0};
            /// the Session User Id
            uint64_t                  mUserId{0};
            /// the server id
            uint64_t                  mServerId{0};
            /// check if the session is connected or not
            bool                      mLogged {false};
            /// a boolean to check wether the session received a message or not
            bool                      mMessageReceived {false};
            /// this the heartbeat message that will be trasnfered between Sessions 
            intern::HeartBeat         mHeartBeat;
        public:

            /**
             * @brief Construct a new Session object
             * 
             * @param aSessionName 
             */
            explicit Session (uint64_t aSessionName,uint64_t aServerId=0) :Routine(std::to_string(aSessionName).c_str(),1000,true),mSessionName(aSessionName),mServerId(aServerId)  {
                mHeartBeat.TYPE = 'H';
                mHeartBeat.PARENTSESSIONID = mServerId;
                mHeartBeat.SESSIONID = mSessionName;
            } ;

            /**
             * @brief Get the Session Name object
             * 
             * @return std::string 
             */
            uint64_t getSessionName() {
                return mSessionName;
            }

            /**
             * @brief Set the User Id object
             * 
             * @param aUserId 
             */
            void setUserId (uint64_t aUserId){
                mUserId = aUserId;
            }

            uint64_t getUserId (){
                return mUserId;
            }

            bool Task()
            {
                return true;
            }

            bool SubTask()
            {
                return true;
            }
             /**
             * @brief 
             * 
             */
            virtual void submit (void * aMessage , size_t aLength) {}

            
            /**
             * @brief 
             * 
             */
            virtual bool prepare () = 0;

            /**
             * @brief 
             * 
             */
            virtual void open (std::string aJobName) = 0;
            
            /**
             * @brief 
             * 
             */
            virtual bool close () = 0;

            /**
             * @brief 
             * 
             */
            virtual bool IsConnected () = 0;
            
            /**
             * @brief 
             * 
             */
            virtual void AddSessionName () {}
            /**
             * @brief Destroy the Session object
             * 
             */
            virtual ~Session () {} ;
    };

    /**
     * @brief a TCP Implementation of a Session
     * 
     */
    template <uint32_t BUFFER_SIZE = 1024*1024 ,class Allocator = core::LinearAllocator >
    class TCPServerSession : public Session , public TCPConnection<BUFFER_SIZE,Allocator> , public Decoder <Allocator>  {
        using Connection = TCPConnection<BUFFER_SIZE,Allocator>;
        public:
            /**
             * @brief Construct a new TCPSession object
             * 
             * @param aSessionName 
             * @param aLogin 
             * @param aPassword 
             * @param aIsCrypted 
             */
            explicit TCPServerSession (int aSock,uint64_t aSessionName,uint64_t aSeverId = 0) :Session(aSessionName,aSeverId),
                                                                        Connection(aSock)
            {
            }

            /**
             * @brief will be called when a peer diiconnnect
             * 
             */
            void onDisconnect () override {
                Connection::onDisconnect();
                notifyObservers(core::Event::ClientDisconnection,Session::mSessionName);
            }

            bool prepare (){
                return true;
            }
            /**
             * @brief 
             * 
             */
            virtual void open (std::string aJobName) override {
                Connection::start();
            }

            bool close () override {
                return Connection::stop ();
            }

            /**
            * @brief This method should process the new message to a peer.
            * @param aId 
            * @param aData The final data to process.
            * @return void
            */
            void processMessage (int aId,  StructData * aData) 
            {
                mMessageReceived = true;
                Decoder <>::Decode(aData->mData,aData->mLength);
                aData->mClientId = mSessionName;
                notifyObservers (core::Event::FromServerSession ,aData);
            }; 

            /**
            * @brief Ensure that we received the full message to process.
            *
            * @param aConnectionId The ConnectionId.
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return StructData * pointer to a full message
            */
            StructData * ensure (int aConnectionId,void * aData,size_t aLength) 
            {
                return Decoder<>::ensure(aConnectionId,aData,aLength);
            }; 


            /**
             * @brief 
             * 
             */
            bool IsConnected () {
                return Connection::mIsConnected;
            };
            
            /**
             * @brief 
             * 
             */
            virtual bool Task () { 
               /* if (mLogged && !mMessageReceived){
                    notifyObservers(core::Event::ClientDisconnection,Session::mSessionName);
                    return false;
                }
                if (mLogged)
                {
                    mMessageReceived = false;
                    SendHeartBeat ();
                    return true;
                }*/
                return true;     
            }

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            void TreatHeartBeat (void*aMessage){ mMessageReceived = true; }

            /**
             * @brief 
             * 
             */
            virtual void SendHeartBeat () {
                struct timespec lTime;
                core::gClockTime(CLOCK_REALTIME, &lTime);
                mHeartBeat.SENDINGTIME = lTime.tv_nsec;
                Connection::push ((void*)&mHeartBeat,sizeof(intern::HeartBeat));
            }

            /**
             * @brief 
             * 
             */
            virtual void submit (void * aMessage , size_t aLength) {Connection::push (aMessage,aLength);}

            /**
             * @brief Destroy the TCPSession object
             * 
             */
            virtual ~TCPServerSession () {}
    };

    /**
     * @brief a TCP Implementation of a Session
     * 
     */
    template <uint32_t BUFFER_SIZE = 1024*1024,class Allocator = core::LinearAllocator >
    class TCPClientSession : public Session , public TCPClient<BUFFER_SIZE,Allocator> , public Decoder <Allocator> {
        using Connection = TCPClient<BUFFER_SIZE,Allocator>;
        public:
            /**
             * @brief Construct a new TCPSession object
             * 
             * @param aSessionName 
             * @param aLogin 
             * @param aPassword 
             * @param aIsCrypted 
             */
            TCPClientSession (const std::string& aAddress,const uint16_t aPort,const std::string& aInterface ,uint64_t aSessionName,uint64_t aSeverId = 0) :
            Session(aSessionName,aSeverId),Connection(){
                Connection::mInterface = aInterface;
                Connection::mAddress = aAddress;
                Connection::mPort = aPort;
            }

            /**
             * @brief 
             * 
             */
            bool prepare () override 
            {
                return Connection::setup();
            }


            /**
             * @brief 
             * 
             */
            void open (std::string aJobName) override 
            {
                Connection::start(aJobName);
            }

            bool close () override 
            {
               return Connection::stop ();
            }

            /**
            * @brief Used to start a server .
            */
            void onDisconnect () override 
            {
                Connection::onDisconnect();
                (core::Event::ServerDisconnection,(size_t)mSessionName);
            };

            /**
            * @brief This method should process the new message to a peer.
            * @param aId 
            * @param aData The final data to process.
            * @return void
            */
            void processMessage (int aId,  StructData * aData) 
            {
                mMessageReceived = true;
                Decoder <>::Decode(aData->mData,aData->mLength);
                aData->mClientId = mSessionName;
                notifyObservers (core::Event::FromServerSession ,aData);
            }; 
            /**
            * @brief Ensure that we received the full message to process.
            *
            * @param aConnectionId The ConnectionId.
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return StructData * pointer to a full message
            */
            StructData * ensure (int aConnectionId,void * aData,size_t aLength) {
                return Decoder<>::ensure(aConnectionId,aData,aLength);
            };
            
            /**
             * @brief 
             * 
             */
            bool IsConnected () {
                return Connection::mIsConnected;
            };

            /**
             * @brief 
             * 
             */
            virtual bool Task () { 
                /*if (mLogged && !mMessageReceived){
                    notifyObservers(core::Event::ServerDisconnection,Session::mSessionName);
                    return false;
                }
                if (mLogged)
                {
                    mMessageReceived = false;
                    SendHeartBeat ();
                    return true;
                }*/
                return true;     
            }

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            void TreatHeartBeat (void*aMessage){ mMessageReceived = true; }

            /**
             * @brief 
             * 
             */
            virtual void SendHeartBeat () {
                struct timespec lTime;
                core::gClockTime(CLOCK_REALTIME, &lTime);
                mHeartBeat.SENDINGTIME = lTime.tv_nsec;
                Connection::push ((void*)&mHeartBeat,sizeof(intern::HeartBeat));
            }

            /**
             * @brief 
             * 
             */
            virtual void submit (void * aMessage , size_t aLength) {Connection::push (aMessage,aLength);}

            /**
             * @brief Destroy the TCPSession object
             * 
             */
            virtual ~TCPClientSession () {} 
    };

    /**
     * @brief 
     * 
     */
    template <uint32_t BUFFER_SIZE = 1024*1024,class Allocator = core::LinearAllocator>
    class UDPClientSession : public Session, public core::UDPClient<BUFFER_SIZE,Allocator>, public Decoder <Allocator> {
        using Connection = core::UDPClient<BUFFER_SIZE,Allocator>;
        public:
            explicit UDPClientSession (const std::string& aInterface,const std::string& aIp,uint16_t aPort,uint64_t aSessionName) :Session(aSessionName),Connection(aInterface,aIp,aPort){};
            /**
             * @brief 
             * 
             */
            virtual bool prepare () {
                return Connection::setup();
            }

            /**
             * @brief 
             * 
             */
            virtual void open (std::string aJobName) 
            {
                Connection::start(aJobName);
            };
            
             /**
             * @brief 
             * 
             */
            virtual bool Task () { 
                if (!mMessageReceived){
                    notifyObservers(core::Event::ClientDisconnection,Session::mSessionName);
                    return true;
                }
                mMessageReceived = false;
                return false;     
            }

            /**
             * @brief 
             * 
             * @param aMessage 
             */
            void TreatHeartBeat (void*aMessage){ std::cout<<"Received Heartbeat"<<std::endl; mMessageReceived = true; }
            
            /**
             * @brief 
             * 
             */
            virtual bool close () {
                return Connection::stop();
            };

            /**
             * @brief 
             * 
             */
            virtual bool IsConnected () {
                return true;
            };
            
             void onPacket(int,void * aMessage,size_t aBytes) override {
                Decoder<>::Decode (aMessage,aBytes);
            };

            
            ~UDPClientSession () = default ;
    };

}

#endif // !_SESSION_H_PP
