#include <sstream>
#include "JsonResponse.h"

using namespace std;

namespace Mongoose
{
    string JsonResponse::getBody()
    {
        ostringstream data;
        data << *this;

        return data.str();
    }
};
