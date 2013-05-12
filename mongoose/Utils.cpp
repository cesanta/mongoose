#include <iostream>
#include <sstream>
#include "Utils.h"

using namespace std;

namespace Mongoose
{
    string Utils::htmlEntities(string data)
    {
        string buffer;
        buffer.reserve(data.size());

        for(size_t pos = 0; pos != data.size(); ++pos) {
            switch(data[pos]) {
                case '&':  buffer.append("&amp;");       break;
                case '\"': buffer.append("&quot;");      break;
                case '\'': buffer.append("&apos;");      break;
                case '<':  buffer.append("&lt;");        break;
                case '>':  buffer.append("&gt;");        break;
                default:   buffer.append(1, data[pos]); break;
            }
        }

        return buffer;
    }
}
