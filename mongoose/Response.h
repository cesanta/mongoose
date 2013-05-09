#ifndef _MONGOOSE_RESPONSE_H
#define _MONGOOSE_RESPONSE_H

#include <map>
#include <sstream>
#include <iostream>

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_FORBIDDEN 403

using namespace std;

/**
 * A response to a request
 */
namespace Mongoose
{
    class Response : public ostringstream
    {
        public:
            Response();

            /**
             * Test if the given header is present
             *
             * @param string the header key
             *
             * @return bool true if the header is set
             */
            bool hasHeader(string key);

            /**
             * Sets the header
             *
             * @param key the header key
             *
             * @param value the header value
             */
            void setHeader(string key, string value);

            /**
             * Get the data of the response, this will contain headers and
             * body
             *
             * @return string the response data
             */
            string getData();

            /**
             * Sets the cookie, note that you can only define one cookie by request
             * for now
             *
             * @param string the key of the cookie
             * @param string value the cookie value
             */
            void setCookie(string key, string value);

        protected:
            int code;
            map<string, string> headers;
    };
};

#endif
