#ifndef _PERSISTENCE_SERVER_HPP_
#define _PERSISTENCE_SERVER_HPP_

#include <boost/core/noncopyable.hpp>
#include <fstream> 
#include <mutex>
#include <CustomArray.hpp>
#include <Trading/Order.hpp>
#include <Trading/Trade.hpp>
#include <Trading/Reject.hpp>
#include <Trading/OrderReply.hpp>
#include <Feed/Order.hpp>
#include <Feed/Trade.hpp>
#include <Feed/Instrument.hpp>

namespace database {
    
       
    /**
    * Implementation of a generic database server allowing implement functional layer
    */
    class DatabaseHelper : private boost::noncopyable   {
        private :
            
            ///A flag used to check if there only one instance in the program of the trading server
            static std::once_flag mFlag;
            
            ///A pointer to the istance of the trading server
            static DatabaseHelper * mInstance;
            /**
            * Create a DataBaseHelper to access Database.
            * @param aFilePath The port that will be used.
            */
            DatabaseHelper (std::string aFilePath) ;
            
            ///File Path
            std::string mFilePath;
            
            /// File stream  data base
            std::ofstream mOrderDataBaseFile;
            
            /// File stream  data base 
            std::ofstream mTradeDataBaseFile;
            

        public :
            
            /**
            * Return an instance of the data base helper.
            * @param aFilePath The port that will be used.
            * @return static databaseHelper*
            */
            static DatabaseHelper* getInstance (std::string aFilePath ="")
            {
                std::call_once(mFlag,
                            [&](DatabaseHelper *&aIntance) {
                                aIntance = new DatabaseHelper(aFilePath);
                            },
                            mInstance);
                return mInstance;
            }
            
            void onNewOrder (trading::Order*);
            
            void onTrade ();
            
            /**
            * Destroy a Trading Server.
            *
            * @return Destuctor
            */
            ~DatabaseHelper () ;
            
            
            
    };
   
}

    database::DatabaseHelper *database::DatabaseHelper::mInstance{0x0};
    std::once_flag database::DatabaseHelper::mFlag;
    

    database::DatabaseHelper::DatabaseHelper(std::string aFilePath){
        std::string lOrderPath = mFilePath.append ("/Order.data");
        mOrderDataBaseFile.open(lOrderPath, std::ios::app | std::ios::binary);
        std::string lTradePath = mFilePath.append ("/Trade.data");
        mTradeDataBaseFile.open(lTradePath, std::ios::app | std::ios::binary);
    };
    
    
    void database::DatabaseHelper::onNewOrder(trading::Order* aOrder) {
        mOrderDataBaseFile.write((char*)aOrder, sizeof(trading::Order)); 
    };
    

    
    database::DatabaseHelper::~DatabaseHelper(){
        mOrderDataBaseFile.close();
        mTradeDataBaseFile.close();
    }


#endif

