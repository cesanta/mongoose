#ifndef _MONGOOSE_WEB_CONTROLLER_H
#define _MONGOOSE_WEB_CONTROLLER_H

#include "Request.h"
#include "Response.h"
#include "Controller.h"
#include "Sessions.h"
#include "Mutex.h"
#include "StreamResponse.h"
#include "Utils.h"

using namespace std;

/**
 * A web controller is a controller that serves HTML pages and has sessions
 */
namespace Mongoose
{
    class WebController : public Controller, public Utils
    {
        public:
            /**
             * Creates a web controller, each gcDivisor request, the sessions will be
             * garbage collected
             */
            WebController(int gcDivisor = 100);

            /**
             * Gets the session for a request/response
             *
             * @param Request the request
             * @param Response the response
             *
             * @return Session the session for the request/response
             */
            Session &getSession(Request &request, Response &response);

            /**
             * Pre process the request, this will set the content type to text/html
             * and ping the user session
             *
             * @param Request the request
             * @param Response the response
             */
            void preProcess(Request &request, Response &response);

        protected:
            Sessions sessions;
            int gcDivisor;
            int counter;
            Mutex mutex;
    };
}

#endif
