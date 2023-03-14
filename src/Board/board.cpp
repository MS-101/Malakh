#include "board.h"
#include "../Pieces/pieces.h"

#include <iostream>

Board::Board() {
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            squares[cur_y][cur_x] = new Square();
        }
    }
}

void Board::InitBoard(
    Essence whitePawnEssence, Essence whiteRookEssence, Essence whiteKnightEssence, Essence whiteBishopEssence,
    Essence blackPawnEssence, Essence blackRookEssence, Essence blackKnightEssence, Essence blackBishopEssence
) {
    int pawnCount = 8;

    // Black pieces
    /*
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        AddPiece(new Pawn(blackPawnEssence, Black), pawnIterator, 1);
    */

    //AddPiece(new Rook(blackRookEssence, Black), 0, 0);
    //AddPiece(new Knight(blackKnightEssence, Black), 1, 0);
    AddPiece(new Bishop(blackBishopEssence, Black), 2, 0);
    //AddPiece(new Queen(Black), 3, 0);
    //AddPiece(new King(Black), 4, 0);
    //AddPiece(new Bishop(blackBishopEssence, Black), 5, 0);
    //AddPiece(new Knight(blackKnightEssence, Black), 6, 0);
    //AddPiece(new Rook(blackRookEssence, Black), 7, 0);
    
    // White pieces
    /*
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        AddPiece(new Pawn(whitePawnEssence, White), pawnIterator, 6);
    */

    //AddPiece(new Rook(whiteRookEssence, White), 0, 7);
    //AddPiece(new Knight(whiteKnightEssence, White), 1, 7);
    //AddPiece(new Bishop(whiteBishopEssence, White), 2, 7);
    //AddPiece(new Queen(White), 3, 7);
    //AddPiece(new King(White), 4, 7);
    //AddPiece(new Bishop(whiteBishopEssence, White), 5, 7);
    //AddPiece(new Knight(whiteKnightEssence, White), 6, 7);
    //AddPiece(new Rook(whiteRookEssence, White), 7, 7);

    PrintBoard();
    InitMoves();
}

bool Board::AddPiece(Piece* newPiece, int x, int y) {
    Square* curSquare = squares[y][x];
    if (newPiece == nullptr || curSquare->occupyingPiece != nullptr)
        return false;

    curSquare->occupyingPiece = newPiece;
    switch (newPiece->owner) {
        case::White:
            whitePieces.push_back(newPiece);
            break;
        case::Black:
            blackPieces.push_back(newPiece);
            break;
    }

    return true;
}

void Board::InitMoves() {
    for (auto whitePieceIterator = whitePieces.begin();
    whitePieceIterator != whitePieces.end(); ++whitePieceIterator) {
        Piece *whitePiece = *whitePieceIterator;
        CalculateMoves(whitePiece);
    }

    for (auto blackPieceIterator = blackPieces.begin();
    blackPieceIterator != blackPieces.end();++blackPieceIterator) {
        Piece *blackPiece = *blackPieceIterator;
        CalculateMoves(blackPiece);
    }
}

void Board::CalculateMoves(Piece* curPiece) {
    std::list<Mobility> mobilities = curPiece->mobility;
    for (auto mobilityIterator = mobilities.begin();
    mobilityIterator != mobilities.end(); ++mobilityIterator) {
        Mobility curMobility = *mobilityIterator;

        int cur_x = curMobility.start_x;
        int cur_y = curMobility.start_y;
        if (curPiece->owner == White)
            cur_y *= -1;

        if (curMobility.limit == 0) {
            while (!CalculateMove(curPiece, curMobility, &cur_x, &cur_y));
        } else {
            for (int i = 0; i < curMobility.limit; i++)
                if (!CalculateMove(curPiece, curMobility, &cur_x, &cur_y))
                    break;
        }
    }

    PrintMoves(curPiece);
}

/**
 * @brief Performs move on current position and moves current position in mobility direction.
 * 
 * @param curPiece 
 * @param curMobility
 * @param cur_x
 * @param cur_y 
 * @return true if move can be performed
 * @return false if move cannot be performed
 */
bool Board::CalculateMove(Piece* curPiece, Mobility curMobility, int* cur_x, int* cur_y) {
    if (*cur_x < 0 || *cur_x > COLUMNS-1 || *cur_y < 0 || *cur_y > ROWS-1) {
        return false;
    }

    // TO DO: CREATE MOVE

    *cur_x += curMobility.direction_x;
    switch(curPiece->owner)
    {
        case::White:
            *cur_y -= curMobility.direction_y;
            break;
        case::Black:
            *cur_y += curMobility.direction_y;
            break;
    }

    return true;
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

void Board::PrintMoves(Piece* curPiece) {
    std::cout << "Printing moves of piece: " << curPiece->name << "\n";

    char pieceMoves[ROWS][COLUMNS];
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            pieceMoves[cur_y][cur_x] = ' ';
        }
    }

    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            if (cur_x > 0)
                std::cout << "|";

            std::cout << pieceMoves[cur_y][cur_x];
        }

        if (cur_y < 7)
            std::cout << "\n-|-|-|-|-|-|-|-\n";           
    }
    std::cout << "\n\n";
}