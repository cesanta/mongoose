#ifndef _MONGOOSE_CONTROLLER_H
#define _MONGOOSE_CONTROLLER_H

#include <iostream>
#include "Request.h"
#include "Response.h"

using namespace std;

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

namespace Mongoose
{
    class Controller
    {
        public:
            Controller();

            virtual void preProcess(Request &request, Response &response);
            virtual Response *process(Request &request);
            virtual void postProcess(Request &request, Response &response);

            virtual Response *handleRequest(Request &request);

            virtual Response *createResponse(Request &request);
    };
};

#endif
