//--------------------------------------------------------------------------------------------------
//
//  File:       m+mPlaybackFromJSONInputService.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the Playback From JSON input service.
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
//  Created:    2015-08-24
//
//--------------------------------------------------------------------------------------------------

#include "m+mPlaybackFromJSONInputService.hpp"
#include "m+mPlaybackFromJSONInputRequests.hpp"
#include "m+mPlaybackFromJSONInputThread.hpp"

#include <m+m/m+mEndpoint.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#include "rapidjson/document.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Playback From JSON input service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Example;
using std::cerr;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Convert a JSON element into a YARP entity and add it to a message.
 @param[in,out] outMessage The message to be added to.
 @param[in] aValue The value to be processed. */
static void
addJSONValueToMessage(yarp::os::Bottle &       outMessage,
                      const rapidjson::Value & aValue);

/*! @brief Convert a JSON array into a list of YARP entities and add it to a message.
 @param[in,out] outMessage The message to be added to.
 @param[in] inValue The value to be processed. */
static void
addJSONArrayToMessage(yarp::os::Bottle &       outMessage,
                      const rapidjson::Value & inValue)
{
    ODL_ENTER(); //####
    ODL_P2("outMessage = ", &outMessage, "inValue = ", &inValue); //####
    yarp::os::Bottle & holder = outMessage.addList();

    for (rapidjson::Value::ConstValueIterator walker(inValue.Begin()); inValue.End() != walker;
         ++walker)
    {
        addJSONValueToMessage(holder, *walker);
    }
    ODL_EXIT(); //####
} // addJSONArrayToMessage

/*! @brief Convert a JSON value to a YARP value.
 @brief outValue The value to be set.
 @brief inValue The value to be processed. */
static void
convertJSONValueToValue(yarp::os::Value &        outValue,
                        const rapidjson::Value & inValue)
{
    ODL_ENTER(); //####
    ODL_P2("outValue = ", &outValue, "inValue = ", &inValue); //####
    yarp::os::Bottle *   asList;
    yarp::os::Property * asDict;

    switch (inValue.GetType())
    {
        case rapidjson::kNullType :
            break;

        case rapidjson::kFalseType :
            outValue = yarp::os::Value(0);
            break;

        case rapidjson::kTrueType :
            outValue = yarp::os::Value(1);
            break;

        case rapidjson::kObjectType :
            asDict = outValue.asDict();
            if (asDict)
            {
                for (rapidjson::Value::ConstMemberIterator walker(inValue.MemberBegin());
                     inValue.MemberEnd() != walker; ++walker)
                {
                    yarp::os::Value newValue;

                    convertJSONValueToValue(newValue, walker->value);
                    asDict->put(walker->name.GetString(), newValue);
                }
            }
            break;

        case rapidjson::kArrayType :
            asList = outValue.asList();
            if (asList)
            {
                for (rapidjson::Value::ConstValueIterator walker(inValue.Begin());
                     inValue.End() != walker; ++walker)
                {
                    addJSONValueToMessage(*asList, *walker);
                }
            }
            break;

        case rapidjson::kStringType :
            outValue = yarp::os::Value(inValue.GetString());
            break;

        case rapidjson::kNumberType :
            if (inValue.IsDouble())
            {
                outValue = yarp::os::Value(inValue.GetDouble());
            }
            else if (inValue.IsInt())
            {
                outValue = yarp::os::Value(static_cast<int>(inValue.GetInt()));
            }
            else if (inValue.IsUint())
            {
                outValue = yarp::os::Value(static_cast<int>(inValue.GetUint()));
            }
            else if (inValue.IsInt64())
            {
                outValue = yarp::os::Value(static_cast<int>(inValue.GetInt64()));
            }
            else if (inValue.IsUint64())
            {
                outValue = yarp::os::Value(static_cast<int>(inValue.GetUint64()));
            }
            else
            {
                outValue = yarp::os::Value(0);
            }
            break;

        default :
            cerr << "unknown" << endl;
            break;

    }
    ODL_EXIT(); //####
} // convertJSONValueToValue

/*! @brief Convert a JSON object into a dictionary of YARP entities and add it to a message.
 @param[in,out] outMessage The message to be added to.
 @param[in] inValue The value to be processed. */
