#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Sessions.h"

using namespace std;

static char charset[] = "abcdeghijklmnpqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
#define CHARSET_SIZE (sizeof(charset)/sizeof(char))

namespace Mongoose
{
    Sessions::Sessions(string key_) : key(key_)
    {
    }

    string Sessions::getId(Request &request, Response &response)
    {
        if (request.hasCookie(key)) {
            return request.getCookie(key);
        } else {
            ostringstream newCookie;
            int i;

            for (i=0; i<30; i++) {
                newCookie << charset[rand()%CHARSET_SIZE];
            }

            ostringstream definition;
            definition << key << "=" << newCookie.str() << "; path=/";

            response.setHeader("Set-cookie", definition.str());

            return newCookie.str();
        }
    }

    Session &Sessions::get(Request &request, Response &response)
    { 
        string id = getId(request, response);
        Session &session = sessions[id];

        return session;
    }

    void Sessions::garbageCollect(int oldAge)
    {
        vector<string> deleteList;
        map<string, Session>::iterator it;
        vector<string>::iterator vit;

        for (it=sessions.begin(); it!=sessions.end(); it++) {
            string name = (*it).first;
            Session &session = (*it).second;

            if (session.getAge() > oldAge) {
                deleteList.push_back(name);
            }
        }

        for (vit=deleteList.begin(); vit!=deleteList.end(); vit++) {
            sessions.erase(*vit);
        }
    }
};
