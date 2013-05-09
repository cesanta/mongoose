#include <iostream>
#include "Request.h"

using namespace std;

namespace Mongoose
{
    Request::Request(struct mg_connection *connection_) : 
        connection(connection_)
    {
        request = mg_get_request_info(connection);

        url = string(request->uri);
        method = string(request->request_method);
    }

    string Request::getUrl()
    {
        return url;
    }

    string Request::getMethod()
    {
        return method;
    }
            
    void Request::writeResponse(Response *response)
    {
        string data = response->getData();

        mg_write(connection, data.c_str(), data.size());
    }
};
