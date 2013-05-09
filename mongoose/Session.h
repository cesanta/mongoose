#ifndef _MONGOOSE_SESSION_H
#define _MONGOOSE_SESSION_H

#include "Container.h"

using namespace std;

/**
 * A session contains the user specific values
 */ 
namespace Mongoose
{
    class Session : public Container
    {
        public:
            void setValue(string key, string value);
            bool hasValue(string key);
            string get(string key, string fallback = "");

        protected:
            map<string, string> values;
    };
};

#endif
