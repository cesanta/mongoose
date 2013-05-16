#include <sstream>
#include <json/json.h>
#include "JsonResponse.h"

using namespace std;

namespace Mongoose
{
    string JsonResponse::getBody()
    {
        Json::FastWriter writer;

        return writer.write(*this);
    }
};
