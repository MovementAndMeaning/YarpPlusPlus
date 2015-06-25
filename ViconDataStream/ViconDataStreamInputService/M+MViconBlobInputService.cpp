//--------------------------------------------------------------------------------------------------
//
//  File:       M+MViconBlobInputService.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the Vicon Blob input service.
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
//  Created:    2015-06-24
//
//--------------------------------------------------------------------------------------------------

#include "M+MViconBlobInputService.h"
#include "M+MViconBlobEventThread.h"
#include "M+MViconBlobInputRequests.h"

#include <mpm/M+MEndpoint.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Vicon Blob input service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::ViconBlob;

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

ViconBlobInputService::ViconBlobInputService(const YarpString & launchPath,
                                             const int          argc,
                                             char * *           argv,
                                             const YarpString & tag,
                                             const YarpString & serviceEndpointName,
                                             const YarpString & servicePortNumber) :
    inherited(launchPath, argc, argv, tag, true, MpM_VICONBLOBINPUT_CANONICAL_NAME_,
              VICONBLOBINPUT_SERVICE_DESCRIPTION_, "", serviceEndpointName,
              servicePortNumber), _eventThread(NULL), _hostName("localhost"), _translationScale(1),
    _hostPort(801)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S4s("launchPath = ", launchPath, "tag = ", tag, "serviceEndpointName = ", //####
               serviceEndpointName, "servicePortNumber = ", servicePortNumber); //####
    OD_LOG_LL1("argc = ", argc); //####
    OD_LOG_P1("argv = ", argv); //####
    OD_LOG_EXIT_P(this); //####
} // ViconBlobInputService::ViconBlobInputService

ViconBlobInputService::~ViconBlobInputService(void)
{
    OD_LOG_OBJENTER(); //####
    stopStreams();
    OD_LOG_OBJEXIT(); //####
} // ViconBlobInputService::~ViconBlobInputService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool ViconBlobInputService::configure(const yarp::os::Bottle & details)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("details = ", &details); //####
    bool result = false;
    
    try
    {
        if (! isActive())
        {
            if (3 == details.size())
            {
                yarp::os::Value firstValue(details.get(0));
                yarp::os::Value secondValue(details.get(1));
                yarp::os::Value thirdValue(details.get(2));

                if ((firstValue.isDouble() || firstValue.isInt()) && secondValue.isString() &&
                    thirdValue.isInt())
                {
                    int thirdNumber = thirdValue.asInt();

                    if (firstValue.isDouble())
                    {
                        _translationScale = firstValue.asDouble();
                    }
                    else
                    {
                        _translationScale = firstValue.asInt();
                    }
                    if ((0 < firstNumber) && (0 < thirdNumber))
                    {
                        std::stringstream buff;

                        _hostName = secondValue.asString();
						OD_LOG_S1s("_hostName <- ", _hostName); //####
                        _hostPort = thirdNumber;
                        OD_LOG_LL1("_hostPort <- ", _hostPort); //####
                        buff << "Translation scale is " << 42 << ", host name is '" <<
                                _hostName.c_str() << "', host port is " << _hostPort;
                        setExtraInformation(buff.str());
                        result = true;
                    }
                }
            }
        }
        result = true;
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ViconBlobInputService::configure

void ViconBlobInputService::restartStreams(void)
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
} // ViconBlobInputService::restartStreams

bool ViconBlobInputService::setUpStreamDescriptions(void)
{
    OD_LOG_OBJENTER(); //####
    bool               result = true;
    ChannelDescription description;
    YarpString         rootName(getEndpoint().getName() + "/");
    
    _outDescriptions.clear();
    description._portName = rootName + "output";
    description._portProtocol = "b";
    description._protocolDescription = T_("A binary blob containing the finger positions and "
                                          "directions");
    _outDescriptions.push_back(description);
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ViconBlobInputService::setUpStreamDescriptions

bool ViconBlobInputService::shutDownOutputStreams(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = inherited::shutDownOutputStreams();
    
    if (_eventThread)
    {
        _eventThread->clearOutputChannel();
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // ViconBlobInputService::shutDownOutputStreams

bool ViconBlobInputService::start(void)
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
} // ViconBlobInputService::start

void ViconBlobInputService::startStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (! isActive())
        {
		    std::stringstream nameAndPort;

			nameAndPort << _hostName.c_str() << ":" << _hostPort;
            _eventThread = new ViconBlobEventThread(getOutletStream(0), nameAndPort.str());
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
} // ViconBlobInputService::startStreams

bool ViconBlobInputService::stop(void)
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
} // ViconBlobInputService::stop

void ViconBlobInputService::stopStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (isActive())
        {
            _eventThread->stop();
            for ( ; _eventThread->isRunning(); )
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
} // ViconBlobInputService::stopStreams

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)