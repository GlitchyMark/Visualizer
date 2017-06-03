#pragma once
#include "Serial.h"

using namespace std;

class Communicator
{
public:
	Communicator();
	Communicator(LPCWSTR portName);
	void writeBytes(byte * bytes);
	~Communicator();
	Serial* SP;
};

