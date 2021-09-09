#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int connect_to_server(char *hostname, int port) {
	int fd;
	struct hostent *hp;
	struct sockaddr_in serveraddr;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	if ((hp = gethostbyname(hostname)) == NULL)
		return -2;

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	return fd;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <hostname>\n", argv[0]);
		return 0;
	}

	char request[1000];
	char response[1000];

	int fd = connect_to_server(argv[1], 80);
	printf("Socket file descriptor: %d\n", fd);
	
	sprintf(request, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", argv[1]);
	write(fd, request, strlen(request));
	puts(request);
	
	while (read(fd, response, sizeof(response)-1) > 0) 
		puts(response);
	
	return 0;
}