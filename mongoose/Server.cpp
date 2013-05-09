#include <unistd.h>
#include <string.h>
#include <iostream>
#include "Server.h"

using namespace std;
using namespace Mongoose;

static int begin_request_handler(struct mg_connection *conn)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    Server *server = (Server *)request_info->user_data;

    if (server != NULL) {
        return server->_beginRequest(conn);
    }
}

namespace Mongoose
{
    Server::Server(int port_, string documentRoot_) : 
        ctx(NULL), port(port_), documentRoot(documentRoot_), options(NULL)
    {
        memset(&callbacks, 0, sizeof(callbacks));
    }

    void Server::start()
    {
        if (ctx == NULL) {
            char buffer[128];
            sprintf(buffer, "%d", port);
            size_t size = extraOptions.size()*2+4+1;

            options = new const char*[size];
            options[0] = "listening_ports";
            options[1] = buffer;
            options[2] = "document_root";
            options[3] = documentRoot.c_str();

            int pos = 0;
            map<string, string>::iterator it;
            for (it=extraOptions.begin(); it!=extraOptions.end(); it++) {
                options[4+2*pos] = (*it).first.c_str();
                options[4+2*pos+1] = (*it).second.c_str();
                pos++;
            }

            options[4+2*pos] = NULL;

            callbacks.begin_request = begin_request_handler;

            mg_start(&callbacks, (void *)this, options);
        } else {
            throw string("Server is already running");
        }
    }

    void Server::stop()
    {
        if (ctx != NULL) {
            mg_stop(ctx);
            ctx = NULL;
        }

        if (options != NULL) {
            delete[] options;
            options = NULL;
        }
    }

    void Server::registerController(Controller *controller)
    {
        controllers.push_back(controller);
    }

    int Server::_beginRequest(struct mg_connection *conn)
    {
        Request request(conn);
        Response *response = beginRequest(request);

        if (response == NULL) {
            return 0;
        } else {
            request.writeResponse(response);
            delete response;
            return 1;
        }
    }

    Response *Server::beginRequest(Request &request)
    {
        Response *response;
        vector<Controller *>::iterator it;

        for (it=controllers.begin(); it!=controllers.end(); it++) {
            Controller *controller = *it;
            response = controller->process(request);

            if (response != NULL) {
                return response;
            }
        }

        return NULL;
    }

    Server::~Server()
    {
        stop();
    }

    void Server::setOption(string key, string value)
    {
        extraOptions[key] = value;
    }
};
