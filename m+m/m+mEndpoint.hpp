//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mEndpoint.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the connection endpoint for an m+m service.
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
//  Created:    2014-02-07
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMEndpoint_HPP_))
# define MpMEndpoint_HPP_ /* Header guard */

# include <m+m/m+mCommon.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the connection endpoint for an m+m service. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Common
    {
        class BaseInputHandler;
        class BaseInputHandlerCreator;
        class ChannelStatusReporter;
        class SendReceiveCounters;
        class ServiceChannel;

        /*! @brief An object that represents an endpoint that provides a bidirectional connection
         for services and clients. */
        class Endpoint
        {
        public :

        protected :

        private :

        public :

            /*! @brief The constructor.
             @param[in] endpointName The YARP name to be assigned to the new endpoint.
             @param[in] portNumber The port being used by the endpoint.
             @exception Exception Thrown if the endpoint could not be constructed. */
            explicit
            Endpoint(const YarpString & endpointName,
                     const YarpString & portNumber = "");

            /*! @brief The destructor. */
            virtual
            ~Endpoint(void);

            /*! @brief Check the format of an endpoint name.
             @param[in] channelName The name to be checked.
             @return @c true if the name is a valid endpoint name and @c false otherwise. */
            static bool
            CheckEndpointName(const YarpString & channelName);

            /*! @brief Stop processing input. */
            void
            close(void);

            /*! @brief Turn off the send / receive metrics collecting. */
            void
            disableMetrics(void);

            /*! @brief Turn on the send / receive metrics collecting. */
            void
            enableMetrics(void);

            /*! @brief Return the YARP name for the endpoint.
             @return The YARP name for the endpoint. */
            YarpString
            getName(void)
            const;

            /*! @brief Return the send / receive counters.
             @param[out] counters The send / receive counters. */
            void
            getSendReceiveCounters(SendReceiveCounters & counters);

            /*! @brief Return the state of the endpoint.
             @return @c true if the endpoint is open and @c false otherwise. */
            inline bool
            isOpen(void)
            const
            {
                return _isOpen;
            } // isOpen

            /*! @brief Return the state of the  send / receive metrics.
             @return @c true if the send / receive metrics are being gathered and @c false
             otherwise. */
            inline bool
            metricsAreEnabled(void)
            const
            {
                return _metricsEnabled;
            } // metricsAreEnabled

            /*! @brief Open the endpoint if it is not already open.
             @param[in] timeToWait The number of seconds allowed before a failure is considered.
             @return @c true if the endpoint is open and @c false otherwise. */
            bool
            open(const double timeToWait);

            /*! @brief Set the input handler for the endpoint.

             Either an input handler or an input handler creator must be set up before any incoming
             data will be processed and the endpoint cannot be open before set up.
             @param[in] handler The input handler to be used by the endpoint to process incoming
             data.
             @return @c true if the input handler was attached to the endpoint. */
            bool
            setInputHandler(BaseInputHandler & handler);

            /*! @brief Set the input handler creator for the endpoint.

             Either an input handler or an input handler creator must be set up before any incoming
             data will be processed and the endpoint cannot be open before set up.
             @param[in] handlerCreator The input handler creator to be used by the endpoint to
             process incoming data.
             @return @c true if the input handler creator was attached to the endpoint. */
            bool
            setInputHandlerCreator(BaseInputHandlerCreator & handlerCreator);

            /*! @brief Set the channel status reporter for the endpoint.
             @param[in] reporter The channel status reporter to be used by the endpoint.
             @param[in] andReportNow @c true if the channel status reporter is to be activated
             immediately.
             @return @c true if the channel status reporter was attached to the endpoint. */
            bool
            setReporter(ChannelStatusReporter & reporter,
                        const bool              andReportNow = false);

            /*! @brief Update the send counters for the endpoint.
             @param[in] numBytes The number of bytes sent. */
            void
            updateSendCounters(const size_t numBytes);

            /*! @brief Returns information on how this endpoint can be reached.
             @return Connection information for the endpoint. */
            yarp::os::Contact
            where(void)
            const;

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            Endpoint(const Endpoint & other);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            Endpoint &
            operator =(const Endpoint & other);

        public :

        protected :

        private :

            /*! @brief The YARP channel to be used by the endpoint. */
            ServiceChannel * _channel;

            /*! @brief The connection details for the endpoint. */
            yarp::os::Contact _contact;

            /*! @brief The input handler for the endpoint. */
            BaseInputHandler * _handler;

            /*! @brief The input handler creator for the endpoint. */
            BaseInputHandlerCreator * _handlerCreator;

            /*! @brief @c true if metrics are enabled and @c false otherwise. */
            bool _metricsEnabled;

            /*! @brief @c true if the endpoint is open and @c false otherwise. */
            bool _isOpen;

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
            /*! @brief Filler to pad to alignment boundary */
            char _filler[6];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

        }; // Endpoint

    } // Common

} // MplusM

#endif // ! defined(MpMEndpoint_HPP_)
