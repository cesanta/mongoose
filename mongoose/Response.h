#ifndef _MONGOOSE_RESPONSE_H
#define _MONGOOSE_RESPONSE_H

#include <map>
#include <sstream>
#include <iostream>

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_FORBIDDEN 403
#define HTTP_SERVER_ERROR 500

using namespace std;

/**
 * A response to a request
 */
namespace Mongoose
{
    class Response 
    {
        public:
            Response();
            virtual ~Response();

            /**
             * Test if the given header is present
             *
             * @param string the header key
             *
             * @return bool true if the header is set
             */
            virtual bool hasHeader(string key);

            /**
             * Sets the header
             *
             * @param key the header key
             *
             * @param value the header value
             */
            virtual void setHeader(string key, string value);

            /**
             * Get the data of the response, this will contain headers and
             * body
             *
             * @return string the response data
             */
            virtual string getData();

            /**
             * Gets the response body
             *
             * @return string the response body
             */
            virtual string getBody()=0;

            /**
             * Sets the cookie, note that you can only define one cookie by request
             * for now
             *
             * @param string the key of the cookie
             * @param string value the cookie value
             */
            virtual void setCookie(string key, string value);

            /**
             * Sets the response code
             */
            virtual void setCode(int code);

        protected:
            int code;
            map<string, string> headers;
    };
}

#endif
