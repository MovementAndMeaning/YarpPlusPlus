//--------------------------------------------------------------------------------------
//
//  File:       M+MRequests.h
//
//  Project:    M+M
//
//  Contains:   The common macro definitions for requests and responses.
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
//  Created:    2014-02-25
//
//--------------------------------------------------------------------------------------

#if (! defined(MpMRequests_H_))
/*! @brief Header guard. */
# define MpMRequests_H_ /* */

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The common macro definitions for requests and responses. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The standard name for the service registry channel. */
# define MpM_REGISTRY_CHANNEL_NAME    "/$ervice"

/*! @brief The channel-independent name of the service registry service. */
# define MpM_REGISTRY_CANONICAL_NAME "Registry"

/*! @brief The standard name for a 'channels' request. */
# define MpM_CHANNELS_REQUEST   "channels"
/*! @brief The name for the 'clients' request. */
# define MpM_CLIENTS_REQUEST    "clients"
/*! @brief The name for the 'count' request. */
# define MpM_COUNT_REQUEST      "count"
/*! @brief The name for the 'detach' request. */
# define MpM_DETACH_REQUEST     "detach"
/*! @brief The name for the 'echo' request. */
# define MpM_ECHO_REQUEST       "echo"
/*! @brief The standard name for an 'info' request. */
# define MpM_INFO_REQUEST       "info"
/*! @brief The standard name for a 'list' request. */
# define MpM_LIST_REQUEST       "list"
/*! @brief The standard name for a 'match' request. */
# define MpM_MATCH_REQUEST      "match"
/*! @brief The standard name for a 'name' request. */
# define MpM_NAME_REQUEST       "name"
/*! @brief The standard name for a 'register' request. */
# define MpM_REGISTER_REQUEST   "register"
/*! @brief The standard name for an 'unregister' request. */
# define MpM_UNREGISTER_REQUEST "unregister"

/*! @brief The number of elements expected in the output of a 'match' request. */
# define MpM_EXPECTED_MATCH_RESPONSE_SIZE 2
/*! @brief The number of elements expected in the output of a 'name' request. */
# define MpM_EXPECTED_NAME_RESPONSE_SIZE  3

/*! @brief The standard response to an invalid 'register' / 'unregister' request. */
# define MpM_FAILED_RESPONSE "FAILED"
/*! @brief The standard response to a valid 'register' / 'unregister' request. */
# define MpM_OK_RESPONSE     "OK"

/*! @brief A service is being added to the registry. */
# define MpM_REGISTRY_STATUS_ADDING   "adding"
/*! @brief A service is being removed from the registry. */
# define MpM_REGISTRY_STATUS_REMOVING "removing"
/*! @brief The registry has just started. */
# define MpM_REGISTRY_STATUS_STARTING "starting"
/*! @brief The registry is stopping. */
# define MpM_REGISTRY_STATUS_STOPPING "stopping"
/*! @brief The registry status is unknown. */
# define MpM_REGISTRY_STATUS_UNKNOWN  "<unknown>"

/*! @brief Request/response specification character - zero or one repetitions of preceding. */
# define MpM_REQREP_0_OR_1     "?"
/*! @brief Request/response specification character - zero or more repetitions of preceding. */
# define MpM_REQREP_0_OR_MORE  "*"
/*! @brief Request/response specification character - one or more repetitions of preceding. */
# define MpM_REQREP_1_OR_MORE  "+"
/*! @brief Request/response specification character - any element. */
# define MpM_REQREP_ANYTHING   "."
/*! @brief Request/response specification character - end of dictionary specification. */
# define MpM_REQREP_DICT_END   "]"
/*! @brief Request/response specification character - key/value separator for dictionary. */
# define MpM_REQREP_DICT_SEP   ":"
/*! @brief Request/response specification character - start of dictionary specification. */
# define MpM_REQREP_DICT_START "["
/*! @brief Request/response specification character - double value. */
# define MpM_REQREP_DOUBLE     "d"
/*! @brief Request/response specification character - integer value. */
# define MpM_REQREP_INT        "i"
/*! @brief Request/response specification character - end of list specification. */
# define MpM_REQREP_LIST_END   ")"
/*! @brief Request/response specification character - start of list specification. */
# define MpM_REQREP_LIST_START "("
/*! @brief Request/response specification character - numeric value (double or integer). */
# define MpM_REQREP_NUMBER     "n"
/*! @brief Request/response specification character - string value. */
# define MpM_REQREP_STRING     "s"

/*! @brief Request/response dictionary key - channel name. */
# define MpM_REQREP_DICT_CHANNELNAME_KEY "channelname"
/*! @brief Request/response dictionary key - details. */
# define MpM_REQREP_DICT_DETAILS_KEY     "details"
/*! @brief Request/response dictionary key - input specification. */
# define MpM_REQREP_DICT_INPUT_KEY       "input"
/*! @brief Request/response dictionary key - keywords. */
# define MpM_REQREP_DICT_KEYWORDS_KEY    "keywords"
/*! @brief Request/response dictionary key - output specification. */
# define MpM_REQREP_DICT_OUTPUT_KEY      "output"
/*! @brief Request/response dictionary key - request specification. */
# define MpM_REQREP_DICT_REQUEST_KEY     "request"
/*! @brief Request/response dictionary key - version. */
# define MpM_REQREP_DICT_VERSION_KEY     "version"

#endif // ! defined(MpMRequests_H_)
