#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main(int argc,char* argv[]) {
	char *ascport;
	int port;
	struct sockaddr_in server_address;
	int reuse_addr = 1;
	struct timeval timeout;
	int readsocks;
	int sock;
	fd_set socks;
	int highsock;
	char buffer[1024];
	char *cur_char;
	int connectlist[5];

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

	ascport = (char *)malloc(sizeof(char)* 6);
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
		FD_ZERO(&socks);

		FD_SET(sock, &socks);

		for(int listnum = 0; listnum < 5; listnum++) {
			if(connectlist[listnum] != 0) {
				FD_SET(connectlist[listnum], &socks);
				if(connectlist[listnum] > highsock) 
					highsock = connectlist[listnum];
			}
		}

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		readsocks = select(highsock+1, &socks, (fd_set *) 0, (fd_set *) 0, &timeout);

		if(readsocks < 0) {
			perror("select");
			exit(EXIT_FAILURE);
		}
		if(readsocks == 0) {
			printf(".");
			fflush(stdout);
		}
		else {
			if(FD_ISSET(sock,&socks)){
				int connection;
				connection = accept(sock, NULL, NULL);
				if (connection < 0) {
					perror("accept");
					exit(EXIT_FAILURE);
				}

				for(int i = 0; (i < 5) && (connection != -1); i ++){
					if (connectlist[i] == 0) {
						printf("\nConnection accepted:   FD=%d; Slot=%d\n",
								connection,i);
						connectlist[i] = connection;
						connection = -1;
					}
					if (connection != -1) {
						printf("\nNo room left for new client.\n");
						close(connection);
					}
				}

				for (int j = 0; j < 5; j++) {
					if (FD_ISSET(connectlist[j],&socks)) {
						if (recv(connectlist[j],buffer,1024,0) <= 0) {
							printf("\nConnection lost: FD=%d;  Slot=%d\n",
									connectlist[j],j);
							close(connectlist[j]);
							connectlist[j] = 0;
						}
						else {
							printf("\nReceived: %s; ",buffer);
						}
					}
				}

			}
		}	
	}
}

