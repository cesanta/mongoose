#include <iostream>
#include "Controller.h"

using namespace std;

namespace Mongoose
{
    Controller::Controller()
    {
    }

    Response *Controller::process(Request &request)
    {
        return NULL;
    }
            
    void Controller::preProcess(Request &request, Response &response)
    {
    }
            
    void Controller::postProcess(Request &request, Response &response)
    {
    }
            
    Response *Controller::createResponse(Request &request)
    {
        Response *response = new Response;
        preProcess(request, *response);

        return response;
    }

    Response *Controller::handleRequest(Request &request)
    {
        Response *response = process(request);

        if (response != NULL) {
            postProcess(request, *response);
        }

        return response;
    }
};
