#ifndef CS_COMMON_PLATFORMS_ESP8266_STUBS_SLIP_H_
#define CS_COMMON_PLATFORMS_ESP8266_STUBS_SLIP_H_

#include <inttypes.h>

void SLIP_send(const void *pkt, uint32_t size);
uint32_t SLIP_recv(void *pkt, uint32_t max_len);

#endif /* CS_COMMON_PLATFORMS_ESP8266_STUBS_SLIP_H_ */
