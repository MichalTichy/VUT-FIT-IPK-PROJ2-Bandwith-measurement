#pragma once
class Reflector
{
public:
	Reflector(int port);
	~Reflector();
	void Start();
private:
	int port;
};

