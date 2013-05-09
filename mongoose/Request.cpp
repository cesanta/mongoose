#include <string.h>
#include <iostream>
#include <mongoose.h>
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

    bool Request::hasVariable(string key)
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
                delete[] buffer;
                buffer = new char[size];
            }
        } while (ret == -2);

        output = string(buffer);
        delete[] buffer;

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
            
    bool Request::hasCookie(string key)
    {
        int i;
        char dummy[10];

        for (i=0; i<request->num_headers; i++) {
            struct mg_request_info::mg_header *header = &request->http_headers[i];
            
            if (strcmp(header->name, "Cookie") == 0) {
                if (mg_get_cookie(header->value, key.c_str(), dummy, sizeof(dummy)) != -1) {
                    return true;
                }
            }
        }

        return false;
    }

    string Request::getCookie(string key, string fallback)
    {
        string output;
        int i;
        int size = 1024;
        int ret;
        char *buffer = new char[size];
        char dummy[10];
        const char *place = NULL;
        
        for (i=0; i<request->num_headers; i++) {
            struct mg_request_info::mg_header *header = &request->http_headers[i];
            
            if (strcmp(header->name, "Cookie") == 0) {
                if (mg_get_cookie(header->value, key.c_str(), dummy, sizeof(dummy)) != -1) {
                    place = header->value;
                }
            }
        }

        if (place == NULL) {
            return fallback;
        }

        do {
            ret = mg_get_cookie(place, key.c_str(), buffer, size);

            if (ret == -2) {
                size *= 2;
                delete[] buffer;
                buffer = new char[size];
            }
        } while (ret == -2);

        output = string(buffer);
        delete[] buffer;

        return output;
    }
};
