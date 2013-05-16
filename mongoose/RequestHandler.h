#ifndef _MONGOOSE_REQUEST_HANDLER_H
#define _MONGOOSE_REQUEST_HANDLER_H

#include "Request.h"
#include "Response.h"
#include "StreamResponse.h"

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
                    delete response;
                    StreamResponse *errorResponse = new StreamResponse;
                    errorResponse->setCode(HTTP_SERVER_ERROR);
                    *errorResponse << "Server internal error: " << exception;
                    return errorResponse;
                } catch (...) {
                    delete response;
                    StreamResponse *errorResponse = new StreamResponse;
                    errorResponse->setCode(HTTP_SERVER_ERROR);
                    *errorResponse << "Unknown server internal error";
                    return errorResponse;
                }

                return response;
            }

        protected:
            T *controller;
            fPtr function;
    };
};

#endif
