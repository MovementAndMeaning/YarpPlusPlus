//
//  YPPMatcher.cpp
//  YarpPlusPlus
//
//  Created by Norman Jaffe on 2014-03-07.
//  Copyright (c) 2014 OpenDragon. All rights reserved.
//

#include "YPPMatcher.h"
#define ENABLE_OD_SYSLOG /* */
#include "ODSyslog.h"
#include <cctype>

using namespace YarpPlusPlusParser;

#if defined(__APPLE__)
# pragma mark Private structures and constants
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

int Matcher::skipWhitespace(const yarp::os::ConstString & inString,
                            const int                     inLength,
                            const int                     startPos)
{
    int result = startPos;
    
    for ( ; result < inLength; ++result)
    {
        char scanChar = inString[result];
        
        if (! isspace(scanChar))
        {
            break;
        }
        
    }
    return result;
} // Matcher::skipWhitespace

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

Matcher::Matcher(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT_P(this);//####
} // Matcher::Matcher

Matcher::~Matcher(void)
{
    OD_SYSLOG_ENTER();//####
    OD_SYSLOG_EXIT();//####
} // Matcher::~Matcher

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
