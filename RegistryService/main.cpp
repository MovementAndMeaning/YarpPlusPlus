//--------------------------------------------------------------------------------------
//
//  File:       RegistryService/main.cpp
//
//  Project:    YarpPlusPlus
//
//  Contains:   The main application for the Service Registry Yarp++ service.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms,
//              with or without modification, are permitted provided that the following
//              conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the
//                  distribution.
//                * Neither the name of the copyright holders nor the names of its
//                  contributors may be used to endorse or promote products derived
//                  from this software without specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created:    2014-02-06
//
//--------------------------------------------------------------------------------------

//#define ENABLE_OD_SYSLOG /* */
#include "ODSyslog.h"
#include "YPPRegistryService.h"
#include "YPPRequests.h"
#include <iostream>
#include <string.h>
#if (defined(__APPLE__) || defined(__linux__))
# include <unistd.h>
#endif // defined(__APPLE__) || defined(__linux__)
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/os/all.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace YarpPlusPlus;
using std::cout;
using std::cerr;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief Set to @c true to use an in-memory database and @c false to use a disk-based database. */
#define USE_INMEMORY false//true

/*! @brief Run loop control; @c true if the service is to keep going and @c false otherwise. */
static bool lKeepRunning;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if (defined(__APPLE__) || defined(__linux__))
/*! @brief The signal handler to catch requests to stop the service.
 @param signal The signal being handled. */
static void stopRunning(int signal)
{
# pragma unused(signal)
    lKeepRunning = false;
} // stopRunning
#endif // defined(__APPLE__) || defined(__linux__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for creating the Service Registry service.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the Service Registry service.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int     argc,
         char ** argv)
{
#if defined(SERVICES_DEBUG_TO_STDERR)
    OD_SYSLOG_INIT(*argv, kODSyslogOptionIncludeProcessID | kODSyslogOptionIncludeThreadID |//####
                   kODSyslogOptionWriteToStderr | kODSyslogOptionEnableThreadSupport);//####
#else // ! defined(SERVICES_DEBUG_TO_STDERR)
    OD_SYSLOG_INIT(*argv, kODSyslogOptionIncludeProcessID | kODSyslogOptionIncludeThreadID |//####
                   kODSyslogOptionEnableThreadSupport);//####
#endif // ! defined(SERVICES_DEBUG_TO_STDERR)
    OD_SYSLOG_ENTER();//####
    try
    {
        if (yarp::os::Network::checkNetwork())
        {
#if (defined(ENABLE_OD_SYSLOG) && defined(DEBUG_INCLUDES_YARP_TRACE))
            yarp::os::Network::setVerbosity(1);
#else // ! (defined(ENABLE_OD_SYSLOG) && defined(DEBUG_INCLUDES_YARP_TRACE))
            yarp::os::Network::setVerbosity(-1);
#endif // ! (defined(ENABLE_OD_SYSLOG) && defined(DEBUG_INCLUDES_YARP_TRACE))
            yarp::os::Network yarp; // This is necessary to establish any connection to the YARP infrastructure
            RegistryService * stuff = NULL;
            
            YarpPlusPlus::Initialize();
            if (1 <= argc)
            {
                switch (argc)
                {
                        // Argument order for tests = endpoint name [, IP address / name [, port]]
                    case 1:
                        stuff = new RegistryService(USE_INMEMORY);
                        break;
                        
                    case 2:
                        stuff = new RegistryService(USE_INMEMORY, argv[1]);
                        break;
                        
                    case 3:
                        stuff = new RegistryService(USE_INMEMORY, argv[1], argv[2]);
                        break;
                        
                    default:
                        break;
                        
                }
            }
            if (stuff)
            {
                if (stuff->start())
                {
                    // Note that the Registry Service is self-registering... so we don't need to call
                    // RegisterLocalService().
                    lKeepRunning = true;
#if (defined(__APPLE__) || defined(__linux__))
                    signal(SIGHUP, stopRunning);
                    signal(SIGINT, stopRunning);
                    signal(SIGINT, stopRunning);
                    signal(SIGUSR1, stopRunning);
#endif // defined(__APPLE__) || defined(__linux__)
                    for ( ; lKeepRunning; )
                    {
#if defined(MAIN_DOES_DELAY_NOT_YIELD)
                        yarp::os::Time::delay(1.0);
#else // ! defined(MAIN_DOES_DELAY_NOT_YIELD)
                        yarp::os::Time::yield();
#endif // ! defined(MAIN_DOES_DELAY_NOT_YIELD)
                    }
                    stuff->stop();
                }
                else
                {
                    OD_SYSLOG("! (stuff->start())");//####
                }
                delete stuff;
            }
            else
            {
                OD_SYSLOG("! (stuff)");//####
            }
        }
        else
        {
            OD_SYSLOG("! (yarp::os::Network::checkNetwork())");//####
            cerr << "YARP network not running." << endl;
        }
    }
    catch (...)
    {
        OD_SYSLOG("Exception caught");//####
    }
    yarp::os::Network::fini();
    OD_SYSLOG_EXIT_L(0);//####
    return 0;
} // main