static void
addJSONObjectToMessage(yarp::os::Bottle &       outMessage,
                       const rapidjson::Value & inValue)
{
    ODL_ENTER(); //####
    ODL_P2("outMessage = ", &outMessage, "inValue = ", &inValue); //####
    yarp::os::Property & holder = outMessage.addDict();

    for (rapidjson::Value::ConstMemberIterator walker(inValue.MemberBegin());
         inValue.MemberEnd() != walker; ++walker)
    {
        yarp::os::Value newValue;

        convertJSONValueToValue(newValue, walker->value);
        holder.put(walker->name.GetString(), newValue);
    }
    ODL_EXIT(); //####
} // addJSONObjectToMessage

static void
addJSONValueToMessage(yarp::os::Bottle &       outMessage,
                      const rapidjson::Value & inValue)
{
    ODL_ENTER(); //####
    ODL_P2("outMessage = ", &outMessage, "aValue = ", &inValue); //####
    switch (inValue.GetType())
    {
        case rapidjson::kNullType :
            outMessage.addList();
            break;

        case rapidjson::kFalseType :
            outMessage.addInt(0);
            break;

        case rapidjson::kTrueType :
            outMessage.addInt(1);
            break;

        case rapidjson::kObjectType :
            addJSONObjectToMessage(outMessage, inValue);
            break;

        case rapidjson::kArrayType :
            addJSONArrayToMessage(outMessage, inValue);
            break;

        case rapidjson::kStringType :
            outMessage.addString(inValue.GetString());
            break;

        case rapidjson::kNumberType :
            if (inValue.IsDouble())
            {
                outMessage.addDouble(inValue.GetDouble());
            }
            else if (inValue.IsInt())
            {
                outMessage.addInt(inValue.GetInt());
            }
            else if (inValue.IsUint())
            {
                outMessage.addInt(inValue.GetUint());
            }
            else if (inValue.IsInt64())
            {
                outMessage.addInt(inValue.GetInt64());
            }
            else if (inValue.IsUint64())
            {
                outMessage.addInt(inValue.GetUint64());
            }
            else
            {
                outMessage.addInt(0);
            }
            break;

        default :
            cerr << "unknown" << endl;
            break;

    }
    ODL_EXIT(); //####
} // addJSONValueToMessage

/*! @brief Convert JSON-formatted input into the expected message format.
 @param[in,out] outMessage The output message buffer to be used.
 @param[in] inData The input JSON-formatted data.
 @return @c true if the input data is recognized as the correct format for JSON-formatted message
 data and @c false otherwise. */
static bool
convertJSONtoMessage(yarp::os::Bottle &    outMessage,
                     rapidjson::Document & inData)
{
    ODL_ENTER(); //####
    ODL_P2("outMessage = ", &outMessage, "inData = ", &inData); //####
    bool okSoFar = true;

    ODL_EXIT_B(okSoFar); //####
    if (inData.IsArray())
    {
        bool   isFirstElement = true;
        double prevTime = -1;

        for (rapidjson::Value::ConstValueIterator walker(inData.Begin());
             okSoFar && (inData.End() != walker); ++walker)
        {
            const rapidjson::Value & aValue(*walker);

            if (aValue.IsObject())
            {
                if (aValue.HasMember("time") && aValue.HasMember("value"))
                {
                    const rapidjson::Value & timeValue = aValue["time"];
                    const rapidjson::Value & valueValue = aValue["value"];

                    if (timeValue.IsNumber())
                    {
                        double aTime;

                        if (timeValue.IsDouble())
                        {
                            aTime = timeValue.GetDouble();
                        }
                        else if (timeValue.IsInt())
                        {
                            aTime = timeValue.GetInt();
                        }
                        else if (timeValue.IsUint())
                        {
                            aTime = timeValue.GetUint();
                        }
                        else if (timeValue.IsInt64())
                        {
                            aTime = timeValue.GetInt64();
                        }
                        else if (timeValue.IsUint64())
                        {
                            aTime = timeValue.GetUint64();
                        }
                        else
                        {
                            cerr << "Unknown numeric type for named JSON field." << endl;
                            okSoFar = false;
                        }
                        if (okSoFar)
                        {
                            if (isFirstElement)
                            {
                                isFirstElement = false;
                            }
                            else
                            {
                                // Convert from 'absolute' milliseconds to relative seconds.
                                outMessage.addDouble((aTime - prevTime) / 1000.0);
                            }
                            yarp::os::Bottle & holder = outMessage.addList();

                            addJSONValueToMessage(holder, valueValue);
                            prevTime = aTime;
                        }
                    }
                    else
                    {
                        cerr << "Invalid type for named JSON field." << endl;
                        okSoFar = false;
                    }
                }
                else
                {
                    cerr << "Missing one or more named JSON fields." << endl;
                    okSoFar = false;
                }
            }
            else
            {
                cerr << "JSON element is not an object." << endl;
                okSoFar = false;
            }
        }
    }
    else
    {
        cerr << "JSON top-level object is not an array." << endl;
        okSoFar = false;
    }
    return okSoFar;
} // convertJSONtoMessage

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

