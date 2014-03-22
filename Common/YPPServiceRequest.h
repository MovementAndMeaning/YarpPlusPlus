//--------------------------------------------------------------------------------------
//
//  File:       YPPServiceRequest.h
//
//  Project:    YarpPlusPlus
//
//  Contains:   The class declaration for a Yarp++ request.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms,
//              with or without modification, are permitted provided that the following
//              conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the
//                  distribution.
//                * Neither the name of the copyright holders nor the names of its
//                  contributors may be used to endorse or promote products derived
//                  from this software without specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created:    2014-02-06
//
//--------------------------------------------------------------------------------------

#if (! defined(YPPSERVICEREQUEST_H_))
# define YPPSERVICEREQUEST_H_ /* */

# include "YPPServiceResponse.h"
# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wc++11-extensions"
#  pragma clang diagnostic ignored "-Wdocumentation"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#  pragma clang diagnostic ignored "-Wpadded"
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wweak-vtables"
# endif // defined(__APPLE__)
# include <yarp/os/Port.h>
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace YarpPlusPlus
{
    class Endpoint;
    
    /*! @brief The data constituting a service request. */
    class ServiceRequest
    {
    public:
        
        /*! @brief The constructor.
         @param requestName The request to be processed.
         @param parameters The (optional) parameters for the request. */
        ServiceRequest(const yarp::os::ConstString & requestName,
                       const yarp::os::Bottle &      parameters = "");
        
        /*! @brief The destructor. */
        virtual ~ServiceRequest(void);
        
        /*! @brief Send the request to an endpoint for processing.
         @param usingPort The port that is to send the request, or @c NULL if an arbitrary port is to be used.
         @param response The response from the request, @c NULL if none is expected.
         @returns @c true if the request was successfully transmitted. */
        bool send(yarp::os::Port &  usingPort,
                  ServiceResponse * response = NULL);

    protected:
        
    private:
        
        /*! @brief Copy constructor.
         
         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        ServiceRequest(const ServiceRequest & other);
        
        /*! @brief Assignment operator.
         
         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        ServiceRequest & operator=(const ServiceRequest & other);
        
        /*! @brief The request name. */
        yarp::os::ConstString _name;
        /*! @brief The response holding area. */
        yarp::os::Bottle      _holder;
        /*! @brief The request parameters. */
        yarp::os::Bottle      _parameters;
        
    }; // ServiceRequest
    
} // YarpPlusPlus

#endif // ! defined(YPPSERVICEREQUEST_H_)
