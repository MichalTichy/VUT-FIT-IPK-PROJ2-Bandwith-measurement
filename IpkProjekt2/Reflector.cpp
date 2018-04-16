#include "Reflector.h"
#include "Socket.h"
#include <netinet/in.h>


Reflector::Reflector(int port): port(port) {
}


Reflector::~Reflector()
{
}

void Reflector::Start()
{

	sockaddr_in client_address, server_address; 
	socklen_t clientlen = sizeof(client_address);

	auto socket = new Socket();
	socket->attachSocket(port);
	while (true)
	{

		std::string rawDataFromClient;
		socket->recieve(rawDataFromClient, &client_address);
		socket->send(rawDataFromClient, client_address);
	}
}
