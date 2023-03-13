#include "board.h"
#include "../Pieces/pieces.h"

#include <iostream>

std::map<Owner, char> ownerTags = {
    { White, 'W' },
    { Black, 'B' }
};

std::map<Essence, char> essenceTags = {
    { Classic, 'C' },
    { Red, 'R' },
    { Blue, 'B' },
    { Yellow, 'Y' }
};

Board::Board() {
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            squares[cur_y][cur_x] = new Square();
        }
    }

    int pawnCount = 8;
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
    {
        Pawn* newWhitePawn = new Pawn(Classic, White); 
        squares[6][pawnIterator]->occupyingPiece = newWhitePawn;

        Pawn* newBlackPawn = new Pawn(Classic, Black);
        squares[1][pawnIterator]->occupyingPiece = newBlackPawn;
    }
}

void Board::PrintBoard() {
    std::cout << "Printing current state of board:\n";

    for (int cur_y = 0; cur_y < COLUMNS; cur_y++) {
        for (int cur_x = 0; cur_x < ROWS; cur_x++) {
            if (cur_x > 0)
                std::cout << "|";

            Square* curSquare = squares[cur_y][cur_x];
            Piece* curPiece = curSquare->occupyingPiece;

            if (curPiece != nullptr) {
                char ownerTag = ownerTags[curPiece->owner];
                char essenceTag = essenceTags[curPiece->essence];
                char pieceTag = curPiece->tag;

                std::cout << ownerTag << essenceTag << pieceTag;
            } else {
                curPiece = nullptr;
                std::cout << "XXX";
            }
        }

        if (cur_y < 7)
            std::cout << "\n---|---|---|---|---|---|---|---\n";
    }

    std::cout << "\n\n";
}