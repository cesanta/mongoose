// https://arm-software.github.io/CMSIS_5/Driver/html/index.html

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CMSIS) && MG_ENABLE_DRIVER_CMSIS

#include "cmsis.h"
#include "net_builtin.h"
#include "queue.h"

extern ARM_DRIVER_ETH_MAC Driver_ETH_MAC0;
extern ARM_DRIVER_ETH_PHY Driver_ETH_PHY0;

static struct mg_tcpip_if *s_ifp;

static void mac_cb(uint32_t);
static bool cmsis_init(struct mg_tcpip_if *);
static bool cmsis_up(struct mg_tcpip_if *);
static size_t cmsis_tx(const void *, size_t, struct mg_tcpip_if *);
static size_t cmsis_rx(void *, size_t, struct mg_tcpip_if *);

struct mg_tcpip_driver mg_tcpip_driver_cmsis = {cmsis_init, cmsis_tx, NULL,
                                                cmsis_up};

static bool cmsis_init(struct mg_tcpip_if *ifp) {
  ARM_ETH_MAC_ADDR addr;
  s_ifp = ifp;

  ARM_DRIVER_ETH_MAC *mac = &Driver_ETH_MAC0;
  ARM_DRIVER_ETH_PHY *phy = &Driver_ETH_PHY0;
  ARM_ETH_MAC_CAPABILITIES cap = mac->GetCapabilities();
  if (mac->Initialize(mac_cb) != ARM_DRIVER_OK) return false;
  if (phy->Initialize(mac->PHY_Read, mac->PHY_Write) != ARM_DRIVER_OK)
    return false;
  if (cap.event_rx_frame == 0)  // polled mode driver
    mg_tcpip_driver_cmsis.rx = cmsis_rx;
  mac->PowerControl(ARM_POWER_FULL);
  if (cap.mac_address) {  // driver provides MAC address
    mac->GetMacAddress(&addr);
    memcpy(ifp->mac, &addr, sizeof(ifp->mac));
  } else {  // we provide MAC address
    memcpy(&addr, ifp->mac, sizeof(addr));
    mac->SetMacAddress(&addr);
  }
  phy->PowerControl(ARM_POWER_FULL);
  phy->SetInterface(cap.media_interface);
  phy->SetMode(ARM_ETH_PHY_AUTO_NEGOTIATE);
  return true;
}

static size_t cmsis_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ARM_DRIVER_ETH_MAC *mac = &Driver_ETH_MAC0;
  if (mac->SendFrame(buf, (uint32_t) len, 0) != ARM_DRIVER_OK) {
    ifp->nerr++;
    return 0;
  }
  ifp->nsent++;
  return len;
}

static bool cmsis_up(struct mg_tcpip_if *ifp) {
  ARM_DRIVER_ETH_PHY *phy = &Driver_ETH_PHY0;
  ARM_DRIVER_ETH_MAC *mac = &Driver_ETH_MAC0;
  bool up = (phy->GetLinkState() == ARM_ETH_LINK_UP) ? 1 : 0;  // link state
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {             // just went up
    ARM_ETH_LINK_INFO st = phy->GetLinkInfo();
    mac->Control(ARM_ETH_MAC_CONFIGURE,
                 (st.speed << ARM_ETH_MAC_SPEED_Pos) |
                     (st.duplex << ARM_ETH_MAC_DUPLEX_Pos) |
                     ARM_ETH_MAC_ADDRESS_BROADCAST);
    MG_DEBUG(("Link is %uM %s-duplex",
              (st.speed == 2) ? 1000
              : st.speed      ? 100
                              : 10,
              st.duplex ? "full" : "half"));
    mac->Control(ARM_ETH_MAC_CONTROL_TX, 1);
    mac->Control(ARM_ETH_MAC_CONTROL_RX, 1);
  } else if ((ifp->state != MG_TCPIP_STATE_DOWN) && !up) {  // just went down
    mac->Control(ARM_ETH_MAC_FLUSH,
                 ARM_ETH_MAC_FLUSH_TX | ARM_ETH_MAC_FLUSH_RX);
    mac->Control(ARM_ETH_MAC_CONTROL_TX, 0);
    mac->Control(ARM_ETH_MAC_CONTROL_RX, 0);
  }
  return up;
}

static void mac_cb(uint32_t ev) {
  if ((ev & ARM_ETH_MAC_EVENT_RX_FRAME) == 0) return;
  ARM_DRIVER_ETH_MAC *mac = &Driver_ETH_MAC0;
  uint32_t len = mac->GetRxFrameSize();  // CRC already stripped
  if (len >= 60 && len <= 1518) {        // proper frame
    char *p;
    if (mg_queue_book(&s_ifp->recv_queue, &p, len) >= len) {  // have room
      if ((len = mac->ReadFrame((uint8_t *) p, len)) > 0) {   // copy succeeds
        mg_queue_add(&s_ifp->recv_queue, len);
        s_ifp->nrecv++;
      }
      return;
    }
    s_ifp->ndrop++;
  }
  mac->ReadFrame(NULL, 0);  // otherwise, discard
}

static size_t cmsis_rx(void *buf, size_t buflen, struct mg_tcpip_if *ifp) {
  ARM_DRIVER_ETH_MAC *mac = &Driver_ETH_MAC0;
  uint32_t len = mac->GetRxFrameSize();  // CRC already stripped
  if (len >= 60 && len <= 1518 &&
      ((len = mac->ReadFrame(buf, (uint32_t) buflen)) > 0))
    return len;
  if (len > 0) mac->ReadFrame(NULL, 0);  // discard bad frames
  (void) ifp;
  return 0;
}

#endif
