#ifndef _MONGOOSE_REQUEST_H
#define _MONGOOSE_REQUEST_H

#include <iostream>
#include <sstream>
#include <mongoose.h>
#include "Response.h"
#include "Container.h"

using namespace std;

namespace Mongoose
{
    class Request : public Container
    {
        public:
            Request(struct mg_connection *connection);

            void writeResponse(Response *response);

            bool hasValue(string key);
            string get(string key, string fallback = "");

            bool hasCookie(string key);
            string getCookie(string key, string fallback = "");

            string getUrl();
            string getMethod();
    
            bool readVariable(const char *data, string key, string &output);

        protected:
            string data;
            string method;
            string url;
            struct mg_connection *connection;
            struct mg_request_info *request;
    };
};

#endif
