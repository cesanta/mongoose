#include <sstream>
#include "Response.h"

using namespace std;

namespace Mongoose
{
    Response::Response() : code(HTTP_OK), headers()
    {
    };
            
    Response::~Response()
    {
    }
            
    void Response::setHeader(string key, string value)
    {
        headers[key] = value;
    }

    bool Response::hasHeader(string key)
    {
        return headers.find(key) != headers.end();
    }

    string Response::getData()
    {
        string body = getBody();
        ostringstream data;

        data << "HTTP/1.0 " << code << "\r\n";

        if (!hasHeader("Content-Length")) {
            ostringstream length;
            length << body.size();
            setHeader("Content-Length", length.str());
        }

        map<string, string>::iterator it;
        for (it=headers.begin(); it!=headers.end(); it++) {
            data << (*it).first << ": " << (*it).second << "\r\n";
        }

        data << "\r\n";

        data << body;

        return data.str();
    }

    void Response::setCookie(string key, string value)
    {
        ostringstream definition;
        definition << key << "=" << value << "; path=/";

        setHeader("Set-cookie", definition.str());
    }

    void Response::setCode(int code_)
    {
        code = code_;
    }
};
