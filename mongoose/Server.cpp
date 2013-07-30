#include <unistd.h>
#include <string.h>
#include <iostream>
#include "Server.h"

using namespace std;
using namespace Mongoose;

/**
 * The handlers below are written in C to do the binding of the C mongoose with
 * the C++ API
 */
static int begin_request_handler(struct mg_connection *conn)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    Server *server = (Server *)request_info->user_data;

    if (server != NULL) {
        return server->_beginRequest(conn);
    }
}

static void websocket_ready_handler(struct mg_connection *conn)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    Server *server = (Server *)request_info->user_data;

    if (server != NULL) {
        server->_webSocketReady(conn);
    }
}

static int websocket_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    Server *server = (Server *)request_info->user_data;

    if (server != NULL) {
        return server->_webSocketData(conn, flags, data, data_len);
    }
}

namespace Mongoose
{
    Server::Server(int port, const char *documentRoot)
        : ctx(NULL), options(NULL), websockets(true)

    {
        memset(&callbacks, 0, sizeof(callbacks));

        ostringstream portOss;
        portOss << port;
        optionsMap["listening_ports"] = portOss.str();
        optionsMap["document_root"] = string(documentRoot);
        optionsMap["enable_keep_alive"] = "yes";
    }

    Server::~Server()
    {
        stop();
    }

    void Server::start()
    {
        if (ctx == NULL) {
#ifdef ENABLE_STATS
            requests = 0;
            startTime = time(NULL);
#endif
            size_t size = optionsMap.size()*2+1;

            options = new const char*[size];

            int pos = 0;
            map<string, string>::iterator it;
            for (it=optionsMap.begin(); it!=optionsMap.end(); it++) {
                options[2*pos] = (*it).first.c_str();
                options[2*pos+1] = (*it).second.c_str();
                pos++;
            }

            options[2*pos] = NULL;

            callbacks.begin_request = begin_request_handler;
#ifdef USE_WEBSOCKET
            callbacks.websocket_ready = websocket_ready_handler;
            callbacks.websocket_data = websocket_data_handler;
#endif

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
        controller->setServer(this);
        controller->setup();
        controllers.push_back(controller);
    }

    void Server::_webSocketReady(struct mg_connection *conn)
    {
        WebSocket *websocket = new WebSocket(conn);
        websockets.add(websocket);
        websockets.clean();

        vector<Controller *>::iterator it;
        for (it=controllers.begin(); it!=controllers.end(); it++) {
            (*it)->webSocketReady(websocket);
        }
    }

    int Server::_webSocketData(struct mg_connection *conn, int flags, char *data, size_t data_len)
    {
        WebSocket *websocket = websockets.getWebSocket(conn);

        if (websocket != NULL) {
            string contents(data, data_len);
            websocket->appendData(contents);

            if (flags & WEBSOCKET_FIN) {
                string fullPacket = websocket->flushData();
                vector<Controller *>::iterator it;
                for (it=controllers.begin(); it!=controllers.end(); it++) {
                    (*it)->webSocketData(websocket, fullPacket);
                }
            }

            if (websocket->isClosed()) {
                websockets.remove(websocket);
                return 0;
            } else {
                return -1;
            }
        } else {
            return 0;
        }
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
 
        mutex.lock();
        requests++;
        mutex.unlock();

        for (it=controllers.begin(); it!=controllers.end(); it++) {
            Controller *controller = *it;
            response = controller->process(request);

            if (response != NULL) {
                return response;
            }
        }

        return NULL;
    }

    void Server::setOption(string key, string value)
    {
        optionsMap[key] = value;
    }
            
    WebSockets &Server::getWebSockets()
    {
        return websockets;
    }

    void Server::printStats()
    {
        int delta = time(NULL)-startTime;

        if (delta) {
            cout << "Requests: " << requests << ", Requests/s: " << (requests*1.0/delta) << endl;
        }
    }
};
