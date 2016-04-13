// Use strace to monitor ifconfig().
// How does ifconfig get the IP address of the local interface(eth0)?
// Can you write code to do the same?

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/sockios.h>

int main() {
	struct sockaddr_in add;
	struct hostent* host;
	struct ifreq req;
	int sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(req.ifr_name, "eth0", IFNAMSIZ);
	if (ioctl(sock, SIOCGIFADDR, &req) < 0) {
		perror("get-ip");
		return -1;
	}
	printf("eth0 IP: %s\n", (char*)inet_ntoa(*(struct in_addr*)&((struct sockaddr_in *)&req.ifr_addr)->sin_addr));
	close(sock);
	return 0;
}

