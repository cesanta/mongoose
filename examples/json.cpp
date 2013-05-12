#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/JsonController.h>

using namespace std;
using namespace Mongoose;

class MyController : public JsonController
{
    public: 
        void hello(Request &request, JsonResponse &response)
        {
            int i;

            for (i=0; i<12; i++) {
                response["users"][i]["Name"] = "Bob";
            }

            response["timestamp"] = (int)time(NULL);
        }
 
        Response *process(Request &request)
        {
            JsonResponse *response = NULL;

            // Hello demo
            MG_GET("/", hello);
            MG_GET("/hello", hello);

            return response;
        }
};

volatile static bool running = true;

void handle_signal(int sig)
{
    if (running) {
        cout << "Exiting..." << endl;
        running = false;
    }
}

int main()
{
    srand(time(NULL));

    signal(SIGINT, handle_signal);

    MyController myController;
    Server server(8080);
    server.registerController(&myController);
    server.setOption("enable_directory_listing", "false");

    server.start();

    while (running) {
        sleep(1);
    }

    server.stop();
    
    return EXIT_SUCCESS;
}
