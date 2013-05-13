#ifndef _MONGOOSE_WEBSOCKET_H
#define _MONGOOSE_WEBSOCKET_H

#include <iostream>
#include <mongoose.h>
#include "Request.h"

using namespace std;

#define WEBSOCKET_TEXT 0x01
#define WEBSOCKET_FIN  0x80

namespace Mongoose
{
    class WebSocket
    {
        public:
            WebSocket(struct mg_connection *connection_);

            /**
             * Sends data through the web socket
             *
             * @param string the data to send
             */
            void send(string data);

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

        protected:
            string data;
            Request request;
            struct mg_connection *connection;
            bool closed;
    };
};

#endif
