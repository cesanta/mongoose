#ifndef _MSC_VER
#include <unistd.h>
#else 
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#endif
#include <string>
#include <string.h>
#include <iostream>
#include "Server.h"

using namespace std;
using namespace Mongoose;

static int getTime()
{
#ifdef _MSC_VER
    time_t ltime;
    time(&ltime);
    return ltime;
#else
    return time(NULL);
#endif
}

/**
 * The handlers below are written in C to do the binding of the C mongoose with
 * the C++ API
 */
static int event_handler(struct mg_event *event)
{
    if (event->type == MG_REQUEST_BEGIN) {
        Server *server = (Server *)event->user_data;

        if (server != NULL) {
            const struct mg_request_info *request_info = event->request_info;
            struct mg_connection *conn = event->conn;

#ifdef USE_WEBSOCKET
            const char *version_header = mg_get_header(event->conn, "Sec-WebSocket-Version");
            if (version_header != NULL) {
                // Websocket request, process it
                if (strcmp(version_header, "13") != 0) {
                    mg_printf(event->conn, "%s", "HTTP/1.1 426 Upgrade Required\r\n\r\n");
                } else {
                    char *data;
                    int bits, len;

                    // Handshake
                    mg_websocket_handshake(event->conn);
                    server->_webSocketReady(conn, request_info);

                    while ((len = mg_websocket_read(conn, &bits, &data)) > 0) {
                        server->_webSocketData(conn, bits, data, len);
                        delete data;
                    }

                }

                return 1;
            }
#endif
        
            return server->_beginRequest(conn, request_info);
        }
    }

    return 0;
}

namespace Mongoose
{
    Server::Server(int port, const char *documentRoot)
        : ctx(NULL), options(NULL), websockets(true)

    {
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
            startTime = getTime();
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
            mg_start(options, event_handler, (void *)this);
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

    void Server::_webSocketReady(struct mg_connection *conn, const struct mg_request_info *request)
    {
        WebSocket *websocket = new WebSocket(conn, request);
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

    int Server::_beginRequest(struct mg_connection *conn, const struct mg_request_info *request_info)
    {
        Request request(conn, request_info);

        mutex.lock();
        currentRequests[conn] = &request;
        mutex.unlock();

        Response *response = beginRequest(request);

        mutex.lock();
        currentRequests.erase(conn);
        mutex.unlock();

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
        int delta = getTime()-startTime;

        if (delta) {
            cout << "Requests: " << requests << ", Requests/s: " << (requests*1.0/delta) << endl;
        }
    }
};
