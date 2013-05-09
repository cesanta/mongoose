#include <stdlib.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;

class MyController : public WebController
{
    public: 
        void hello(Request &request, Response &response)
        {
            response << "Hello " << request.get("name", "anonymous you") << endl;
        }

        void form(Request &request, Response &response)
        {
            response << "<form method=\"POST\">" << endl;
            response << "<input type=\"text\" name=\"test\" /><br >" << endl;
            response << "<input type=\"submit\" value=\"Envoyer\" />" << endl;
            response << "</form>" << endl;
        }

        void formPost(Request &request, Response &response)
        {
            response << "Test=" << request.get("test", "(unknown)");
        }
 
        Response *process(Request &request)
        {
            Response *response = NULL;

            MG_GET("/form", form);
            MG_POST("/form", formPost);
            MG_GET("/hello", hello);

            return response;
        }
};

int main()
{
    MyController myController;
    Server server(8080);
    server.registerController(&myController);

    server.start();

    while(1) {
        sleep(1);
    }
}
