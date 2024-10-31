#pragma once
class ProcessHelper
{
public:
	static std::wstring stdInPipeName, stdOutPipeName;
	static HANDLE stdInPipe, stdOutPipe;

	static void create_powershell_process();
	static std::string read_output();
	static void write_input(std::string cmd);
private:

};

