//
//  YPPBaseService.h
//  YarpPlusPlus
//
//  Created by Norman Jaffe on 2014-02-06.
//  Copyright (c) 2014 OpenDragon. All rights reserved.
//

#if (! defined(YPPBASESERVICE_H_))
# define YPPBASESERVICE_H_ /* */

# include "YPPEndpoint.h"
# include <map>
# include <yarp/os/ConstString.h>

namespace YarpPlusPlus
{
    class BaseServiceInputHandler;
    class BaseServiceInputHandlerCreator;
    class RequestHandler;
    
    /*! @brief The minimal functionality required for a Yarp++ service. */
    class BaseService
    {
    public:
        
        /*! @brief The constructor.
         @param useMultipleHandlers @c true if simultaneous handlers are allowed, @c false if one handler is used.
         @param serviceEndpointName The YARP name to be assigned to the new service.
         @param serviceHostName The name or IP address of the machine running the service.
         @param servicePortNumber The port being used by the service. */
        BaseService(const bool                    useMultipleHandlers,
                    const yarp::os::ConstString & serviceEndpointName,
                    const yarp::os::ConstString & serviceHostName = "",
                    const yarp::os::ConstString & servicePortNumber = "");
        
        /*! @brief The constructor.
         @param useMultipleHandlers @c true if simultaneous handlers are allowed, @c false if one handler is used.
         @param argc The number of arguments in 'argv'.
         @param argv The arguments to be used to specify the new service. */
        BaseService(const bool useMultipleHandlers,
                    const int  argc,
                    char **    argv);
        
        /*! @brief The destructor. */
        virtual ~BaseService(void);
        
        /*! @brief Construct the response to a 'list' request.
         @param reply The Bottle to hold the reply. */
        void fillInListReply(yarp::os::Bottle & reply);
        
        /*! @brief Construct the response to an 'info' request.
         @param reply The Bottle to hold the reply.
         @param requestName The name of the request that is being looked at. */
        void fillInRequestInfo(yarp::os::Bottle &            reply,
                               const yarp::os::ConstString & requestName);
        
        /*! @brief Return the associated endpoint.
         @returns The associated endpoint. */
        inline Endpoint & getEndpoint(void)
        const
        {
            return *_endpoint;
        } // getEndpoint
        
        /*! @brief Return the state of the service.
         @returns @c true if the service has been started and @c false otherwise. */
        inline bool isStarted(void)
        const
        {
            return _started;
        } // isStarted
        
        /*! @brief Process partially-structured input data.
         @param request The requested operation.
         @param restOfInput The arguments for the operation.
         @param replyMechanism @c NULL if no reply is expected and non-@c NULL otherwise.
         @returns @c true if the input was correctly structured and successfully processed. */
        bool processRequest(const yarp::os::ConstString & request,
                            const yarp::os::Bottle &      restOfInput,
                            yarp::os::ConnectionWriter *  replyMechanism);
        
        /*! @brief Start processing requests.
         @returns @c true if the service was started and @c false if it was not. */
        virtual bool start(void);
        
        /*! @brief Stop processing requests.
         @returns @c true if the service was stopped and @c false it if was not. */
        virtual bool stop(void);
        
    protected:
        
        typedef std::map<std::string, RequestHandler *> RequestHandlerMap;
        
        typedef RequestHandlerMap::value_type RequestHandlerMapValue;
        
        /*! @brief Return the function corresponding to a particular request.
         @param request The requested operation.
         @returns A pointer to the function to be invoked for the request, or @c NULL if it is not recognized. */
        RequestHandler * lookupRequestHandler(const yarp::os::ConstString & request);

        /*! @brief Remember the function to be used to handle a particular request.
         @param request The requested operation.
         @param handler The function to be called for the operation. */
        void registerRequestHandler(const yarp::os::ConstString & request,
                                    RequestHandler *              handler);
        
        /*! @brief Remember the function to be used to handle unrecognized requests.
         @param handler The function to be called by default. */
        void setDefaultRequestHandler(RequestHandler * handler);
        
    private:
        
        /*! @brief Copy constructor.

         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        BaseService(const BaseService & other);
        
        /*! @brief Assignment operator.

         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        BaseService & operator=(const BaseService & other);
        
        /*! @brief Set up the standard request handlers. */
        void setUpStandardHandlers(void);
        
        /*! @brief The connection point for the service. */
        Endpoint * _endpoint;
        
        /*! @brief The input handler for the service. */
        BaseServiceInputHandler * _handler;

        /*! @brief The input handler creator for the service. */
        BaseServiceInputHandlerCreator * _handlerCreator;
        
        /*! @brief The map between requests and request handlers. */
        RequestHandlerMap _requestHandlers;
        
        /*! @brief The default handler to use for unrecognized requests. */
        RequestHandler * _defaultHandler;
        
        /*! @brief The current state of the service - @c true if active and @c false otherwise. */
        bool _started;
        
        /*! @brief Whether to use a handler creator or a handler - @c true for a creator and @c false otherwise. */
        bool _useMultipleHandlers;
        
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-private-field"
        /*! @brief Filler to pad to alignment boundary */
        char _filler[6];
# pragma clang diagnostic pop
        
    }; // BaseService
    
} // YarpPlusPlus

#endif // ! defined(YPPBASESERVICE_H_)
