#include "fairyStockfish.h"
#include <iostream>
#include <sstream>
#include <fstream>


#define BUFSIZE 4096

bool FairyStockfish::start(EssenceArgs essenceArgs)
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

    setConfigFile(essenceArgs);

    std::string command = "trainer\\fairy-stockfish.exe load chess-evolved-tmp.ini";
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

void FairyStockfish::setConfigFile(EssenceArgs essenceArgs)
{
    Board board;
    board.initBoard(essenceArgs);

    std::string inputFile = "trainer\\chess-evolved.ini";
    std::string outputFile = "trainer\\chess-evolved-tmp.ini";

    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Could not open input file '" << inputFile << "'" << std::endl;
        return;
    }

    std::ofstream outFile(outputFile, std::ios::trunc);
    if (!outFile) {
        std::cerr << "Error: Could not open output file '" << outputFile << "'" << std::endl;
        return;
    }

    outFile << inFile.rdbuf();
    outFile << "promotionPieceTypesWhite = "
        << Piece(White, Bishop, essenceArgs.whiteBishop).toChar()
        << Piece(White, Knight, essenceArgs.whiteKnight).toChar()
        << Piece(White, Rook, essenceArgs.whiteRook).toChar()
        << "q" << std::endl;
    outFile << "promotionPieceTypesBlack = "
        << Piece(Black, Bishop, essenceArgs.blackBishop).toChar()
        << Piece(Black, Knight, essenceArgs.blackKnight).toChar()
        << Piece(Black, Rook, essenceArgs.blackRook).toChar()
        << "q" << std::endl;
    outFile << "startFen = " << board.toString() << std::endl;

    inFile.close();
    outFile.close();
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
        //std::cout << "Sent command to FairyStockfish: " << command << std::endl;

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
        //std::cout << "Received response from FairyStockfish: " << response << std::endl;

        return response;
    }
    
    return std::string();
}
