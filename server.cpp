#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <server.h>
using namespace std;

//Prototypes
static void fillAddr(const string &address, unsigned short port, sockaddr_in &addr);

//Custom Exception
//constructor
SocketException::SocketException(const string &message, bool inclSysMsg) 
	throw() : userMessage(inclSysMsg ? message + ": "+strerror(errorno) : message) {
}

//destructor
SocketException::~SocketException() throw() {
}

//return user message
const char *SocketException::what() const throw() {
	  return userMessage.c_str();
}

//Socket Implementation
Socket::Socket(int type, int protocol) throw(SocketException){
	((sockDesc = socket(AF_INET, type, protocol))<0)?
		throw SocketException("Failed to create socket!",true):;
}

Socket::Socket(int sockDesc) {
	this->sockDesc = sockDesc;
}

string Socket::getAddress() throw(SocketException) {
	sockaddr_in addr;
	unsigned int addrLn = sizeof(addr);

	(getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addrLn)<0)?
		throw SocketException("Get Address Filed!", true):
		return inet_ntoa(addr.sin_addr);
}

string Socket::getPort() throw(SocketException) {
	sockaddr_in addr;
	unsigned int addrLn = sizeof(addr);

	(getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addrLn)<0)?
		throw SocketException("Get Port Filed!", true):
		return inet_ntoa(addr.sin_port);
}

void Socket::setPort(unsigned short localPort) throw(SocketException) {
	sockaddr_in Addr;
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Addr.sin_port = htons(localPort);

	(bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0)? 
		throw SocketException("Set port failed!", true):;
}

void Socket::setAddressAndPort(const string &Address, unsigned short Port=0) throw(SocketException) {
	sockaddr_in Addr;
	fillAddr(Address,Port,Addr);
	
	(bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0)?
		throw SocketException("Set address and port failed!", true):;
}

Socket::~Socket() {
	close(sockDesc);
	sockDesc=-1;
}


















