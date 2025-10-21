#include <stdio.h>
#include "config.h"
#include "mongoose.h"

#define PUBLIC_CERT 	  	  "./data/public.crt"
#define FILE_CNF              "/tmp/dl_state"
#define FILE_PATH             "/tmp/test.zip"
#define _LOG 			 	  printf
#define _HEX_LOG
#define HTTP_BUFFER_SIZE  	  4096*10
#define POLL_FOR_EVENTS_TIME  50
#define BUFF_SIZE 			  4096
#define TEST_URL              "http://127.0.0.1:80/dl/your_file_zip"
#define RETRY_COUNT           3
#define IDLE_TIMEOUT 		  5 // seconds
#define BYTE_COPY_BUFFER      4096

#ifndef MIN
#define MIN(a,b) 		 		(((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) 		 		(((a)>(b))?(a):(b))
#endif

#ifndef FALSE
#define FALSE             0
#endif

#ifndef TRUE
#define TRUE              1
#endif

typedef struct cache_data{
	char data[BUFF_SIZE];
	int  is_set;
}t_cache_data;

typedef struct dl_file_state{
	int64_t   n_file_part;
	int64_t   n_total_size;
}t_dl_file_state;

typedef struct dl_args{

	char  	   dl_addr[BUFF_SIZE];
	char  	   file_path[BUFF_SIZE];
	int   	   dl_with_resume;

	time_t 	   last_io;

	int64_t    n_retry_count ;

	int64_t    n_start_range ;
	int64_t    n_end_range;

	t_cache_data cache;

}t_dl_args;

struct mg_tls_opts opts;
int done_flag = FALSE;

static void my_log(char c, void *param){

	static char buf[1024+1] = {0};
	char* s_param = (char*)param;
	int len = strlen(buf);

	//NOTE
	//static pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
	//if multithread support needed
	//pthread_mutex_lock(&mutex);

	if(len == 0)
		len += snprintf(buf+len, sizeof(buf), "%s", "[MONGOOSE]");

	snprintf(buf+len, sizeof(buf)-len, "%c", c);

	if(c == '\n')
	{
		_LOG("%s", buf);
		memset(buf, 0, sizeof(buf));
	}
	//pthread_mutex_unlock(&mutex);
}

static int64_t safe_atoi(char* str, int endStrPtrIndex)
{
	if(str != NULL)
	{
		int len = strlen(str);

		if(len < endStrPtrIndex && len > 0)
		{
			char* endptr = NULL;
			int64_t rr = strtol(str, &endptr, 10);
			/*
			if (endptr == str) {
				printf("No digits were found.\n");
			}
			else if (*endptr != '\0') {
				printf("Invalid character: %c\n", *endptr);
			}
			else {
			//valid num
			}*/

			return rr;
		}
		else
		{
			_LOG("Exception : atoi overflow str %s - %d len %d \n" , str, endStrPtrIndex, len);
		}
	}
	return 0;
}

static int file_exist(const char* path)
{
	struct stat buffer;
	return (stat (path, &buffer) == 0) ? TRUE : FALSE;
}

static int64_t file_size(const char* file_path)
{
	struct stat buf;
	int ret = -1;

	ret = stat(file_path, &buf);
	if(ret < 0)
	{
		_LOG("stat return %d, errno=%d,%s, P1=%s \n", ret, errno, strerror(errno), (char*)file_path);
		return ret;
	}
	return buf.st_size;
}

static FILE* file_exist_or_create(char* file_name){
	FILE* fd = fopen(file_name, "r+");
	if(fd == NULL)
	{
		//file doesn't exist, so create it
		fd = fopen(file_name, "w");
	}
	return fd;
}

static int save_dl_file_state(char* file_path, void* data, int data_len){

	int written = -1;
	FILE* fd = file_exist_or_create(file_path);
	if(fd != NULL)
	{
		written = fwrite(data, data_len, 1, fd);
		fclose(fd);
	}
	return written;
}

static int get_dl_file_state(const char* file_path, void* data, int data_len){

	int read = -1;
	FILE* fd = fopen(file_path, "r+");
	if(fd != NULL)
	{
		read = fread(data, data_len, 1, fd);
		fclose(fd);
	}
	return read;
}


