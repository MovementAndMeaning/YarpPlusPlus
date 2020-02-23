//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mMatchValueList.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for a pattern matcher for lists of simple values.
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

#if (! defined(MpMMatchValueList_HPP_))
# define MpMMatchValueList_HPP_ /* Header guard */

# include <m+m/m+mBaseMatcher.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for a pattern matcher for lists of simple values. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Parser
    {
        class MatchValue;

        /*! @brief A pattern matcher for lists of simple values. */
        class MatchValueList : public BaseMatcher
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef BaseMatcher inherited;

            /*! @brief A sequence of values. */
            typedef std::vector<MatchValue *> MatchValueListData;

            /*! @brief The size-type for sequence data. */
            typedef MatchValueListData::size_type MatchValueListSize;

        public :

            /*! @brief The destructor. */
            virtual
            ~MatchValueList(void);

            /*! @brief Generate a proper SQL string value corresponding to this match value.
             @param[in] fieldName The name to be used in the SQL matching expression.
             @param[in] negated @c true if the matching is negated and @c false otherwise.
             @return A string representing the value as a string suitable for use with SQL. */
            YarpString
            asSQLString(const char * fieldName,
                        const bool   negated)
            const;

            /*! @brief Create a printable representation of the value list.
             @return A printable representation of the value list. */
            const YarpString
            asString(void)
            const;

            /*! @brief Return the number of elements in the value list.
             @return The number of elements in the value list. */
            int
            count(void)
            const;

            /*! @brief Create a pattern matcher if the next substring would be a valid value list.
             @param[in] inString The string being scanned.
             @param[in] inLength The length of the string being scanned.
             @param[in] startPos Where in the string to start scanning.
             @param[in,out] endPos Where the scan terminated, if successful.
             @return A non-null matcher if the string would be a valid value and @c NULL
             otherwise. */
            static MatchValueList *
            CreateMatcher(const YarpString & inString,
                          const size_t       inLength,
                          const size_t       startPos,
                          size_t &           endPos);

            /*! @brief Return an element from the value list.
             @param[in] index The zero-origin index of the element.
             @return An element of the value list or @c NULL if the index is outside the range of
             the value list. */
            const MatchValue *
            element(const int index)
            const;

            /*! @brief The character used to signal the beginning of a value list.
             @return The character that ends a value list. */
            static char
            ListInitiatorCharacter(void);

            /*! @brief The character used between value list elements.
             @return The character that separates value list elements. */
            static char
            ListSeparatorCharacter(void);

            /*! @brief The character used to signal the end of a value list.
             @return The character that ends a value list. */
            static char
            ListTerminatorCharacter(void);

        protected :

            /*! @brief The constructor. */
            MatchValueList(void);

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            MatchValueList(const MatchValueList & other);

            /*! @brief Remove all the list elements. */
            void
            empty(void);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            MatchValueList &
            operator =(const MatchValueList & other);

        public :

        protected :

        private :

            /*! @brief The elements of the list. */
            MatchValueListData _values;

        }; // MatchValueList

    } // Parser

} // MplusM

#endif // ! defined(MpMMatchValueList_HPP_)
