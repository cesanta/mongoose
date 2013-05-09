#ifndef _MONGOOSE_WEB_CONTROLLER_H
#define _MONGOOSE_WEB_CONTROLLER_H

#include "Request.h"
#include "Response.h"
#include "Controller.h"
#include "Sessions.h"

using namespace std;

namespace Mongoose
{
    class WebController : public Controller
    {
        public:
            WebController(int gcDivisor = 100);

            Session &getSession(Request &request, Response &response);

            void preProcess(Request &request, Response &response);

        protected:
            Sessions sessions;
            int gcDivisor;
            int counter;
    };
};

#endif
