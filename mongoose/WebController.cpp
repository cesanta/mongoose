#include "WebController.h"
#include "Session.h"

namespace Mongoose
{        
    WebController::WebController(int gcDivisor_) :
        gcDivisor(gcDivisor_), counter(0), Controller()
    {
    }

    void WebController::preProcess(Request &request, Response &response)
    {
        mutex.lock();
        counter++;

        if (counter > gcDivisor) {
            counter = 0;
            sessions.garbageCollect();
        }
        mutex.unlock();

        Session &session = sessions.get(request, response);
        session.ping();
        response.setHeader("Content-type", "text/html");
    }

    Session &WebController::getSession(Request &request, Response &response)
    {
        return sessions.get(request, response);
    }
};
