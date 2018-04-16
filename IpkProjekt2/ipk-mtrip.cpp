#include "Reflector.h"
#include "Socket.h"
#include <netinet/in.h>
#include "Client.h"
#include <chrono>
#include <iostream>
#include <string>
#include <string>

int main(int argc, char* argv[])
{
	int port;
	std::string host;
	int sondSize;
	int time;
	bool meter = false;
	if (argc>2)
	{
		std::string arg1 = argv[1];
		if (arg1=="meter")
		{
			meter = true;
			if (argc==10)
			{
				host=argv[3];
				port=atoi(argv[5]);
				sondSize=atoi(argv[7]);
				time=atoi(argv[9]);

				if (time<=0)
				{
					exit(EXIT_FAILURE);
				}
				if (port<=0 || port>= 65535)
				{
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				exit(EXIT_FAILURE);
			}
		}
		else if(arg1=="reflect")
		{
			if (argc==4)
			{
				port = atoi(argv[3]);
			}
			else
			{
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	if (meter)
	{
		auto client = new Client();
		client->Start(host, port, sondSize, time);

	}
	else
	{
		auto reflector = new Reflector(port);
		printf("SERVER REFLECTOR AT ");
		printf(std::to_string(port).c_str());
		reflector->Start();
	}
}
