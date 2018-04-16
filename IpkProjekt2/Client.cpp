#include "Client.h"
#include <bits/basic_string.h>
#include <cstring>
#include <string>
#include <list>
#include <netinet/in.h>
#include "Socket.h"
#include <map>
#include <chrono>
#include <bits/algorithmfwd.h>
#include <chrono>
#include <map>
#include <string>
#include <string>
#include <unistd.h>

Client::Client()
{
}


Client::~Client()
{
}

int Client::CalcBandwith(std::size_t size, int sond_size,int time)
{
	// two way trip => size *2
	return ((size * 2) * sizeof(char)*sond_size) / time;
}

int Client::CalcAverageRtt(std::map<int, long> send, std::map<int, long> received)
{
	std::list<int> RTTs = {};
	for (auto receivedData : received)
	{
		auto sendData = send[receivedData.first]/1000;
		auto duration = receivedData.second/1000;
		RTTs.push_back(duration - sendData);
	}

	long sum = 0;
	for (auto rtt : RTTs)
	{
		sum += rtt;
	}
	return sum / RTTs.size();
}

void Client::Start(std::string host, int port, int sondSize, int testLenghtSEC)
{
	
	int packedId = 0;
	auto defaultData = GenerateData(sondSize);

	std::map<int, long> packetSendTimes = {};
	std::map<int, long> packetReceiveTimes = {};

	auto socket = new Socket();
	socket->InitSocket();
	sockaddr_in addr = socket->TranslateAddress(host, port);
	bool t = socket->checkIfThereAreAnyData();
	std::string rawDataFromClient;
	std::chrono::duration<float> fs;

	std::chrono::time_point<std::chrono::system_clock>::duration start = std::chrono::system_clock::now().time_since_epoch();
	do
	{
		auto packetPayload = InsertPacketIdIntoData(defaultData, packedId);
		socket->send(packetPayload, addr);
		packetSendTimes.insert(std::pair<int, long>(packedId, std::chrono::system_clock::now().time_since_epoch().count()));
		if (socket->checkIfThereAreAnyData())
		{
			socket->recieve(rawDataFromClient, &addr);
			int packetId = ExtractPacketIdFromData(rawDataFromClient);
			packetReceiveTimes.insert(std::pair<int,long>(packetId, std::chrono::system_clock::now().time_since_epoch().count()));
		}

		fs = std::chrono::system_clock::now().time_since_epoch()-start;
		packedId++;
	} while (fs.count() < testLenghtSEC);


	while (socket->checkIfThereAreAnyData())
	{
		socket->recieve(rawDataFromClient, &addr);
		int packetId = ExtractPacketIdFromData(rawDataFromClient);
		packetReceiveTimes.insert(std::pair<int, long>(packetId, std::chrono::system_clock::now().time_since_epoch().count()));
	}
	
	int bandwith = CalcBandwith(packetReceiveTimes.size(), sondSize, testLenghtSEC);
	int averageRTT = CalcAverageRtt(packetSendTimes, packetReceiveTimes);

	printf("BANDWIDTH: ");
	printf(std::to_string(bandwith/1000000).c_str());
	printf("MB/s ");
	printf("AVERAGE RTT: ");
	printf(std::to_string(averageRTT).c_str());
	printf("ms");
	printf("\n");
}
std::string Client::InsertPacketIdIntoData(std::string data,int packetId)
{
	auto packetIdString = std::to_string(packetId);
	auto dataLen = strlen(data.c_str());
	auto countOfCharsToRemove = strlen(packetIdString.c_str());
	auto shortenedData = data.substr(0, dataLen - countOfCharsToRemove);
	shortenedData = packetIdString.append(shortenedData);
	return packetIdString;
}

int Client::ExtractPacketIdFromData(std::string data)
{
	int dataLenght = data.length();
	int firstDummyDataIndex = data.find_first_of("a");
	return std::stoi(data.substr(0, firstDummyDataIndex + 1));
}
std::string Client::GenerateData(int size)
{
	std::string string = "";
	for (int i = 0; i < size; ++i)
	{
		string = string.append("a");
	}
	return string;
}
