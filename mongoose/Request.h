#ifndef _MONGOOSE_REQUEST_H
#define _MONGOOSE_REQUEST_H

#include <iostream>
#include <sstream>
#include <mongoose.h>
#include "Response.h"

using namespace std;

namespace Mongoose
{
    class Request
    {
        public:
            Request(struct mg_connection *connection);

            void writeResponse(Response *response);

            string getUrl();
            string getMethod();

        protected:
            string method;
            string url;
            struct mg_connection *connection;
            struct mg_request_info *request;
    };
};

#endif
