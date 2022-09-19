static bool my_random(void) {
  return mg_millis() & 1;
}

static bool mock_init(uint8_t *mac, void *data) {
  (void) mac, (void) data;
  return my_random();
}

static size_t mock_tx(const void *buf, size_t len, void *data) {
  (void) buf, (void) len, (void) data;
  return len;
}

static size_t mock_rx(void *buf, size_t len, void *data) {
  (void) buf, (void) len, (void) data;
  return len;
}

static bool mock_up(void *data) {
  (void) data;
  return my_random();
}

struct mip_driver mip_driver_mock = {mock_init, mock_tx, mock_rx, mock_up, 0};
