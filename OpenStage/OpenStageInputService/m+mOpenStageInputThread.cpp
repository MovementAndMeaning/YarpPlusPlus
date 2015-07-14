//--------------------------------------------------------------------------------------------------
//
//  File:       m+mOpenStageInputThread.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for a thread that generates output from Organic Motion
//				OpenStage data.
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
//  Created:    2015-06-25
//
//--------------------------------------------------------------------------------------------------

#include "m+mOpenStageInputThread.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wshadow"
#endif // defined(__APPLE__)
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for a thread that generates output from Organic Motion %OpenStage
 data. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::OpenStage;
using namespace om;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The number if actor stream reports to buffer. */
#define ACTOR_QUEUE_DEPTH_ 2

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

OpenStageInputThread::OpenStageInputThread(Common::GeneralChannel * outChannel,
	                                       const YarpString &       name,
										   const int                port) :
	inherited(), _address(name), _port(port), _outChannel(outChannel), _client(NULL),
	_actorStream(NULL), _actorViewJoint(NULL)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("outChannel = ", outChannel); //####
    OD_LOG_S1s("name = ", name); //####
    OD_LOG_LL1("port = ", port); //####
    OD_LOG_EXIT_P(this); //####
} // OpenStageInputThread::OpenStageInputThread

OpenStageInputThread::~OpenStageInputThread(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // OpenStageInputThread::~OpenStageInputThread

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void OpenStageInputThread::clearOutputChannel(void)
{
    OD_LOG_OBJENTER(); //####
    _outChannel = NULL;
    OD_LOG_OBJEXIT(); //####
} // OpenStageInputThread::clearOutputChannel

void OpenStageInputThread::processData(om::sdk2::ActorDataListConstPtr & actorData)
{
	OD_LOG_OBJENTER(); //####
	OD_LOG_P1("actorData = ", &actorData); //####
	size_t numActors = actorData->GetSize();

	if (0 < numActors)
	{
		yarp::os::Bottle   message;
		yarp::os::Bottle & actorList = message.addList();

		for (size_t ii = 0; ii < numActors; ++ii)
		{
			yarp::os::Bottle &     anActor = actorList.addList();
			sdk2::SkeletonConstPtr skel = actorData->GetAt(ii).skeleton;
			sdk2::JointTreePtr     jointTree = sdk2::CreateJointTree();

			sdk2::ConvertSkeletonToJointTreeAbsolute(skel, &jointTree);
			sdk2::JointTreeConstIterator jointTreeIt = jointTree->Begin();
			sdk2::JointTreeConstIterator jointTreeItEnd = jointTree->End();

			for ( ; jointTreeIt != jointTreeItEnd; ++jointTreeIt)
			{
				yarp::os::Property & jointProps = anActor.addDict();
				sdk2::Matrix44       transform = jointTreeIt->second->transform;
				glm::mat3x3          jointTransform(transform.m[0][0], transform.m[0][1],
											        transform.m[0][2], transform.m[1][0],
											        transform.m[1][1], transform.m[1][2],
					                                transform.m[2][0], transform.m[2][1],
											        transform.m[2][2]);
				glm::quat            rotQuat = glm::quat_cast(jointTransform);

				jointProps.put("id", static_cast<const char *>(*jointTreeIt->first));
				yarp::os::Value    posStuff;
				yarp::os::Bottle * posList = posStuff.asList();

				if (posList)
				{
					posList->addDouble(transform.m[3][0]);
					posList->addDouble(transform.m[3][1]);
					posList->addDouble(transform.m[3][2]);
					jointProps.put("position", posStuff);
				}
				yarp::os::Value    quatStuff;
				yarp::os::Bottle * quatList = quatStuff.asList();

				if (quatList)
				{
					quatList->addDouble(rotQuat.x);
					quatList->addDouble(rotQuat.y);
					quatList->addDouble(rotQuat.z);
					quatList->addDouble(rotQuat.w);
					jointProps.put("rotation", quatStuff);
				}
			}
		}
		if (_outChannel)
		{
			if (!_outChannel->write(message))
			{
				OD_LOG("(! _outChannel->write(message))"); //####
#if defined(MpM_StallOnSendProblem)
				Stall();
#endif // defined(MpM_StallOnSendProblem)
			}
		}
	}
	OD_LOG_OBJEXIT(); //####
} // OpenStageInputThread::processData

void OpenStageInputThread::run(void)
{
    OD_LOG_OBJENTER(); //####
	_actorStream->Start();
	for ( ; ! isStopping(); )
    {
		if (_client->WaitAnyUpdateAll())
		{
			// The streams are updated, now get the data.
			sdk2::ActorDataListConstPtr actorData;

			_actorStream->GetData(&actorData);
			processData(actorData);
		}
        yarp::os::Time::yield();
    }
    OD_LOG_OBJEXIT(); //####
} // OpenStageInputThread::run

bool OpenStageInputThread::threadInit(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = true;
    
	// Create the necessary objects.
	_client = sdk2::CreateClient();

	// Connect to the device.
	_client->SetEndpoint(_address.c_str(), _port);
	_actorStream = sdk2::CreateActorStream(_client);
	_actorViewJoint = sdk2::CreateActorViewJoint();
	_actorStream->SetBufferSize(ACTOR_QUEUE_DEPTH_);
	OD_LOG_OBJEXIT_B(result); //####
    return result;
} // OpenStageInputThread::threadInit

void OpenStageInputThread::threadRelease(void)
{
    OD_LOG_OBJENTER(); //####
	_actorStream->Stop();
    OD_LOG_OBJEXIT(); //####
} // OpenStageInputThread::threadRelease

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
