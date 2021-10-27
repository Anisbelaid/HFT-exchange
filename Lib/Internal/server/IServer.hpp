#ifndef _ISERVER_HPP_
#define _ISERVER_HPP_

#include <pattern/Observer.hpp>
#include <server/Defines.hpp>
#include <logger/OutputListner.hpp>

#include<string>

namespace core {

    /**
     * @brief an interface to handle server events 
     * 
     */
    class IServer : public core::OutputListner , public Subject , public Observer   {
        protected :
            /// this the Id tht will be used for the while architecture
            uint64_t mServerId {0};

            std::string mAddress{LOCALHOST};  

            /// Server Port.
            uint16_t mPort {0};
            
            /// Socket descriptor.
            int mServerSocket{0};

        public:

            virtual ~IServer () {};

            /**
             * @brief Set the Server Id object
             * 
             * @param aId 
             */
            void setServerId (const uint64_t& aId){mServerId = aId;}

            /**
             * @brief Get the Server Id object
             * 
             * @param aId 
             * @return uint16_t 
             */
            const uint64_t& getServerId (){return mServerId;}

            /**
             * @brief Get the Server Address object
             * 
             * @return const std::string& 
             */
            const std::string& getServerAddress (){return mAddress;} 

            /**
             * @brief Get the Server Id object
             * 
             * @param aId 
             * @return uint16_t 
             */
            const uint16_t getPort (){return mPort;}

            /**
             * @brief Set the Server Id object
             * 
             * @param aId 
             */
            void setPort (const uint16_t aPort){mPort = aPort;}

            /**
             * @brief Set the Server Address object
             * 
             */
            void setServerAddress (const std::string & aAddress) {mAddress = aAddress;}
            
            /**
            * @brief Callback before starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreStart () {}; 
            
            /**
            * @brief  Callback after starting server.
            * It can be overloaded to implement some treatment
            */
            virtual void onStart (){}; 
            
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onPreClose () {}; 
            /**
            * @brief  Callback after shutting down the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onClose () {}; 
            
            /**
            * @brief Callback when a client connect to the server.
            * It can be overloaded to implement some treatment
            */
            virtual void onConnection (int) = 0; 

            /**
            * @brief Used to start a server .
            * @param aConId The Id of the connection
            */
            virtual void onDisconnect (int aConId) =0;
            
            /**
            * @brief Used to configure a server .
            * @return setup result
            */
            virtual bool setup () = 0; 
            /**
            * @brief  Used to stop a server .
            * @return stop result
            */
            virtual bool stop () = 0;
            
            /**
            * @brief Used to start a server .
            * @return start result
            */
            virtual bool start () = 0;

            /**
             * Update the state of this observer
              */
            virtual void update(Event aEvent ,  StructData * aData = nullptr) {
                if (aEvent == Event::Broadcast)
                    push(aData->mData,aData->mLength);
            };

            /**
            * @brief Push data into the async server queue.
            *
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return void
            */
            virtual void push (void * aData, size_t aLength) = 0;

            /**
            * Update the state of this observer
            */
            virtual void update(Event aEvent,size_t aData = 0 ) {};

    };
}

#endif 