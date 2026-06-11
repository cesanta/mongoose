#pragma once

#include "net.h"

// Modbus TCP function codes passed in mg_modbus_req.func.
#define MG_MODBUS_FUNC_READ_COILS 1             // Read 1-2000 discrete output coils
#define MG_MODBUS_FUNC_READ_DISCRETE_INPUTS 2   // Read 1-2000 discrete input coils
#define MG_MODBUS_FUNC_READ_HOLDING_REGISTERS 3 // Read 1-125 holding registers
#define MG_MODBUS_FUNC_READ_INPUT_REGISTERS 4   // Read 1-125 input registers
#define MG_MODBUS_FUNC_WRITE_SINGLE_COIL 5      // Write one output coil
#define MG_MODBUS_FUNC_WRITE_SINGLE_REGISTER 6  // Write one holding register
#define MG_MODBUS_FUNC_WRITE_MULTIPLE_COILS 15  // Write 1-1968 output coils
#define MG_MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS 16 // Write 1-123 holding registers

// Standard Modbus exception codes. Set mg_modbus_req.error to one of these
// in the MG_EV_MODBUS_REQ handler to return an exception response to the client.
#define MG_MODBUS_ERR_NONE 0             // No error; send a normal response
#define MG_MODBUS_ERR_ILLEGAL_FUNCTION 1 // Function code not supported
#define MG_MODBUS_ERR_ILLEGAL_ADDRESS 2  // Starting address or range out of bounds
#define MG_MODBUS_ERR_ILLEGAL_VALUE 3    // Data value not accepted
#define MG_MODBUS_ERR_DEVICE_FAILURE 4   // Unrecoverable device error

// Modbus request passed as ev_data to the MG_EV_MODBUS_REQ event handler.
//
// For READ functions (READ_COILS, READ_DISCRETE_INPUTS, READ_HOLDING_REGISTERS,
// READ_INPUT_REGISTERS): u.bits / u.regs points to a zeroed, heap-allocated
// array of len entries. The handler fills in the values to return.
//
// For WRITE functions: u.bits / u.regs is pre-populated from the client
// request. len=1 for single-write functions.
//
// Do NOT free u.bits or u.regs - the library frees them after the handler
// returns and the response has been sent.
//
// Set error to a non-zero MG_MODBUS_ERR_* code to send an exception response
// instead of a data response. The response is sent automatically on return.
struct mg_modbus_req {
  uint8_t func;   // Function code, one of MG_MODBUS_FUNC_*
  uint8_t error;  // Exception code; set to MG_MODBUS_ERR_* to return an error
  uint16_t addr;  // Starting coil/register address from the request
  union {
    bool *bits;      // Coil values; used by coil function codes
    uint16_t *regs;  // Register values; used by register function codes
  } u;
  uint16_t len;  // Number of entries in u.bits or u.regs
};

// Creates a Modbus TCP listener on url. Fires MG_EV_MODBUS_REQ for each
// complete request; ev_data is struct mg_modbus_req *. Returns the listening
// connection, or NULL on error.
struct mg_connection *mg_modbus_listen(struct mg_mgr *mgr, const char *url,
                                       mg_event_handler_t fn, void *fn_data);
