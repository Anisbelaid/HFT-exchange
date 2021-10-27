/* Copyright (C) 2019 EUAMEX - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the XYZ license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the eaummes license with
 * this file. If not, please write to: mondher@eaumex.com | anis@eaumex.com, or visit :eaumex.com
 */

#include <Application/Defines.hpp>
#include <Application/Configuration.hpp>
#include <Application/Envirement.hpp>
#include <DataBase/DataBaseHelper.hpp>
#include <Server/PersistenceServer.hpp>
#include <Logger/LoggerHelper.hpp>
#include <application.hpp>
#include <CPUTimer.hpp>
#include <config.h.in>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void LogHeader (){
    INFO(PERSISTANCE,"**************************************************************************************");
    INFO(PERSISTANCE,"Starting Market Data Server version"<<PROJECT_VER);
    INFO(PERSISTANCE,"**************************************************************************************");    
}

class ServerApp : public core::application {
    private :
        
    public :
        
        ServerApp () = default ;
        ~ServerApp() = default ;
        
        ///callback in case of disconnection 
        virtual void onDisconnect (int aCon) {
            
            INFO(PERSISTANCE,"Stopping Market Data Sever ... ");
            
            server::PersistenceServer::getInstance()->stop();
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            INFO(PERSISTANCE,"**************************************************************************************");
            INFO(PERSISTANCE,"Persistance sever stopped");
            INFO(PERSISTANCE,"**************************************************************************************");
        };
        
        bool Initialize () {
            
            Logger::initFromConfig("../config/logger.ini");
            
            LogHeader();
            
            INFO(PERSISTANCE,"Loading logger configuration");
            
            bool lResult = core::application::Initialize();

            marketdata::Configuration* lConfig = (marketdata::Configuration*)mConfig;
            
            INFO(PERSISTANCE,"Strating dataBase helper  ...");
            
            database::DatabaseHelper::getInstance(lConfig->getDataBasePath()) ;
            
            INFO(PERSISTANCE,"Starting logger server ...");
            
            server::PersistenceServer::getInstance(lConfig->getServerPort()) ;
            
            return lResult;
        };
        
        core::Configuration * createConfiguration () { return new marketdata::Configuration (mEnvirement->getConfigFolder(),"server.ini");}

        core::Envirement    * createEnvirement () { return new marketdata::Envirement ();}

        core::SignalHandler * createSignalHandler () { return new core::SignalHandler (*this);}
        
};

ServerApp app;

int main (int argc,char** argv)
{
    trading::Order lOrder;
    //Logger::initFromConfig("../config/logger.ini");
    logger::LoggerHelper * lHelper = logger::LoggerHelper::getInstance ();
    
    logger::LoggerHelper::EventEnum aOrderEvent = logger::LoggerHelper::EventEnum::Order;
    lHelper->onNewEvent(aOrderEvent,(void*)&lOrder);
    //app.main(argc,argv);
    //return 0;
}
    
    