PlaybackFromJSONInputService::PlaybackFromJSONInputService(const YarpString &
                                                                                        inputPath,
                                                           const Utilities::DescriptorVector &
                                                                                    argumentList,
                                                           const YarpString &
                                                                                        launchPath,
                                                           const int                           argc,
                                                           char * *                            argv,
                                                           const YarpString &                  tag,
                                                           const YarpString &
                                                                                serviceEndpointName,
                                                           const YarpString &
                                                                                servicePortNumber) :
    inherited(argumentList, launchPath, argc, argv, tag, true,
              MpM_PLAYBACKFROMJSONINPUT_CANONICAL_NAME_, PLAYBACKFROMJSONINPUT_SERVICE_DESCRIPTION_,
              "", serviceEndpointName, servicePortNumber),
    _generator(NULL), _inPath(inputPath), _initialDelay(0), _playbackRatio(1), _loopPlayback(false)
{
    ODL_ENTER(); //####
    ODL_S4s("launchPath = ", launchPath, "inputPath = ", inputPath, "tag = ", tag, //####
            "serviceEndpointName = ", serviceEndpointName); //####
    ODL_S1s("servicePortNumber = ", servicePortNumber); //####
    ODL_P2("argumentList = ", &argumentList, "argv = ", argv); //####
    ODL_I1("argc = ", argc); //####
    ODL_EXIT_P(this); //####
} // PlaybackFromJSONInputService::PlaybackFromJSONInputService

