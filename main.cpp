#include "src/uci.h"

int main() {
    uci* api = new uci();
    api->Run();

    /*
    Game* newGame = new Game(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    Board* board = newGame->myBoard;

    FILE* myFile = freopen("output.txt", "w", stdout);

    board->PrintBoard();
    board->PrintMoves();

    std::cout << "Performing move...\n\n";

    board->PerformMove(1, 6, 1, 4);
    board->PrintBoard();
    board->PrintMoves();
    */

    return 0;
}