static void get_http_header(struct mg_http_message *hm, const char* header, char* val)
{
	char buff[BUFF_SIZE] = {0};
    struct mg_str* mg = mg_http_get_header(hm, header);
    if (mg->buf && mg->len > 0)
    {
        // Copy only up to available space minus 1 for null terminator
        size_t n = mg->len < sizeof(buff)-1 ? mg->len : sizeof(buff)-1;
        memcpy(buff, mg->buf, n);
        buff[n] = '\0'; // Null-terminate
        strcpy(val, buff);
    }
}

static void http1_file_dl(struct mg_connection *c,  char* addr, char* dst_file_name, int is_new_attempt)
{
	int64_t range_start=0, range_end = 0;
	int len = 0;
	struct mg_str host = mg_url_host(addr);
	char buff[BUFF_SIZE] = {0};
	t_dl_args* ptr =(t_dl_args*)c->fn_data;

	t_dl_file_state state;
	get_dl_file_state(FILE_CNF, &state, sizeof(t_dl_file_state));

	int exist = file_exist(dst_file_name);
	if(exist == TRUE)
	{
		range_start = file_size(dst_file_name);
		range_end = (state.n_total_size - range_start > HTTP_BUFFER_SIZE) ? (HTTP_BUFFER_SIZE + range_start) : (state.n_total_size);
	}

	//NOTE
	//start to resume from one last part - if there are any incomplete buffer from prev download
	if(exist==TRUE && is_new_attempt==TRUE)
	{
		if(range_start > HTTP_BUFFER_SIZE)
			range_start = range_start - HTTP_BUFFER_SIZE;

		range_end = (state.n_total_size - range_start > HTTP_BUFFER_SIZE) ? (HTTP_BUFFER_SIZE + range_start) : (state.n_total_size);
		_LOG("re-check.....re-range....[%ld-%ld] \n", range_start, range_end);
	}

	if(exist==FALSE || (is_new_attempt == TRUE))
	{
		//NOTE
		//Send HTTP GET request with Range header
		len = snprintf(buff,
				  sizeof(buff),
				  "GET %s HTTP/1.1\r\n"
				  "Range: bytes=%ld-%ld\r\n"
				  "Accept: */*\r\n"
				  "Accept-Encoding: gzip, deflate, br\r\n"
				  "Connection: keep-alive\r\n"
				  "\r\n",
				  mg_url_uri(addr),
				  (int) host.len,
				  host.buf,
				  range_start,
				  range_end
			 );

		//NOTE
		//this way we avoid extra download request, because we could not determine
		//when the body data is complete

		if(memcmp(ptr->cache.data, buff, BUFF_SIZE) != 0 || ptr->cache.is_set == FALSE)
		{
			mg_send(c, buff, len);
			memcpy(ptr->cache.data, buff, BUFF_SIZE);
			ptr->cache.is_set = TRUE;
		}
		else if (memcmp(ptr->cache.data, buff, sizeof(buff)) == 0)
		{
			_LOG("1 duplicated request .. skip it \n");
		}
		else
		{
			_LOG("1 unknown state... \n");
		}

		_LOG("Try to get Actual Size from %d-%d \n", range_start, range_end);
		ptr->n_end_range = range_end;
		ptr->n_start_range = range_start;
	}
	else if(exist == TRUE)
	{
		if(ptr->dl_with_resume == FALSE)
		{
			// Send HTTP GET request with Range header
			len = snprintf(buff,
					  sizeof(buff),
					  "GET %s HTTP/1.1\r\n"
					  "Host: %.*s\r\n"
					  "Connectn: close\r\n"
					  "\r\n",
					  mg_url_uri(addr),
					  (int) host.len,
					  host.buf
				 );

			//to clean if anything exist
			remove(dst_file_name);
			_LOG("Start a New Download... \n");

			if(memcmp(ptr->cache.data, buff, sizeof(buff)) != 0 || ptr->cache.is_set == FALSE)
			{
				mg_send(c, buff, len);
				memcpy(ptr->cache.data, buff, sizeof(buff));
				ptr->cache.is_set = TRUE;
			}
			else if (memcmp(ptr->cache.data, buff, sizeof(buff)) == 0)
			{
				_LOG("2 duplicated request .. skip it \n");
			}
			else
			{
				_LOG("2 unknown state... \n");
			}
		}
		else
		{
			len = snprintf(buff,
					sizeof(buff),
					"GET %s HTTP/1.1\r\n"
					"Host: %.*s\r\n"
					"Range: bytes=%ld-%ld\r\n"
					"Connectn: keep-alive\r\n"
					"\r\n",
					mg_url_uri(addr),
					(int) host.len,
					host.buf,
					range_start,
					range_end
				);

			//NOTE
			//this way we avoid extra download request, because we could not determine
			//when the body data is complete

			if(memcmp(ptr->cache.data, buff, sizeof(buff)) != 0 || ptr->cache.is_set == FALSE)
			{
				mg_send(c, buff, len);
				memcpy(ptr->cache.data, buff, sizeof(buff));
				ptr->cache.is_set = TRUE;
			}
			else if(memcmp(ptr->cache.data, buff, sizeof(buff)) == 0)
			{
				_LOG("3 duplicated request .. skip it");
			}
			else
			{
				_LOG("3 unknown state...");
			}
		}

		ptr->n_end_range = range_end;
		ptr->n_start_range = range_start;
	}


    if (c->send.len > 0) {
        _LOG("--- Sending Data --- Try %d \n", ptr->n_retry_count);
        // Log the data directly from the send buffer
        _LOG("%.*s\n", (int) c->send.len, c->send.buf);
        _LOG("----------------------\n");
    }
}

