//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mMatchFieldName.h
//
//  Project:    m+m
//
//  Contains:   The class declaration for a pattern matcher for field names.
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
//  Created:    2014-03-07
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMMatchFieldName_H_))
# define MpMMatchFieldName_H_ /* Header guard */

# include <m+m/m+mBaseMatcher.h>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for a pattern matcher for field names. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Parser
    {
        class BaseNameValidator;
        
        /*! @brief A pattern matcher for field names. */
        class MatchFieldName : public BaseMatcher
        {
        public :
        
        protected :
        
        private :
            
            /*! @brief The class that this class is derived from. */
            typedef BaseMatcher inherited;
            
        public :
            
            /*! @brief The destructor. */
            virtual
            ~MatchFieldName(void);
            
            /*! @brief Generate a proper SQL string value corresponding to this match value.
             @returns A string representing the value as a string suitable for use with SQL. */
            inline const YarpString &
            asSQLString(void)
            const
            {
                return _matchingString;
            } // asSQLString
            
            /*! @brief Return the match value.
             @returns The matching substring. */
            inline const YarpString &
            asString(void)
            const
            {
                return _matchingString;
            } // asString
            
            /*! @brief Create a pattern matcher if the next substring would be a valid value.
             @param inString The string being scanned.
             @param inLength The length of the string being scanned.
             @param startPos Where in the string to start scanning.
             @param endPos Where the scan terminated, if successful.
             @param validator A function that returns @c true if the field name is valid and @c
             false otherwise.
             @returns A non-null matcher if the string would be a valid value and @c NULL
             otherwise. */
            static MatchFieldName *
            CreateMatcher(const YarpString &  inString,
                          const size_t        inLength,
                          const size_t        startPos,
                          size_t &            endPos,
                          BaseNameValidator * validator = NULL);
            
            /*! @brief Return whether or not the field name was followed by the negation character.
             @returns @c true if the field name was followed by the negation character and @c false
             otherwise. */
            inline bool
            isNegated(void)
            const
            {
                return _isNegated;
            } // isNegated
            
        protected :
            
        private :
            
            /*! @brief The constructor.
             @param inString The matching segment of the original string.
             @param negationSeen @c true if the field name was followed by the negation character
             and @c false otherwise. */
            MatchFieldName(const YarpString & inString,
                           const bool         negationSeen);
            
            COPY_AND_ASSIGNMENT_(MatchFieldName);
            
        public :
        
        protected :
        
        private :
            
            /*! @brief The subtring that (maximally) matched as a value. */
            YarpString _matchingString;
            
            /*! @brief The field name was followed by the negation character (an exclamation
             mark). */
            bool _isNegated;
            
# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
            /*! @brief Filler to pad to alignment boundary */
            char _filler[7];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)
            
        }; // MatchFieldName
        
    } // Parser
    
} // MplusM

#endif // ! defined(MpMMatchFieldName_H_)
