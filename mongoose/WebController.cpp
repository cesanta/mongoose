#include "WebController.h"

namespace Mongoose
{        
    void WebController::preProcess(Request &request, Response &response)
    {
        response.setHeader("Content-type", "text/html");
    }
};