PlaybackFromJSONInputService::~PlaybackFromJSONInputService(void)
{
    ODL_OBJENTER(); //####
    stopStreams();
    ODL_OBJEXIT(); //####
} // PlaybackFromJSONInputService::~PlaybackFromJSONInputService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool
PlaybackFromJSONInputService::configure(const yarp::os::Bottle & details)
{
    ODL_OBJENTER(); //####
    ODL_P1("details = ", &details); //####
    bool result = false;

    try
    {
        if (3 <= details.size())
        {
            yarp::os::Value firstValue(details.get(0));
            yarp::os::Value secondValue(details.get(1));
            yarp::os::Value thirdValue(details.get(2));

            if (firstValue.isDouble() && secondValue.isDouble() && thirdValue.isInt())
            {
                double firstNumber = firstValue.asDouble();
                double secondNumber = secondValue.asDouble();
                int    thirdNumber = thirdValue.asInt();

                if ((0 < firstNumber) && (0 <= secondNumber))
                {
                    std::stringstream buff;

                    _playbackRatio = firstNumber;
                    _initialDelay = secondNumber;
                    _loopPlayback = (0 != thirdNumber);
                    ODL_D2("_playbackRatio <- ", _playbackRatio, "_initialDelay <- ", //####
                           _initialDelay); //####
                    ODL_B1("_loopPlayback <- ", _loopPlayback); //####
                    buff << "Input file path is '" << _inPath.c_str() << "', playback ratio is " <<
                            _playbackRatio << ", initial delay is " << _initialDelay <<
                            ", playback does " << (_loopPlayback ? "loop" : "not loop");
                    setExtraInformation(buff.str());
                    result = true;
                }
                else
                {
                    cerr << "One or more inputs are out of range." << endl;
                }
            }
            else
            {
                cerr << "One or more inputs have the wrong type." << endl;
            }
        }
        else
        {
            cerr << "Missing input(s)." << endl;
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_B(result); //####
    return result;
} // PlaybackFromJSONInputService::configure

bool
PlaybackFromJSONInputService::getConfiguration(yarp::os::Bottle & details)
{
    ODL_OBJENTER(); //####
    ODL_P1("details = ", &details); //####
    bool result = true;

    details.clear();
    details.addDouble(_playbackRatio);
    details.addDouble(_initialDelay);
    details.addInt(_loopPlayback ? 1 : 0);
    ODL_OBJEXIT_B(result); //####
    return result;
} // PlaybackFromJSONInputService::getConfiguration

bool
PlaybackFromJSONInputService::setUpStreamDescriptions(void)
{
    ODL_OBJENTER(); //####
    bool               result = true;
    ChannelDescription description;
    YarpString         rootName(getEndpoint().getName() + "/");

    _outDescriptions.clear();
    description._portName = rootName + "output";
    description._portProtocol = "*";
    description._protocolDescription = "Arbitrary YARP messages";
    _outDescriptions.push_back(description);
    ODL_OBJEXIT_B(result); //####
    return result;
} // PlaybackFromJSONInputService::setUpStreamDescriptions

bool
PlaybackFromJSONInputService::shutDownOutputStreams(void)
{
    ODL_OBJENTER(); //####
    bool result = inherited::shutDownOutputStreams();

    if (_generator)
    {
        _generator->clearOutputChannel();
    }
    ODL_EXIT_B(result); //####
    return result;
} // PlaybackFromJSONInputService::shutDownOutputStreams

bool
PlaybackFromJSONInputService::startService(void)
{
    ODL_OBJENTER(); //####
    try
    {
        if (! isStarted())
        {
            inherited::startService();
            if (isStarted())
            {
                FILE * inFile;
                int    why;

#if MAC_OR_LINUX_
                inFile = fopen(_inPath.c_str(), "r");
                why = errno;
#else // ! MAC_OR_LINUX_
                why = fopen_s(&inFile, _inPath.c_str(), "r");
                if (why)
                {
                    inFile = NULL;
                }
#endif // ! MAC_OR_LINUX_
                if (inFile)
                {
                    // Convert the JSON-formatted data into JSON structures.
                    YarpString          dataSource;
                    char                buffer[10240];
                    size_t              numRead;
                    rapidjson::Document jsonData;

                    for ( ; ! feof(inFile); )
                    {
                        numRead = fread(buffer, 1, sizeof(buffer) - 1, inFile);
                        if (numRead)
                        {
                            buffer[numRead] = '\0';
                            dataSource += buffer;
                        }
                    }
                    fclose(inFile);
                    jsonData.Parse<rapidjson::kParseFullPrecisionFlag>(dataSource.c_str());
                    if (jsonData.HasParseError())
                    {
                        cerr << "JSON problem at byte " << jsonData.GetErrorOffset() << " = " <<
                                jsonData.GetParseError() << endl;
                    }
                    else
                    {
                        _outMessage.clear();
                        if (! convertJSONtoMessage(_outMessage, jsonData))
                        {
                            cerr << "Could not convert JSON input to expected message format." <<
                                    endl;
                            _outMessage.clear();
                        }
                    }
                }
                else
                {
                    cerr << "Could not open file '" << _inPath.c_str() <<
                            "' for reading, error code = " << why << "." << endl;
                }
            }
            else
            {
                ODL_LOG("! (isStarted())"); //####
            }
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_B(isStarted()); //####
    return isStarted();
} // PlaybackFromJSONInputService::startService

void
PlaybackFromJSONInputService::startStreams(void)
{
    ODL_OBJENTER(); //####
    try
    {
        if (! isActive())
        {
            if (0 < _outMessage.size())
            {
                _generator = new PlaybackFromJSONInputThread(getOutletStream(0), _outMessage,
                                                             _playbackRatio, _initialDelay,
                                                             _loopPlayback);
                if (_generator->start())
                {
                    setActive();
                }
                else
                {
                    cerr << "Could not start auxiliary thread." << endl;
                }
            }
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // PlaybackFromJSONInputService::startStreams

void
PlaybackFromJSONInputService::stopStreams(void)
{
    ODL_OBJENTER(); //####
    try
    {
        if (isActive())
        {
            _generator->stop();
            for ( ; _generator->isRunning(); )
            {
                ConsumeSomeTime(IO_SERVICE_DELAY_FACTOR_);
            }
            delete _generator;
            _generator = NULL;
            clearActive();
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // PlaybackFromJSONInputService::stopStreams

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
