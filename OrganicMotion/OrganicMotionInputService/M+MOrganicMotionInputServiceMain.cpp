//--------------------------------------------------------------------------------------------------
//
//  File:       M+MOrganicMotionInputServiceMain.cpp
//
//  Project:    M+M
//
//  Contains:   The main application for the Organic Motion input service.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by H Plus Technologies Ltd. and Simon Fraser University.
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
//  Created:    2015-06-25
//
//--------------------------------------------------------------------------------------------------

#include "M+MOrganicMotionInputService.h"
#include "M+MOrganicMotionInputRequests.h"

#include <mpm/M+MAddressArgumentDescriptor.h>
#include <mpm/M+MEndpoint.h>
#include <mpm/M+MPortArgumentDescriptor.h>
#include <mpm/M+MUtilities.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The main application for the Organic Motion input service. */

/*! @dir OrganicMotion
 @brief The set of files that implement the Organic Motion input service. */

/*! @dir OrganicMotionInputService
 @brief The set of files that implement the Organic Motion input service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::OrganicMotion;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Display the available commands. */
static void displayCommands(void)
{
    OD_LOG_ENTER(); //####
    cout << "Commands:" << endl;
    cout << "  ? - display this list" << endl;
    cout << "  b - start (begin) the output stream," << endl;
    cout << "  c - configure the service" << endl;
    cout << "  e - stop (end) the output stream" << endl;
    cout << "  q - quit the application" << endl;
    cout << "  r - restart the output stream" << endl;
    cout << "  u - reset the configuration (unconfigure) so that it will be reprocessed" << endl;
    OD_LOG_EXIT(); //####
} // displayCommands

/*! @brief Set up the environment and start the Organic Motion input service.
 @param hostName The IP address for the device server.
 @param hostPort The port for the device server.
 @param argumentList Descriptions of the arguments to the executable.
 @param progName The path to the executable.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the exemplar input service.
 @param tag The modifier for the service name and port names.
 @param serviceEndpointName The YARP name to be assigned to the new service.
 @param servicePortNumber The port being used by the service.
 @param goWasSet @c true if the service is to be started immediately.
 @param stdinAvailable @c true if running in the foreground and @c false otherwise.
 @param reportOnExit @c true if service metrics are to be reported on exit and @c false otherwise.
 */
