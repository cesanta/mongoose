#include <sstream>
#include "Response.h"

using namespace std;

namespace Mongoose
{
    Response::Response() : ostringstream(), code(HTTP_OK)
    {
    };
            
    void Response::setHeader(string key, string value)
    {
        headers[key] = value;
    }

    string Response::getData()
    {
        ostringstream data;

        data << "HTTP/1.0 " << code << "\r\n";
        
        map<string, string>::iterator it;
        for (it=headers.begin(); it!=headers.end(); it++) {
            data << (*it).first << ": " << (*it).second << "\r\n";
        }

        data << "\r\n";

        data << this->str();

        return data.str();
    }
};
