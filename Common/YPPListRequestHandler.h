//
//  YPPListRequestHandler.h
//  YarpPlusPlus
//
//  Created by Norman Jaffe on 2014-02-26.
//  Copyright (c) 2014 OpenDragon. All rights reserved.
//

#if (! defined(YPPLISTREQUESTHANDLER_H_))
# define YPPLISTREQUESTHANDLER_H_ /* */

# include "YPPRequestHandler.h"

namespace YarpPlusPlus
{
    /*! @brief The 'list' request handler. */
    class ListRequestHandler : public RequestHandler
    {
    public:
        
        /*! @brief The constructor.
         @param service The service that responds to this request. */
        ListRequestHandler(BaseService & service);
        
        /*! @brief The destructor. */
        virtual ~ListRequestHandler(void);
        
        /*! @brief Fill in a description dictionary for the request.
         @param info The dictionary to be filled in. */
        virtual void fillInDescription(yarp::os::Property & info);
        
        /*! @brief Process a request.
         @param restOfInput The arguments to the operation.
         @param replyMechanism non-@c NULL if a reply is expected and @c NULL otherwise. */
        virtual bool operator() (const yarp::os::Bottle &     restOfInput,
                                 yarp::os::ConnectionWriter * replyMechanism);
        
    protected:
        
    private:
        
        typedef RequestHandler inherited;
        
        /*! @brief Copy constructor.
         
         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        ListRequestHandler(const ListRequestHandler & other);
        
        /*! @brief Assignment operator.
         
         Note - not implemented and private, to prevent unexpected copying.
         @param other Another object to construct from. */
        ListRequestHandler & operator=(const ListRequestHandler & other);
        
    }; // ListRequestHandler
    
} // YarpPlusPlus

#endif // ! defined(YPPLISTREQUESTHANDLER_H_)
