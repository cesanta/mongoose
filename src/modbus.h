#pragma once

#include "net.h"

// Command codes
#define MG_MODBUS_COMMAND_READ_COILS 1
#define MG_MODBUS_COMMAND_READ_DISCRETE_INPUTS 2
#define MG_MODBUS_COMMAND_READ_HOLDING_REGISTERS 3
#define MG_MODBUS_COMMAND_READ_INPUT_REGISTERS 4
#define MG_MODBUS_COMMAND_WRITE_SINGLE_COIL 5
#define MG_MODBUS_COMMAND_WRITE_SINGLE_REGISTER 6
#define MG_MODBUS_COMMAND_WRITE_MULTIPLE_COILS 15
#define MG_MODBUS_COMMAND_WRITE_MULTIPLE_REGISTERS 16

// Error codes
#define MG_MODBUS_ERROR_NONE 0
#define MG_MODBUS_ERROR_ILLEGAL_FUNCTION 1
#define MG_MODBUS_ERROR_ILLEGAL_ADDRESS 2
#define MG_MODBUS_ERROR_ILLEGAL_VALUE 3
#define MG_MODBUS_ERROR_ILLEGAL_DEVICE_FAILURE 4

struct mg_modbus_cmd {
  uint8_t command;    // Command code, one of MG_MODBUS_COMMAND_*
  uint8_t error;      // Error code - user handler can set
  uint16_t address;   // Address
  uint16_t *values;   // Values
  size_t num_values;  // Number of values
};

struct mg_connection *mg_modbus_listen(struct mg_mgr *mgr, const char *url,
                                       mg_event_handler_t fn, void *fn_data);
