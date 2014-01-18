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
            response << "<form method=\"post\">" << endl;
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

        void uploadForm(Request &request, StreamResponse &response)
        {
            response << "<h1>File upload demo (don't forget to create a tmp/ directory)</h1>";
            response << "<form enctype=\"multipart/form-data\" method=\"post\">";
            response << "Choose a file to upload: <input name=\"file\" type=\"file\" /><br />";
            response << "<input type=\"submit\" value=\"Upload File\" />";
            response << "</form>";
        }

        void upload(Request &request, StreamResponse &response)
        {
            request.handleUploads();

            // Iterate through all the uploaded files
            vector<UploadFile>::iterator it = request.uploadFiles.begin();
            for (; it != request.uploadFiles.end(); it++) {
                UploadFile file = *it;
                file.saveTo("tmp/");
                response << "Uploaded file: " << file.getName() << endl;
            }
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

            // File upload demo
            addRoute("GET", "/upload", MyController, uploadForm);
            addRoute("POST", "/upload", MyController, upload);
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
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    server.stop();
    
    return EXIT_SUCCESS;
}
