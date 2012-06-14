#ifndef __SocketClass_Included__
#define __SocketClass_Included__
#include <string>
#include <exception>
using namespace std;

//class declareation
class SocketException : public exception {
	public:
		SocketException(const string &message, bool inclSysMsg = false) throw();
		const char *what() const throw();
		~SocketException() throw();
	private:
		string userMessage;
};

//Socket Implementation
class Socket {
	public:
		string getAddress() throw(SocketException);
		string getPort() throw(SocketException);
		void setPort(unsigned short localPort) throw(SocketException);
		void setAddressAndPort(const string &Address, unsigned short Port=0) throw(SocketException);
		~Socket();
	private:
		Socket(const Socket &sock);
		void operator=(const Socket &sock);
	protected:
		int sockDesc;
		Socket(int type, int protocol) throw(SocketException);
		Socket(int sockDesc);
};
#endif
