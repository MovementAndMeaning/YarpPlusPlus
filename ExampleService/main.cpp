//
//  ExampleService/main.cpp
//  YarpPlusPlus
//
//  Created by Norman Jaffe on 2014-02-06.
//  Copyright (c) 2014 OpenDragon. All rights reserved.
//

#define ENABLE_OD_SYSLOG /* */
#include "ODSyslog.h"
#include "YPPEndpoint.h"
#include "YPPExampleService.h"
#include "YPPRegistryService.h"
#include <yarp/os/all.h>
#include <yarp/conf/version.h>
#include <iostream>
#if defined(__APPLE__)
# include <unistd.h>
#endif // defined(__APPLE__)

using namespace YarpPlusPlusExample;
using std::cout;
using std::cerr;
using std::endl;

#pragma mark Private structures and constants

/*! @brief Run loop control; @c true if the service is to keep going and @c false otherwise. */
static bool lKeepRunning;

#pragma mark Local functions

/*! @brief The signal handler to catch requests to stop the service.
 @param signal The signal being handled. */
static void stopRunning(int signal)
{
#pragma unused(signal)
    lKeepRunning = false;
} // stopRunning

#pragma mark Global functions

/*! @brief The entry point for creating an example service.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the example service.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int      argc,
         char * * argv)
{
    OD_SYSLOG_INIT(*argv, kODSyslogOptionIncludeProcessID | kODSyslogOptionIncludeThreadID |//####
                   kODSyslogOptionEnableThreadSupport);//####
    OD_SYSLOG_ENTER();//####
    yarp::os::Network yarp; // This is necessary to establish any connection to the YARP infrastructure

    cout << "YARP++ Version " << YPP_VERSION << ", YARP Version " << YARP_VERSION_STRING << endl;
    if (argc > 1)
    {
        ExampleService * stuff = new ExampleService(argc - 1, argv + 1);
        
        if (stuff)
        {
            yarp::os::ConstString portName(stuff->getEndpoint().getName());

            if (stuff->start() && YarpPlusPlus::RegistryService::registerLocalService(portName))
            {
                lKeepRunning = true;
#if defined(__APPLE__)
                signal(SIGHUP, stopRunning);
                signal(SIGINT, stopRunning);
                signal(SIGINT, stopRunning);
                signal(SIGUSR1, stopRunning);
#endif // defined(__APPLE__)
                for ( ; lKeepRunning; )
                {
#if defined(__APPLE__)
					//TODO: find windows equivalent
                    sleep(1);
#endif // defined(__APPLE__)
                }
                stuff->stop();
            }
            delete stuff;
        }
    }
    OD_SYSLOG_EXIT_L(0);//####
    return 0;
} // main

