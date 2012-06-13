#include <iostream.h>
#include <cmath.h>
using namespace std;

int sock;










void build_select_list();

void setnonblocking(int sock);

int main(char* args[]) {
	char *ascport;
	int port;
	struct sockaddr_in server_address;
	int reuse_addr = 1;
	struct timeval timout;
	int readsocks;

	if (argc < 2) {
		printf("usage: %s port\r\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));

	setnonblocking(sock);

	ascport = malloc(sizeof char * 6);
	memcpy(ascport, argv[1], sizeof(*ascport));

	port = atoi(ascport);

	memset((char *) &server_address, 0, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = port;
	if (bind (sock, (struct sockaddr *) &server_address, sizeof(server_address)) <0) {
		perror("bind");
		close(sock);
		exit(EXIT_FAILURE);
	}

	listen(sock, 5);

	highsock = sock;
	memset((char *) &connectlist, 0 , sizeof(connectlist));

	while(1) {
		
	}
}

void build_select_list() {
	int listnum;

	FD_ZERO(&socks);

	FD_SET(sock, &socks);

	for(listnum = 0; listnum < 5; listnum++) {
		if(connectlist[listnum] != 0) {
			FD_SET(connectlist[listnum], &socks);
			if(connectlist[listnum] > highsock) 
				highsock = connectlist[listnum];
		}
	}
}

void setnonblocking(int sock) {
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}
