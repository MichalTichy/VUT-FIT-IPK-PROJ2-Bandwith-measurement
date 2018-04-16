#pragma once
#include <bits/stringfwd.h>
#include "Socket.h"
#include <functional>
#include <map>
#include <chrono>

class Client
{
public:
	Client();
	~Client();
	int CalcBandwith(std::size_t size, int sond_size, int time);
	int CalcAverageRtt(std::map<int, long> send, std::map<int,long> received);
	void Start(std::string host, int port, int sondSize, int testLenghtSEC);
private:
	std::string InsertPacketIdIntoData(std::string data, int packetId);
	int ExtractPacketIdFromData(std::string data);
	std::string GenerateData(int size);
};

