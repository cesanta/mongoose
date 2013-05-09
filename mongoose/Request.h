#ifndef _MONGOOSE_REQUEST_H
#define _MONGOOSE_REQUEST_H

#include <iostream>
#include <sstream>
#include <mongoose.h>
#include "Response.h"
#include "Container.h"

using namespace std;

/**
 * Request is a wrapper for the clients requests
 */
namespace Mongoose
{
    class Request : public Container
    {
        public:
            Request(struct mg_connection *connection);

            /**
             * Sends a given response to the client
             *
             * @param Response a response for this request
             */
            void writeResponse(Response *response);

            /**
             * Check if the variable given by key is present in GET or POST data
             *
             * @param string the name of the variable
             *
             * @return bool true if the param is present, false else
             */
            bool hasVariable(string key);

            /**
             * Get the value for a certain variable
             *
             * @param string the name of the variable
             * @param string the fallback value if the variable doesn't exists
             *
             * @return string the value of the variable if it exists, fallback else
             */
            string get(string key, string fallback = "");

            /**
             * Checks if the given cookie exists
             *
             * @param string the name of the cookie
             *
             * @return bool true if the given cookie is set
             */
            bool hasCookie(string key);

            /**
             * Try to get the cookie value
             *
             * @param string the name of the cookie
             * @param string the fallback value
             *
             * @retun the value of the cookie if it exists, fallback else
             */
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
