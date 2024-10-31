#include <Windows.h>
#include <iostream>
#include "ProcessHelper.h"

int main() {
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