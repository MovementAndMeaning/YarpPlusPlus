//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mBaseInputOutputService.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the minimal functionality required for an m+m input/output
//              service.
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
//  Created:    2014-06-23
//
//--------------------------------------------------------------------------------------------------

#include "m+mBaseInputOutputService.h"

#include "m+mArgumentDescriptionsRequestHandler.h"
#include "m+mConfigurationRequestHandler.h"
#include "m+mConfigureRequestHandler.h"
#include "m+mRestartStreamsRequestHandler.h"
#include "m+mStartStreamsRequestHandler.h"
#include "m+mStopStreamsRequestHandler.h"

#include <m+m/m+mChannelStatusReporter.h>
#include <m+m/m+mClientChannel.h>
#include <m+m/m+mEndpoint.h>
#include <m+m/m+mGeneralChannel.h>
#include <m+m/m+mUtilities.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if MAC_OR_LINUX_
# include <termios.h>
#else // ! MAC_OR_LINUX_
# include <conio.h>
#endif // ! MAC_OR_LINUX_

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the minimal functionality required for an m+m input/output
 service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The mode of the input terminal. */
enum TtyMode
{
    /*! @brief The terminal is in 'normal' mode. */
    TTY_RESET_,

    /*! @brief The terminal is in 'raw' mode. */
    TTY_RAW_,

    /*! @brief The terminal is in 'cbreak' mode. */
    TTY_CBREAK_

}; // TtyMode

/*! @brief Set to @c true to simulate a request to stop the service. */
static bool lStopTheService = false;

#if MAC_OR_LINUX_
/*! @brief The active terminal attributes. */
static struct termios lTermattr;
#endif // MAC_OR_LINUX_

#if MAC_OR_LINUX_
/*! @brief The save terminal attributes. */
static struct termios lSaveTermattr;
#endif // MAC_OR_LINUX_

#if MAC_OR_LINUX_
/*! @brief The saved file descriptor for the input terminal. */
static int lTtySaveFd = -1;
#endif // MAC_OR_LINUX_

#if MAC_OR_LINUX_
/*! @brief The mode of the input terminal. */
static TtyMode lTtyState = TTY_RESET_;
#endif // MAC_OR_LINUX_

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Display the available commands.
 @param helpText The help text to be displayed.
 @param forAdapter @c true if for an adapter and @c false for a service. */
static void
displayCommands(const YarpString & helpText,
                const bool         forAdapter)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("helpText = ", helpText); //####
    OD_LOG_B1("forAdapter = ", forAdapter); //####
    if (0 < helpText.length())
    {
        cout << helpText.c_str() << endl;
    }
    cout << "Commands:" << endl;
    cout << "  ? - display this list" << endl;
    cout << "  b - start (begin) the input and output streams" << endl;
    cout << "  c - configure the " << (forAdapter ? "adapter" : "service") << endl;
    cout << "  e - stop (end) the input and output streams" << endl;
    cout << "  q - quit the application" << endl;
    cout << "  r - restart the input and output streams" << endl;
    OD_LOG_EXIT(); //####
} // displayCommands

/*! @brief Place the standard input in 'cbreak' mode. */
static int
set_tty_cbreak(void)
{
#if MAC_OR_LINUX_
    int ii = tcgetattr(STDIN_FILENO, &lTermattr);
#endif // MAC_OR_LINUX_
    int res = 0;

#if MAC_OR_LINUX_
    if (ii < 0)
    {
        printf("tcgetattr() returned %d for fildes=%d\n", ii, STDIN_FILENO);
        perror("");
        res = -1;
    }
    else
    {
        lSaveTermattr = lTermattr;
        lTermattr.c_lflag &= ~(ECHO | ICANON);
        lTermattr.c_cc[VMIN] = 1;
        lTermattr.c_cc[VTIME] = 0;
        ii = tcsetattr(STDIN_FILENO, TCSANOW, &lTermattr);
        if (ii < 0)
        {
            printf("tcsetattr() returned %d for fildes=%d\n", ii, STDIN_FILENO);
            perror("");
            res = -1;
        }
        else
        {
            lTtyState = TTY_CBREAK_;
            lTtySaveFd = STDIN_FILENO;
        }
    }
#endif // MAC_OR_LINUX_
    return res;
} // set_tty_cbreak

