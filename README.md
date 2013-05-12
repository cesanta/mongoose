# Mongoose-C++

Mongoose-C++ is a fork of the popular [mongoose](https://github.com/valenok/mongoose)
lightweight web server which aims to add C++ bindings and a easy-to-use 
API.

# Features

- Object-Oriented high level API, keeping the lightweight mongoose implementation
  as a backend
- Easy-to-use controllers sytem to build an application with modules
- Possibility of enabling JsonCPP to create a json compliant web application
- Session system to store data about an user using cookies and garbage collect cleaning
- Simple access to GET & POST requests

# Building examples

You can build examples using CMake:

```
mkdir builddir
cd builddir
cmake ..
make
```

This will build you the `cpp` program with examples of GET, POST (form), session and 
HTTP response code

You can also enable Json example using the `-DHAS_JSONCPP=ON` option when cmake'ing,
this will build the `json` executable

# Development

The code writing take places in the `mongoose/` directory and the whole repository
will be merged as often as possible from the original mongoose project.

# License

This project is under MIT license, like the original project it's based on
