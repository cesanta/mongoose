#ifndef _MONGOOSE_CONTAINER_H
#define _MONGOOSE_CONTAINER_H

#include <iostream>
#include <map>

using namespace std;

/**
 * A container has values, which can be get and whose presence
 * can be checked
 */
namespace Mongoose
{
    class Container
    {
        public:
            /**
             * Is key present in the container ?
             *
             * @param key the key to be tested
             *
             * @return bool true if the key is present
             */
            virtual bool hasValue(string key)=0;

            /**
             * Gets the value of key, fallback is the value
             * which will be returned if the key is not present
             *
             * @param string key the searched key
             *
             * @param string fallback the fallback value
             */
            virtual string get(string key, string fallback = "")=0;
    };
};

#endif
