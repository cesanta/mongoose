#include <stdlib.h>
#include <mongoose/Server.h>
#include <mongoose/Controller.h>

class MyController : public Mongoose::Controller
{
    public: 
        void hello(Mongoose::Request &request, Mongoose::Response &response)
        {
            response << "Hello world!" << endl;
        }

        void preProcess(Mongoose::Request &request, Mongoose::Response &response)
        {
            response.setHeader("Content-type", "text/html");
        }
 
        Mongoose::Response *process(Mongoose::Request &request)
        {
            Mongoose::Response *response = NULL;

            MG_GET("/hello", hello);

            return response;
        }
};

int main()
{
    MyController myController;
    Mongoose::Server server(8080);
    server.registerController(&myController);

    server.start();

    while(1) {
        sleep(1);
    }
}
