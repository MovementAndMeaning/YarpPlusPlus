//--------------------------------------------------------------------------------------------------
//
//  File:       mpm/M+MBailOutThread.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a timeout thread for M+M.
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
//  Created:    2014-04-01
//
//--------------------------------------------------------------------------------------------------

#include <mpm/M+MBailOutThread.h>
#include <mpm/M+MAdapterChannel.h>
#include <mpm/M+MClientChannel.h>
#include <mpm/M+MGeneralChannel.h>
#include <mpm/M+MServiceChannel.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a timeout thread for M+M. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;

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
#endif // definullptrned(__APPLE__)

BailOutThread::BailOutThread(const double timeToWait) :
    inherited(), _adapterChannel(nullptr), _clientChannel(nullptr), _generalChannel(nullptr),
    _serviceChannel(nullptr), _timeToWait(timeToWait)
{
    OD_LOG_ENTER(); //####
    OD_LOG_D1("timeToWait = ", timeToWait); //####
    OD_LOG_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::BailOutThread(AdapterChannel & channelOfInterest,
                             const double     timeToWait) :
    inherited(), _adapterChannel(&channelOfInterest), _clientChannel(nullptr),
    _generalChannel(nullptr), _serviceChannel(nullptr), _timeToWait(timeToWait)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("channelOfInterest = ", &channelOfInterest); //####
    OD_LOG_D1("timeToWait = ", timeToWait); //####
    OD_LOG_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::BailOutThread(ClientChannel & channelOfInterest,
                             const double    timeToWait) :
    inherited(), _adapterChannel(nullptr), _clientChannel(&channelOfInterest),
    _generalChannel(nullptr), _serviceChannel(nullptr), _timeToWait(timeToWait)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("channelOfInterest = ", &channelOfInterest); //####
    OD_LOG_D1("timeToWait = ", timeToWait); //####
    OD_LOG_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::BailOutThread(GeneralChannel & channelOfInterest,
                             const double     timeToWait) :
    inherited(), _adapterChannel(nullptr), _clientChannel(nullptr),
    _generalChannel(&channelOfInterest), _serviceChannel(nullptr), _timeToWait(timeToWait)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("channelOfInterest = ", &channelOfInterest); //####
    OD_LOG_D1("timeToWait = ", timeToWait); //####
    OD_LOG_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::BailOutThread(ServiceChannel & channelOfInterest,
                             const double     timeToWait) :
    inherited(), _adapterChannel(nullptr), _clientChannel(nullptr), _generalChannel(nullptr),
    _serviceChannel(&channelOfInterest), _timeToWait(timeToWait)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("channelOfInterest = ", &channelOfInterest); //####
    OD_LOG_D1("timeToWait = ", timeToWait); //####
    OD_LOG_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::~BailOutThread(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // BailOutThread::~BailOutThread

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void BailOutThread::run(void)
{
    OD_LOG_OBJENTER(); //####
    for ( ; ! isStopping(); )
    {
        if (_endTime <= yarp::os::Time::now())
        {
            OD_LOG("(_endTime <= yarp::os::Time::now())"); //####
            if (_adapterChannel)
            {
                _adapterChannel->interrupt();
            }
            else if (_clientChannel)
            {
                _clientChannel->interrupt();
            }
            else if (_generalChannel)
            {
                _generalChannel->interrupt();
            }
            else if (_serviceChannel)
            {
                _serviceChannel->interrupt();
            }
            raise(STANDARD_SIGNAL_TO_USE);
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif // defined(__APPLE__)
            break;
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)
        }
        yarp::os::Time::yield();
    }
    OD_LOG_OBJEXIT(); //####
} // BailOutThread::run

bool BailOutThread::threadInit(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = true;
    
    _endTime = yarp::os::Time::now() + _timeToWait;
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // BailOutThread::threadInit

void BailOutThread::threadRelease(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // BailOutThread::threadRelease

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
