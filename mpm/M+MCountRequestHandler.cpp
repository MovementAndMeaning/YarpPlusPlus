//--------------------------------------------------------------------------------------------------
//
//  File:       mpm/M+MCountRequestHandler.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the request handler for the standard 'count' request.
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
//  Created:    2014-02-27
//
//--------------------------------------------------------------------------------------------------

#include "M+MCountRequestHandler.h"

#include <mpm/M+MBaseService.h>
#include <mpm/M+MRequests.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the request handler for the standard 'count' request. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The protocol version number for the 'count' request. */
#define COUNT_REQUEST_VERSION_NUMBER "1.0"

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

CountRequestHandler::CountRequestHandler(BaseService & service) :
    inherited(MpM_COUNT_REQUEST), _service(service)
{
    OD_LOG_ENTER(); //####
    OD_LOG_EXIT_P(this); //####
} // CountRequestHandler::CountRequestHandler

CountRequestHandler::~CountRequestHandler(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // CountRequestHandler::~CountRequestHandler

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void CountRequestHandler::fillInAliases(StringVector & alternateNames)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(alternateNames)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("alternateNames = ", &alternateNames); //####
    OD_LOG_OBJEXIT(); //####
} // CountRequestHandler::fillInAliases

void CountRequestHandler::fillInDescription(const yarp::os::ConstString & request,
                                            yarp::os::Property &          info)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("request = ", request); //####
    OD_LOG_P1("info = ", &info); //####
    try
    {
        info.put(MpM_REQREP_DICT_REQUEST_KEY, request);
        info.put(MpM_REQREP_DICT_VERSION_KEY, COUNT_REQUEST_VERSION_NUMBER);
        info.put(MpM_REQREP_DICT_OUTPUT_KEY, MpM_REQREP_INT MpM_REQREP_DOUBLE);
        info.put(MpM_REQREP_DICT_DETAILS_KEY,
                 "Report the number of requests seen and the current time\n"
                 "Input: nothing\n"
                 "Output: the number of requests (two integers, high first) and the time");
        yarp::os::Value    keywords;
        yarp::os::Bottle * asList = keywords.asList();
        
        asList->addString(request);
        info.put(MpM_REQREP_DICT_KEYWORDS_KEY, keywords);
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // CountRequestHandler::fillInDescription

bool CountRequestHandler::processRequest(const yarp::os::ConstString & request,
                                         const yarp::os::Bottle &      restOfInput,
                                         const yarp::os::ConstString & senderChannel,
                                         yarp::os::ConnectionWriter *  replyMechanism)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(request,restOfInput,senderChannel)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S3s("request = ", request, "restOfInput = ", restOfInput.toString(), //####
               "senderChannel = ", senderChannel); //####
    OD_LOG_P1("replyMechanism = ", replyMechanism); //####
    bool result = true;
    
    try
    {
        if (replyMechanism)
        {
            OD_LOG("(replyMechanism)"); //####
            double           elapsedTime;
            int64_t          counter;
            yarp::os::Bottle reply;
            
            _service.getStatistics(counter, elapsedTime);
            reply.addInt(static_cast<int> (counter >> 32));
            reply.addInt(static_cast<int> (counter));
            reply.addDouble(elapsedTime);
            OD_LOG_S1s("reply <- ", reply.toString()); //####
            if (! reply.write(*replyMechanism))
            {
                OD_LOG("(! reply.write(*replyMechanism))"); //####
#if defined(MpM_StallOnSendProblem)
                Common::Stall();
#endif // defined(MpM_StallOnSendProblem)
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // CountRequestHandler::processRequest

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)