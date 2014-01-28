#ifndef _MONGOOSE_STREAM_RESPONSE_H
#define _MONGOOSE_STREAM_RESPONSE_H

#include <map>
#include <sstream>
#include <iostream>

#include "Response.h"

using namespace std;

/**
 * A stream response to a request
 */
namespace Mongoose
{
    class StreamResponse : public ostringstream, public Response
    {
        public:
            /**
             * Gets the response body
             *
             * @return string the response body
             */
            virtual string getBody();
    };
}

#endif
