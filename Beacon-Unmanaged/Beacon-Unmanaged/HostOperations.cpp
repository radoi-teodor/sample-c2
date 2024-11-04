#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>

#include "Utils.h"
#include "HostOperations.h"

#define UNLEN       256

std::string HostOperations::GetBeaconId() {
	std::string id = "";

	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD realSizeOfComputerName = MAX_COMPUTERNAME_LENGTH + 1;
	if (!GetComputerNameA(computerName, &realSizeOfComputerName)) {
		std::cout << "[!] Error: " << GetLastError() << std::endl;
		return "";
	}

	char username[UNLEN + 1];
	DWORD realSizeOfUsername = UNLEN + 1;
	if (!GetUserNameA(username, &realSizeOfUsername)) {
		std::cout << "[!] Error: " << GetLastError() << std::endl;
		return "";
	}

	id = computerName + std::string("-") + username;
	id = Utils::base64encode(id);

	return id;
}