/*! @brief Restore the standard input to its original mode. */
static int
set_tty_cooked(void)
{
    int res = 0;

#if MAC_OR_LINUX_
    if ((TTY_CBREAK_ == lTtyState) || (TTY_RAW_ == lTtyState))
    {
        int ii = tcsetattr(STDIN_FILENO, TCSAFLUSH, &lSaveTermattr);

        if (ii < 0)
        {
            res = -1;
        }
        else
        {
            lTtyState = TTY_RESET_;
        }
    }
#endif // MAC_OR_LINUX_
    return res;
} // set_tty_cooked

/*! @brief Place the standard input in 'raw' mode. */
static int
set_tty_raw(void)
{
#if MAC_OR_LINUX_
    int ii = tcgetattr(STDIN_FILENO, &lTermattr);
#endif // MAC_OR_LINUX_
    int res = 0;

#if MAC_OR_LINUX_
    if (ii < 0)
    {
        printf("tcgetattr() returned %d for fildes=%d\n", ii, STDIN_FILENO);
        perror("");
        res = -1;
    }
    else
    {
        lSaveTermattr = lTermattr;
        lTermattr.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        lTermattr.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        lTermattr.c_cflag &= ~(CSIZE | PARENB);
        lTermattr.c_cflag |= CS8;
        lTermattr.c_oflag &= ~(OPOST);
        lTermattr.c_cc[VMIN] = 1;  /* or 0 for some Unices;  see note 1 */
        lTermattr.c_cc[VTIME] = 0;
        ii = tcsetattr(STDIN_FILENO, TCSANOW, &lTermattr);
        if (ii < 0)
        {
            printf("tcsetattr() returned %d for fildes=%d\n", ii, STDIN_FILENO);
            perror("");
            res = -1;
        }
        else
        {
            lTtyState = TTY_RAW_;
            lTtySaveFd = STDIN_FILENO;
        }
    }
#endif // MAC_OR_LINUX_
    return res;
} // set_tty_raw

/*! @brief Get the next character available, or @c 0 if none is available.
 @returns The next available character or @c 0 if none is available. */
static char
kb_getc(void)
{
#if MAC_OR_LINUX_
    int           ii;
    ssize_t       size;
#endif // MAC_OR_LINUX_
    unsigned char ch;

#if MAC_OR_LINUX_
    lTermattr.c_cc[VMIN] = 0; /* uncomment if needed */
    ii = tcsetattr(STDIN_FILENO, TCSANOW, &lTermattr);
    size = read(STDIN_FILENO, &ch, 1);
    lTermattr.c_cc[VMIN] = 1; /* uncomment if needed */
    ii = tcsetattr(STDIN_FILENO, TCSANOW, &lTermattr);
    if (0 == size)
    {
        ch = 0;
    }
#else // ! MAC_OR_LINUX_
    if (_kbhit())
    {
        ch = _getch();
    }
    else
    {
        ch = 0;
    }
#endif // ! MAC_OR_LINUX_
    return static_cast<char>(ch);
} // kb_getc

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