static void setUpAndGo(YarpString &                        hostName,
	                   int &                               hostPort,
	                   const Utilities::DescriptorVector & argumentList,
					   const YarpString &                  progName,
                       const int                           argc,
                       char * *                            argv,
                       const YarpString &                  tag,
                       const YarpString &                  serviceEndpointName,
                       const YarpString &                  servicePortNumber,
                       const bool                          goWasSet,
                       const bool                          stdinAvailable,
                       const bool                          reportOnExit)
{
    OD_LOG_ENTER(); //####
	OD_LOG_S4s("hostName = ", hostName, "progName = ", progName, "tag = ", tag, //####
		"serviceEndpointName = ", serviceEndpointName); //####
	OD_LOG_S1s("servicePortNumber = ", servicePortNumber); //####
	OD_LOG_LL2("hostPort = ", hostPort, "argc = ", argc); //####
    OD_LOG_P1("argv = ", argv); //####
    OD_LOG_B3("goWasSet = ", goWasSet, "stdinAvailable = ", stdinAvailable, //####
              "reportOnExit = ", reportOnExit); //####
    OrganicMotionInputService * aService = new OrganicMotionInputService(progName, argc, argv, tag,
                                                                         serviceEndpointName,
                                                                         servicePortNumber);

    if (aService)
    {
        if (aService->start())
        {
            YarpString channelName(aService->getEndpoint().getName());
            
            OD_LOG_S1s("channelName = ", channelName); //####
            if (RegisterLocalService(channelName, *aService))
            {
                bool             configured = false;
                yarp::os::Bottle configureData;
                
                StartRunning();
                SetSignalHandlers(SignalRunningStop);
                aService->startPinger();
                if (goWasSet || (! stdinAvailable))
                {
					configureData.clear();
					configureData.addString(hostName);
					configureData.addInt(hostPort);
					if (aService->configure(configureData))
                    {
                        aService->startStreams();
                    }
                }
                for ( ; IsRunning(); )
                {
                    if ((! goWasSet) && stdinAvailable)
                    {
                        char inChar;
                        
                        cout << "Operation: [? b c e q r u]? ";
                        cout.flush();
                        cin >> inChar;
                        switch (inChar)
                        {
                            case '?' :
                                // Help
                                displayCommands();
                                break;
                                
                            case 'b' :
                            case 'B' :
                                // Start streams
                                if (! configured)
                                {
									configureData.clear();
									configureData.addString(hostName);
									configureData.addInt(hostPort);
									if (aService->configure(configureData))
                                    {
                                        configured = true;
                                    }
                                }
                                if (configured)
                                {
                                    aService->startStreams();
                                }
                                break;
                                
                            case 'c' :
                            case 'C' :
                                // Configure
								configureData.clear();
								configureData.addString(hostName);
								configureData.addInt(hostPort);
								if (aService->configure(configureData))
                                {
                                    configured = true;
                                }
                                break;
                                
                            case 'e' :
                            case 'E' :
                                // Stop streams
                                aService->stopStreams();
                                break;
                                
                            case 'q' :
                            case 'Q' :
                                // Quit
                                StopRunning();
                                break;
                                
                            case 'r' :
                            case 'R' :
                                // Restart streams
                                if (! configured)
                                {
									configureData.clear();
									configureData.addString(hostName);
									configureData.addInt(hostPort);
									if (aService->configure(configureData))
                                    {
                                        configured = true;
                                    }
                                }
                                if (configured)
                                {
                                    aService->restartStreams();
                                }
                                break;
                                
                            case 'u' :
                            case 'U' :
                                // Unconfigure
                                configured = false;
                                break;
                                
                            default :
                                cout << "Unrecognized request '" << inChar << "'." << endl;
                                break;
                                
                        }
                    }
                    else
                    {
#if defined(MpM_MainDoesDelayNotYield)
                        yarp::os::Time::delay(ONE_SECOND_DELAY_ / 10.0);
#else // ! defined(MpM_MainDoesDelayNotYield)
                        yarp::os::Time::yield();
#endif // ! defined(MpM_MainDoesDelayNotYield)
                    }
                }
                UnregisterLocalService(channelName, *aService);
                if (reportOnExit)
                {
                    yarp::os::Bottle metrics;
                    
                    aService->gatherMetrics(metrics);
                    YarpString converted(Utilities::ConvertMetricsToString(metrics));
                    
                    cout << converted.c_str() << endl;
                }
                aService->stop();
            }
            else
            {
                OD_LOG("! (RegisterLocalService(channelName, *aService))"); //####
#if MAC_OR_LINUX_
                GetLogger().fail("Service could not be registered.");
#else // ! MAC_OR_LINUX_
                cerr << "Service could not be registered." << endl;
#endif // ! MAC_OR_LINUX_
            }
        }
        else
        {
            OD_LOG("! (aService->start())"); //####
#if MAC_OR_LINUX_
            GetLogger().fail("Service could not be started.");
#else // ! MAC_OR_LINUX_
            cerr << "Service could not be started." << endl;
#endif // ! MAC_OR_LINUX_
        }
        delete aService;
    }
    else
    {
        OD_LOG("! (aService)"); //####
    }
    OD_LOG_EXIT(); //####
} // setUpAndGo

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for running the Organic Motion input service.
 
 The second, optional, argument is the port number to be used and the first, optional, argument is
 the name of the channel to be used. There is no output.
 The option 'p' specifies the burst period, in seconds, while the option 's' specifies the number of
 random values to generate in each burst.
 The option 'r' indicates that the service metrics are to be reported on exit.
 The option 't' specifies the tag modifier, which is applied to the name of the channel, if the
 name was not specified. It is also applied to the service name as a suffix.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the exemplar input service.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int      argc,
         char * * argv)
{
    YarpString progName(*argv);

#if defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(progName.c_str(), kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionWriteToStderr | //####
                kODLoggingOptionEnableThreadSupport); //####
#else // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(progName.c_str(), kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionEnableThreadSupport); //####
#endif // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_ENTER(); //####
#if MAC_OR_LINUX_
    SetUpLogger(progName);
#endif // MAC_OR_LINUX_
    try
    {
        bool                                 goWasSet = false;
        bool                                 nameWasSet = false; // not used
        bool                                 reportOnExit = false;
        bool                                 stdinAvailable = CanReadFromStandardInput();
		int                                  hostPort;
		YarpString                           hostName;
		YarpString                           serviceEndpointName;
        YarpString                           servicePortNumber;
        YarpString                           tag;
		Utilities::AddressArgumentDescriptor firstArg("hostname",
			                                          T_("IP address for the device server"),
			                                          "localhost", true, &hostName);
		Utilities::PortArgumentDescriptor    secondArg("port", T_("Port for the device server"),
			                                           ORGANICMOTIONINPUT_DEFAULT_PORT_, true,
													   false, &hostPort);
		Utilities::DescriptorVector          argumentList;

        if (ProcessStandardServiceOptions(argc, argv, argumentList,
                                          DEFAULT_ORGANICMOTIONINPUT_SERVICE_NAME_,
                                          ORGANICMOTIONINPUT_SERVICE_DESCRIPTION_, "", 2015,
                                          STANDARD_COPYRIGHT_NAME_, goWasSet, nameWasSet,
                                          reportOnExit, tag, serviceEndpointName, servicePortNumber,
                                          kSkipNone))
        {
            Utilities::CheckForNameServerReporter();
            if (Utilities::CheckForValidNetwork())
            {
                yarp::os::Network yarp; // This is necessary to establish any connections to the
                                        // YARP infrastructure
                
                Initialize(progName);
                if (Utilities::CheckForRegistryService())
                {
					setUpAndGo(hostName, hostPort, argumentList, progName, argc, argv, tag,
						       serviceEndpointName, servicePortNumber, goWasSet, stdinAvailable,
							   reportOnExit);
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