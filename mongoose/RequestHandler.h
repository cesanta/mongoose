#ifndef _MONGOOSE_REQUEST_HANDLER_H
#define _MONGOOSE_REQUEST_HANDLER_H

#include "Request.h"
#include "Response.h"
#include <string>

namespace Mongoose
{
    class RequestHandlerBase
    {
        public:
            virtual Response *process(Request &request)=0;
    };

    template<typename T, typename R>
    class RequestHandler : public RequestHandlerBase
    {
        public:
            typedef void (T::*fPtr)(Request &request, R &response);

            RequestHandler(T *controller_, fPtr function_)
                : controller(controller_), function(function_)
            {
            }

            Response *process(Request &request)
            {
                R *response = new R;

                try {
                    controller->preProcess(request, *response);
                    (controller->*function)(request, *response);
                } catch (string exception) {
                    return controller->serverInternalError(exception);
                } catch (...) {
                    return controller->serverInternalError("Unknown error");
                }

                return response;
            }

        protected:
            T *controller;
            fPtr function;
    };
}

#endif
