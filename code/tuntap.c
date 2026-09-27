#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *clonedev = "/dev/net/tun";
  int tapfd = open(clonedev, O_RDWR | O_CLOEXEC);
  if (tapfd < 0) {
    error(EXIT_FAILURE, errno, "open (%s)", clonedev);
  }
  struct ifreq ifr = {};
  char *tap_name = "tun0";
  strncpy(ifr.ifr_name, tap_name, IFNAMSIZ);
  ifr.ifr_flags = IFF_TUN;
  int len = 12;
  int r = ioctl(tapfd, TUNSETIFF, (void *)&ifr);
  if (r < 0) {
    close(tapfd);
    error(EXIT_FAILURE, errno, "ioctl");
  }
  while (true) {
  }
  return EXIT_SUCCESS;
}
