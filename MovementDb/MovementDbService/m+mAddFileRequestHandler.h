//--------------------------------------------------------------------------------------------------
//
//  File:       m+mAddFileRequestHandler.h
//
//  Project:    m+m
//
//  Contains:   The class declaration for the request handler for an 'addfile' request.
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
//  Created:    2014-09-02
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMAddFileRequestHandler_H_))
# define MpMAddFileRequestHandler_H_ /* Header guard */

# include <m+m/m+mBaseRequestHandler.h>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the request handler for an 'addfile' request. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace MovementDb
    {
        class MovementDbService;
        
        /*! @brief The 'addfile' request handler for the movement database service.
         
         The inputs for the request is the file-system path to the file; there is no output for the
         request. */
        class AddFileRequestHandler : public Common::BaseRequestHandler
        {
        public :
        
        protected :
        
        private :
            
            /*! @brief The class that this class is derived from. */
            typedef BaseRequestHandler inherited;
            
        public :
            
            /*! @brief The constructor.
             @param service The service that has registered this request. */
            explicit
            AddFileRequestHandler(MovementDbService & service);
            
            /*! @brief The destructor. */
            virtual
            ~AddFileRequestHandler(void);
            
        protected :
            
        private :
            
            DECLARE_FILLINALIASES_;
            
            DECLARE_FILLINDESCRIPTION_;
            
            DECLARE_PROCESSREQUEST_;
            
            COPY_AND_ASSIGNMENT_(AddFileRequestHandler);
            
        public :
        
        protected :
        
        private :
            
        }; // AddFileRequestHandler
        
    } // MovementDb
    
} // MplusM

#endif // ! defined(MpMAddFileRequestHandler_H_)
