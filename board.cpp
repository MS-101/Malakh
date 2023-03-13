#include "board.h"
#include "pieces/pieces.h"

#include <iostream>

Board::Board() {
    for (int i = 0; i < 8; i++) {
        ClassicPawn *newPawn;
        newPawn->myOwner = white;
        squares[0][i]->occupyingPiece = newPawn;
    }
}

/*
void Board::MovePiece(int x1, int y1, int x2, int y2) {
    Square *from = squares[y1][x1];
    Square *to = squares[y2][x2];
    Piece *movedPiece = from->occupyingPiece;

    from->occupyingPiece = (Piece*)0;
    to->occupyingPiece = movedPiece;

    CalculateMoves(movedPiece);
    
    // TO DO: UNBLOCK AND BLOCK MOVES
}
*/

/*
void Board::CalculateMoves() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Square *curSquare = squares[y][x];
            Piece *curPiece = curSquare->occupyingPiece;

            if (curPiece != (Piece *)0) {
                CalculateMoves(curPiece);
            }
        }
    }
}
*/

/*
void Board::RemoveMoves(Piece *curPiece) {
    std::list<std::list<AvailableMove>> curAvailableMovesList = curPiece->availableMoves;

    while (curAvailableMovesList.size() != 0) {
        std::list<AvailableMove> curAvailableMoves = curPiece->availableMoves.front();

        while (curAvailableMoves.size() != 0) {
            AvailableMove curAvailableMove = curAvailableMoves.front();

            int cur_x = curAvailableMove.x;
            int cur_y = curAvailableMove.y;
            Square *curSquare = squares[cur_y][cur_x];

            std::list<AvailableMove> availableMoves = *curSquare->availableMoves;
            for (AvailableMove curAvailableMove : availableMoves) {
                if (curAvailableMove.origin == curPiece) {
                    availableMoves.remove(curAvailableMove);
                }
            }

            curAvailableMoves.pop_front();
        }

        curAvailableMovesList.pop_front();
    }
}
*/

/*
void Board::CalculateMoves(Piece *curPiece) {
    RemoveMoves(curPiece);

    for (auto const& curMovement : curPiece->movements) {
        std::list<AvailableMove> newMoveList;
        curPiece->availableMoves.push_back(newMoveList);

        int cur_x = curPiece->x + curMovement.start_x;
        int cur_y = curPiece->y + curMovement.start_y;
        
        AvailableMove newMove = AvailableMove(curPiece, curMovement, cur_x, cur_y);
        newMoveList.push_back(newMove);

        Square *curSquare = squares[cur_y][cur_x];
        curSquare->availableMoves->push_back(newMove);

        for (int i = 0; i < curMovement.limit - 1; i++) {
            cur_x += curMovement.direction_x;
            cur_y += curMovement.direction_y;

            AvailableMove newMove = AvailableMove(curPiece, curMovement, cur_x, cur_y);
            newMoveList.push_back(newMove);

            curSquare = squares[cur_y][cur_x];
            curSquare->availableMoves->push_back(newMove);
        }
    }
}
*/

void Board::PrintBoard() {
    for (int cur_y = 0; cur_y < COLUMNS; cur_y++) {
        for (int cur_x = 0; cur_x < ROWS; cur_x++) {
            if (cur_x > 0)
                std::cout << "|";
           
            Square *curSquare = squares[cur_y][cur_x];
            Piece *curPiece = curSquare->occupyingPiece;

            if (curPiece != (Piece *)0)
            {
                if (curPiece->myOwner == white)
                {
                    std::cout << "W" + curPiece->label;
                }
                else
                {
                    std::cout << "B" + curPiece->label;
                }
            }
            else
            {
                std::cout << "--";
            }
        }

        std::cout << "\n";
    }
}

/*
void Board::PrintMoves(Piece *curPiece) {
    char pieceMovement[COLUMNS][ROWS];
    for (int cur_y = 0; cur_y < COLUMNS; cur_y++) {
        for (int cur_x = 0; cur_x < ROWS; cur_x++) {
            pieceMovement[cur_y][cur_x] = '-';
        }
    }

    pieceMovement[curPiece->y][curPiece->x] = '0';

    for (std::list<AvailableMove> curMoveList : curPiece->availableMoves) {
        for (std::list<AvailableMove>::iterator curMove = curMoveList.begin(); curMove != curMoveList.end(); curMove++) {
            int cur_x = curMove->x;
            int cur_y = curMove->y;

            pieceMovement[cur_y][cur_x] = 'X';
        }
    }

    for (int cur_y = 0; cur_y < COLUMNS; cur_y++) {
        for (int cur_x = 0; cur_x < ROWS; cur_x++) {
            std::cout << pieceMovement[cur_y][cur_x];
        }

        std::cout << "\n";
    }   
}
*/
