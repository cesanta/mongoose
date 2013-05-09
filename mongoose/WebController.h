#ifndef _MONGOOSE_WEB_CONTROLLER_H
#define _MONGOOSE_WEB_CONTROLLER_H

#include "Request.h"
#include "Response.h"
#include "Controller.h"

using namespace std;

namespace Mongoose
{
    class WebController : public Controller
    {
        public:
            void preProcess(Request &request, Response &response);
    };
};

#endif
