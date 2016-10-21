FROM cesanta/mongoose

COPY load_balancer.c /mongoose/
WORKDIR /mongoose
RUN mkdir /mongoose/certs; \
    sed -i 's:#include "../../mongoose.h":#include "mongoose.h":' load_balancer.c; \
    cc load_balancer.c mongoose.c -o load_balancer -W -Wall -pthread -DMG_ENABLE_SSL -lssl -lcrypto
EXPOSE 8000
VOLUME ["/mongoose/certs"]
ENTRYPOINT ["/mongoose/load_balancer"]
