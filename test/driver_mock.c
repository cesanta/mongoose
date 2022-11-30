static bool mock_init(uint8_t *mac, void *data) {
  (void) mac, (void) data;
  return true;
}

static size_t mock_tx(const void *buf, size_t len, void *data) {
  (void) buf, (void) len, (void) data;
  return len;
}

static size_t mock_rx(void *buf, size_t len, void *data) {
  (void) buf, (void) len, (void) data;
  return 0;
}

static bool mock_up(void *data) {
  (void) data;
  return true;
}

struct mip_driver mip_driver_mock = {mock_init, mock_tx, mock_rx, mock_up, 0};
