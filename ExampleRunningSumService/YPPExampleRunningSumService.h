//--------------------------------------------------------------------------------------
//
//  File:       YPPExampleRunningSumService.h
//
//  Project:    YarpPlusPlus
//
//  Contains:   The class declaration for a simple Yarp++ service.
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
//  Created:    2014-03-18
//
//--------------------------------------------------------------------------------------

#if (! defined(YPPEXAMPLERUNNINGSUMSERVICE_H_))
# define YPPEXAMPLERUNNINGSUMSERVICE_H_ /* */

# include "YPPBaseService.h"

/*! @brief The port name to use for the service if not provided. */
# define DEFAULT_RUNNINGSUM_SERVICE_NAME "/service/runningSum"

namespace YarpPlusPlusExample
{
    /*! @brief An example Yarp++ service, handling 'random' requests. */
    class ExampleRunningSumService : public YarpPlusPlus::BaseService
    {
    public:
        
        /*! @brief The constructor.
         @param serviceEndpointName The YARP name to be assigned to the new service.
         @param serviceHostName The name or IP address of the machine running the service.
         @param servicePortNumber The port being used by the service. */
        ExampleRunningSumService(const yarp::os::ConstString & serviceEndpointName,
                                 const yarp::os::ConstString & serviceHostName = "",
                                 const yarp::os::ConstString & servicePortNumber = "");
        
        /*! @brief The destructor. */
        virtual ~ExampleRunningSumService(void);
        
        /*! @brief Start processing requests.
         @returns @c true if the service was started and @c false if it was not. */
        virtual bool start(void);
        
        /*! @brief Stop processing requests.
         @returns @c true if the service was stopped and @c false it if was not. */
        virtual bool stop(void);

        /*! @brief Add to the running sum for the given client.
         @param key The client-provided key.
         @param value The value to be added to the running sum.
         @returns The running sum, including the newly added value. */
        double addToSum(const yarp::os::ConstString & key,
                        const double                  value);

        /*! @brief Reset the running sum for the given client.
         @param key The client-provided key. */
        void resetSum(const yarp::os::ConstString & key);
        
        /*! @brief Start a running sum for the given client.
         @param key The client-provided key. */
        void startSum(const yarp::os::ConstString & key);
        
        /*! @brief Stop a running sum for the given client.
         @param key The client-provided key. */
        void stopSum(const yarp::os::ConstString & key);
        
    protected:
        
    private:
        
        /*! @brief The class that this class is derived from. */
        typedef BaseService inherited;

        /*! @brief Copy constructor.
         
         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        ExampleRunningSumService(const ExampleRunningSumService & other);
        
        /*! @brief Assignment operator.
         
         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        ExampleRunningSumService & operator=(const ExampleRunningSumService & other);
        
        /*! @brief Set up the standard request handlers. */
        void setUpRequestHandlers(void);
        
    }; // ExampleRunningSumService
    
} // YarpPlusPlusExample

#endif // ! defined(YPPEXAMPLERUNNINGSUMSERVICE_H_)
