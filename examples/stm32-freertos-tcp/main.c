// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "device.h"
#include "mongoose.h"

static const char *s_debug_level = "2";
static const char *s_listening_address = "http://0.0.0.0:80";

// Event handler for the listening connection.
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "hello, %s!\n", "world");
  }
  (void) fn_data;
  (void) ev_data;
}

static void server(void *args) {
  struct mg_mgr mgr;
  mg_log_set(s_debug_level);
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_listening_address, cb, &mgr);  // Web listener
  while (args == NULL) mg_mgr_poll(&mgr, 1000);         // Infinite event loop
  mg_mgr_free(&mgr);                                    // Unreachable
}

static void blinker(void *args) {
  uint16_t pin = ((char *) args)[0] == '1' ? LED2 : LED3;
  int ms = pin == LED2 ? 750 : 1500;
  for (;;) {
    gpio_toggle(pin);
    vTaskDelay(pdMS_TO_TICKS(ms));
  }
}

int main(void) {
  init_hardware();
  xTaskCreate(server, "server", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(blinker, "blinker", 128, "1", configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(blinker, "blinker", 128, "2", configMAX_PRIORITIES - 1, NULL);
  vTaskStartScheduler();  // This blocks
  return 0;               // Unreachable
}

// clang-format off
// Stubs for FreeRTOS-TCP network interface
BaseType_t xNetworkInterfaceInitialise(void) { return 0; } 
BaseType_t xNetworkInterfaceOutput(NetworkBufferDescriptor_t *const pxNetworkBuffer, BaseType_t xReleaseAfterSend) { (void)pxNetworkBuffer;  (void) xReleaseAfterSend; return 0; }
void vNetworkInterfaceAllocateRAMToBuffers(NetworkBufferDescriptor_t pxNetworkBuffers[ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS]) { (void) pxNetworkBuffers; }
BaseType_t xGetPhyLinkStatus(void) { return 0; }
void pxGetNetworkBufferWithDescriptor(void){}
void vReleaseNetworkBufferAndDescriptor(void){}
void xNetworkBuffersInitialise(void) {}
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t ev){(void) ev;}
BaseType_t xApplicationGetRandomNumber( uint32_t * p){ *p = 0; return 1;}
uint32_t ulApplicationGetNextSequenceNumber(uint32_t a, uint16_t b, uint32_t c, uint16_t d){return a^b^c^d;}
void xBufferAllocFixedSize(void){}

// Libc stubs
extern uint32_t _data_end;
void *_sbrk(int incr) {
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;
  if (heap == NULL) heap = (unsigned char *) &_data_end;
  prev_heap = heap;
  heap += incr;
  return prev_heap;
}
int _close(int fd) { (void) fd; return -1; }
int _fstat(int fd, struct stat *st) { (void) fd; st->st_mode = S_IFCHR; return 0; }
int _isatty(int fd) { (void) fd; return 0; }
int _lseek(int fd, int ptr, int dir) { (void) fd; (void) ptr; (void) dir; return 0; }
void _exit(int status) { (void) status; __asm("BKPT #0"); for(;;); }
void _kill(int pid, int sig) { (void) pid; (void) sig; }
int _getpid(void) { return -1; }
int _write(int fd, char *ptr, int len) { (void) fd; (void) ptr; (void) len; return -1; }
int _read(int fd, char *ptr, int len) { (void) fd; (void) ptr; (void) len; return -1; }
int _gettimeofday(struct timeval *tv, void *tz) { (void) tz; unsigned long ms = mg_millis(); tv->tv_sec = ms / 1000; tv->tv_usec = ms % 1000; return 0;};
// clang-format on
