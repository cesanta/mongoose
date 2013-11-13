#ifdef _MSC_VER
#include <windows.h>
#include <time.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;

class MyController : public WebController
{
    public: 
        void webSocketReady(WebSocket *websocket)
        {
            cout << "Opening new websocket on " << websocket->getRequest().getUrl() << endl;
            websocket->send("server ready");
        
            ostringstream oss;
            oss << "Your id is: " << websocket->getId();
            websocket->send(oss.str());
        }

        void webSocketData(WebSocket *websocket, string data)
        {
            cout << "[recv] " << data << endl;
            websocket->send(data);

            if (data == "exit") {
                cout << "Client exiting" << endl;
                websocket->close();
            }
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
    server.setOption("document_root", "websocket_html_root");
    server.start();

    cout << "Server started, routes:" << endl;
    myController.dumpRoutes();

    while (running) {
#ifdef WIN32
		Sleep(10000);
#else
        sleep(10);
#endif
    }

    server.stop();

    return EXIT_SUCCESS;
}
