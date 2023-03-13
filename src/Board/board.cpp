#include "board.h"
#include "../Pieces/pieces.h"

#include <iostream>

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

    // Black first row
    squares[0][0]->occupyingPiece = new Rook(Classic, Black);
    squares[0][1]->occupyingPiece = new Knight(Classic, Black);
    squares[0][2]->occupyingPiece = new Bishop(Classic, Black);
    squares[0][3]->occupyingPiece = new Queen(Classic, Black);
    squares[0][4]->occupyingPiece = new King(Classic, Black);
    squares[0][5]->occupyingPiece = new Bishop(Classic, Black);
    squares[0][6]->occupyingPiece = new Knight(Classic, Black);
    squares[0][7]->occupyingPiece = new Rook(Classic, Black);

    // White first row
    squares[7][0]->occupyingPiece = new Rook(Classic, White);
    squares[7][1]->occupyingPiece = new Knight(Classic, White);
    squares[7][2]->occupyingPiece = new Bishop(Classic, White);
    squares[7][3]->occupyingPiece = new Queen(Classic, White);
    squares[7][4]->occupyingPiece = new King(Classic, White);
    squares[7][5]->occupyingPiece = new Bishop(Classic, White);
    squares[7][6]->occupyingPiece = new Knight(Classic, White);
    squares[7][7]->occupyingPiece = new Rook(Classic, White);
}

void Board::MovePiece(int x1, int y1, int x2, int y2) {
    Square* from = squares[y1][x1];
    Square* to = squares[y2][x2];
    Piece* movingPiece = from->occupyingPiece;

    to->occupyingPiece = movingPiece;
    from->occupyingPiece = nullptr;
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
                char ownerTag = curPiece->owner;
                char essenceTag = curPiece->essence;
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