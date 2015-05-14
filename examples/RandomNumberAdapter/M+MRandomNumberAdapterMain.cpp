//--------------------------------------------------------------------------------------------------
//
//  File:       M+MRandomNumberAdapterMain.cpp
//
//  Project:    M+M
//
//  Contains:   The main application for the Random Number adapter.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and / or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-03-27
//
//--------------------------------------------------------------------------------------------------

#include "M+MRandomNumberAdapterData.h"
#include "M+MRandomNumberClient.h"
#include "M+MRandomNumberInputHandler.h"

#include <mpm/M+MAdapterChannel.h>
#include <mpm/M+MAdapterTwoArguments.h>
#include <mpm/M+MUtilities.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The main application for the Random Number adapter. */

/*! @dir RandomNumberAdapter
 @brief The set of files that implement the Random Number adapter. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Example;
using std::cerr;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The criteria used by the adapter to find its service. */
#define MATCHING_CRITERIA "keyword: random"

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Set up the environment and perform the operation.
 @param inputName The name to be used for the input channel.
 @param outputName The name to be used for the output channel. */
#if defined(MpM_ReportOnConnections)
static void setUpAndGo(const YarpString &      inputName,
                       const YarpString &      outputName,
                       ChannelStatusReporter * reporter)
#else // ! defined(MpM_ReportOnConnections)
static void setUpAndGo(const YarpString & inputName,
                       const YarpString & outputName)
