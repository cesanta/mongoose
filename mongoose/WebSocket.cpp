#include <iostream>
#include <sstream>
#include "WebSocket.h"
#include "WebSockets.h"

using namespace std;

namespace Mongoose
{
    WebSocket::WebSocket(struct mg_connection *connection_)
        : connection(connection_), closed(false), request(connection_), data(""), id(-1)
    {
    }

    void WebSocket::setId(int id_)
    {
        id = id_;
    }

    int WebSocket::getId()
    {
        return id;
    }

    void WebSocket::appendData(string data_)
    {
        data += data_;
    }

    string WebSocket::flushData()
    {
        string oldData = "";
        data.swap(oldData);

        return oldData;
    }

    Request &WebSocket::getRequest()
    {
        return request;
    }

    void WebSocket::send(string data, int opcode)
    {
        if (isClosed()) {
            return;
        }

        mutex.lock();
        if (!mg_websocket_write(connection, opcode, data.c_str(), data.size())) {
            closed = true;
        }
        mutex.unlock();
    }

    void WebSocket::notifyContainers()
    {
        vector<WebSockets *>::iterator it;

        mutex.lock();
        for (it=containers.begin(); it!=containers.end(); it++) {
            (*it)->remove(this);
        }
        mutex.unlock();
    }

    void WebSocket::close()
    {
        closed = true;
    }

    bool WebSocket::isClosed()
    {
        return closed;
    }

    void WebSocket::addContainer(WebSockets *websockets)
    {
        mutex.lock();
        containers.push_back(websockets);
        mutex.unlock();
    }

    void WebSocket::removeContainer(WebSockets *websockets)
    {
        mutex.lock();
        vector<WebSockets *>::iterator it;

        for (it=containers.begin(); it!=containers.end(); it++) {
            if (*it == websockets) {
                containers.erase(it);
                break;
            }
        }
        mutex.unlock();
    }

    struct mg_connection *WebSocket::getConnection()
    {
        return connection;
    }
};

