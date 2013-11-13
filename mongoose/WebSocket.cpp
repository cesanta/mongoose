#include <iostream>
#include <sstream>
#include "WebSocket.h"
#include "WebSockets.h"

using namespace std;

namespace Mongoose
{
    WebSocket::WebSocket(struct mg_connection *connection_, const struct mg_request_info *request_)
        : connection(connection_), closed(false), request(connection_, request_), data(""), id(-1)
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

    void WebSocket::send(string data)
    {
        if (isClosed()) {
            return;
        }

        mutex.lock();
        int remaining = data.size();
        int chunk = 0;

        while (!isClosed() && remaining > 0) {
            ostringstream packetOss;
            string part = data.substr(chunk*255, min(remaining, 255));;
            chunk++;
            remaining -= 255;
            
            if (remaining > 0) {
                packetOss << (char)0x01;
            } else {
                packetOss << (char)0x81;
            }

            packetOss << (unsigned char)part.size() << part;

            string packet = packetOss.str();
            if (mg_write(connection, packet.c_str(), packet.size()) <= 0) {
                closed = true;
            }
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
        if (!closed) {
            mg_close_connection(connection);
        }
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