static void check_idle_timer(struct mg_connectn *c, int ev)
{
	t_dl_args* ptr =(t_dl_args*)c->fn_data;
	switch(ev)
	{
	case MG_EV_POLL:       //
        if (time(NULL) - ptr->last_ > IDLE_TIMEOUT) {
            c->is_closing = done_flag = TRUE;
        }
		break;
	case MG_EV_CONNECT:    //
	case MG_EV_READ:       //
	case MG_EV_WRITE:      //
	case MG_EV_CLOSE:      //
	case MG_EV_HTTP_HDRS:  //
	case MG_EV_HTTP_MSG:   //
		ptr->last_io = time(NULL);
		break;
	}

}

static void log_event_type(int ev)
{
	switch(ev)
	{
	case MG_EV_ERROR:
		_LOG("[MONGOOSE] MG_EV_ERROR Error char *error_message \n");
		break;
	case MG_EV_OPEN:       //
		_LOG("[MONGOOSE] MG_EV_OPEN Connection created \n");
		break;
	case MG_EV_POLL:       //
		_LOG("[MONGOOSE] MG_EV_POLL mg_mgr_poll iteration  uint64_t *uptime_millis \n");
		break;
	case MG_EV_RESOLVE:    //
		_LOG("[MONGOOSE] MG_EV_RESOLVE Host name is resolved  \n");
		break;
	case MG_EV_CONNECT:    //
		_LOG("[MONGOOSE] MG_EV_CONNECT Connection established  \n");
		break;
	case MG_EV_ACCEPT:     //
		_LOG("[MONGOOSE] MG_EV_ACCEPT Connection accepted   \n");
		break;
	case MG_EV_TLS_HS:     // ;
		_LOG("[MONGOOSE] MG_EV_TLS_HS  TLS handshake succeeded  _LOG(\" MG_EV_ERROR Error char *error_message\") \n");
		break;
	case MG_EV_READ:       //
		_LOG("[MONGOOSE] MG_EV_READ Data received from socket long *bytes_read \n");
		break;
	case MG_EV_WRITE:      //
		_LOG("[MONGOOSE] MG_EV_WRITE  Data written to socket long *bytes_written \n");
		break;
	case MG_EV_CLOSE:      //
		_LOG("[MONGOOSE] MG_EV_CLOSE Connection closed \n");
		break;
	case MG_EV_HTTP_HDRS:  //
		_LOG("[MONGOOSE] MG_EV_HTTP_HDRS HTTP headers struct mg_http_message * \n");
		break;
	case MG_EV_HTTP_MSG:   //
		_LOG("[MONGOOSE] MG_EV_HTTP_MSG Full HTTP request/response   struct mg_http_message * \n");
		break;
	case MG_EV_WS_OPEN:    //
		_LOG("[MONGOOSE] MG_EV_WS_OPEN Websocket handshake done  struct mg_http_message * \n");
		break;
	case MG_EV_WS_MSG:     //
		_LOG("[MONGOOSE] MG_EV_WS_MSG Websocket msg, text or bin  struct mg_ws_message * \n");
		break;
	case MG_EV_WS_CTL:     //
		_LOG("[MONGOOSE] MG_EV_WS_CTL Websocket control msg  struct mg_ws_message * \n");
		break;
	case MG_EV_MQTT_CMD:   //
		_LOG("[MONGOOSE] MG_EV_MQTT_CMD MQTT low-level command struct mg_mqtt_message * \n");
		break;
	case MG_EV_MQTT_MSG:   //
		_LOG("[MONGOOSE] MG_EV_MQTT_MSG MQTT PUBLISH received struct mg_mqtt_message * \n");
		break;
	case MG_EV_MQTT_OPEN:  //
		_LOG("[MONGOOSE] MG_EV_MQTT_OPEN MQTT CONNACK received int *connack_status_code \n");
		break;
	case MG_EV_SNTP_TIME:  //
		_LOG("[MONGOOSE] MG_EV_SNTP_TIME SNTP time received uint64_t *epoch_millis \n");
		break;
	case MG_EV_WAKEUP:     //
		_LOG("[MONGOOSE] MG_EV_WAKEUP mg_wakeup() data received struct mg_str *data \n");
		break;
	case MG_EV_USER:        //
		_LOG("[MONGOOSE] MG_EV_USER Starting ID for user events \n");
		break;
	}
}

