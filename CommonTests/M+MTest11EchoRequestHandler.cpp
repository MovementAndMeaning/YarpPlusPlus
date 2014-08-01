//--------------------------------------------------------------------------------------------------
//
//  File:       CommonTests/M+MTest11EchoRequestHandler.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a simple request handler used by the unit tests.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
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
//  Created:    2014-02-28
//
//--------------------------------------------------------------------------------------------------

#include "M+MTest11EchoRequestHandler.h"
#include <mpm/M+MRequests.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a simple request handler used by the unit tests. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Test;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

Test11EchoRequestHandler::Test11EchoRequestHandler(void) :
    inherited(MpM_ECHO_REQUEST)
{
    OD_LOG_ENTER(); //####
    OD_LOG_EXIT_P(this); //####
} // Test11EchoRequestHandler::Test11EchoRequestHandler

Test11EchoRequestHandler::~Test11EchoRequestHandler(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // Test11EchoRequestHandler::~Test11EchoRequestHandler

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void Test11EchoRequestHandler::fillInAliases(MplusM::Common::StringVector & alternateNames)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(alternateNames)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("alternateNames = ", &alternateNames); //####
    OD_LOG_OBJEXIT(); //####
} // Test11EchoRequestHandler::fillInAliases

void Test11EchoRequestHandler::fillInDescription(const yarp::os::ConstString & request,
                                                 yarp::os::Property &          info)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(request,info)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("request = ", request); //####
    OD_LOG_P1("info = ", &info); //####
    OD_LOG_OBJEXIT(); //####
} // Test11EchoRequestHandler::fillInDescription

bool Test11EchoRequestHandler::processRequest(const yarp::os::ConstString & request,
                                              const yarp::os::Bottle &      restOfInput,
                                              const yarp::os::ConstString & senderChannel,
                                              yarp::os::ConnectionWriter *  replyMechanism)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(request,senderChannel)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S3s("request = ", request, "restOfInput = ", restOfInput.toString(), //####
               "senderChannel = ", senderChannel); //####
    OD_LOG_P1("replyMechanism = ", replyMechanism); //####
    bool result = true;
    
    if (replyMechanism)
    {
        OD_LOG("(replyMechanism)"); //####
        yarp::os::Bottle argsCopy(restOfInput);
        
        OD_LOG_S1s("argsCopy <- ", argsCopy.toString()); //####
        if (! argsCopy.write(*replyMechanism))
        {
            OD_LOG("(! argsCopy.write(*replyMechanism))"); //####
#if defined(MpM_StallOnSendProblem)
            Common::Stall();
#endif // defined(MpM_StallOnSendProblem)
        }
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // Test11EchoRequestHandler::processRequest

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)