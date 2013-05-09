#include <string.h>
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

        // Downloading POST data
        ostringstream postData;
        char post[1024];
        while (mg_read(connection, post, sizeof(post))) {
            postData << post;
        }
        data = postData.str();
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

    bool Request::hasValue(string key)
    {
        const char *dataField;
        char dummy[10];

        // Looking on the query string
        dataField = request->query_string;
        if (dataField != NULL && mg_get_var(dataField, strlen(dataField), key.c_str(), dummy, 1) != -1) {
            return true;
        }

        // Looking on the POST data
        dataField = data.c_str();
        if (dataField != NULL && mg_get_var(dataField, strlen(dataField), key.c_str(), dummy, 1) != -1) {
            return true;
        }

        return false;
    }

    bool Request::readVariable(const char *data, string key, string &output)
    {
        int size = 1024;
        int ret, n;
        char *buffer = new char[size];
        n = strlen(data);

        do {
            ret = mg_get_var(data, n, key.c_str(), buffer, size);

            if (ret == -1) {
                return false;
            }

            if (ret == -2) {
                size *= 2;
                delete buffer;
                buffer = new char[size];
            }
        } while (ret == -2);

        output = string(buffer);

        return true;
    }

    string Request::get(string key, string fallback)
    {
        const char *dataField;
        string output;

        // Looking on the query string
        dataField = request->query_string;
        if (dataField != NULL && readVariable(dataField, key, output)) {
            return output;
        }

        // Looking on the POST data
        dataField = data.c_str();
        if (dataField != NULL && readVariable(dataField, key, output)) {
            return output;
        }

        return fallback;
    }
};
