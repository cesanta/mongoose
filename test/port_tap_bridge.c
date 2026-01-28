#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

#ifndef __OpenBSD__
#include <linux/if.h>
#include <linux/if_tun.h>
#else
#include <net/if.h>
#include <net/if_tun.h>
#include <net/if_types.h>
#endif

static int s_signo;
void signal_handler(int signo) {
  s_signo = signo;
}

int main(void) {
  // Setup interface
  const char *iface = "tap0";             // Network iface
  const uint16_t port = 0x55AA;           // UDP port
#ifndef __OpenBSD__
  const char *tuntap_device = "/dev/net/tun";
#else
  const char *tuntap_device = "/dev/tap0";
#endif
  int fd = open(tuntap_device, O_RDWR);
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, iface, IFNAMSIZ);
#ifndef __OpenBSD__
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
#else
  ifr.ifr_flags = (short) (IFF_UP | IFF_BROADCAST | IFF_MULTICAST);
  if (ioctl(fd, TUNSIFMODE, (void *) &ifr) < 0) {
#endif
    printf("Failed to setup TAP interface: %s", ifr.ifr_name);
    return EXIT_FAILURE;
  }
  printf("Opened TAP interface: %s\n", iface);

  int sockfd;
  struct sockaddr_in tap_addr, port_addr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	tap_addr.sin_family = AF_INET;
	tap_addr.sin_addr.s_addr = htonl(0x7f000001U);  // 127.0.0.1
	tap_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &tap_addr, sizeof(tap_addr)) < 0) return EXIT_FAILURE; 
  printf("Opened UDP socket: 127.0.0.1:%u (0x%04x)\n", port, port);

  memset(&port_addr, 0, sizeof(port_addr));

  while (s_signo == 0) {
    struct pollfd pfd[2];
		int result;
		do {
			pfd[0].fd = fd;
			pfd[0].events = POLLIN | POLLPRI;		// will wait for input available (read ready), use POLLOUT for write
			pfd[1].fd = sockfd;
			pfd[1].events = POLLIN | POLLPRI;		// will wait for input available (read ready), use POLLOUT for write
			
			// only read ready shown here, for write ready use the 3rd arg, and the 4th for exceptions
			result = poll(pfd, 2, 1000);		// 2 pfd, TIMEOUT in milliseconds
			switch(result) {
			case -1: 	// error
				break;
			case 0: 	// timeout (if a timeout struct was passed to select)
				break;
			default: 	// the number of fd/sockets with data available (see pfd.revents for each one);
        uint8_t buf[1540];
        if (pfd[0].revents & (POLLIN | POLLPRI)) {
          ssize_t nread = read(fd, buf, 1540);
//        printf("Got at TAP: %d bytes\n", (int) nread);
          if (port_addr.sin_family != 0) { // something has been written to this struct (we got a conterpart)
    				sendto(sockfd, buf, (size_t) nread, 0, (struct sockaddr *) &port_addr, sizeof(port_addr)); 
//  				printf("Sent to %s port %d\n", inet_ntoa(port_addr.sin_addr), ntohs(port_addr.sin_port));
          }
        }
        if (pfd[1].revents & (POLLIN | POLLPRI)) {
  				socklen_t len = sizeof(port_addr);
  				ssize_t nread = recvfrom(sockfd, buf, 1540, 0, (struct sockaddr *) &port_addr, &len); 
//        printf("Got at UDP: %d bytes from %s port %d\n", nread, inet_ntoa(port_addr.sin_addr), ntohs(port_addr.sin_port));
  				write(fd, buf, (size_t) nread);
// 				printf("Sent to TAP\n");
        }
				break;
			}
		} while (result >= 0);
    printf("looks like we got an error here... should I quit ? Waiting for CTRL-C\n");
	} 
  close(sockfd);
  close(fd);
}
