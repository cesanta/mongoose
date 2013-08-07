#ifndef _MSC_VER
#include <unistd.h>
#include <stdlib.h>
#else
#include <time.h>
#endif
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

        void form(Request &request, StreamResponse &response)
        {
            response << "<form method=\"POST\">" << endl;
            response << "<input type=\"text\" name=\"test\" /><br >" << endl;
            response << "<input type=\"submit\" value=\"Envoyer\" />" << endl;
            response << "</form>" << endl;
        }

        void formPost(Request &request, StreamResponse &response)
        {
            response << "Test=" << htmlEntities(request.get("test", "(unknown)"));
        }

        void session(Request &request, StreamResponse &response)
        {
            Session &session = getSession(request, response);

            if (session.hasValue("try")) {
                response << "Session value: " << session.get("try");
            } else {
                ostringstream val;
                val << time(NULL);
                session.setValue("try", val.str());
                response << "Session value set to: " << session.get("try");
            }
        }

        void forbid(Request &request, StreamResponse &response)
        {
            response.setCode(HTTP_FORBIDDEN);
            response << "403 forbidden demo";
        }

        void exception(Request &request, StreamResponse &response)
        {
            throw string("Exception example");
        }

        void setup()
        {
            // Hello demo
            addRoute("GET", "/hello", MyController, hello);
            addRoute("GET", "/", MyController, hello);

            // Form demo
            addRoute("GET", "/form", MyController, form);
            addRoute("POST", "/form", MyController, formPost);

            // Session demo
            addRoute("GET", "/session", MyController, session);

            // Exception example
            addRoute("GET", "/exception", MyController, exception);

            // 403 demo
            addRoute("GET", "/403", MyController, forbid);
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
	int t;
#ifdef _MSC_VER
    time_t ltime;
	time(&ltime);
	t = ltime;
#else
	t = time(NULL);
#endif
	srand(t);

    signal(SIGINT, handle_signal);

    MyController myController;
    Server server(8080);
    server.registerController(&myController);
    server.setOption("enable_directory_listing", "false");
    server.start();
    
    cout << "Server started, routes:" << endl;
    myController.dumpRoutes();

    while (running) {
#ifdef WIN32
		Sleep(10000);
#else
        sleep(10);
#endif
        server.printStats();
    }

    server.stop();
    
    return EXIT_SUCCESS;
}
