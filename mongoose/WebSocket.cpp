#include <iostream>
#include <sstream>
#include "WebSocket.h"

using namespace std;

namespace Mongoose
{
    WebSocket::WebSocket(struct mg_connection *connection_)
        : connection(connection_), closed(false), request(connection_), data("")
    {
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
    }

    void WebSocket::close()
    {
        if (!closed) {
            mg_close_connection(connection);
            closed = true;
        }
    }

    bool WebSocket::isClosed()
    {
        return closed;
    }
};

