#ifndef _MONGOOSE_WEBSOCKET_H
#define _MONGOOSE_WEBSOCKET_H

#include <vector>
#include <iostream>
#include <mongoose.h>
#include "Request.h"
#include "Mutex.h"

using namespace std;

#define WEBSOCKET_FIN 0x80

enum {
    WEBSOCKET_OPCODE_CONTINUATION = 0x0,
    WEBSOCKET_OPCODE_TEXT = 0x1,
    WEBSOCKET_OPCODE_BINARY = 0x2,
    WEBSOCKET_OPCODE_CONNECTION_CLOSE = 0x8,
    WEBSOCKET_OPCODE_PING = 0x9,
    WEBSOCKET_OPCODE_PONG = 0xa,
};

namespace Mongoose
{
    class WebSockets;

    class WebSocket
    {
        public:
            WebSocket(struct mg_connection *connection_);

            /**
             * Sends data through the web socket
             *
             * @param string the data to send
             */
            void send(string data, int opcode = WEBSOCKET_OPCODE_TEXT);

            /**
             * Returns the connection request
             *
             * @return Request& the request
             */
            Request &getRequest();

            /**
             * Closes the connection
             */
            void close();

            /**
             * Is the connection closed ?
             *
             * @return bool true if the connection is closed
             */
            bool isClosed();

            /**
             * Append data to the internal receive buffer
             *
             * @param string data
             */
            void appendData(string data);

            /**
             * Gets the internal receive buffer and clear it
             *
             * @return string data
             */
            string flushData();

            /**
             * Gets the internal mg connection
             *
             * @return struct mg_connection* the connection
             */
            struct mg_connection *getConnection();

            /**
             * Adding this websocket in a container
             *
             * @param WebSockets* the container
             */
            void addContainer(WebSockets *websockets);

            /**
             * Removing this websocket from a container
             */
            void removeContainer(WebSockets *websockets);

            /**
             * Notify all the containers that the websocket is now closed and unusable
             */
            void notifyContainers();

            /**
             * Sets the identifier of this websocket
             *
             * @param int the websocket identifier
             */
            void setId(int id_);

            /**
             * Gets the websocket identifier
             *
             * @return int the identifier of this websocket
             */
            int getId();

        protected:
            int id;
            Mutex mutex;
            string data;
            Request request;
            struct mg_connection *connection;
            bool closed;

            vector<WebSockets *> containers;
    };
}

#endif
