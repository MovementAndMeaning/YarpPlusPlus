//--------------------------------------------------------------------------------------------------
//
//  File:       m+mOpenStageBlobInputService.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the Organic Motion OpenStage Blob input service.
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
//  Created:    2015-07-14
//
//--------------------------------------------------------------------------------------------------

#include "m+mOpenStageBlobInputService.h"

#include "m+mOpenStageBlobInputRequests.h"
#include "m+mOpenStageBlobInputThread.h"

#include <m+m/m+mEndpoint.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Organic Motion %OpenStage %Blob input service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::OpenStageBlob;

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

OpenStageBlobInputService::OpenStageBlobInputService(const Utilities::DescriptorVector &
                                                                                     argumentList,
                                                     const YarpString &                  launchPath,
                                                     const int                           argc,
                                                     char * *                            argv,
                                                     const YarpString &                  tag,
                                                     const YarpString &
                                                                             serviceEndpointName,
                                                     const YarpString &
                                                                             servicePortNumber) :
    inherited(argumentList, launchPath, argc, argv, tag, true,
              MpM_OPENSTAGEBLOBINPUT_CANONICAL_NAME_, OPENSTAGEBLOBINPUT_SERVICE_DESCRIPTION_, "",
              serviceEndpointName, servicePortNumber), _eventThread(NULL),
    _hostName(SELF_ADDRESS_NAME_), _translationScale(1), _hostPort(OPENSTAGEBLOBINPUT_DEFAULT_PORT_)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P2("argumentList = ", &argumentList, "argv = ", argv); //####
    OD_LOG_S4s("launchPath = ", launchPath, "tag = ", tag, "serviceEndpointName = ", //####
               serviceEndpointName, "servicePortNumber = ", servicePortNumber); //####
    OD_LOG_LL1("argc = ", argc); //####
    OD_LOG_EXIT_P(this); //####
} // OpenStageBlobInputService::OpenStageBlobInputService

OpenStageBlobInputService::~OpenStageBlobInputService(void)
{
    OD_LOG_OBJENTER(); //####
    stopStreams();
    OD_LOG_OBJEXIT(); //####
} // OpenStageBlobInputService::~OpenStageBlobInputService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool OpenStageBlobInputService::configure(const yarp::os::Bottle & details)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("details = ", &details); //####
    bool result = false;
    
    try
    {
        if (3 == details.size())
        {
            yarp::os::Value firstValue(details.get(0));
            yarp::os::Value secondValue(details.get(1));
            yarp::os::Value thirdValue(details.get(2));
            
            if ((firstValue.isDouble() || firstValue.isInt()) && secondValue.isString() &&
                thirdValue.isInt())
            {
                int thirdNumber = secondValue.asInt();
                
                if (firstValue.isDouble())
                {
                    _translationScale = firstValue.asDouble();
                }
                else
                {
                    _translationScale = firstValue.asInt();
                }
                if ((0 < _translationScale) && (0 < thirdNumber))
                {
                    std::stringstream buff;
                    
                    _hostName = secondValue.asString();
                    OD_LOG_S1s("_hostName <- ", _hostName); //####
                    _hostPort = thirdNumber;
                    OD_LOG_LL1("_hostPort <- ", _hostPort); //####
                    buff << "Translation scale is " << _translationScale <<
                            ", host name is '" << _hostName.c_str() << "', host port is " <<
                            _hostPort;
                    setExtraInformation(buff.str());
                    result = true;
                }
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
} // OpenStageBlobInputService::configure

bool OpenStageBlobInputService::getConfiguration(yarp::os::Bottle & details)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("details = ", &details); //####
    bool result = true;

    details.clear();
    details.addDouble(_translationScale);
    details.addString(_hostName);
    details.addInt(_hostPort);
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // OpenStageBlobInputService::getConfiguration

void OpenStageBlobInputService::restartStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        // No special processing needed.
        stopStreams();
        startStreams();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // OpenStageBlobInputService::restartStreams

bool OpenStageBlobInputService::setUpStreamDescriptions(void)
{
    OD_LOG_OBJENTER(); //####
    bool               result = true;
    ChannelDescription description;
    YarpString         rootName(getEndpoint().getName() + "/");
    
    _outDescriptions.clear();
    description._portName = rootName + "output";
    description._portProtocol = "OM";
    description._protocolDescription = "A dictionary with position values";
    _outDescriptions.push_back(description);
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // OpenStageBlobInputService::setUpStreamDescriptions

bool OpenStageBlobInputService::shutDownOutputStreams(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = inherited::shutDownOutputStreams();
    
    if (_eventThread)
    {
		_eventThread->clearOutputChannel();
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // OpenStageBlobInputService::shutDownOutputStreams

bool OpenStageBlobInputService::start(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (! isStarted())
        {
            inherited::start();
            if (isStarted())
            {
            
            }
            else
            {
                OD_LOG("! (isStarted())"); //####
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(isStarted()); //####
    return isStarted();
} // OpenStageBlobInputService::start

void OpenStageBlobInputService::startStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (! isActive())
        {
            _eventThread = new OpenStageBlobInputThread(getOutletStream(0), _hostName, _hostPort);
            _eventThread->setScale(_translationScale);
			_eventThread->start();
            setActive();
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // OpenStageBlobInputService::startStreams

bool OpenStageBlobInputService::stop(void)
{
    OD_LOG_OBJENTER(); //####
    bool result;
    
    try
    {
        result = inherited::stop();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // OpenStageBlobInputService::stop

void OpenStageBlobInputService::stopStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (isActive())
        {
            _eventThread->stop();
			for (; _eventThread->isRunning();)
            {
				yarp::os::Time::delay(ONE_SECOND_DELAY_ / 3.9);
            }
            delete _eventThread;
            _eventThread = NULL;
            clearActive();
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // OpenStageBlobInputService::stopStreams

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)