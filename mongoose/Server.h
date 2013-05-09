#ifndef _MONGOOSE_SERVER_H
#define _MONGOOSE_SERVER_H

#include <vector>
#include <iostream>
#include <mongoose.h>
#include "Request.h"
#include "Response.h"
#include "Controller.h"

using namespace std;

namespace Mongoose
{
    class Server
    {
        public:
            Server(int port = 80, string documentRoot = "www");
            ~Server();

            void start();
            void stop();

            void registerController(Controller *);

            int _beginRequest(struct mg_connection *conn);
            Response *beginRequest(Request &request);

        protected:
            string documentRoot;
            int port;
            struct mg_callbacks callbacks;
            struct mg_context *ctx;

            vector<Controller *> controllers;
    };
};

#endif
