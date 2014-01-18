#include <iostream>
#include "Controller.h"
#include "StreamResponse.h"

using namespace std;

namespace Mongoose
{
    Controller::Controller() 
        : prefix(""), server(NULL)
    {
    }

    void Controller::setup()
    {
    }

    void Controller::setServer(Server *server_)
    {
        server = server_;
    }

    void Controller::webSocketReady(WebSocket *websocket)
    {
    }

    void Controller::webSocketData(WebSocket *websocket, string data)
    {
    }
    
    Controller::~Controller()
    {
        map<string, RequestHandlerBase *>::iterator it;

        for (it=routes.begin(); it!=routes.end(); it++) {
            delete (*it).second;
        }

        routes.clear();
    }
            
    bool Controller::handles(string method, string url)
    { 
        string key = method + ":" + url;

        return (routes.find(key) != routes.end());
    }

    Response *Controller::process(Request &request)
    {
        Response *response = NULL;

#ifdef ENABLE_REGEX_URL
        map<string, RequestHandlerBase *>::iterator it; 
        for (it=routes.begin(); it!=routes.end(); it++) {
            if (request.match(it->first)){
              response = it->second->process(request);
              break;
            }   
        }   
#else
        string key = request.getMethod() + ":" + request.getUrl();
        if (routes.find(key) != routes.end()) {
            response = routes[key]->process(request);
        }
#endif
        
        return response;
    }
            
    void Controller::preProcess(Request &request, Response &response)
    {
    }
            
    void Controller::postProcess(Request &request, Response &response)
    {
    }

    Response *Controller::handleRequest(Request &request)
    {
        Response *response = process(request);

        if (response != NULL) {
            postProcess(request, *response);
        }

        return response;
    }

    void Controller::setPrefix(string prefix_)
    {
        prefix = prefix_;
    }
            
    void Controller::registerRoute(string httpMethod, string route, RequestHandlerBase *handler)
    {
        string key = httpMethod + ":" + prefix + route;
        routes[key] = handler;
        urls.push_back(prefix + route);
    }

    void Controller::dumpRoutes()
    {
        map<string, RequestHandlerBase *>::iterator it;

        for (it=routes.begin(); it!=routes.end(); it++) {
            cout << (*it).first << endl;
        }

    }

    Response *Controller::serverInternalError(string message)
    {
        StreamResponse *response = new StreamResponse;

        response->setCode(HTTP_SERVER_ERROR);
        *response << "[500] Server internal error: " << message;

        return response;
    }

    vector<string> Controller::getUrls()
    {
        return urls;
    }
};
