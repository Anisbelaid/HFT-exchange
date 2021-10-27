/* Copyright (C) 2019 EUAMEX - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the XYZ license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the eaummes license with
 * this file. If not, please write to: mondher@eaumex.com | anis@eaumex.com, or visit :eaumex.com
 */

//#include <Intern/Gemini.hpp>

#include <Application/Application.hpp>
#include <Application/Defines.hpp>
#include <Application/Configuration.hpp>
#include <Session/SessionManager.hpp>
#include <skeleton/Application.hpp>

#include <config.h.in>


broker::BrokerApp app;

int main (int argc,char** argv)
{
    app.main(argc,argv,true);
    return 0;
}
    
    

