#include "JsonController.h"
#include "Session.h"

namespace Mongoose
{        
    JsonController::JsonController(int gcDivisor_) :
        WebController(gcDivisor_)
    {
    }

    void JsonController::preProcess(Request &request, Response &response)
    {
        WebController::preProcess(request, response);

        // RFC 4627
        // Json content type is application/json
        response.setHeader("Content-type", "application/json");
    }            
};
