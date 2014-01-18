# Mongoose-C++

Mongoose-C++ is a fork of the popular [mongoose](https://github.com/valenok/mongoose)
lightweight web server which aims to add C++ bindings and a easy-to-use 
API.

# Features

- Object-Oriented high level API, keeping the lightweight mongoose implementation
  as a backend
- Easy-to-use controllers sytem to build an application with modules
- Possibility of enabling JsonCPP to create a json compliant web application
- URL dispatcher using regex matches (C++11)
- Session system to store data about an user using cookies and garbage collect cleaning
- Simple access to GET & POST requests
- Websockets support

# Hello world

Here is an example, this will serve the static files from `www/` directory (which
is the default setting) and the `/hello` page will be answered by a controller which
will display the GET `name` variable, for instance `/hello?name=bob` will display
the string "Hello bob". Default parameter value, if not provided, will be
"... waht's your name ?". This is the `helloworld` program build in the examples:

```c++
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;

class MyController : public WebController
{
    public: 
        void hello(Request &request, StreamResponse &response)
        {
            response << "Hello " << htmlEntities(request.get("name", "... what's your name ?")) << endl;
        }

        void setup()
        {
            addRoute("GET", "/hello", MyController, hello);
        }
};


int main()
{
    MyController myController;
    Server server(8080);
    server.registerController(&myController);

    server.start(); 

    while (1) {
        sleep(10);
    }
}
```

# Building examples

You can build examples using CMake:

```
mkdir build
cd build
cmake -DEXAMPLES=ON ..
make
```

This will build you the `cpp` program with examples of GET, POST (form), session and 
HTTP response code

You can also enable Json example using the `-DHAS_JSONCPP=ON` option when cmake'ing,
this will build the `json` executable. You also have to specify the `JSONCPP_DIR` that is the [JsonCpp](http://jsoncpp.sourceforge.net/) installation directory.

Websockets are also supported and will be compiled if the `-DWEBSOCKET=ON` option is
set with cmake (which is the default). `websocket.cpp` will be compiled to the
`cpp_websocket` executable which let you see an example. Note that references to the
`WebSocket*` clients can be keeped to dispatch data to them, which can be really
useful to push data to some clients.

To enable url regex matching dispatcher use `-DENABLE_REGEX_URL=ON` option.
Note that this depends on C++11.

# Development

The code writing take places in the `mongoose/` directory and the whole repository
will be merged as often as possible from the original mongoose project.

# License

The mongoose binding (`mongoose/` folder) is under MIT license

However, the original mongoose project license is different, have a look to the
`LICENSE` file for more information.
