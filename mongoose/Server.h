#ifndef _MONGOOSE_SERVER_H
#define _MONGOOSE_SERVER_H

#include <vector>
#include <iostream>
#include <mongoose.h>
#include "Request.h"
#include "Response.h"
#include "Controller.h"

using namespace std;

/**
 * Wrapper for the Mongoose server
 */
namespace Mongoose
{
    class Server
    {
        public:
            /**
             * Constructs the server
             *
             * @param int the number of the port to listen to
             * @param string documentRoot the root that should be used for static files
             */
            Server(int port = 80, string documentRoot = "www");
            virtual ~Server();

            /**
             * Runs the Mongoose server
             */
            void start();

            /**
             * Stops the Mongoose server
             */
            void stop();

            /**
             * Register a new controller on the server
             *
             * @param Controller* a pointer to a controller
             */
            void registerController(Controller *);

            /**
             * Internally used to process a request
             *
             * @param struct mg_connection* the mongoose connection
             */
            int _beginRequest(struct mg_connection *conn);

            /**
             * Process the request by controllers
             *
             * @param Request the request
             *
             * @return Response the response if one of the controllers can handle it,
             *         NULL else
             */
            Response *beginRequest(Request &request);

            /**
             * Sets a mongoose extra option
             *
             * @param string the name of the option
             * @param string the value of the option
             */
            void setOption(string key, string value);

        protected:
            map<string, string> extraOptions;
            string documentRoot;
            int port;
            struct mg_callbacks callbacks;
            struct mg_context *ctx;
            const char **options;

            vector<Controller *> controllers;
    };
};

#endif
