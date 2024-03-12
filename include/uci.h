#pragma once

#include <searching.h>
#include <string>


class uci {
public:
	void run();
	bool parseCommand(std::string command);
private:
	EssenceArgs essenceConfig;
	Board board;
};