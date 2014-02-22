//
//  YPPCommonTest.cpp
//  YarpPlusPlus
//
//  Created by Norman Jaffe on 2014-02-06.
//  Copyright (c) 2014 OpenDragon. All rights reserved.
//

#include "YPPServiceRequest.h"
#include "YPPBaseClient.h"
#include "YPPBaseService.h"
#define ENABLE_OD_SYSLOG /* */
#include "ODSyslog.h"
#include <yarp/os/all.h>
#include <yarp/conf/version.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

#pragma mark Private structures and constants

#pragma mark Local functions

/*! @brief The critical information for each volume.
 
 This class represents the information for a volume that is needed to perform the data delete operations. */
class EndpointStatusReporter : public yarp::os::PortReport
{
public:
    
    /*! @brief The constructor. */
    EndpointStatusReporter(void);
    
    /*! @brief The destructor. */
    virtual ~EndpointStatusReporter(void);
    
    /*! @brief Write out the endpoint event / state information.
     @param info The event / state information from the endpoint. */
    virtual void report(const yarp::os::PortInfo & info);

protected:

private:

}; // EndpointStatusReporter

#pragma mark Class methods

#pragma mark Constructors and destructors

EndpointStatusReporter::EndpointStatusReporter(void) :
        PortReport()
{
} // EndpointStatusReporter::EndpointStatusReporter

EndpointStatusReporter::~EndpointStatusReporter(void)
{
} // EndpointStatusReporter::~EndpointStatusReporter

#pragma mark Actions

void EndpointStatusReporter::report(const yarp::os::PortInfo & info)
{
    OD_SYSLOG_LL1("tag = ", info.tag);
    OD_SYSLOG_B2("incoming = ", info.incoming, "created = ", info.created);
    OD_SYSLOG_S4("portName = ", info.portName.c_str(), "sourceName = ", info.sourceName.c_str(),
                 "targetName = ", info.targetName.c_str(), "carrierName = ", info.carrierName.c_str());
    OD_SYSLOG_S1("message = ", info.message.c_str());
} // EndpointStatusReporter::report

#pragma mark Accessors

#pragma mark Local functions

/*! @brief Create an endpoint for a test.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the endpoint constructor.
 @returns A newly created endpoint, or @c NULL if one could not be created. */
static YarpPlusPlus::Endpoint * doCreateEndpointForTest(const int argc,
                                                        char **   argv)
{
    YarpPlusPlus::Endpoint * stuff = NULL;
    
    if (argc > 0)
    {
        switch (argc)
        {
            // Argument order for tests = endpoint name [, IP address / name [, port [, carrier]]]
            case 1:
                stuff = new YarpPlusPlus::Endpoint(*argv);
                break;
                
            case 2:
                stuff = new YarpPlusPlus::Endpoint(*argv, argv[1]);
                break;
                
            case 3:
                stuff = new YarpPlusPlus::Endpoint(*argv, argv[1], argv[2]);
                break;
                
            default:
                break;
                
        }
    }
    return stuff;
} // doCreateEndpointForTest

