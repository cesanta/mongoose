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

static int do_i_handle(struct mg_connection *connection)
{
    Server *server = (Server *)connection->server_param;

    return server->handles(connection->request_method, connection->uri);
}

/**
 * The handlers below are written in C to do the binding of the C mongoose with
 * the C++ API
 */
static int event_handler(struct mg_connection *connection)
{
    Server *server = (Server *)connection->server_param;

    if (server != NULL) {
#ifndef NO_WEBSOCKET
        if (connection->is_websocket) {
            server->_webSocketReady(connection);
        }
#endif
        server->_handleRequest(connection);
    }

    return 1;
}

#ifndef NO_WEBSOCKET
static int iterate_callback(struct mg_connection *connection)
{
    if (connection->is_websocket && connection->content_len) {
        Server *server = (Server *)connection->server_param;
        server->_webSocketData(connection, string(connection->content, connection->content_len));
    }

    return 1;
}
#endif

static void *server_poll(void *param)
{
    Server *server = (Server *)param;
    server->poll();

    return NULL;
}

namespace Mongoose
{
    Server::Server(int port, const char *documentRoot)
        : server(NULL),
        stopped(false)
#ifndef NO_WEBSOCKET 
          ,websockets(NULL)
#endif

    {
        ostringstream portOss;
        portOss << port;
        optionsMap["listening_port"] = portOss.str();
        optionsMap["document_root"] = string(documentRoot);
        optionsMap["enable_keep_alive"] = "yes";
    }

    Server::~Server()
    {
        stop();
    }

    void Server::start()
    {
        if (server == NULL) {
#ifdef ENABLE_STATS
            requests = 0;
            startTime = getTime();
#endif
            server = mg_create_server(this);
            size_t size = optionsMap.size()*2+1;

            map<string, string>::iterator it;
            for (it=optionsMap.begin(); it!=optionsMap.end(); it++) {
                mg_set_option(server, (*it).first.c_str(), (*it).second.c_str());
            }

            mg_add_uri_handler(server, "/", event_handler);
            mg_server_do_i_handle(server, do_i_handle);

            stopped = false;
            mg_start_thread(server_poll, this);
        } else {
            throw string("Server is already running");
        }
    }

    void Server::poll()
    {
        unsigned int current_timer = 0;
        while (!stopped) {
            mg_poll_server(server, 1000);
#ifndef NO_WEBSOCKET
            mg_iterate_over_connections(server, iterate_callback, &current_timer);
#endif
        }

        mg_destroy_server(&server);
    }

    void Server::stop()
    {
        stopped = true;
    }

    void Server::registerController(Controller *controller)
    {
        controller->setServer(this);
        controller->setup();
        controllers.push_back(controller);
    }

#ifndef NO_WEBSOCKET
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

    int Server::_webSocketData(struct mg_connection *conn, string data)
    {
        WebSocket *websocket = websockets.getWebSocket(conn);

        if (websocket != NULL) {
            websocket->appendData(data);

            string fullPacket = websocket->flushData();
            vector<Controller *>::iterator it;
            for (it=controllers.begin(); it!=controllers.end(); it++) {
                (*it)->webSocketData(websocket, fullPacket);
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
#endif

    int Server::_handleRequest(struct mg_connection *conn)
    {
        Request request(conn);

        mutex.lock();
        currentRequests[conn] = &request;
        mutex.unlock();

        Response *response = handleRequest(request);

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

    bool Server::handles(string method, string url)
    {
#ifndef NO_WEBSOCKET
        if (url == "/websocket") {
            return true;
        }
#endif

        vector<Controller *>::iterator it;
        for (it=controllers.begin(); it!=controllers.end(); it++) {
            if ((*it)->handles(method, url)) {
                return true;
            }
        }

        return false;
    }

    Response *Server::handleRequest(Request &request)
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

#ifndef NO_WEBSOCKET
    WebSockets &Server::getWebSockets()
    {
        return websockets;
    }
#endif

    void Server::printStats()
    {
        int delta = getTime()-startTime;

        if (delta) {
            cout << "Requests: " << requests << ", Requests/s: " << (requests*1.0/delta) << endl;
        }
    }
};
