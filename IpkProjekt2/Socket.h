#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef enum {
    IPv4,
    IPv6
} AddressType;


class Socket {
public:
    Socket();
    ~Socket();
	void InitSocket();
	void attachSocket(int port);

    int recieve(std::string& data, sockaddr_in* source);
	bool checkIfThereAreAnyData();

	void send(const std::string& data, sockaddr_in target);

    void closeSocket();
	sockaddr_in TranslateAddress(std::string server_hostname, int port);

private:
    int socketNumber;
	sockaddr_in address;
};


#endif
