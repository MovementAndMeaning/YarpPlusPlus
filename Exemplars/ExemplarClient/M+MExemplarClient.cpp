//--------------------------------------------------------------------------------------------------
//
//  File:       M+MExemplarClient.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the exemplar client.
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
//  Created:    2014-09-15
//
//--------------------------------------------------------------------------------------------------

#include "M+MExemplarClient.h"
#include "M+MExemplarRequests.h"

#include <mpm/M+MServiceResponse.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the exemplar client. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Exemplars;

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
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ExemplarClient::ExemplarClient(void) :
    inherited("exemplars/simple_")
{
    OD_LOG_ENTER(); //####
    OD_LOG_EXIT_P(this); //####
} // ExemplarClient::ExemplarClient

ExemplarClient::~ExemplarClient(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ExemplarClient::~ExemplarClient

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool ExemplarClient::getOneRandomNumber(double & result)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("result = ", &result); //####
    bool okSoFar = false;
    
    try
    {
        yarp::os::Bottle        parameters;
        Common::ServiceResponse response;
        
        reconnectIfDisconnected(NULL, NULL);
        if (send(MpM_SIMPLE_REQUEST, parameters, &response))
        {
            if (1 == response.count())
            {
                yarp::os::Value retrieved(response.element(0));
                
                if (retrieved.isDouble())
                {
                    result = retrieved.asDouble();
                    okSoFar = true;
                }
                else
                {
                    OD_LOG("! (retrieved.isDouble())"); //####
                }
            }
            else
            {
                OD_LOG("! (1 == response.count())"); //####
                OD_LOG_S1s("response = ", response.asString()); //####
            }
        }
        else
        {
            OD_LOG("! (send(MpM_SIMPLE_REQUEST, parameters, &response))"); //####
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(okSoFar); //####
    return okSoFar;
} // ExemplarClient::getOneRandomNumber

bool ExemplarClient::getRandomNumbers(const int              howMany,
                                      Common::DoubleVector & result)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("howMany = ", howMany); //####
    OD_LOG_P1("result = ", &result); //####
    bool okSoFar = false;
    
    try
    {
        if (0 < howMany)
        {
            yarp::os::Bottle        parameters;
            Common::ServiceResponse response;
            
            parameters.addInt(howMany);
            reconnectIfDisconnected(NULL, NULL);
            if (send(MpM_SIMPLE_REQUEST, parameters, &response))
            {
                if (howMany == response.count())
                {
                    result.clear();
                    okSoFar = true;
                    for (int ii = 0; ii < howMany; ++ii)
                    {
                        yarp::os::Value retrieved(response.element(ii));
                        
                        if (retrieved.isDouble())
                        {
                            result.push_back(retrieved.asDouble());
                        }
                        else
                        {
                            OD_LOG("! (retrieved.isDouble())"); //####
                            okSoFar = false;
                            break;
                        }
                        
                    }
                }
                else
                {
                    OD_LOG("! (howMany == response.count())"); //####
                    OD_LOG_S1s("response = ", response.asString()); //####
                }
            }
            else
            {
                OD_LOG("! (send(MpM_SIMPLE_REQUEST, parameters, &response))"); //####
            }
        }
        else
        {
            OD_LOG("! (0 < howMany)"); //####
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(okSoFar); //####
    return okSoFar;
} // ExemplarClient::getRandomNumbers

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
