#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include <board.h>


class FairyStockfish {
public:
	bool start(EssenceArgs essenceArgs);
	void exit();
	bool sendCommand(std::string command);
	void setConfigFile(EssenceArgs essenceArgs);
	std::string waitForResponse(std::string target);
	std::string receiveResponse();
private:
	PROCESS_INFORMATION piProcInfo{};
	HANDLE handleStdinRead = NULL;
	HANDLE handleStdinWrite = NULL;
	HANDLE handleStdoutRead = NULL;
	HANDLE handleStdoutWrite = NULL;
};