static size_t append_file_chunk(const char* path, int offset, int chunk_size, uint8_t* chunk_array)
{
	FILE *f;
	int status = FALSE;
	size_t ret = 0;

	_LOG("Byte by Byte Append With offset... offset %d Chunk size %d \n", offset, chunk_size);

	f = fopen(path, "r+");
	if(f != NULL)
	{
		fseek(f, offset, SEEK_SET);
		int64_t destFileSize = ftell(f);

		ret = fwrite(chunk_array, sizeof(uint8_t), chunk_size, f); //write to new
		if (ret == chunk_size)
		{
			status = TRUE;
		}
		else
		{
			_LOG("Error while writing bytes::: %d \n", ret);
		}

		fseek(f, 0, SEEK_END);
		destFileSize = ftell(f);

		_LOG("Size After Write Bytes::: %d \n", destFileSize);
		fclose(f);
	}
	return status;
}

static void base_name(const char* path,  char* file_name,  char* base_path)
{
	char tmpName[128] = {0};
	char tmpPath[128] = {0};

	if(path == NULL || file_name == NULL)
		return;

	if(strlen(path) > 0)
	{
		for (int i=0; i<(int)strlen(path); i++)
		{
			if (path[i] == '/' /* || path[i]  == '//' || path[i]  == '\\' || */ || path[i] == ':')
			{
				memset(tmpName, 0, sizeof(tmpName));
				memset(tmpPath, 0, sizeof(tmpPath));

				strcpy(tmpName, &path[i]+1);
				strncpy(tmpPath, path, i+1);
			}
		}
	}

	if(base_path != NULL)
		strcpy(base_path, tmpPath);

	strcpy(file_name, tmpName);
}

