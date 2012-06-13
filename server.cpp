#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <errno.h>
using namespace std;

//class declareation
class SocketException : public exception {
	public:
		SocketException(const string &message, bool inclSysMsg = false) throw();
		~SocketException() throw();
		const char *what() const throw();
	private:
		string userMessage;
};

//constructor
SocketException::SocketException(const string &message, bool inclSysMsg) throw() : userMessage(message) {
	if (inclSysMsg) {
		userMessage.append(": ");
		userMessage.append(strerror(errno));
	}
}

//destructor
SocketException::~SocketException() throw() {
}

//return user message
const char *SocketException::what() const throw() {
	  return userMessage.c_str();
}


