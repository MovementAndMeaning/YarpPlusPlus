//--------------------------------------------------------------------------------------------------
//
//  File:       m+mLeapServiceThread.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for an output service thread for the Leap Motion display
//              output service.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2016 by OpenDragon.
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
//  Created:    2016-05-14
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMLeapServiceThread_HPP_))
# define MpMLeapServiceThread_HPP_ /* Header guard */

# include "m+mLeapDisplayDataTypes.hpp"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for an output service thread for the %Leap Motion display output
 service */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace LeapDisplay
{
    class LeapDisplayOutputService;
    
    /*! @brief A convenience class to run an input / output service in a thread. */
    class LeapServiceThread : public Thread
    {
    public :
    
    protected :
    
    private :
        
        /*! @brief The class that this class is derived from. */
        typedef Thread inherited;
        
    public :
        
        /*! @brief The constructor.
         @param[in] service The service associated with the request.
         @param[in] helpText The help text to be displayed. */
        LeapServiceThread(LeapDisplayOutputService * service,
                          const YarpString &         helpText);
        
        /*! @brief The destructor. */
        virtual
        ~LeapServiceThread(void);
        
        /*! @brief Return the associated service.
         @return The associated service. */
        LeapDisplayOutputService *
        getService(void)
        const
        {
            return _service;
        } // getService

        /*! @brief Set the associated service.
         @param[in] aService The service to associate with the thread. */
        void
        setService(LeapDisplayOutputService * aService);
        
    protected :
    
    private :
        
        /*! @brief The copy constructor.
         @param[in] other The object to be copied. */
        LeapServiceThread(const LeapServiceThread & other);
        
        /*! @brief The assignment operator.
         @param[in] other The object to be copied.
         @return The updated object. */
        LeapServiceThread &
        operator =(const LeapServiceThread & other);
        
        /*! @brief The thread main body. */
        virtual void
        run(void);
        
    public :
    
    protected :
    
    private :
        
        /*! @brief The service that is associated with the thread. */
        ScopedPointer<LeapDisplayOutputService> _service;
        
        /*! @brief The help text for the service. */
        YarpString _helpText;
        
    }; // LeapServiceThread
    
} // LeapDisplay

#endif // ! defined(MpMLeapServiceThread_HPP_)