static void event_callback(struct mg_connection *c, int ev, void *ev_data)
{
	t_dl_args* ptr =(t_dl_args*)c->fn_data;
	int offset = 0;

	if(ptr == NULL)
	{
		_LOG("NULL dl args provided \n");
		return;
	}

	log_event_type(ev);
	check_idle_timer(c, ev);

    switch(ev)
    {
    	case MG_EV_CONNECT:
    	{
    		//NOTE
    		//it was tested on embedTLS with self-signed cert

    		//TODO
    		//test with real cert
    		if(c->is_tls)
    		{
        	    memset(&opts, 0, sizeof(opts));

        	    extern struct mg_fs mg_fs_posix;
        	    opts.ca = mg_file_read(&mg_fs_posix, PUBLIC_CERT);

        	    const char* dummy_name = "localhost";
        	    opts.name.buf = (char*)dummy_name;
        	    opts.name.len = strlen(dummy_name);

        	    //for self-signed
        	    opts.skip_verification = TRUE;

        	    mg_tls_init(c, &opts);
    		}
    		else
    		{
    			http1_file_dl(c, ptr->dl_addr, ptr->file_path, TRUE);
    		}
    	}
    	break;
    	case MG_EV_POLL:
    	{
            // Check if there is data in the send queue
    	}
    	break;
    	case MG_EV_TLS_HS:
    	{
    		http1_file_dl(c, ptr->dl_addr, ptr->file_path, TRUE);
    	}
    	break;

    	case MG_EV_HTTP_MSG:
    	case MG_EV_HTTP_HDRS:
		{
			struct mg_http_message *hm = (struct mg_http_message *) ev_data;

			// Check the status code.
			switch (mg_http_status(hm))
			{
			    case 200:
			    {
			    	//NOTE
			    	//DOWNLOAD WITH NO RESUME
			    	ptr->dl_with_resume = FALSE;
					_LOG("[200] Starting new download (server may support or may not support resuming) \n");
					//TODO add it in future
					c->is_closing = done_flag = TRUE;
				}
			    break;
			    case 206:
				{
			    	//NOTE DOWNLOAD WITH RESUME
					ptr->dl_with_resume = TRUE;

					struct mg_str* content_range = mg_http_get_header(hm, "Content-Range");

					int64_t total_size = 0, start_range =0 , nEndRange =0, nContentLen = 0;

			        char val[32] = {0};
			        get_http_header(hm, "Content-Length", val);
			        nContentLen = safe_atoi(val, strlen(val)+1);

					_LOG("[206] Resuming download from byte ... Content-Length %lld BodyLength %ld - recv %lld \n", nContentLen, (int64_t)hm->body.len, c->recv.len);
				    sscanf(content_range->buf, "bytes %lld-%lld/%lld", &start_range, &nEndRange, &total_size);

				    int64_t filesize = file_size(ptr->file_path);

		    		int exist = file_exist(ptr->file_path);
		    		if(exist == FALSE)
		    		{
		    			char base_path[BUFF_SIZE] = {0};
		    			char file_name[BUFF_SIZE] = {0};
		    			base_name(ptr->file_path, file_name, base_path);

		    			mkdir(base_path, S_IRWXU);

		    			//NOTE: OPEN OR CREATE mode
		    			int created = FALSE;
		    			FILE* fd = file_exist_or_create(ptr->file_path);
		    			if(fd != NULL)
		    			{
		    				created = TRUE;
		    				fclose(fd);
		    			}

		    			//-----------------------------------
		    			_LOG("total_size saved %d created %d...CONTINUE... \n", created, total_size, created);
					}
		    		/*
		    		else if(nTotalSize == filesize-offset)
		    		{
		    			_LOG("done");
		    			c->is_closing = TRUE;
		    		}
		    		*/

		    		//TODO : separated
		    		if( done_flag == FALSE && (total_size >= filesize) /* && (filesize < nEndRange) */ && (nContentLen == hm->body.len))
		    		{
		    	        // Log the entire response to the console
		    			_LOG("--- Received HTTP Response ---\n");
		    	        _LOG("%.*s\n", (int) hm->message.len, hm->message.buf);
		    	        _LOG("------------------------------\n");
						_HEX_LOG(hm->body.buf, hm->body.len);

						int64_t appended = append_file_chunk((const char*)ptr->file_path, start_range, (int64_t)hm->body.len, (uint8_t*)hm->body.buf);

						t_dl_file_state state;
						get_dl_file_state(FILE_CNF, &state, sizeof(state));
						_LOG("appended %d - data exist %d \n", appended, state.n_file_part);

						state.n_total_size = total_size;
						state.n_file_part = start_range;
						save_dl_file_state(FILE_CNF, &state, sizeof(state));

						if(total_size > filesize-offset)
						{
							//get next parts
							http1_file_dl(c, ptr->dl_addr, ptr->file_path, FALSE);
						}
						else if(total_size < filesize-offset)
						{
							_LOG("file size exception \n");
							c->is_closing = done_flag = TRUE;
						}
						else
						{
							_LOG("http => done done done \n");
							//_LOG("Waiting for extra recv buffer Timer %d", System::timerCheck(TMS_TIMER_INDEX));
							c->is_closing = done_flag = TRUE;
						}
		    		}
		    		else
		    		{
		    			_LOG("no action needed - incompelte http message - filesize %d Total %d startRange %d endRange %d \n", filesize, total_size, start_range, nEndRange);
		    		}
				}
				break;

		        case 301:
		        case 302:
		        {
		        	//TODO
		        	// This part is not tested - if you want to follow the redirect and finally download the file
		            char url[BUFF_SIZE]= {0};

		            struct mg_str* re_location = mg_http_get_header(hm, "Location");
		            if(re_location != NULL && re_location->len > 0)
		            {
		                //ctx->redirects++;
		                _LOG("Redirecting to: %s \n", re_location->buf);
		                c->is_closing = TRUE;

		                //static struct mg_mgr smgr;
		                memset(ptr->dl_addr, 0, sizeof(ptr->dl_addr));
		                strcpy(ptr->dl_addr, re_location->buf);

		                mg_http_connect(c->mgr, url, event_callback, ptr);
		                return;
		            }
		        }
		        break;

			    default:
			    {
			    	_LOG("[%003d] download failed with http status code \n", mg_http_status(hm));
					c->is_closing = done_flag = TRUE;
			    }
			    break;
			}
		}
		break;
    	case MG_EV_CLOSE:
    	{
    		_LOG("[MONGOOSE] Try to Close the Connection \n");
			c->is_closing = done_flag = TRUE;
    	}
		break;
    	case MG_EV_ERROR:
    	{
			_LOG("[MONGOOSE] errno = [%d], strerror => %s \n", errno, strerror(errno));
			c->is_closing = done_flag = TRUE;
    	}
    	break;
    	default:
    	{
    		_LOG("[MONGOOSE] unhandled case \n");
    	}
    	break;
    }

    _LOG("END loop \n");
}

