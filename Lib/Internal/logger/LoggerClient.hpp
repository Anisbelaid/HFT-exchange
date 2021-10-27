#ifndef _LOGGER_CLIENT_HPP_
#define _LOGGER_CLIENT_HPP_


#include <client/TCPClient.hpp>
#include <pattern/Singleton.hpp>
#include <skeleton/Configuration.hpp>
#include <skeleton/Session.hpp>
namespace core { 

    /**
    * Implementation of a generic LogClient that will allow to process all message to be logged in another Server
    * @tparam LOGGER_TYPE Indcate Logger Instance type.
    */
    class LoggerSession : public core::TCPClientSession<>  {
        using Client = core::TCPClientSession<>;
           
        public :

            LoggerSession (const std::string& aAddress,const uint16_t aPort,const std::string& aInterface ,uint64_t aSessionName) :
            Client(aAddress,aPort,aInterface,aSessionName){
            }

            ~LoggerSession () = default ;
            
            /**
            * @brief Ensure that we received the full message to process.
            *
            * @param aConnectionId The ConnectionId.
            * @param aData A pointer to the data we want to push.
            * @param aLength The size of the data we want to push .
            * @return StructData * pointer to a full message
            */
            StructData * ensure (int aConnectionId,void * aData,size_t aLength) {
                return nullptr;
            };
            
            /**
            * @brief This method should process the new message to a peer.
            *
            * @param aId The final data to process.
            * @param aData The final data to process.
            * @return void
            */
            void processMessage (int aId,StructData * aData) {
            }; 
            
    };

}

#endif

