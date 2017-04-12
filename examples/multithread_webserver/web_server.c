#include <string.h>
#include "mongoose.h"

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
#define root_path "/www/"
/*define the number of thread.*/
#define threadnum 10
vector<struct mg_server *> srvlist ;

static int event_handler(struct mg_connection *conn, enum mg_event ev) {
    if (ev == MG_AUTH) {
        return MG_TRUE;   // Authorize all requests
    }
    else if (ev == MG_REQUEST && !strcmp(conn->uri, "/hello")) {
        mg_printf_data(conn, "%s", "Hello world");
        return MG_TRUE;   // Mark as processed
    } else if(ev==MG_REQUEST){
        return MG_FALSE;
    }else if(ev==MG_POLL){
        return MG_FALSE;
    }else if(ev==MG_HTTP_ERROR){
        return MG_FALSE;
    }
    
}

static void *serve(void *server) {
  for (;;) mg_poll_server((struct mg_server *) server, 1000);

  return NULL;
}

void
createinstance(){
	for(int i=0;i<threadnum;++i){
	struct mg_server *tmp ;
	char * mark = (char*)malloc(sizeof(char)*32);
		sprintf(mark,"%4d",i);
		if((tmp = mg_create_server((void *) mark, event_handler)) != NULL){
	        if(i==0){
                mg_set_option(tmp, "document_root", root_path);
                mg_set_option(tmp, "listening_port", "8080");
                mg_set_option(tmp,"enable_directory_listing","no");
            }else{
                mg_copy_listeners(srvlist[0], tmp);
        	    mg_set_option(tmp, "document_root", root_path);
            }
			srvlist.push_back(tmp);

		}else{
			printf("start mongoose failed\n");
		}	
	}
	for(int i =0;i<threadnum;++i){
	    mg_start_thread(serve, srvlist[i]);
	}
	
		
}


int main(void) {
    createinstance();
    getchar();
    return 0;
}