void clear_all(const char* cnf_path, const char* file_path){
	if(cnf_path != NULL)
		remove(cnf_path);

	if(file_path != NULL)
		remove(file_path);
}

int http1_download(const char* addr, const char* path, int clear)
{
	t_dl_args dl_args;
	int result = FALSE;
	struct mg_mgr mgr;

	memset(&dl_args, 0, sizeof(dl_args));
	strcpy(dl_args.dl_addr, addr);
	strcpy(dl_args.file_path, path);

	if(clear == TRUE)
		clear_all(FILE_CNF, path);

	int k = 0;
	while(k < RETRY_COUNT)///////////////
	{
		///////////////
		done_flag = FALSE;

		mg_mgr_init(&mgr);  // Initialize the event manager.
		mg_log_set_fn(my_log, stderr);
		mg_log_set(MG_LL_VERBOSE);

		//NOTE to start timer
		dl_args.last_io = time(NULL);
		mg_http_connect(&mgr, addr, event_callback, &dl_args);

		// Main event loop.
		while(done_flag == FALSE){
			mg_mgr_poll(&mgr, POLL_FOR_EVENTS_TIME);
		}

		mg_mgr_free(&mgr);  // Clean up resources.
		k++;

		t_dl_file_state state;
		get_dl_file_state(FILE_CNF, &state, sizeof(state));

		if(file_size(dl_args.file_path) == state.n_total_size)
		{
			_LOG("dl is done ");
			result = TRUE;
			break;
		}
	}

	return result;
}

int main(int argc, char **argv) {

	//TRUE for clean download
	//FALSE for resume download case

	//TODO add argv to get download_url and clear flag from cmd

	_LOG("Mongoose Simple Http Download Manager");
	http1_download(TEST_URL, FILE_PATH, TRUE);
}
