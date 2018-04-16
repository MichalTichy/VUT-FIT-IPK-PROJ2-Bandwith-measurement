#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Socket.h"
#include <string>
#include <cstring>
#include <unistd.h>

Socket::Socket() {
}

//in destructor check if socket is still open and then close it
Socket::~Socket() {
	if (this->socketNumber > 0)
		close(socketNumber);
}

void Socket::InitSocket()
{
	if ((this->socketNumber = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
}

void Socket::attachSocket(int port)
{
	int opt = 1;

	InitSocket();
	if (setsockopt(this->socketNumber, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->address.sin_port = htons(port);

	if (bind(this->socketNumber, (struct sockaddr *)&address,
		sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

int Socket::recieve(std::string &data, sockaddr_in* source) {
	char buffer[1024];
	unsigned long bytesRead = 0;
	socklen_t addr_len = sizeof(source);

	bytesRead = recvfrom(this->socketNumber, buffer, 1024, 0, reinterpret_cast<struct sockaddr *>(source), &addr_len);
	if (bytesRead < 0)
	{
		throw "Could not read from server.";
	}
	data.erase();
	//append data to data buffer from parameter
	data.append(buffer, bytesRead);
	return (int)bytesRead;
}

bool Socket::checkIfThereAreAnyData() {

	bool             res;
	fd_set          sready;
	struct timeval  nowait;

	FD_ZERO(&sready);
	FD_SET((unsigned int)this->socketNumber, &sready);
	//bzero((char *)&nowait,sizeof(nowait));
	memset((char *)&nowait, 0, sizeof(nowait));

	res = select(this->socketNumber + 1, &sready, NULL, NULL, &nowait);
	return (FD_ISSET(this->socketNumber, &sready));
}

void Socket::send(const std::string& data, sockaddr_in target)
{
	sendto(this->socketNumber, data.c_str(), data.length(), 0, reinterpret_cast<struct sockaddr *>(&target), sizeof(target));
}

void Socket::closeSocket() {
	close(this->socketNumber);
}

sockaddr_in Socket::TranslateAddress(std::string server_hostname, int port)
{

	struct hostent *server;
	struct sockaddr_in server_address;

	if ((server = gethostbyname(server_hostname.c_str())) == NULL) {
		fprintf(stderr, "ERROR: no such host as %s\n", server_hostname.c_str());
		exit(EXIT_FAILURE);
	}

	/* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
	bzero((char *)&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port);

	return server_address;

}

