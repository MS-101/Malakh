#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class FairyStockfish {
public:
	bool start();
	void exit();
	bool sendCommand(std::string command);
	std::string waitForResponse(std::string target);
	std::string receiveResponse();
private:
	PROCESS_INFORMATION piProcInfo{};
	HANDLE handleStdinRead = NULL;
	HANDLE handleStdinWrite = NULL;
	HANDLE handleStdoutRead = NULL;
	HANDLE handleStdoutWrite = NULL;
};