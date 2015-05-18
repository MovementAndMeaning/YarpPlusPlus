//--------------------------------------------------------------------------------------------------
//
//  File:       mpm/M+MStringArgumentDescriptor.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the minimal functionality required to represent a
//              string-type command-line argument.
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
//  Created:    2015-05-15
//
//--------------------------------------------------------------------------------------------------

#include <mpm/M+MStringArgumentDescriptor.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The definition for the minimal functionality required to represent a string-type
 command-line argument. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Utilities;

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

StringArgumentDescriptor::StringArgumentDescriptor(const YarpString & argName,
                                                   const YarpString & argDescription,
                                                   const YarpString & defaultValue,
                                                   const bool         isOptional,
                                                   YarpString *       argumentReference) :
    inherited(argName, argDescription, defaultValue, isOptional, argumentReference)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S3("argName = ", argName, "argDescription = ", argDescription, "defaultValue = ", //####
              defaultValue); //####
    OD_LOG_B1("isOptional = ", isOptional); //####
    OD_LOG_P1("argumentReference = ", argumentReference); //####
    OD_LOG_EXIT_P(this); //####
} // StringArgumentDescriptor::StringArgumentDescriptor

StringArgumentDescriptor::~StringArgumentDescriptor(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // StringArgumentDescriptor::~StringArgumentDescriptor

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

Common::YarpString StringArgumentDescriptor::toString(void)
const
{
    OD_LOG_OBJENTER(); //####
    Common::YarpString result(isOptional() ? "s" : "S");
    
    result += standardFields();
    OD_LOG_OBJEXIT_s(result); //####
    return result;
} // StringArgumentDescriptor::toString

bool StringArgumentDescriptor::validate(const Common::YarpString & value)
const
{
    OD_LOG_OBJENTER(); //####
    bool result = true;
    
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // StringArgumentDescriptor::validate

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)