#pragma mark *** Test Case 01 ***

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase01(const int argc,
                    char **   argv) // create endpoint
{
    int                      result;
    YarpPlusPlus::Endpoint * stuff = doCreateEndpointForTest(argc, argv);
    
    if (stuff)
    {
        if (stuff->open())
        {
            OD_SYSLOG_S1("endpoint name = ", stuff->getName().c_str());//####
            result = 0;
        }
        else
        {
            result = 1;
        }
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase01

#pragma mark *** Test Case 02 ***

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase02(const int argc,
                    char **   argv) // connect to endpoint
{
    int                      result;
    YarpPlusPlus::Endpoint * stuff = doCreateEndpointForTest(argc, argv);
    
    if (stuff)
    {
        EndpointStatusReporter reporter;
        
        if (stuff->open() && stuff->setReporter(reporter, true))
        {
            OD_SYSLOG_S1("endpoint name = ", stuff->getName().c_str());//####
            // Now we try to connect!
            yarp::os::Port        outPort;
            yarp::os::ConstString aName = YarpPlusPlus::Endpoint::getRandomPortName();
            
            OD_SYSLOG_S1("opening ", aName.c_str());//####
            if (outPort.open(aName))
            {
                OD_SYSLOG("(outPort.open(aName))");//####
                outPort.getReport(reporter);
                if (outPort.addOutput(stuff->getName()))
                {
                    OD_SYSLOG("(outPort.addOutput(stuff->getName()))");//####
                    result = 0;
                }
                else
                {
                    OD_SYSLOG("! (outPort.addOutput(stuff->getName()))");//####
                    result = 1;
                }
                outPort.close();
            }
            else
            {
                OD_SYSLOG("! (outPort.open(aName))");//####
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase02

#pragma mark *** Test Case 03 ***

/*! @brief A test input handler. */
class Test03Handler : public YarpPlusPlus::InputHandler
{
public:
    
    /*! @brief The constructor. */
    Test03Handler(void);
    
    /*! @brief The destructor. */
    virtual ~Test03Handler(void);
    
    /*! @brief Process partially-structured input data.
     @param input The partially-structured input data.
     @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
     @returns @c true if the input was correctly structured and successfully processed. */
    virtual bool handleInput(yarp::os::Bottle &           input,
                             yarp::os::ConnectionWriter * replyMechanism);
    
protected:
    
private:

}; // Test03Handler

#pragma mark Class methods

#pragma mark Constructors and destructors

Test03Handler::Test03Handler(void) :
        InputHandler()
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test03Handler::Test03Handler

Test03Handler::~Test03Handler(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test03Handler::~Test03Handler

#pragma mark Actions

bool Test03Handler::handleInput(yarp::os::Bottle &           input,
                                yarp::os::ConnectionWriter * replyMechanism)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_P1("replyMechanism = ", replyMechanism);//####
    OD_SYSLOG_S1("got ", input.toString().c_str());//####
    OD_SYSLOG_EXIT_B(TRUE);//####
    return true;
} // Test03Handler::handleInput

#pragma mark Accessors

#pragma mark Local functions

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase03(const int argc,
                    char **   argv) // send to endpoint
{
    int                      result;
    YarpPlusPlus::Endpoint * stuff = doCreateEndpointForTest(argc, argv);
    
    if (stuff)
    {
        EndpointStatusReporter reporter;
        Test03Handler          handler;

        if (stuff->setInputHandler(handler) && stuff->open() && stuff->setReporter(reporter, true))
        {
            OD_SYSLOG_S1("endpoint name = ", stuff->getName().c_str());//####
            // Now we try to connect!
            yarp::os::Port        outPort;
            yarp::os::ConstString aName = YarpPlusPlus::Endpoint::getRandomPortName();
            
            OD_SYSLOG_S1("opening ", aName.c_str());//####
            if (outPort.open(aName))
            {
                OD_SYSLOG("(outPort.open(aName))");//####
                outPort.getReport(reporter);
                if (outPort.addOutput(stuff->getName()))
                {
                    OD_SYSLOG("(outPort.addOutput(stuff->getName()))");//####
                    yarp::os::Bottle message;
                    
                    message.addString(aName);
                    message.addString("howdi");
                    if (outPort.write(message))
                    {
                        OD_SYSLOG("(outPort.write(message))");//####
                        result = 0;
                    }
                    else
                    {
                        OD_SYSLOG("! (outPort.write(message))");//####
                        result = 1;
                    }
                }
                else
                {
                    OD_SYSLOG("! (outPort.addOutput(stuff->getName()))");//####
                    result = 1;
                }
                outPort.close();
            }
            else
            {
                OD_SYSLOG("! (outPort.open(aName))");//####
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase03

#pragma mark *** Test Case 04 ***

/*! @brief A test input handler. */
class Test04Handler : public YarpPlusPlus::InputHandler
{
public:
    
    /*! @brief The constructor. */
    Test04Handler(void);
    
    /*! @brief The destructor. */
    virtual ~Test04Handler(void);
    
    /*! @brief Process partially-structured input data.
     @param input The partially-structured input data.
     @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
     @returns @c true if the input was correctly structured and successfully processed. */
    virtual bool handleInput(yarp::os::Bottle &           input,
                             yarp::os::ConnectionWriter * replyMechanism);
    
protected:
    
private:
    
}; // Test04Handler

#pragma mark Class methods

#pragma mark Constructors and destructors

Test04Handler::Test04Handler(void) :
        InputHandler()
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test04Handler::Test04Handler

Test04Handler::~Test04Handler(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test04Handler::~Test04Handler

#pragma mark Actions

bool Test04Handler::handleInput(yarp::os::Bottle &           input,
                                yarp::os::ConnectionWriter * replyMechanism)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_P1("replyMechanism = ", replyMechanism);//####
    OD_SYSLOG_S1("got ", input.toString().c_str());//####
    if (replyMechanism)
    {
        input.write(*replyMechanism);
    }
    OD_SYSLOG_EXIT_B(TRUE);//####
    return true;
} // Test04Handler::handleInput

#pragma mark Accessors

#pragma mark Local functions

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase04(const int argc,
                    char **   argv) // send to endpoint
{
    int                      result;
    YarpPlusPlus::Endpoint * stuff = doCreateEndpointForTest(argc, argv);
    
    if (stuff)
    {
        EndpointStatusReporter reporter;
        Test04Handler          handler;

        if (stuff->setInputHandler(handler) && stuff->open() && stuff->setReporter(reporter, true))
        {
            OD_SYSLOG_S1("endpoint name = ", stuff->getName().c_str());//####
            // Now we try to connect!
            yarp::os::Port        outPort;
            yarp::os::ConstString aName = YarpPlusPlus::Endpoint::getRandomPortName();
            
            OD_SYSLOG_S1("opening ", aName.c_str());//####
            if (outPort.open(aName))
            {
                OD_SYSLOG("(outPort.open(aName))");//####
                outPort.getReport(reporter);
                if (outPort.addOutput(stuff->getName()))
                {
                    OD_SYSLOG("(outPort.addOutput(stuff->getName()))");//####
                    yarp::os::Bottle message;
                    yarp::os::Bottle response;
                    
                    message.addString(aName);
                    message.addString("howdi");
                    if (outPort.write(message, response))
                    {
                        OD_SYSLOG("(outPort.write(message, response))");//####
                        OD_SYSLOG_S1("got ", response.toString().c_str());//####
                        result = 0;
                    }
                    else
                    {
                        OD_SYSLOG("! (outPort.write(message, response))");//####
                        result = 1;
                    }
                }
                else
                {
                    OD_SYSLOG("! (outPort.addOutput(stuff->getName()))");//####
                    result = 1;
                }
                outPort.close();
            }
            else
            {
                OD_SYSLOG("! (outPort.open(aName))");//####
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase04

#pragma mark *** Test Case 05 ***

/*! @brief A test input handler. */
class Test05Handler : public YarpPlusPlus::InputHandler
{
public:
    
    /*! @brief The constructor. */
    Test05Handler(void);
    
    /*! @brief The destructor. */
    virtual ~Test05Handler(void);
    
    /*! @brief Process partially-structured input data.
     @param input The partially-structured input data.
     @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
     @returns @c true if the input was correctly structured and successfully processed. */
    virtual bool handleInput(yarp::os::Bottle &           input,
                             yarp::os::ConnectionWriter * replyMechanism);
    
protected:
    
private:
    
}; // Test05Handler

#pragma mark Class methods

#pragma mark Constructors and destructors

Test05Handler::Test05Handler(void) :
        InputHandler()
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test05Handler::Test05Handler

Test05Handler::~Test05Handler(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test05Handler::~Test05Handler

#pragma mark Actions

bool Test05Handler::handleInput(yarp::os::Bottle &           input,
                                yarp::os::ConnectionWriter * replyMechanism)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_P1("replyMechanism = ", replyMechanism);//####
    OD_SYSLOG_S1("got ", input.toString().c_str());//####
    if (replyMechanism)
    {
        input.write(*replyMechanism);
    }
    OD_SYSLOG_EXIT_B(TRUE);//####
    return true;
} // Test05Handler::handleInput

#pragma mark Accessors

/*! @brief A test input handler creator. */
class Test05HandlerCreator : public YarpPlusPlus::InputHandlerCreator
{
public:
    
    /*! @brief The constructor. */
    Test05HandlerCreator(void);
    
    /*! @brief The destructor. */
    virtual ~Test05HandlerCreator(void);
    
    /*! @brief Create a new InputHandler object to process input data.
     @returns A new InputHandler or @c NULL if one cannot be created. */
    virtual YarpPlusPlus::InputHandler * create(void);

protected:
    
private:
    
}; // Test05HandlerCreator

#pragma mark Class methods

#pragma mark Constructors and destructors

Test05HandlerCreator::Test05HandlerCreator(void) :
        InputHandlerCreator()
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test05HandlerCreator::Test05HandlerCreator

Test05HandlerCreator::~Test05HandlerCreator(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test05HandlerCreator::~Test05HandlerCreator

#pragma mark Actions

YarpPlusPlus::InputHandler * Test05HandlerCreator::create(void)
{
    return new Test05Handler;
} // Test05HandlerCreator::create

#pragma mark Accessors

#pragma mark Local functions

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase05(const int argc,
                    char **   argv) // send to endpoint
{
    int                      result;
    YarpPlusPlus::Endpoint * stuff = doCreateEndpointForTest(argc, argv);
    
    if (stuff)
    {
        EndpointStatusReporter reporter;
        Test05HandlerCreator   handlerCreator;
        
        if (stuff->setInputHandlerCreator(handlerCreator) && stuff->open() && stuff->setReporter(reporter, true))
        {
            OD_SYSLOG_S1("endpoint name = ", stuff->getName().c_str());//####
                                                                       // Now we try to connect!
            yarp::os::Port        outPort;
            yarp::os::ConstString aName = YarpPlusPlus::Endpoint::getRandomPortName();
            
            OD_SYSLOG_S1("opening ", aName.c_str());//####
            if (outPort.open(aName))
            {
                OD_SYSLOG("(outPort.open(aName))");//####
                outPort.getReport(reporter);
                if (outPort.addOutput(stuff->getName()))
                {
                    OD_SYSLOG("(outPort.addOutput(stuff->getName()))");//####
                    yarp::os::Bottle message;
                    yarp::os::Bottle response;
                    
                    message.addString(aName);
                    message.addString("howdi");
                    if (outPort.write(message, response))
                    {
                        OD_SYSLOG("(outPort.write(message, response))");//####
                        OD_SYSLOG_S1("got ", response.toString().c_str());//####
                        result = 0;
                    }
                    else
                    {
                        OD_SYSLOG("! (outPort.write(message, response))");//####
                        result = 1;
                    }
                }
                else
                {
                    OD_SYSLOG("! (outPort.addOutput(stuff->getName()))");//####
                    result = 1;
                }
                outPort.close();
            }
            else
            {
                OD_SYSLOG("! (outPort.open(aName))");//####
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase05

#pragma mark *** Test Case 06 ***

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase06(const int argc,
                    char **   argv) // create request
{
    int result;
    
    if (0 == argc)
    {
        result = 1;
    }
    else
    {
        yarp::os::Bottle parameters;
        
        for (int ii = 1; ii < argc; ++ii)
        {
            parameters.addString(argv[ii]);
        }
        YarpPlusPlus::ServiceRequest * stuff = new YarpPlusPlus::ServiceRequest(*argv, parameters);
        
        delete stuff;
        result = 0;
    }
    return result;
} // doCase06

#pragma mark *** Test Case 07 ***

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase07(const int argc,
                    char **   argv) // create request
{
    int              result;
    yarp::os::Bottle parameters;
    
    for (int ii = 0; ii < argc; ++ii)
    {
        parameters.addString(argv[ii]);
    }
    YarpPlusPlus::ServiceResponse * stuff = new YarpPlusPlus::ServiceResponse(parameters);
    
    delete stuff;
    result = 0;
    return result;
} // doCase07

#pragma mark *** Test Case 08 ***

/*! @brief A test input handler. */
class Test08Handler : public YarpPlusPlus::InputHandler
{
public:
    
    /*! @brief The constructor. */
    Test08Handler(void);
    
    /*! @brief The destructor. */
    virtual ~Test08Handler(void);
    
    /*! @brief Process partially-structured input data.
     @param input The partially-structured input data.
     @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
     @returns @c true if the input was correctly structured and successfully processed. */
    virtual bool handleInput(yarp::os::Bottle &           input,
                             yarp::os::ConnectionWriter * replyMechanism);
    
protected:
    
private:
    
}; // Test08Handler

#pragma mark Class methods

#pragma mark Constructors and destructors

Test08Handler::Test08Handler(void) :
        InputHandler()
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test08Handler::Test08Handler

Test08Handler::~Test08Handler(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test08Handler::~Test08Handler

#pragma mark Actions

bool Test08Handler::handleInput(yarp::os::Bottle &           input,
                                yarp::os::ConnectionWriter * replyMechanism)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_P1("replyMechanism = ", replyMechanism);//####
    OD_SYSLOG_S1("got ", input.toString().c_str());//####
    if (replyMechanism)
    {
        input.write(*replyMechanism);
    }
    OD_SYSLOG_EXIT_B(TRUE);//####
    return true;
} // Test08Handler::handleInput

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase08(const int argc,
                    char **   argv) // create request
{
    int                      result;
    YarpPlusPlus::Endpoint * stuff = doCreateEndpointForTest(argc, argv);
    
    if (stuff)
    {
        EndpointStatusReporter reporter;
        Test08Handler          handler;
        
        if (stuff->setInputHandler(handler) && stuff->open() && stuff->setReporter(reporter, true))
        {
            OD_SYSLOG_S1("endpoint name = ", stuff->getName().c_str());//####
            yarp::os::Bottle              parameters("some to send");
            YarpPlusPlus::ServiceRequest  request("echo", parameters);
            YarpPlusPlus::ServiceResponse response;
            
            if (request.send(*stuff, &response))
            {
                OD_SYSLOG_LL1("response size = ", response.count());//####
                for (int ii = 0; ii < response.count(); ++ii)
                {
                    OD_SYSLOG_S1("response value = ", response.element(ii).toString().c_str());//####
                }
                result = 0;
            }
            else
            {
                result = 1;
            }
        }
        else
        {
            result = 1;
        }
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase08

#pragma mark *** Test Case 09 ***

/*! @brief A test input handler. */
class Test09Service : public YarpPlusPlus::BaseService
{
public:
    
    /*! @brief The constructor.
     @param argc The number of arguments in 'argv'.
     @param argv The arguments to be used to specify the new service. */
    Test09Service(const int argc,
                  char **   argv);
    
    /*! @brief The destructor. */
    virtual ~Test09Service(void);
    
    /*! @brief Process partially-structured input data.
     @param input The partially-structured input data.
     @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
     @returns @c true if the input was correctly structured and successfully processed. */
    virtual bool processRequest(yarp::os::Bottle &           input,
                                yarp::os::ConnectionWriter * replyMechanism);
    
protected:
    
private:
    
}; // Test09Service

#pragma mark Class methods

#pragma mark Constructors and destructors

Test09Service::Test09Service(const int argc,
                             char **   argv) :
        BaseService(false, argc, argv)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test09Service::Test09Service

Test09Service::~Test09Service(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test09Service::~Test09Service

#pragma mark Actions

bool Test09Service::processRequest(yarp::os::Bottle &           input,
                                   yarp::os::ConnectionWriter * replyMechanism)
{
    OD_SYSLOG_ENTER();//####
    bool result = true;
    
    
    if (replyMechanism)
    {
        input.write(*replyMechanism);
    }
    OD_SYSLOG_EXIT_B(result);//####
    return result;
} // Test09Service::processRequest

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase09(const int argc,
                    char **   argv) // create request
{
    int             result;
    Test09Service * stuff = new Test09Service(argc, argv);
    
    if (stuff && stuff->start())
    {
        yarp::os::Bottle              parameters("some to send");
        YarpPlusPlus::ServiceRequest  request("echo", parameters);
        YarpPlusPlus::ServiceResponse response;
        
        if (request.send(stuff->getEndpoint(), &response))
        {
            OD_SYSLOG_LL1("response size = ", response.count());//####
            for (int ii = 0; ii < response.count(); ++ii)
            {
                OD_SYSLOG_S1("response value = ", response.element(ii).toString().c_str());//####
            }
            result = 0;
        }
        else
        {
            result = 1;
        }
        stuff->stop();
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase09

#pragma mark *** Test Case 10 ***

/*! @brief A test input handler. */
class Test10Service : public YarpPlusPlus::BaseService
{
public:
    
    /*! @brief The constructor.
     @param argc The number of arguments in 'argv'.
     @param argv The arguments to be used to specify the new service. */
    Test10Service(const int argc,
                  char **   argv);
    
    /*! @brief The destructor. */
    virtual ~Test10Service(void);
    
    /*! @brief Process partially-structured input data.
     @param input The partially-structured input data.
     @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
     @returns @c true if the input was correctly structured and successfully processed. */
    virtual bool processRequest(yarp::os::Bottle &           input,
                                yarp::os::ConnectionWriter * replyMechanism);
    
protected:
    
private:
    
}; // Test10Service

#pragma mark Class methods

#pragma mark Constructors and destructors

Test10Service::Test10Service(const int argc,
                             char **   argv) :
        BaseService(true, argc, argv)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test10Service::Test10Service

Test10Service::~Test10Service(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Test10Service::~Test10Service

#pragma mark Actions

bool Test10Service::processRequest(yarp::os::Bottle &           input,
                                   yarp::os::ConnectionWriter * replyMechanism)
{
    OD_SYSLOG_ENTER();//####
    bool result = true;
    
    
    if (replyMechanism)
    {
        input.write(*replyMechanism);
    }
    OD_SYSLOG_EXIT_B(result);//####
    return result;
} // Test10Service::processRequest

/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase10(const int argc,
                    char **   argv) // create request
{
    int             result;
    Test10Service * stuff = new Test10Service(argc, argv);
    
    if (stuff && stuff->start())
    {
        yarp::os::Bottle              parameters("some to send");
        YarpPlusPlus::ServiceRequest  request("echo", parameters);
        YarpPlusPlus::ServiceResponse response;
        
        if (request.send(stuff->getEndpoint(), &response))
        {
            OD_SYSLOG_LL1("response size = ", response.count());//####
            for (int ii = 0; ii < response.count(); ++ii)
            {
                OD_SYSLOG_S1("response value = ", response.element(ii).toString().c_str());//####
            }
            result = 0;
        }
        else
        {
            result = 1;
        }
        stuff->stop();
        delete stuff;
    }
    else
    {
        result = 1;
    }
    return result;
} // doCase10




#if 0
/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase12(const int argc,
                    char **   argv) // create service
{
#pragma unused(argc,argv)
    // Simple new / delete
    YarpPlusPlus::BaseService * stuff = new YarpPlusPlus::BaseService;
    
    delete stuff;
    return 0;
} // doCase12
#endif//0

#if 0
/*! @brief Perform a test case.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used for the test.
 @returns @c 0 on success and @c 1 on failure. */
static int doCase13(const int argc,
                    char **   argv) // create client
{
#pragma unused(argc,argv)
    // Simple new / delete
    YarpPlusPlus::BaseClient * stuff = new YarpPlusPlus::BaseClient;
    
    delete stuff;
    return 0;
} // doCase13
#endif//0

#pragma mark Global functions

/*! @brief The entry point for unit tests.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the unit tests.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int     argc,
         char ** argv)
{
    OD_SYSLOG_INIT(*argv, kODSyslogOptionIncludeProcessID | kODSyslogOptionIncludeThreadID);//####
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_S2("YARP Version = ", YARP_VERSION_STRING, "YARP++ Version = ", YPP_VERSION);//####
    yarp::os::Network yarp; // This is necessary to establish any connection to the YARP infrastructure
    int               result;
    
    if (--argc > 0)
    {
        int selector = atoi(argv[1]);
        
        //cout << selector << endl;
        OD_SYSLOG_LL1("selector <- ", selector);//####
        switch (selector)
        {
            case 1:
                result = doCase01(argc - 1, argv + 2);
                break;
                
            case 2:
                result = doCase02(argc - 1, argv + 2);
                break;
                
            case 3:
                result = doCase03(argc - 1, argv + 2);
                break;
                
            case 4:
                result = doCase04(argc - 1, argv + 2);
                break;
                
            case 5:
                result = doCase05(argc - 1, argv + 2);
                break;
                
            case 6:
                result = doCase06(argc - 1, argv + 2);
                break;
                
            case 7:
                result = doCase07(argc - 1, argv + 2);
                break;
                
            case 8:
                result = doCase08(argc - 1, argv + 2);
                break;
                
            case 9:
                result = doCase09(argc - 1, argv + 2);
                break;

            case 10:
                result = doCase10(argc - 1, argv + 2);
                break;
                
//            case 11:
//                result = doCase11(argc - 1, argv + 2);
//                break;
                
//            case 12:
//                result = doCase12(argc - 1, argv + 2);
//                break;
                
//            case 13:
//                result = doCase13(argc - 1, argv + 2);
//                break;
                
            default:
                result = 1;
                break;
                
        }
    }
    else
    {
        result = 1;
    }
    OD_SYSLOG_EXIT_L(result);//####
    return result;
} // main
