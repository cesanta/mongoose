#include <iostream>
#include "Session.h"

using namespace std;

namespace Mongoose
{
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
};
