#ifndef _MONGOOSE_CONTROLLER_H
#define _MONGOOSE_CONTROLLER_H

#include <map>
#include <iostream>
#include "Request.h"
#include "RequestHandler.h"
#include "StreamResponse.h"
#include "WebSocket.h"

using namespace std;

#define addRoute(httpMethod, url, controllerType, method) \
    registerRoute(httpMethod, url, new RequestHandler<controllerType, StreamResponse>(this, &controllerType::method ));

#define addRouteResponse(httpMethod, url, controllerType, method, responseType) \
    registerRoute(httpMethod, url, new RequestHandler<controllerType, responseType>(this, &controllerType::method ));

/**
 * A controller is a module that respond to requests
 * 
 * You can override the preProcess, process and postProcess to answer to
 * the requests
 */
namespace Mongoose
{
    class Server;

    class Controller
    {
        public:
            Controller();
            virtual ~Controller();

            /**
             * Sets the reference to the server hosting this controller
             *
             * @param Server the hosting server
             */
            virtual void setServer(Server *server);

            /**
             * Called before a request is processed
             *
             * @param Request the request
             * @param Response the response
             */
            virtual void preProcess(Request &request, Response &response);

            /**
             * Called to process a request
             *
             * @param Request the request
             *
             * @return Response the created response, or NULL if the controller 
             *         does not handle this request
             */
            virtual Response *process(Request &request);
            
            /**
             * Called after a request is processed, if the controller responded
             *
             * @param Request the request
             * @param Response the response
             */
            virtual void postProcess(Request &request, Response &response);

            /**
             * Handle a request, this will try to match the request, if this
             * controller handles it, it will preProcess, process then postProcess it
             *
             * @param Request the request
             *
             * @return Response the created response, or NULL if the controller
             *         does not handle this request
             */
            virtual Response *handleRequest(Request &request);

            /**
             * Sets the controller prefix, for instance "/api"
             *
             * @param string the prefix of all urls for this controller
             */
            void setPrefix(string prefix);

            /**
             * Called when a new websocket connection is ready
             *
             * @param WebSocket the instance of the connection 
             */
            virtual void webSocketReady(WebSocket *websocket);

            /**
             * Called when data arrive in a websocket connection
             *
             * @param WebSocket the instance of the connection 
             * @param string the data arriving
             */
            virtual void webSocketData(WebSocket *websocket, string data);

            /**
             * Registers a route to the controller
             *
             * @param string the route path
             * @param RequestHandlerBase the request handler for this route
             */
            virtual void registerRoute(string httpMethod, string route, RequestHandlerBase *handler);

            /**
             * Initializes the route and settings
             */
            virtual void setup();

            /**
             * Dump all routes
             */
            void dumpRoutes();

            /**
             * Called when an exception occur during the rendering
             *
             * @param string the error message
             *
             * @return response a response to send, 404 will occur if NULL
             */
            virtual Response *serverInternalError(string message);

            bool handles(string method, string url);
            vector<string> getUrls();

        protected:
            Server *server;
            string prefix;
            map<string, RequestHandlerBase*> routes;
            vector<string> urls;
    };
}

#endif
