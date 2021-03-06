//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mServiceResponse.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the response to an m+m request.
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
//  Created:    2014-02-06
//
//--------------------------------------------------------------------------------------------------

#include "m+mServiceResponse.hpp"

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the response to an m+m request. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global constants and variables
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

ServiceResponse::ServiceResponse(void) :
    _values()
{
    ODL_ENTER(); //####
    ODL_EXIT_P(this); //####
} // ServiceResponse::ServiceResponse

ServiceResponse::ServiceResponse(const yarp::os::Bottle & values) :
    _values(values)
{
    ODL_ENTER(); //####
    ODL_I1("values size = ", values.size()); //####
    ODL_S1s("values = ", values.toString()); //####
    ODL_EXIT_P(this); //####
} // ServiceResponse::ServiceResponse

ServiceResponse::~ServiceResponse(void)
{
    ODL_OBJENTER(); //####
    ODL_OBJEXIT(); //####
} // ServiceResponse::~ServiceResponse

ServiceResponse &
ServiceResponse::operator =(const yarp::os::Bottle & values)
{
    ODL_OBJENTER(); //####
    ODL_I1("values size = ", values.size()); //####
    ODL_S1s("values = ", values.toString()); //####
    _values = values;
    ODL_OBJEXIT_P(this); //####
    return *this;
} // ServiceResponse::operator=

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

/*! @brief Return a printable version of the response.
 @return A printable version of the response. */
YarpString
ServiceResponse::asString(void)
const
{
    ODL_OBJENTER(); //####
    YarpString result(_values.toString());

    ODL_OBJEXIT_S(result.c_str()); //####
    return result;
} // ServiceResponse::asString

yarp::os::Value
ServiceResponse::element(const int index)
const
{
    ODL_OBJENTER(); //####
    yarp::os::Value result;

    try
    {
        if ((index >= 0) && (index < static_cast<int>(_values.size())))
        {
            result = _values.get(index);
        }
        else
        {
            ODL_LOG("! ((index >= 0) && (index < static_cast<int>(_values.size())))"); //####
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_S(result.toString().c_str()); //####
    return result;
} // ServiceResponse::element

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
