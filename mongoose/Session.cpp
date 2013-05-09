#include <time.h>
#include <iostream>
#include "Session.h"

using namespace std;

namespace Mongoose
{
    Session::Session()
    {
        ping();
    }

    void Session::ping()
    {
        date = time(NULL);
    }

    void Session::setValue(string key, string value)
    {
        values[key] = value;
    }

    bool Session::hasValue(string key)
    {
        return values.find(key) != values.end();
    }

    string Session::get(string key, string fallback)
    {
        if (hasValue(key)) {
            return values[key];
        } else {
            return fallback;
        }
    }

    int Session::getAge()
    {
        return time(NULL)-date;
    }
};
