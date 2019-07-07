//
// Created by fuzzit.dev inc.
//

//
// Created by fuzzit.dev inc.
//

#include <stdint.h>

#include "mongoose.h"

int LLVMFuzzerTestOneInput(const uint8_t * Data, size_t Size) {
    struct http_message req;
    mg_parse_http((const char *)Data, Size, &req, 0);
    return 0;
}


