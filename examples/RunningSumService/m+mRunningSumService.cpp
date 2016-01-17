//--------------------------------------------------------------------------------------------------
//
//  File:       m+mRunningSumService.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the Running Sum service.
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
//  Created:    2014-03-18
//
//--------------------------------------------------------------------------------------------------

#include "m+mRunningSumService.h"
#include "m+mAddToSumRequestHandler.h"
#include "m+mResetSumRequestHandler.h"
#include "m+mRunningSumContext.h"
#include "m+mRunningSumRequests.h"
#include "m+mStartSumRequestHandler.h"
#include "m+mStopSumRequestHandler.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Running Sum service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Example;

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

RunningSumService::RunningSumService(const YarpString & launchPath,
                                     const int          argc,
                                     char * *           argv,
                                     const YarpString & tag,
                                     const YarpString & serviceEndpointName,
                                     const YarpString & servicePortNumber) :
    inherited(kServiceKindNormal, launchPath, argc, argv, tag, true, MpM_RUNNINGSUM_CANONICAL_NAME_,
              RUNNINGSUM_SERVICE_DESCRIPTION_,
              "add - add one or more values to the running sum and return the sum\n"
              "reset - clear the running sum\n"
              "start - start adding values to the running sum\n"
              "stop - stop adding values to the running sum", serviceEndpointName,
              servicePortNumber), _addToSumHandler(NULL), _resetSumHandler(NULL),
    _startSumHandler(NULL), _stopSumHandler(NULL)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S4s("launchPath = ", launchPath, "tag = ", tag, "serviceEndpointName = ", //####
               serviceEndpointName, "servicePortNumber = ", servicePortNumber); //####
    OD_LOG_LL1("argc = ", argc); //####
    OD_LOG_P1("argv = ", argv); //####
    attachRequestHandlers();
    OD_LOG_EXIT_P(this); //####
} // RunningSumService::RunningSumService

RunningSumService::~RunningSumService(void)
{
    OD_LOG_OBJENTER(); //####
    detachRequestHandlers();
    OD_LOG_OBJEXIT(); //####
} // RunningSumService::~RunningSumService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

double
RunningSumService::addToSum(const YarpString & key,
                            const double       value)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("key = ", key); //####
    OD_LOG_D1("value = ", value); //####
    double result = 0.0;
    
    try
    {
        RunningSumContext * context = (RunningSumContext *) findContext(key);
        
        if (! context)
        {
            context = new RunningSumContext;
            addContext(key, context);
        }
        context->sum() += value;
        result = context->sum();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_D(result); //####
    return result;
} // RunningSumService::addToSum

void
RunningSumService::attachRequestHandlers(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        _addToSumHandler = new AddToSumRequestHandler(*this);
        _resetSumHandler = new ResetSumRequestHandler(*this);
        _startSumHandler = new StartSumRequestHandler(*this);
        _stopSumHandler = new StopSumRequestHandler(*this);
        if (_addToSumHandler && _resetSumHandler && _startSumHandler && _stopSumHandler)
        {
            registerRequestHandler(_addToSumHandler);
            registerRequestHandler(_resetSumHandler);
            registerRequestHandler(_startSumHandler);
            registerRequestHandler(_stopSumHandler);
        }
        else
        {
            OD_LOG("! (_addToSumHandler && _resetSumHandler && _startSumHandler && " //####
                   "_stopSumHandler)"); //####
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RunningSumService::attachRequestHandlers

void
RunningSumService::detachRequestHandlers(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (_addToSumHandler)
        {
            unregisterRequestHandler(_addToSumHandler);
            delete _addToSumHandler;
            _addToSumHandler = NULL;
        }
        if (_resetSumHandler)
        {
            unregisterRequestHandler(_resetSumHandler);
            delete _resetSumHandler;
            _resetSumHandler = NULL;
        }
        if (_startSumHandler)
        {
            unregisterRequestHandler(_startSumHandler);
            delete _startSumHandler;
            _startSumHandler = NULL;
        }
        if (_stopSumHandler)
        {
            unregisterRequestHandler(_stopSumHandler);
            delete _stopSumHandler;
            _stopSumHandler = NULL;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RunningSumService::detachRequestHandlers

void
RunningSumService::resetSum(const YarpString & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("key = ", key); //####
    try
    {
        RunningSumContext * context = (RunningSumContext *) findContext(key);
        
        if (! context)
        {
            context = new RunningSumContext;
            addContext(key, context);
        }
        context->sum() = 0.0;
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RunningSumService::resetSum

DEFINE_STARTSERVICE_(RunningSumService)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    try
    {
        if (! isStarted())
        {
            inherited::startService();
            if (isStarted())
            {
                
            }
            else
            {
                OD_LOG("! (isStarted())"); //####
            }
        }
        result = isStarted();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // RunningSumService::startService

void
RunningSumService::startSum(const YarpString & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("key = ", key); //####
    try
    {
        RunningSumContext * context = (RunningSumContext *) findContext(key);
        
        if (! context)
        {
            context = new RunningSumContext;
            addContext(key, context);
        }
        context->sum() = 0.0;
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RunningSumService::startSum

DEFINE_STOPSERVICE_(RunningSumService)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    try
    {
        result = inherited::stopService();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // RunningSumService::stopService

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
