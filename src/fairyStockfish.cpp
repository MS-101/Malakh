#include "fairyStockfish.h"
#include <iostream>
#include <sstream>


#define BUFSIZE 4096

bool FairyStockfish::start()
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&handleStdoutRead, &handleStdoutWrite, &saAttr, 0))
        return false;

    if (!SetHandleInformation(handleStdoutRead, HANDLE_FLAG_INHERIT, 0))
        return false;

    if (!CreatePipe(&handleStdinRead, &handleStdinWrite, &saAttr, 0))
        return false;

    if (!SetHandleInformation(handleStdinWrite, HANDLE_FLAG_INHERIT, 0))
        return false;

    STARTUPINFO siStartInfo{};
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = handleStdoutWrite;
    siStartInfo.hStdOutput = handleStdoutWrite;
    siStartInfo.hStdInput = handleStdinRead;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    std::string command = "trainer\\fairy-stockfish.exe load variants.ini";
    if (CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo))
    {
        receiveResponse();

        return true;
    }
    else
    {
        DWORD error = GetLastError();
        std::cout << "Failed to start fairy stockfish: " << error << std::endl;

        return false;
    }
}

void FairyStockfish::exit()
{
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    CloseHandle(handleStdoutWrite);
    CloseHandle(handleStdinRead);
}

bool FairyStockfish::sendCommand(std::string command)
{
    DWORD dwWritten;
    std::string commandEndl = command + "\n";

    if (WriteFile(handleStdinWrite, commandEndl.c_str(), commandEndl.size(), &dwWritten, NULL) || dwWritten == 0)
    {
        std::cout << "Sent command to FairyStockfish: " << command << std::endl;

        return true;
    }
    else
    {
        DWORD error = GetLastError();
        std::cout << "Failed to send command to FairyStockfish: " << error << std::endl;

        return false;
    }
}

std::string FairyStockfish::waitForResponse(std::string target)
{
    while (true)
    {
        std::string response = receiveResponse();
        std::istringstream responseSS(response);

        std::string line;
        while (std::getline(responseSS, line))
        {
            if (line.starts_with(target))
                return line;
        }
    }
}

std::string FairyStockfish::receiveResponse()
{
    DWORD dwRead;
    CHAR chBuf[BUFSIZE];

    if (ReadFile(handleStdoutRead, chBuf, BUFSIZE, &dwRead, NULL) && dwRead > 0)
    {
        std::string response(chBuf, dwRead);
        std::cout << "Received response from FairyStockfish: " << response << std::endl;

        return response;
    }
    
    return std::string();
}