#endif // ! defined(MpM_ReportOnConnections)
{
    OD_LOG_ENTER(); //####
#if defined(MpM_ReportOnConnections)
    OD_LOG_P1("reporter = ", reporter); //####
#endif // defined(MpM_ReportOnConnections)
    OD_LOG_S2s("inputName = ", inputName, "outputName = ", outputName); //####
    RandomNumberClient * stuff = new RandomNumberClient;
    
    if (stuff)
    {
        StartRunning();
        SetSignalHandlers(SignalRunningStop);
        if (stuff->findService(MATCHING_CRITERIA))
        {
#if defined(MpM_ReportOnConnections)
            stuff->setReporter(*reporter, true);
#endif // defined(MpM_ReportOnConnections)
            if (stuff->connectToService())
            {
                Common::AdapterChannel *   inputChannel = new Common::AdapterChannel(false);
                Common::AdapterChannel *   outputChannel = new Common::AdapterChannel(true);
                RandomNumberAdapterData    sharedData(stuff, outputChannel);
                RandomNumberInputHandler * inputHandler = new RandomNumberInputHandler(sharedData);
                
                if (inputChannel && outputChannel && inputHandler)
                {
#if defined(MpM_ReportOnConnections)
                    inputChannel->setReporter(*reporter);
                    inputChannel->getReport(*reporter);
                    outputChannel->setReporter(*reporter);
                    outputChannel->getReport(*reporter);
#endif // defined(MpM_ReportOnConnections)
                    if (inputChannel->openWithRetries(inputName, STANDARD_WAIT_TIME) &&
                        outputChannel->openWithRetries(outputName, STANDARD_WAIT_TIME))
                    {
                        double announcedTime = yarp::os::Time::now();
                        
                        stuff->addAssociatedChannel(inputChannel);
                        stuff->addAssociatedChannel(outputChannel);
                        sharedData.activate();
                        inputChannel->setReader(*inputHandler);
                        for ( ; IsRunning() && sharedData.isActive(); )
                        {
#if defined(MpM_MainDoesDelayNotYield)
                            yarp::os::Time::delay(ONE_SECOND_DELAY);
#else // ! defined(MpM_MainDoesDelayNotYield)
                            yarp::os::Time::yield();
#endif // ! defined(MpM_MainDoesDelayNotYield)
                            if (IsRunning())
                            {
                                double now = yarp::os::Time::now();
                                
                                if ((announcedTime + ANNOUNCE_INTERVAL) <= now)
                                {
                                    // Report associated channels again, in case the
                                    // Registry Service has been restarted.
                                    announcedTime = now;
                                    stuff->addAssociatedChannel(inputChannel);
                                    stuff->addAssociatedChannel(outputChannel);
                                }
                            }
                            else
                            {
                                sharedData.deactivate();
                            }
                        }
                        stuff->removeAssociatedChannels();
                    }
                    else
                    {
                        OD_LOG("! (inputChannel->openWithRetries(inputName, " //####
                               "STANDARD_WAIT_TIME) && " //####
                               "outputChannel->openWithRetries(outputName, " //####
                               "STANDARD_WAIT_TIME))"); //####
#if MAC_OR_LINUX_
                        GetLogger().fail("Problem opening a channel.");
#endif // MAC_OR_LINUX_
                    }
#if defined(MpM_DoExplicitClose)
                    inputChannel->close();
                    outputChannel->close();
#endif // defined(MpM_DoExplicitClose)
                }
                else
                {
                    OD_LOG("! (controlChannel && inputChannel && outputChannel && " //####
                           "controlHandler && inputHandler)"); //####
#if MAC_OR_LINUX_
                    GetLogger().fail("Problem creating a channel.");
#endif // MAC_OR_LINUX_
                }
                BaseChannel::RelinquishChannel(inputChannel);
                BaseChannel::RelinquishChannel(outputChannel);
                if (! stuff->disconnectFromService())
                {
                    OD_LOG("(! stuff->disconnectFromService())"); //####
#if MAC_OR_LINUX_
                    GetLogger().fail("Problem disconnecting from the service.");
#endif // MAC_OR_LINUX_
                }
            }
            else
            {
                OD_LOG("! (stuff->connectToService())"); //####
#if MAC_OR_LINUX_
                GetLogger().fail("Could not connect to the required service.");
#else // ! MAC_OR_LINUX_
                cerr << "Could not connect to the required service." << endl;
#endif // ! MAC_OR_LINUX_
            }
        }
        else
        {
            OD_LOG("! (stuff->findService(\"keyword: random\"))"); //####
#if MAC_OR_LINUX_
            GetLogger().fail("Could not find the required service.");
#else // ! MAC_OR_LINUX_
            cerr << "Could not find the required service." << endl;
#endif // ! MAC_OR_LINUX_
        }
        delete stuff;
    }
    else
    {
        OD_LOG("! (stuff)"); //####
    }
    OD_LOG_EXIT(); //####
} // setUpAndGo

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for creating the Random Number adapter.
 
 The program creates two YARP ports: an output port and an input port. Integers received in the
 input port are sent to the service as the number of random numbers to generate, with zero
 indicating that the program is to exit.
 The first, optional, argument is the port name to be used for the input port and the second,
 optional, argument is the name to be used for the output port. If the first argument is missing,
 the input port will be named ADAPTER_PORT_NAME_BASE+input/randomnumber and if the second argument
 is missing, the output port will be named ADAPTER_PORT_NAME_BASE+output/randomnumber.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the application.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int      argc,
         char * * argv)
{
    OD_LOG_INIT(*argv, kODLoggingOptionIncludeProcessID | kODLoggingOptionIncludeThreadID | //####
                kODLoggingOptionEnableThreadSupport | kODLoggingOptionWriteToStderr); //####
    OD_LOG_ENTER(); //####
#if MAC_OR_LINUX_
    SetUpLogger(*argv);
#endif // MAC_OR_LINUX_
    try
    {
        YarpString          inputName;
        YarpString          outputName;
        AdapterTwoArguments argsHandler(T_(" [inputName [outputName]]"),
                                        T_("  inputName  Optional name for the input channel\n"
                                           "  outputName Optional name for the output channel"),
                                        T_(ADAPTER_PORT_NAME_BASE "input/randomnumber"),
                                        T_(ADAPTER_PORT_NAME_BASE "output/randomnumber"),
                                        inputName, outputName);
        
        if (Utilities::ProcessStandardAdapterOptions(argc, argv, argsHandler,
                                                     "The Random Number adapter", MATCHING_CRITERIA,
                                                     2014, STANDARD_COPYRIGHT_NAME))
        {
            Utilities::SetUpGlobalStatusReporter();
            Utilities::CheckForNameServerReporter();
            if (Utilities::CheckForValidNetwork())
            {
#if defined(MpM_ReportOnConnections)
                ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)
                yarp::os::Network       yarp; // This is necessary to establish any connections to
                                              // the YARP infrastructure
                
                Initialize(*argv);
                if (Utilities::CheckForRegistryService())
                {
#if defined(MpM_ReportOnConnections)
                    setUpAndGo(inputName, outputName, reporter);
#else // ! defined(MpM_ReportOnConnections)
                    setUpAndGo(inputName, outputName);
#endif // ! defined(MpM_ReportOnConnections)
                }
                else
                {
                    OD_LOG("! (Utilities::CheckForRegistryService())"); //####
#if MAC_OR_LINUX_
                    GetLogger().fail("Registry Service not running.");
#else // ! MAC_OR_LINUX_
                    cerr << "Registry Service not running." << endl;
#endif // ! MAC_OR_LINUX_
                }
            }
            else
            {
                OD_LOG("! (Utilities::CheckForValidNetwork())"); //####
#if MAC_OR_LINUX_
                GetLogger().fail("YARP network not running.");
#else // ! MAC_OR_LINUX_
                cerr << "YARP network not running." << endl;
#endif // ! MAC_OR_LINUX_
            }
            Utilities::ShutDownGlobalStatusReporter();
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
    }
    yarp::os::Network::fini();
    OD_LOG_EXIT_L(0); //####
    return 0;
} // main
