//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mRequestMap.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the mapping between requests and request handlers.
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
//  Created:    2014-02-28
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMRequestMap_HPP_))
# define MpMRequestMap_HPP_ /* Header guard */

# include <m+m/m+mCommon.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the mapping between requests and request handlers. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Common
    {
        class BaseRequestHandler;
        class BaseService;

        /*! @brief A class to manage the mapping from requests to request handlers. */
        class RequestMap
        {
        public :

        protected :

        private :

            /*! @brief A mapping from strings to requests. */
            typedef std::map<YarpString, BaseRequestHandler *> RequestHandlerMap;

            /*! @brief The entry-type for the mapping. */
            typedef RequestHandlerMap::value_type RequestHandlerMapValue;

        public :

            /*! @brief The constructor.
             @param[in] owner The service that owns the mapping. */
            explicit
            RequestMap(BaseService & owner);

            /*! @brief The destructor. */
            virtual
            ~RequestMap(void);

            /*! @brief Construct the response to a 'list' request.
             @param[in,out] reply The package to hold the reply. */
            void
            fillInListReply(yarp::os::Bottle & reply);

            /*! @brief Construct the response to an 'info' request.
             @param[in,out] reply The package to hold the reply.
             @param[in] requestName The name of the request that is being looked at. */
            void
            fillInRequestInfo(yarp::os::Bottle & reply,
                              const YarpString & requestName);

            /*! @brief Return the function corresponding to a particular request.
             @param[in] request The requested operation.
             @return A pointer to the function to be invoked for the request, or @c NULL if it is
             not recognized. */
            BaseRequestHandler *
            lookupRequestHandler(const YarpString & request);

            /*! @brief Remember the function to be used to handle a particular request.
             @param[in] handler The function to be called for the request. */
            void
            registerRequestHandler(BaseRequestHandler * handler);

            /*! @brief Remember the function to be used to handle unrecognized requests.
             @param[in] handler The function to be called by default. */
            void
            setDefaultRequestHandler(BaseRequestHandler * handler);

            /*! @brief Forget the function to be used to handle a particular request.
             @param[in] handler The function that was called for the request. */
            void
            unregisterRequestHandler(BaseRequestHandler * handler);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            RequestMap(const RequestMap & other);

            /*! @brief Lock the data unless the lock would block.
             @return @c true if the data was locked and @c false otherwise. */
            inline bool
            conditionallyLock(void)
            {
                return _lock.tryLock();
            } // conditionallyLock

            /*! @brief Lock the data. */
            inline void
            lock(void)
            {
                _lock.lock();
            } // lock

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            RequestMap &
            operator =(const RequestMap & other);

            /*! @brief Unlock the data. */
            inline void
            unlock(void)
            {
                _lock.unlock();
            } // unlock

        public :

        protected :

        private :

            /*! @brief The contention lock used to avoid inconsistencies. */
            yarp::os::Mutex _lock;

            /*! @brief The default handler to use for unrecognized requests. */
            BaseRequestHandler * _defaultHandler;

            /*! @brief The map between requests and request handlers. */
            RequestHandlerMap _handlers;

            /*! @brief The service that owns this map. */
            BaseService & _owner;

        }; // RequestMap

    } // Common

} // MplusM

#endif // ! defined(MpMRequestMap_HPP_)
