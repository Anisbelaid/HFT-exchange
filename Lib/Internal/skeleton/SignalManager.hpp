#ifndef __SIGNALHANDLER_H__
#define __SIGNALHANDLER_H_

#include <pattern/Observer.hpp>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdexcept>
#include <signal.h>
#include <errno.h>
#include <atomic>
#include <stdio.h>
#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <boost/stacktrace.hpp>
#include <iostream>
#include <functional>


using namespace std::placeholders;

namespace core {
    
    /**
    * A class that implements Signal handeling.
    */
    class SignalHandler : public Subject 
    {
        static  SignalHandler * mHandler;
        public: 
            
            static   std::atomic_bool mbGotExitSignal; 
            /**
            * Default Contructor.
            */
            SignalHandler(){
                mHandler = this;
            };
            
            /**
            * Destructor.
            */
            virtual ~SignalHandler() = default;
            
            void PrintBacktrace ();
            
            /**
            * Set up the signal handlers for CTRL-C.
            */
            virtual void setupSignalCallbacks();
            
            /**
            * Set up the signal handlers for CTRL-C.
            */
            void onStop(){
                notifyObservers (Event::ServerShutdown,10);
            };
            
            /**
            * Sets exit signal to true.
            * @param[in] _ignored Not used but required by function prototype
            *                     to match required handler.
            */
            static void stopApplication(int _ignored);
            static void SIGHUPHandler(int _ignored);
            static void SIGSYSHandler(int _ignored);
            static void SIGPOLLHandler(int _ignored);
            static void SIGQUITHandler(int _ignored);
            static void SIGILLHandler(int _ignored);
            static void SIGWINCHHandler(int _ignored);
            static void SIGIOHandler(int _ignored);
            static void SIGTRAPHandler(int _ignored);
            static void SIGABRTHandler(int _ignored);
            static void SIGIOTHandler(int _ignored);
            static void SIGBUSHandler(int _ignored);
            static void SIGFPEHandler(int _ignored);
            static void SIGKILLHandler(int _ignored);
            static void SIGUSR1Handler(int _ignored);
            static void SIGSEGVHandler(int _ignored);
            static void SIGUSR2Handler(int _ignored);
            static void SIGCLDHandler(int _ignored);
            static void SIGCHLDHandler(int _ignored);
            static void SIGALRMHandler(int _ignored);
            static void SIGSTKFLTHandler(int _ignored);
            static void SIGTERMHandler(int _ignored);
            static void SIGTTOUHandler(int _ignored);
            static void SIGURGHandler(int _ignored);
            static void SIGTSTPHandler(int _ignored);
            static void SIGSTOPHandler(int _ignored);
            static void SIGXCPUHandler(int _ignored);
            static void SIGPWRHandler(int _ignored);
            static void SIGPROFHandler(int _ignored);
            static void SIGVTALRMHandler(int _ignored);
            static void SIGXFSZHandler(int _ignored);
            static void SIGTTINHandler(int _ignored);
            static void SIGCONTHandler(int _ignored);
            static void SIGPIPEHandler(int _ignored);
    };
}

  std::atomic_bool core::SignalHandler::mbGotExitSignal;
core::SignalHandler * core::SignalHandler ::mHandler{nullptr};

void core::SignalHandler::PrintBacktrace (){
    
    std::stringstream ss ;
    ss<<boost::stacktrace::stacktrace();
    
}

void core::SignalHandler::stopApplication(int aNum)
{
    if (mHandler) 
        mHandler->onStop();
    mbGotExitSignal = true;
    exit(1);  
}

/*void core::SignalHandler::SIGHUPHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGHUP Detected");
    exit(0);
}

void core::SignalHandler::SIGSYSHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGSYS Detected");
    exit(0);
 }

void core::SignalHandler::SIGPOLLHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGPOLL Detected");
    exit(0);
}

void core::SignalHandler::SIGQUITHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGQUIT Detected");
}


void core::SignalHandler::SIGILLHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGILL Detected");
    exit(0);
}

void core::SignalHandler::SIGWINCHHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGWINCH Detected");
    exit(0);
}


void core::SignalHandler::SIGIOHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGIO Detected");
    exit(0);
}

void core::SignalHandler::SIGTRAPHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGTRAP Detected");
    exit(0);
}


void core::SignalHandler::SIGABRTHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGABRT Detected");
    exit(0);
}

void core::SignalHandler::SIGIOTHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGIOT Detected");
    exit(0);
}

void core::SignalHandler::SIGBUSHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGBUS Detected");
    exit(0);
    
}

void core::SignalHandler::SIGFPEHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGFPE Detected");
    exit(0);
}

void core::SignalHandler::SIGKILLHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGKILL Detected");
    exit(0);
}

void core::SignalHandler::SIGUSR1Handler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGUSR1 Detected");
    exit(0);
}

void core::SignalHandler::SIGSEGVHandler(int aNum)
{
    NANO_LOG(ERRORR, "SIGNAL SIGSEGV Detected %d , %s",aNum);
    exit(0);
}

void core::SignalHandler::SIGUSR2Handler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGUSR2 Detected");
    exit(0);
}


void core::SignalHandler::SIGCLDHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGCLDH Detected");
    exit(0);
}

void core::SignalHandler::SIGCHLDHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGCHLD Detected");
    exit(0);
}


void core::SignalHandler::SIGALRMHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGALR Detected");
    exit(0);
}


void core::SignalHandler::SIGSTKFLTHandler(int _ignored)
{
    NANO_LOG(ERRORR, "SIGNAL SIGSTKFL Detected");
    exit(0);
}

void core::SignalHandler::SIGTERMHandler(int _ignored)
{
    exit(0);
}

void core::SignalHandler::SIGTTOUHandler(int _ignored)
{

    exit(0);
}

void core::SignalHandler::SIGURGHandler(int _ignored)
{
    exit(0);
}

void core::SignalHandler::SIGTSTPHandler(int _ignored)
{
    exit(0);
}

void core::SignalHandler::SIGSTOPHandler(int _ignored)
{
    exit(0);
}

void core::SignalHandler::SIGXCPUHandler(int _ignored)
{
    exit(0);
}

void core::SignalHandler::SIGPWRHandler(int _ignored)
{
    exit(0);
}
void core::SignalHandler::SIGPIPEHandler(int _ignored)
{
     std::cerr <<"SIGNAL SIGPIPE Detected "<<_ignored;
    exit(0);
}
void core::SignalHandler::SIGCONTHandler(int _ignored)
{
    exit(0);
}
void core::SignalHandler::SIGTTINHandler(int _ignored)
{
    exit(0);
}
void core::SignalHandler::SIGXFSZHandler(int _ignored)
{
    exit(0);
}
void core::SignalHandler::SIGVTALRMHandler(int _ignored)
{
    exit(0);
}
void core::SignalHandler::SIGPROFHandler(int _ignored)
{

    exit(0);
}*/


