//--------------------------------------------------------------------------------------------------
//
//  File:       m+mRandomNumberAdapterData.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the data shared between the input handlers and main thread
//              of the Random Number adapter.
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
//  Created:    2015-05-27
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMRandomNumberAdapterData_HPP_))
# define MpMRandomNumberAdapterData_HPP_ /* Header guard */

# include <m+m/m+mBaseAdapterData.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the data shared between the input handlers and main thread of the
 Random Number adapter. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Example
    {
        class RandomNumberClient;

        /*! @brief The data shared between the input handlers and the main thread of
         the Random Number adapter. */
        class RandomNumberAdapterData : public Common::BaseAdapterData
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef BaseAdapterData inherited;

        public :

            /*! @brief The constructor.
             @param[in] client The client connection that is used to communicate with the service.
             @param[in] output The output channel that will receive the service responses. */
            explicit
            RandomNumberAdapterData(RandomNumberClient *  client = NULL,
                                    Common::BaseChannel * output = NULL);

            /*! @brief The destructor. */
            virtual
            ~RandomNumberAdapterData(void);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            RandomNumberAdapterData(const RandomNumberAdapterData & other);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @returns The updated object. */
            RandomNumberAdapterData &
            operator =(const RandomNumberAdapterData & other);

        public :

        protected :

        private :

        }; // RandomNumberAdapterData

    } // Example

} // MplusM

#endif // ! defined(MpMRandomNumberAdapterData_HPP_)