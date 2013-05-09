#ifndef _MONGOOSE_SESSIONS_H
#define _MONGOOSE_SESSIONS_H

#include "Request.h"
#include "Container.h"
#include "Session.h"

using namespace std;

/**
 * A session contains the user specific values
 */ 
namespace Mongoose
{
    class Sessions
    {
        public:
            Session &get(Request &request);

        protected:
            map<string, Session> sessions;
    };
};

#endif
