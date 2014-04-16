//--------------------------------------------------------------------------------------
//
//  File:       ClientList/main.cpp
//
//  Project:    M+M
//
//  Contains:   A utility application to list the clients of a service or all services.
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
//  Created:    2014-03-12
//
//--------------------------------------------------------------------------------------

#include "M+MBaseClient.h"
#include "M+MClientChannel.h"
#include "M+MRequests.h"
#include "M+MServiceRequest.h"
#include "M+MServiceResponse.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#include <iostream>
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

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief A utility application to list the clients of a service or all services. */

/*! @dir ClientList
 @brief The ClientList application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using std::cout;
using std::cerr;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(SERVICES_HAVE_CONTEXTS)
/*! @brief Process the response to the 'list' request sent to a service.
 @param serviceName The name of the service that generated the response.
 @param response The response to be processed.
 @returns @c true if some output was generated and @c false otherwise. */
static bool processResponse(const yarp::os::ConstString &           serviceName,
                            const MplusM::Common::ServiceResponse & response)
{
    OD_LOG_ENTER();//####
    OD_LOG_S1("serviceName = ", serviceName.c_str());//####
    OD_LOG_P1("response = ", &response);//####
    bool result = false;
    
    OD_LOG_S1("response = ", response.asString().c_str());//####
    for (int ii = 0, howMany = response.count(); ii < howMany; ++ii)
    {
        yarp::os::Value element(response.element(ii));
        
        if (element.isString())
        {
            yarp::os::ConstString clientString(element.toString());
            
            if (! result)
            {
                cout << "Service: " << serviceName.c_str() << endl << "Clients: " << endl;
            }
            cout << "   " << clientString.c_str() << endl;
            result = true;
        }
    }
    OD_LOG_EXIT_B(result);//####
    return result;
} // processResponse
#endif // defined(SERVICES_HAVE_CONTEXTS)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for listing the clients of a service.
 
 The optional argument is the name of the channel for the service. If the channel is not specified, all service channels
 will be reported. Standard output will receive a list of the specified clients.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the example client.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int      argc,
         char * * argv)
{
#if (! defined(SERVICES_HAVE_CONTEXTS))
# pragma unused(argc)
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    OD_LOG_INIT(*argv, kODLoggingOptionIncludeProcessID | kODLoggingOptionIncludeThreadID |//####
                kODLoggingOptionEnableThreadSupport | kODLoggingOptionWriteToStderr);//####
    OD_LOG_ENTER();//####
#if defined(SERVICES_HAVE_CONTEXTS)
    try
    {
        if (yarp::os::Network::checkNetwork())
        {
            yarp::os::Network     yarp; // This is necessary to establish any connection to the YARP infrastructure
            yarp::os::ConstString channelNameRequest(MpM_REQREP_DICT_CHANNELNAME_KEY ":");
            
            MplusM::Common::Initialize(*argv);
            if (1 < argc)
            {
                channelNameRequest += argv[1];
            }
            else
            {
                channelNameRequest += "*";
            }
            MplusM::Common::Package matches(MplusM::Common::FindMatchingServices(channelNameRequest.c_str()));
            
            if (MpM_EXPECTED_MATCH_RESPONSE_SIZE == matches.size())
            {
                // First, check if the search succeeded.
                yarp::os::ConstString matchesFirstString(matches.get(0).toString());
                
                if (strcmp(MpM_OK_RESPONSE, matchesFirstString.c_str()))
                {
                    OD_LOG("(strcmp(MpM_OK_RESPONSE, matchesFirstString.c_str()))");//####
                    yarp::os::ConstString reason(matches.get(1).toString());
                    
                    cerr << "Failed: " << reason.c_str() << "." << endl;
                }
                else
                {
                    // Now, process the second element.
                    MplusM::Common::Package * matchesList = matches.get(1).asList();
                    
                    if (matchesList)
                    {
                        int matchesCount = matchesList->size();
                        
                        if (matchesCount)
                        {
                            yarp::os::ConstString           aName =
                                                        MplusM::Common::GetRandomChannelName("/clientlist/channel_");
                            MplusM::Common::ClientChannel * newChannel = new MplusM::Common::ClientChannel;
                            
                            if (newChannel)
                            {
                                if (newChannel->openWithRetries(aName))
                                {
                                    bool                    sawRequestResponse = false;
                                    MplusM::Common::Package parameters;

                                    for (int ii = 0; ii < matchesCount; ++ii)
                                    {
                                        yarp::os::ConstString aMatch(matchesList->get(ii).toString());
                                        
                                        if (MplusM::Common::NetworkConnectWithRetries(aName, aMatch))
                                        {
                                            MplusM::Common::ServiceRequest  request(MpM_CLIENTS_REQUEST, parameters);
                                            MplusM::Common::ServiceResponse response;
                                            
                                            if (request.send(*newChannel, &response))
                                            {
                                                OD_LOG("(request.send(*newChannel, &response))");//####
                                                if (0 < response.count())
                                                {
                                                    OD_LOG("(0 < response.count())");//####
                                                    if (processResponse(aMatch, response))
                                                    {
                                                        sawRequestResponse = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                OD_LOG("! (request.send(*newChannel, &response))");//####
                                                cerr << "Problem communicating with " << aMatch.c_str() << "." <<
                                                endl;
                                            }
# if defined(MpM_DO_EXPLICIT_DISCONNECT)
                                            if (! MplusM::Common::NetworkDisconnectWithRetries(aName, aMatch))
                                            {
                                                OD_LOG("(! MplusM::Common::NetworkDisconnectWithRetries(aName, "//####
                                                       "aMatch))");//####
                                            }
# endif // defined(MpM_DO_EXPLICIT_DISCONNECT)
                                        }
                                        else
                                        {
                                            OD_LOG("! (MplusM::Common::NetworkConnectWithRetries(aName, "//####
                                                   "aMatch))");//####
                                        }
                                    }
                                    if (! sawRequestResponse)
                                    {
                                        cout << "No client connections found." << endl;
                                    }
# if defined(MpM_DO_EXPLICIT_CLOSE)
                                    newChannel->close();
# endif // defined(MpM_DO_EXPLICIT_CLOSE)
                                }
                                else
                                {
                                    OD_LOG("! (newChannel->openWithRetries(aName))");//####
                                }
                                delete newChannel;
                            }
                            else
                            {
                                OD_LOG("! (newChannel)");//####
                            }
                        }
                        else
                        {
                            cout << "No services found." << endl;
                        }
                    }
                    else
                    {
                        OD_LOG("! (matchesList)");//####
                    }
                }
            }
            else
            {
                OD_LOG("! (MpM_EXPECTED_MATCH_RESPONSE_SIZE == matches.size())");//####
                cerr << "Problem getting information from the Service Registry." << endl;
            }
        }
        else
        {
            OD_LOG("! (yarp::os::Network::checkNetwork())");//####
            cerr << "YARP network not running." << endl;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
    }
    yarp::os::Network::fini();
#else // ! defined(SERVICES_HAVE_CONTEXTS)
    cout << "Services do not have contexts, so the clients cannot be determined." << endl;
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    OD_LOG_EXIT_L(0);//####
    return 0;
} // main