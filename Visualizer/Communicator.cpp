#include "Communicator.h"


Communicator::Communicator()
{
}
Communicator::Communicator(LPCWSTR portName)
{
	SP = new Serial(portName);

	if (SP->IsConnected())
		cout << ("Connected") << endl;
	else
		cout << "Could not establish connection." << endl;
}
void Communicator::writeBytes(byte *bytes)
{

		int byteSize = sizeof(bytes)*4;
		if (!SP->WriteData(bytes, byteSize))
		{
			cout << "Didn't write!" << endl;
		}
}
Communicator::~Communicator()
{
}
