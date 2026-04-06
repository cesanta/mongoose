#pragma once

#include "net.h"

// Functions
#define MG_MODBUS_FUNC_READ_COILS 1
#define MG_MODBUS_FUNC_READ_DISCRETE_INPUTS 2
#define MG_MODBUS_FUNC_READ_HOLDING_REGISTERS 3
#define MG_MODBUS_FUNC_READ_INPUT_REGISTERS 4
#define MG_MODBUS_FUNC_WRITE_SINGLE_COIL 5
#define MG_MODBUS_FUNC_WRITE_SINGLE_REGISTER 6
#define MG_MODBUS_FUNC_WRITE_MULTIPLE_COILS 15
#define MG_MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS 16

// Error codes
#define MG_MODBUS_ERR_NONE 0
#define MG_MODBUS_ERR_ILLEGAL_FUNCTION 1
#define MG_MODBUS_ERR_ILLEGAL_ADDRESS 2
#define MG_MODBUS_ERR_ILLEGAL_VALUE 3
#define MG_MODBUS_ERR_DEVICE_FAILURE 4

struct mg_modbus_req {
  uint8_t func;      // Function code, one of MG_MODBUS_FUNC_*
  uint8_t error;     // Error code user handler can set
  uint16_t addr;     // Address
  union {
    bool *bits;
    uint16_t *regs;
  } u;
  uint16_t len;      // Number of registers or bits
};

struct mg_connection *mg_modbus_listen(struct mg_mgr *mgr, const char *url,
                                       mg_event_handler_t fn, void *fn_data);
