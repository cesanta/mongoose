#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;

class MyController : public WebController
{
    public: 
        void hello(Request &request, StreamResponse *response)
        {
            *response << "Hello " << request.get("name", "anonymous you") << endl;
        }

        void form(Request &request, StreamResponse *response)
        {
            *response << "<form method=\"POST\">" << endl;
            *response << "<input type=\"text\" name=\"test\" /><br >" << endl;
            *response << "<input type=\"submit\" value=\"Envoyer\" />" << endl;
            *response << "</form>" << endl;
        }

        void formPost(Request &request, StreamResponse *response)
        {
            *response << "Test=" << request.get("test", "(unknown)");
        }

        void session(Request &request, StreamResponse *response)
        {
            Session &session = getSession(request, response);

            if (session.hasValue("try")) {
                *response << "Session value: " << session.get("try");
            } else {
                ostringstream val;
                val << time(NULL);
                session.setValue("try", val.str());
                *response << "Session value set to: " << session.get("try");
            }
        }
 
        Response *process(Request &request)
        {
            StreamResponse *response = NULL;

            MG_GET("/form", form);
            MG_POST("/form", formPost);
            MG_GET("/hello", hello);
            MG_GET("/session", session);

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

    server.start();

    while (running) {
        sleep(1);
    }

    server.stop();
    
    return EXIT_SUCCESS;
}