BaseInputOutputService::BaseInputOutputService(const Utilities::DescriptorVector & argumentList,
                                               const ServiceKind                   theKind,
                                               const YarpString &                  launchPath,
                                               const int                           argc,
                                               char * *                            argv,
                                               const YarpString &                  tag,
                                               const bool
                                                                               useMultipleHandlers,
                                               const YarpString &                  canonicalName,
                                               const YarpString &                  description,
                                               const YarpString &
                                                                               requestsDescription,
                                               const YarpString &
                                                                               serviceEndpointName,
                                               const YarpString &
                                                                               servicePortNumber) :
    inherited(theKind, launchPath, argc, argv, tag, useMultipleHandlers, canonicalName, description,
              requestsDescription, serviceEndpointName, servicePortNumber),
    _argumentList(argumentList), _argumentDescriptionsHandler(NULL), _configurationHandler(NULL),
    _configureHandler(NULL), _restartStreamsHandler(NULL), _startStreamsHandler(NULL),
    _stopStreamsHandler(NULL), _active(false), _needsIdle(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P2("argumentList = ", &argumentList, "argv = ", argv); //####
    OD_LOG_S4s("launchPath = ", launchPath, "tag = ", tag, "canonicalName = ", canonicalName, //####
               "description = ", description); //####
    OD_LOG_S3s("requestsDescription = ", requestsDescription, "serviceEndpointName = ", //####
               serviceEndpointName, "servicePortNumber = ", servicePortNumber); //####
    OD_LOG_LL1("argc = ", argc); //####
    OD_LOG_B1("useMultipleHandlers = ", useMultipleHandlers); //####
    attachRequestHandlers();
    OD_LOG_EXIT_P(this); //####
} // BaseInputOutputService::BaseInputOutputService