void core::SignalHandler::setupSignalCallbacks()
{
    if (signal((int) SIGINT, SignalHandler::stopApplication) == SIG_ERR)
        std::cerr <<"Failed to register signal handler SIGINT"<<std::endl;
    
   /* if (signal((int) SIGHUP, SignalHandler::SIGHUPHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGHUP");
    
    if (signal((int) SIGSYS, SignalHandler::SIGSYSHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGSYS");
    
    if (signal((int) SIGPWR, SignalHandler::SIGPWRHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGPWR");
    
    if (signal((int) SIGPOLL, SignalHandler::SIGPOLLHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGPOLL");
    
    if (signal((int) SIGWINCH, SignalHandler::SIGWINCHHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGWINCH");
    
    if (signal((int) SIGIO, SignalHandler::SIGIOHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGIO");
    
    if (signal((int) SIGPROF, SignalHandler::SIGPROFHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGPROF");
    
    if (signal((int) SIGVTALRM, SignalHandler::SIGVTALRMHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGVTALRM");
    
    if (signal((int) SIGXFSZ, SignalHandler::SIGXFSZHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGXFSZ");
    
    if (signal((int) SIGXCPU, SignalHandler::SIGXCPUHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGXCPU");
    
    if (signal((int) SIGURG, SignalHandler::SIGURGHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGURG");
    
    if (signal((int) SIGSTOP, SignalHandler::SIGSTOPHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGSTOP");
    
    if (signal((int) SIGTTOU, SignalHandler::SIGTTOUHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGTTOU");
    
    if (signal((int) SIGTTIN, SignalHandler::SIGTTINHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGTTIN");
    
    if (signal((int) SIGTSTP, SignalHandler::SIGTSTPHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGTSTP");
    
    if (signal((int) SIGCONT, SignalHandler::SIGCONTHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGCONT");
    
    if (signal((int) SIGCLD, SignalHandler::SIGCLDHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGCLD");
    
    if (signal((int) SIGCHLD, SignalHandler::SIGCHLDHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGCHLD");
    
    if (signal((int) SIGALRM, SignalHandler::SIGALRMHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGALRM");
    
    if (signal((int) SIGSTKFLT, SignalHandler::SIGSTKFLTHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGSTKFLT");
    
    if (signal((int) SIGTERM, SignalHandler::SIGTERMHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGTERM");
    
    if (signal((int) SIGABRT, SignalHandler::SIGABRTHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGABRT");
    
    if (signal((int) SIGIOT, SignalHandler::SIGIOTHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGIOT");
    
    if (signal((int) SIGBUS, SignalHandler::SIGBUSHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGBUS");
    
    if (signal((int) SIGFPE, SignalHandler::SIGFPEHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGFPE");
    
    if (signal((int) SIGKILL, SignalHandler::SIGKILLHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGKILL");
    
    if (signal((int) SIGUSR1, SignalHandler::SIGUSR1Handler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGUSR1");
    
    if (signal((int) SIGSEGV, SignalHandler::SIGSEGVHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGSEGV");
    
    if (signal((int) SIGUSR2, SignalHandler::SIGUSR2Handler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGUSR2");
    
    if (signal((int) SIGPIPE, SignalHandler::SIGPIPEHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGPIPE");
    
    if (signal((int) SIGTRAP, SignalHandler::SIGTRAPHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGTRAP");
    
    if (signal((int) SIGILL, SignalHandler::SIGILLHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGILL");
    
    if (signal((int) SIGQUIT, SignalHandler::SIGQUITHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGQUIT");
    
    if (signal((int) SIGHUP, SignalHandler::SIGHUPHandler) == SIG_ERR)
        NANO_LOG(NOTICE, "Failed to register signal handler SIGHUP");*/
    };
    
#endif
          
