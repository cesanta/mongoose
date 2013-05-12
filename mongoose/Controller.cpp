#include <iostream>
#include "Controller.h"
#include "StreamResponse.h"

using namespace std;

namespace Mongoose
{
    Controller::Controller() : prefix("")
    {
    }

    Response *Controller::process(Request &request)
    {
        return NULL;
    }
            
    void Controller::preProcess(Request &request, Response *response)
    {
    }
            
    void Controller::postProcess(Request &request, Response *response)
    {
    }
            
    StreamResponse *Controller::createResponse(Request &request)
    {
        StreamResponse *response = new StreamResponse();
        preProcess(request, response);

        return response;
    }

    Response *Controller::handleRequest(Request &request)
    {
        Response *response = process(request);

        if (response != NULL) {
            postProcess(request, response);
        }

        return response;
    }

    void Controller::setPrefix(string prefix_)
    {
        prefix = prefix_;
    }
};