BaseInputOutputService::~BaseInputOutputService(void)
{
    OD_LOG_OBJENTER(); //####
    shutDownClientStreams();
    shutDownInputStreams();
    shutDownOutputStreams();
    detachRequestHandlers();
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::~BaseInputOutputService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool
BaseInputOutputService::addClientStreamsFromDescriptions(const ChannelVector & descriptions)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("descriptions = ", &descriptions); //####
    bool result = true;
    
    try
    {
        if (0 < descriptions.size())
        {
#if defined(MpM_ReportOnConnections)
            ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)
            
            for (ChannelVector::const_iterator walker(descriptions.begin());
                 result && (descriptions.end() != walker); ++walker)
            {
                ClientChannel * newChannel = new ClientChannel;
                
                if (newChannel)
                {
                    ChannelDescription aDescription(*walker);
                    
#if defined(MpM_ReportOnConnections)
                    newChannel->setReporter(*reporter);
                    newChannel->getReport(*reporter);
#endif // defined(MpM_ReportOnConnections)
                    if (newChannel->openWithRetries(aDescription._portName, STANDARD_WAIT_TIME_))
                    {
                        if (metricsAreEnabled())
                        {
                            newChannel->enableMetrics();
                        }
                        else
                        {
                            newChannel->disableMetrics();
                        }
                        _clientStreams.push_back(newChannel);
                    }
                    else
                    {
                        OD_LOG("! (newChannel->openWithRetries(aDescription._portName, " //####
                               "STANDARD_WAIT_TIME_))"); //####
                        MpM_FAIL_("Problem opening input channel.");
                        result = false;
                    }
                }
                else
                {
                    OD_LOG("! (newChannel)"); //####
                    result = false;
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
} // BaseInputOutputService::addClientStreamsFromDescriptions

bool
BaseInputOutputService::addInStreamsFromDescriptions(const ChannelVector & descriptions)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("descriptions = ", &descriptions); //####
    bool result = true;
    
    try
    {
        if (0 < descriptions.size())
        {
#if defined(MpM_ReportOnConnections)
            ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)

            for (ChannelVector::const_iterator walker(descriptions.begin());
                 result && (descriptions.end() != walker); ++walker)
            {
                GeneralChannel * newChannel = new GeneralChannel(false);
                
                if (newChannel)
                {
                    ChannelDescription aDescription(*walker);
                    
#if defined(MpM_ReportOnConnections)
                    newChannel->setReporter(*reporter);
                    newChannel->getReport(*reporter);
#endif // defined(MpM_ReportOnConnections)
                    if (newChannel->openWithRetries(aDescription._portName, STANDARD_WAIT_TIME_))
                    {
                        newChannel->setProtocol(aDescription._portProtocol,
                                                aDescription._protocolDescription);
                        if (metricsAreEnabled())
                        {
                            newChannel->enableMetrics();
                        }
                        else
                        {
                            newChannel->disableMetrics();
                        }
                        _inStreams.push_back(newChannel);
                    }
                    else
                    {
                        OD_LOG("! (newChannel->openWithRetries(aDescription._portName, " //####
                               "STANDARD_WAIT_TIME_))"); //####
                        MpM_FAIL_("Problem opening input channel.");
                        result = false;
                    }
                }
                else
                {
                    OD_LOG("! (newChannel)"); //####
                    result = false;
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
} // BaseInputOutputService::addInStreamsFromDescriptions

bool
BaseInputOutputService::addOutStreamsFromDescriptions(const ChannelVector & descriptions)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("descriptions = ", &descriptions); //####
    bool result = true;
    
    try
    {
        if (0 < descriptions.size())
        {
#if defined(MpM_ReportOnConnections)
            ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)
            
            for (ChannelVector::const_iterator walker(descriptions.begin());
                 result && (descriptions.end() != walker); ++walker)
            {
                GeneralChannel * newChannel = new GeneralChannel(true);
                
                if (newChannel)
                {
                    ChannelDescription aDescription(*walker);
                    
#if defined(MpM_ReportOnConnections)
                    newChannel->setReporter(*reporter);
                    newChannel->getReport(*reporter);
#endif // defined(MpM_ReportOnConnections)
                    if (newChannel->openWithRetries(aDescription._portName, STANDARD_WAIT_TIME_))
                    {
                        newChannel->setProtocol(aDescription._portProtocol,
                                                aDescription._protocolDescription);
                        if (metricsAreEnabled())
                        {
                            newChannel->enableMetrics();
                        }
                        else
                        {
                            newChannel->disableMetrics();
                        }
                        _outStreams.push_back(newChannel);
                    }
                    else
                    {
                        OD_LOG("! (newChannel->openWithRetries(newName, " //####
                               "STANDARD_WAIT_TIME_))"); //####
                        MpM_FAIL_("Problem opening output channel.");
                        result = false;
                    }
                }
                else
                {
                    OD_LOG("! (newChannel)"); //####
                    result = false;
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
} // BaseInputOutputService::addOutStreamsFromDescriptions

void
BaseInputOutputService::attachRequestHandlers(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        _argumentDescriptionsHandler = new ArgumentDescriptionsRequestHandler(*this);
        _configurationHandler = new ConfigurationRequestHandler(*this);
        _configureHandler = new ConfigureRequestHandler(*this);
        _restartStreamsHandler = new RestartStreamsRequestHandler(*this);
        _startStreamsHandler = new StartStreamsRequestHandler(*this);
        _stopStreamsHandler = new StopStreamsRequestHandler(*this);
        if (_argumentDescriptionsHandler && _configurationHandler && _configureHandler &&
            _restartStreamsHandler && _startStreamsHandler && _stopStreamsHandler)
        {
            registerRequestHandler(_argumentDescriptionsHandler);
            registerRequestHandler(_configurationHandler);
            registerRequestHandler(_configureHandler);
            registerRequestHandler(_restartStreamsHandler);
            registerRequestHandler(_startStreamsHandler);
            registerRequestHandler(_stopStreamsHandler);
        }
        else
        {
            OD_LOG("! (_argumentDescriptionsHandler && _configurationHandler && " //####
                   "_configureHandler && _restartStreamsHandler && _startStreamsHandler && " //####
                   "_stopStreamsHandler)"); //####
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::attachRequestHandlers

void
BaseInputOutputService::detachRequestHandlers(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (_argumentDescriptionsHandler)
        {
            unregisterRequestHandler(_argumentDescriptionsHandler);
            delete _argumentDescriptionsHandler;
            _argumentDescriptionsHandler = NULL;
        }
        if (_configurationHandler)
        {
            unregisterRequestHandler(_configurationHandler);
            delete _configurationHandler;
            _configurationHandler = NULL;
        }
        if (_configureHandler)
        {
            unregisterRequestHandler(_configureHandler);
            delete _configureHandler;
            _configureHandler = NULL;
        }
        if (_restartStreamsHandler)
        {
            unregisterRequestHandler(_restartStreamsHandler);
            delete _restartStreamsHandler;
            _restartStreamsHandler = NULL;
        }
        if (_startStreamsHandler)
        {
            unregisterRequestHandler(_startStreamsHandler);
            delete _startStreamsHandler;
            _startStreamsHandler = NULL;
        }
        if (_stopStreamsHandler)
        {
            unregisterRequestHandler(_stopStreamsHandler);
            delete _stopStreamsHandler;
            _stopStreamsHandler = NULL;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::detachRequestHandlers

DEFINE_DISABLEMETRICS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    inherited::disableMetrics();
    if (0 < _clientStreams.size())
    {
        for (ClientChannelVector::const_iterator walker(_clientStreams.begin());
             _clientStreams.end() != walker; ++walker)
        {
            ClientChannel * aChannel = *walker;
            
            if (aChannel)
            {
                aChannel->disableMetrics();
            }
        }
    }
    if (0 < _inStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_inStreams.begin());
             _inStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                aChannel->disableMetrics();
            }
        }
    }
    if (0 < _outStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_outStreams.begin());
             _outStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                aChannel->disableMetrics();
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::disableMetrics

DEFINE_DOIDLE_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::doIdle

DEFINE_ENABLEMETRICS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    inherited::enableMetrics();
    if (0 < _clientStreams.size())
    {
        for (ClientChannelVector::const_iterator walker(_clientStreams.begin());
             _clientStreams.end() != walker; ++walker)
        {
            ClientChannel * aChannel = *walker;
            
            if (aChannel)
            {
                aChannel->enableMetrics();
            }
        }
    }
    if (0 < _inStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_inStreams.begin());
             _inStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                aChannel->enableMetrics();
            }
        }
    }
    if (0 < _outStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_outStreams.begin());
             _outStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                aChannel->enableMetrics();
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::enableMetrics

DEFINE_FILLINSECONDARYCLIENTCHANNELSLIST_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("channels = ", &channels); //####
    inherited::fillInSecondaryClientChannelsList(channels);
    if (0 < _clientStreams.size())
    {
        for (ClientChannelVector::const_iterator walker(_clientStreams.begin());
             _clientStreams.end() != walker; ++walker)
        {
            ClientChannel * aChannel = *walker;
            
            if (aChannel)
            {
                OD_LOG_S1s("aChannel = ", aChannel->name()); //####
                ChannelDescription descriptor;
                
                descriptor._portName = aChannel->name();
                descriptor._portProtocol = "";
                descriptor._portMode = kChannelModeTCP;
                descriptor._protocolDescription = "";
                channels.push_back(descriptor);
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::fillInSecondaryClientChannelsList

DEFINE_FILLINSECONDARYINPUTCHANNELSLIST_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("channels = ", &channels); //####
    inherited::fillInSecondaryInputChannelsList(channels);
    if (0 < _inStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_inStreams.begin());
             _inStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                OD_LOG_S1s("aChannel = ", aChannel->name()); //####
                ChannelDescription descriptor;
                
                descriptor._portName = aChannel->name();
                descriptor._portProtocol = aChannel->protocol();
                descriptor._portMode = kChannelModeTCP;
                descriptor._protocolDescription = aChannel->protocolDescription();
                channels.push_back(descriptor);
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::fillInSecondaryInputChannelsList

DEFINE_FILLINSECONDARYOUTPUTCHANNELSLIST_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("channels = ", &channels); //####
    inherited::fillInSecondaryOutputChannelsList(channels);
    if (0 < _outStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_outStreams.begin());
             _outStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                OD_LOG_S1s("aChannel = ", aChannel->name()); //####
                ChannelDescription descriptor;
                
                descriptor._portName = aChannel->name();
                descriptor._portProtocol = aChannel->protocol();
                descriptor._portMode = kChannelModeTCP;
                descriptor._protocolDescription = aChannel->protocolDescription();
                channels.push_back(descriptor);
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::fillInSecondaryOutputChannelsList

DEFINE_GATHERMETRICS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("metrics = ", &metrics); //####
    SendReceiveCounters counters;
    
    inherited::gatherMetrics(metrics);
    for (ClientChannelVector::const_iterator walker(_clientStreams.begin());
         _clientStreams.end() != walker; ++walker)
    {
        ClientChannel * aChannel = *walker;
        
        if (aChannel)
        {
            aChannel->getSendReceiveCounters(counters);
            counters.addToList(metrics, aChannel->name());
        }
    }
    for (GeneralChannelVector::const_iterator walker(_inStreams.begin());
         _inStreams.end() != walker; ++walker)
    {
        GeneralChannel * aChannel = *walker;
        
        if (aChannel)
        {
            aChannel->getSendReceiveCounters(counters);
            counters.addToList(metrics, aChannel->name());
        }
    }
    for (GeneralChannelVector::const_iterator walker(_outStreams.begin());
         _outStreams.end() != walker; ++walker)
    {
        GeneralChannel * aChannel = *walker;
        
        if (aChannel)
        {
            aChannel->getSendReceiveCounters(counters);
            counters.addToList(metrics, aChannel->name());
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::gatherMetrics

size_t
BaseInputOutputService::getClientCount(void)
const
{
    OD_LOG_OBJENTER(); //####
    size_t result = _clientStreams.size();
    
    OD_LOG_OBJEXIT_L(result); //#####
    return result;
} // BaseInputOutputService::getClientCount

ClientChannel *
BaseInputOutputService::getClientStream(const size_t index)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("index = ", index); //####
    ClientChannel * result = _clientStreams.at(index);
    
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // BaseInputOutputService::getClientStream

size_t
BaseInputOutputService::getInletCount(void)
const
{
    OD_LOG_OBJENTER(); //####
    size_t result = _inStreams.size();
    
    OD_LOG_OBJEXIT_L(result); //#####
    return result;
} // BaseInputOutputService::getInletCount

GeneralChannel *
BaseInputOutputService::getInletStream(const size_t index)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("index = ", index); //####
    GeneralChannel * result = _inStreams.at(index);
    
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // BaseInputOutputService::getInletStream

size_t
BaseInputOutputService::getOutletCount(void)
const
{
    OD_LOG_OBJENTER(); //####
    size_t result = _outStreams.size();
    
    OD_LOG_OBJEXIT_L(result); //#####
    return result;
} // BaseInputOutputService::getOutletCount

GeneralChannel *
BaseInputOutputService::getOutletStream(const size_t index)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("index = ", index); //####
    GeneralChannel * result = _outStreams.at(index);
    
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // BaseInputOutputService::getOutletStream

void
BaseInputOutputService::performLaunch(const YarpString & helpText,
                                      const bool         goWasSet,
                                      const bool         stdinAvailable,
                                      const bool         reportOnExit)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("helpText = ", helpText); //####
    OD_LOG_B3("goWasSet = ", goWasSet, "stdinAvailable = ", stdinAvailable, //####
              "reportOnExit = ", reportOnExit); //####
    if (startService())
    {
        YarpString channelName(getEndpoint().getName());
        
        OD_LOG_S1s("channelName = ", channelName); //####
        if (RegisterLocalService(channelName, *this))
        {
            StartRunning();
            SetSignalHandlers(SignalRunningStop);
            startPinger();
            runService(helpText, false, goWasSet, stdinAvailable, reportOnExit);
            UnregisterLocalService(channelName, *this);
            if (reportOnExit)
            {
                yarp::os::Bottle metrics;
                
                gatherMetrics(metrics);
                YarpString converted(Utilities::ConvertMetricsToString(metrics));
                
                cout << converted.c_str() << endl;
            }
            stopService();
        }
        else
        {
            OD_LOG("! (RegisterLocalService(channelName, *this))"); //####
            MpM_FAIL_(MSG_SERVICE_NOT_REGISTERED);
        }
    }
    else
    {
        OD_LOG("! (startService())"); //####
        MpM_FAIL_(MSG_SERVICE_NOT_STARTED);
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::performLaunch

void
BaseInputOutputService::requestServiceStop(void)
{
    OD_LOG_ENTER(); //####
    lStopTheService = true;
    OD_LOG_EXIT(); //####
} // BaseInputOutputService::requestServiceStop

void
BaseInputOutputService::runService(const YarpString & helpText,
                                   const bool         forAdapter,
                                   const bool         goWasSet,
                                   const bool         stdinAvailable,
                                   const bool         reportOnExit)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("helpText = ", helpText); //####
    OD_LOG_B4("forAdapter = ", forAdapter, "goWasSet = ", goWasSet, "stdinAvailable = ", //####
              stdinAvailable, "reportOnExit = ", reportOnExit); //####
    bool             configured = false;
    bool             firstLine = true;
    yarp::os::Bottle configureData;

    if (goWasSet || (! stdinAvailable))
    {
        Utilities::CopyArgumentsToBottle(_argumentList, configureData);
        if (configure(configureData))
        {
            startStreams();
        }
    }
    for ( ; IsRunning() && (! lStopTheService); )
    {
        if ((! goWasSet) && stdinAvailable)
        {
            char inChar;
            
            cout << "Operation: [? b c e q r]? ";
            cout.flush();
            for (set_tty_cbreak(); ! lStopTheService; )
            {
                inChar = kb_getc();
                if (inChar)
                {
                    cout << inChar;
                    cout.flush();
                    if (! isspace(inChar))
                    {
                        break;
                    }
                    
                }
                else
                {
                    if (isStarted())
                    {
                        if (_needsIdle)
                        {
                            doIdle();
                        }
                    }
                    ConsumeSomeTime();
                }
            }
            // Watch for a newline here!
            if (IsRunning() && (! firstLine))
            {
                for ( ; ! lStopTheService; )
                {
                    char peekChar = kb_getc();
                    
                    if (peekChar)
                    {
                        if (isspace(peekChar))
                        {
                            set_tty_cooked();
                            break;
                        }
                        
                    }
                    else
                    {
                        if (isStarted())
                        {
                            if (_needsIdle)
                            {
                                doIdle();
                            }
                        }
                        ConsumeSomeTime();
                    }
                }
            }
            set_tty_cooked();
            if (firstLine)
            {
                firstLine = false;
            }
            else
            {
                cout << endl;
                cout.flush();
            }
            if (! lStopTheService)
            {
                switch (inChar)
                {
                    case '?' :
                        // Help
                        displayCommands(helpText, forAdapter);
                        break;
                        
                    case 'b' :
                    case 'B' :
                        // Start streams
                        if (! configured)
                        {
                            configured = Utilities::PromptForValues(_argumentList);
                            if (configured)
                            {
                                Utilities::CopyArgumentsToBottle(_argumentList, configureData);
                                if (! configure(configureData))
                                {
                                    configured = false;
                                }
                            }
                            else
                            {
                                cout << "One or more values out of range." << endl;
                            }
                        }
                        if (configured)
                        {
                            startStreams();
                        }
                        break;
                        
                    case 'c' :
                    case 'C' :
                        // Configure
                        configured = Utilities::PromptForValues(_argumentList);
                        if (configured)
                        {
                            Utilities::CopyArgumentsToBottle(_argumentList, configureData);
                            if (! configure(configureData))
                            {
                                configured = false;
                            }
                        }
                        else
                        {
                            cout << "One or more values out of range." << endl;
                        }
                        break;
                        
                    case 'e' :
                    case 'E' :
                        // Stop streams
                        stopStreams();
                        break;
                        
                    case 0 : // Some external event has caused us to exit!
                    case 'q' :
                    case 'Q' :
                        // Quit
                        StopRunning();
                        cout << endl;
                        cout.flush();
                        break;
                        
                    case 'r' :
                    case 'R' :
                        // Restart streams
                        if (! configured)
                        {
                            configured = Utilities::PromptForValues(_argumentList);
                            if (configured)
                            {
                                Utilities::CopyArgumentsToBottle(_argumentList, configureData);
                                if (! configure(configureData))
                                {
                                    configured = false;
                                }
                            }
                            else
                            {
                                cout << "One or more values out of range." << endl;
                            }
                        }
                        if (configured)
                        {
                            restartStreams();
                        }
                        break;
                        
                    default :
                        cout << "Unrecognized request '" << inChar << "'." << endl;
                        break;
                        
                }
            }
        }
        else
        {
            if (_needsIdle)
            {
                doIdle();
            }
            ConsumeSomeTime();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // BaseInputOutputService::runService

DEFINE_SETUPCLIENTSTREAMS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = shutDownClientStreams(); // clear out existing streams first
    
    OD_LOG_EXIT_B(result); //####
    return result;
} // BaseInputOutputService::setUpClientStreams

DEFINE_SETUPINPUTSTREAMS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = shutDownInputStreams(); // clear out existing streams first
    
    OD_LOG_EXIT_B(result); //####
    return result;
} // BaseInputOutputService::setUpInputStreams

DEFINE_SETUPOUTPUTSTREAMS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = shutDownOutputStreams(); // clear out existing streams first
    
    OD_LOG_EXIT_B(result); //####
    return result;
} // BaseInputOutputService::setUpOutputStreams

DEFINE_SHUTDOWNCLIENTSTREAMS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = true; // by default, always true
    
    if (0 < _clientStreams.size())
    {
        for (ClientChannelVector::const_iterator walker(_clientStreams.begin());
             _clientStreams.end() != walker; ++walker)
        {
            ClientChannel * aChannel = *walker;
            
            if (aChannel)
            {
                OD_LOG_P1("aChannel = ", aChannel); //####
                delete aChannel;
            }
        }
        _clientStreams.clear();
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // BaseInputOutputService::shutDownClientStreams

DEFINE_SHUTDOWNINPUTSTREAMS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = true; // by default, always true
    
    if (0 < _inStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_inStreams.begin());
             _inStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                OD_LOG_P1("aChannel = ", aChannel); //####
                delete aChannel;
            }
        }
        _inStreams.clear();
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // BaseInputOutputService::shutDownInputStreams

DEFINE_SHUTDOWNOUTPUTSTREAMS_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = true; // by default, always true
    
    if (0 < _outStreams.size())
    {
        for (GeneralChannelVector::const_iterator walker(_outStreams.begin());
             _outStreams.end() != walker; ++walker)
        {
            GeneralChannel * aChannel = *walker;
            
            if (aChannel)
            {
                OD_LOG_P1("aChannel = ", aChannel); //####
                delete aChannel;
            }
        }
        _outStreams.clear();
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // BaseInputOutputService::shutDownOutputStreams

DEFINE_STARTSERVICE_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    try
    {
        if (! isStarted())
        {
            inherited::startService();
            if (isStarted() && setUpStreamDescriptions() && setUpClientStreams() &&
                setUpInputStreams() && setUpOutputStreams())
            {
            
            }
            else
            {
                OD_LOG("! (isStarted() && setUpStreamDescriptions() && " //####
                       "setUpClientStreams() && setUpInputStreams() && " //####
                       "setUpOutputStreams())"); //####
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
} // BaseInputOutputService::startService

DEFINE_STOPSERVICE_(BaseInputOutputService)
{
    OD_LOG_OBJENTER(); //####
    bool result = true;
    
    try
    {
        if (! shutDownClientStreams())
        {
            result = false;
        }
        if (! shutDownInputStreams())
        {
            result = false;
        }
        if (! shutDownOutputStreams())
        {
            result = false;
        }
        if (! inherited::stopService())
        {
            result = false;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // BaseInputOutputService::stopService

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
