#ifndef _MONGOOSE_WEBSOCKETS_H
#define _MONGOOSE_WEBSOCKETS_H

#include <map>
#include <iostream>
#include <mongoose.h>
#include "WebSocket.h"
#include "Mutex.h"

using namespace std;

/**
 * WebSockets is an array that contains WebSocket connections, this
 * can be used for instance to broadcast informations to them.
 *
 * The function clean() allow to remove closed connections from the
 * array.
 */
namespace Mongoose
{
    class WebSockets
    {
        public:
            /**
             * Creates a websockets array, the responsible false specify whether the
             * container will be responsible for cleaning the websocket
             */
            WebSockets(bool responsible = false);
            virtual ~WebSockets();

            /**
             * Adds the given websocket to the poll. This container will not
             * become responsible for cleaning this object
             *
             * @param WebSocket* the websocket object
             */
            void add(WebSocket *websocket);

            /**
             * Send data to all sockets in this container
             */
            void sendAll(string data);

            /**
             * Gets the websocket corresponding to the given connection
             *
             * @param strut mg_connection* the mongoose connection
             */
            WebSocket *getWebSocket(struct mg_connection *connection);

            /**
             * Cleans all the connections that are hold in this object and that are
             * closed
             */
            void clean();

            /**
             * Removes the websocket from the container
             *
             * @param WebSocket* the websocket object
             */
            void remove(WebSocket *websocket, bool lock = true);
    
            /**
             * Gets the websockets having the id 
             *
             * @param int id
             */
            WebSocket *getWebSocket(int id);

        protected:
            Mutex mutex;
            map<struct mg_connection*, WebSocket*> websockets;
            map<int, WebSocket*> websocketsById;
            bool responsible;

            int id;
    };
}

#endif
