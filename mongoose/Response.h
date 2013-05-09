#ifndef _MONGOOSE_RESPONSE_H
#define _MONGOOSE_RESPONSE_H

#include <map>
#include <sstream>
#include <iostream>

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_FORBIDDEN 403

using namespace std;

namespace Mongoose
{
    class Response : public ostringstream
    {
        public:
            Response();

            void setHeader(string key, string value);
            string getData();

        protected:
            int code;
            map<string, string> headers;
    };
};

#endif
