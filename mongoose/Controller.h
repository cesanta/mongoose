#ifndef _MONGOOSE_CONTROLLER_H
#define _MONGOOSE_CONTROLLER_H

#include <iostream>
#include "Request.h"
#include "Response.h"

using namespace std;

/**
 * Those macros can be used to do some tests ont he request and route it to
 * the appropriate controller method, for instance, put in your process :
 *
 * MG_GET("/hello", hello)
 *
 * Will call hello() with request and a new response if the request is using the
 * GET method and its url is "/hello"
 */
#define MG_GET(url, method) \
    if (request.getMethod() == "GET" && request.getUrl() == url) { \
        response = createResponse(request); \
        method (request, *response); \
    } 

#define MG_POST(url, method) \
    if (request.getMethod() == "POST" && request.getUrl() == url) { \
        response = createResponse(request); \
        method (request, *response); \
    } 

/**
 * A controller is a module that respond to requests
 * 
 * You can override the preProcess, process and postProcess to answer to
 * the requests
 */
namespace Mongoose
{
    class Controller
    {
        public:
            Controller();

            /**
             * Called before a request is processed
             *
             * @param Request the request
             * @param Response the response
             */
            virtual void preProcess(Request &request, Response &response);

            /**
             * Called to process a request
             *
             * @param Request the request
             *
             * @return Response the created response, or NULL if the controller 
             *         does not handle this request
             */
            virtual Response *process(Request &request);
            
            /**
             * Called after a request is processed, if the controller responded
             *
             * @param Request the request
             * @param Response the response
             */
            virtual void postProcess(Request &request, Response &response);

            /**
             * Handle a request, this will try to match the request, if this
             * controller handles it, it will preProcess, process then postProcess it
             *
             * @param Request the request
             *
             * @return Response the created response, or NULL if the controller
             *         does not handle this request
             */
            virtual Response *handleRequest(Request &request);

            /**
             * Creates a new response and pre process it
             *
             * @return the created response
             */
            virtual Response *createResponse(Request &request);
    };
};

#endif
