static bool mock_init(struct mip_if *ifp) {
  (void) ifp;
  return true;
}

static size_t mock_tx(const void *buf, size_t len, struct mip_if *ifp) {
  (void) buf, (void) len, (void) ifp;
  return len;
}

static size_t mock_rx(void *buf, size_t len, struct mip_if *ifp) {
  (void) buf, (void) len, (void) ifp;
  return 0;
}

static bool mock_up(struct mip_if *ifp) {
  (void) ifp;
  return true;
}

struct mip_driver mip_driver_mock = {mock_init, mock_tx, mock_rx, mock_up};
