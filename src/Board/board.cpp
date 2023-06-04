#include "board.h"
#include "../Pieces/pieces.h"

#include <iostream>
#include <algorithm>

Board::Board() {
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            squares[cur_y][cur_x] = new Square(cur_x, cur_y);
        }
    }
}

void Board::InitBoard(
    Essence whitePawnEssence, Essence whiteRookEssence, Essence whiteKnightEssence, Essence whiteBishopEssence,
    Essence blackPawnEssence, Essence blackRookEssence, Essence blackKnightEssence, Essence blackBishopEssence
) {
    /*
    int pawnCount = 8;

    // Black pieces
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        AddPiece(new Pawn(blackPawnEssence, Black), pawnIterator, 1);

    AddPiece(new Rook(blackRookEssence, Black), 0, 0);
    AddPiece(new Knight(blackKnightEssence, Black), 1, 0);
    AddPiece(new Bishop(blackBishopEssence, Black), 2, 0);
    AddPiece(new Queen(Black), 3, 0);
    AddPiece(new King(Black), 4, 0);
    AddPiece(new Bishop(blackBishopEssence, Black), 5, 0);
    AddPiece(new Knight(blackKnightEssence, Black), 6, 0);
    AddPiece(new Rook(blackRookEssence, Black), 7, 0);
    
    // White pieces
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        AddPiece(new Pawn(whitePawnEssence, White), pawnIterator, 6);

    AddPiece(new Rook(whiteRookEssence, White), 0, 7);
    AddPiece(new Knight(whiteKnightEssence, White), 1, 7);
    AddPiece(new Bishop(whiteBishopEssence, White), 2, 7);
    AddPiece(new Queen(White), 3, 7);
    AddPiece(new King(White), 4, 7);
    AddPiece(new Bishop(whiteBishopEssence, White), 5, 7);
    AddPiece(new Knight(whiteKnightEssence, White), 6, 7);
    AddPiece(new Rook(whiteRookEssence, White), 7, 7);
    */

    AddPiece(new Rook(Classic, White), 7, 7);
    AddPiece(new Bishop(Classic, White), 4, 7);

    /*
    AddPiece(new King(White), 4, 4);
    AddPiece(new Rook(Classic, Black), 5, 5);
    AddPiece(new Bishop(Classic, Black), 6, 6);
    */

    CalculateMoves();
}

bool Board::AddPiece(Piece* newPiece, int x, int y)
{
    Square* curSquare = squares[y][x];
    if (newPiece == nullptr || curSquare->occupyingPiece != nullptr)
        return false;

    newPiece->x = x;
    newPiece->y = y;

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

void Board::RemovePiece(Piece* removedPiece)
{
    if (removedPiece == nullptr)
        return;

    RemoveMoves(removedPiece);

    Square* curSquare = squares[removedPiece->y][removedPiece->x];
    curSquare->occupyingPiece = nullptr;

    switch (removedPiece->owner)
    {
        case::White:
            whitePieces.remove(removedPiece);
            break;
        case::Black:
            blackPieces.remove(removedPiece);
            break;
    }
}

void Board::CalculateMoves()
{
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

void Board::CalculateMoves(Piece* curPiece)
{
    RemoveMoves(curPiece);

    std::list<Mobility*> mobilities = curPiece->mobilities;
    for (auto mobilityIterator = mobilities.begin();
    mobilityIterator != mobilities.end(); ++mobilityIterator) {
        Mobility* curMobility = *mobilityIterator;
        CalculateMoves(curPiece, curMobility, nullptr);
    }
}

void Board::CalculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove)
{
    ValidateMove(curPiece, prevMove);

    if (curMobility->limit == 0) {
        while ((prevMove = CalculateMove(curPiece, curMobility, prevMove)) != nullptr);
    } else {
        // if move has a limit we need to find out how many moves can be performed
        int curMoveIndex = 0;
        if (prevMove != nullptr)
        {
            // find the movement list on current piece with calculating moves mobility
            auto findMovement = [&](Movement *curMovement) -> bool
            {
                return curMovement->mobility == curMobility;
            };

            auto it = std::find_if(curPiece->availableMoves.begin(), curPiece->availableMoves.end(), findMovement);
            if (it == curPiece->availableMoves.end())
                return;

            // iterate through list of movements until our move is found
            Movement* curMovement = *it;
            while(curMovement != nullptr && curMovement != prevMove)
            {
                curMovement = curMovement->next;
                curMoveIndex++;
            }

            if (curMovement == nullptr)
                return;

            // prev move was already calculated
            curMoveIndex++;
        }

        for (int i = curMoveIndex; i < curMobility->limit; i++)
            if ((prevMove = CalculateMove(curPiece, curMobility, prevMove)) == nullptr)
                break;
    }
}

void Board::ValidateMove(Piece *curPiece, Movement *curMovement)
{
    if (curMovement == nullptr)
        return;

    bool legal = true;

    Square* targetSquare = squares[curMovement->y][curMovement->x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    switch (curMovement->mobility->type)
    {
        case::Move:
            // when moving you cant attack any pieces
            if (targetPiece != nullptr)
                legal = false;
            break;
        case::Attack:
            // when attacking you cant move to empty squares or attack your own pieces
            if (targetPiece == nullptr || targetPiece->owner == curPiece->owner)
                legal = false;
            break;
        case::AttackMove:
            // when attack moving you cant attack your own pieces
            if (targetPiece != nullptr && targetPiece->owner == curPiece->owner)
                legal = false;
            break;
    }

    curMovement->legal = legal;
}

Movement* Board::CalculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove)
{
    int cur_x, cur_y;

    if (prevMove != nullptr)
    {
        // calculate position of next move
        cur_x = prevMove->x + curMobility->direction_x;
        cur_y = prevMove->y;

        switch(curPiece->owner)
        {
            case::White:
                cur_y -= curMobility->direction_y;
                break;
            case::Black:
                cur_y += curMobility->direction_y;
                break;
        }
    }
    else
    {
        // calculate positon of first move
        cur_x = curPiece->x + curMobility->start_x;
        cur_y = curPiece->y;

        switch(curPiece->owner)
        {
            case::White:
                cur_y -= curMobility->start_y;
                break;
            case::Black:
                cur_y += curMobility->start_y;
                break;
        }
    }

    // when new move is out of bounds return null
    if (cur_x < 0 || cur_x > COLUMNS-1 || cur_y < 0 || cur_y > ROWS-1)
        return nullptr;

    Square* targetSquare = squares[cur_y][cur_x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    // create new movement
    Movement* newMove = new Movement(cur_x, cur_y, false, curMobility, nullptr);
    ValidateMove(curPiece, newMove);

    // add new movement to current piece
    if (prevMove == nullptr)
        curPiece->availableMoves.push_back(newMove);
    else
        prevMove->next = newMove;

    // add new movement to target square
    PieceMovement* newPieceMove = new PieceMovement(curPiece, newMove);  
    targetSquare->movements.push_back(newPieceMove);

    // when targeting piece return null
    if (targetPiece != nullptr)
        return nullptr;

    return newMove;
}

void Board::RemoveMoves(Piece* curPiece)
{
    if (curPiece == nullptr)
        return;

    std::list<Movement*> movements = curPiece->availableMoves;
    for (auto movementIterator = movements.begin();
    movementIterator != movements.end(); ++movementIterator) {
        Movement* curMovement = *movementIterator;

        while (curMovement != nullptr)
        {
            Square* curSquare = squares[curMovement->y][curMovement->x];

            auto removePieceMovements = [&](PieceMovement* pieceMovement) -> bool
            {
                return pieceMovement->piece == curPiece && pieceMovement->movement == curMovement;
            };
            curSquare->movements.remove_if(removePieceMovements);

            curMovement = curMovement->next;
        }
    }

    curPiece->availableMoves.clear(); 
}

void Board::CutMovement(PieceMovement* curPieceMovement)
{
    Piece* curPiece = curPieceMovement->piece;
    Movement* curMovement = curPieceMovement->movement;

    curMovement->legal = false;
    while (curMovement != nullptr)
    {
        Square* curSquare = squares[curMovement->y][curMovement->x];

        auto removePieceMovements = [&](PieceMovement* pieceMovement) -> bool
        {
            return pieceMovement->piece == curPiece && pieceMovement->movement == curMovement;
        };
        curSquare->movements.remove_if(removePieceMovements);

        Movement* nextMovement = curMovement->next;
        curMovement->next = nullptr;
        curMovement = nextMovement;
    }
}

void Board::MovePiece(Piece* curPiece, int x, int y)
{
    if (curPiece == nullptr)
        return;

    Square* sourceSquare = squares[curPiece->y][curPiece->x];
    Square* destinationSquare = squares[y][x];
    Piece* removedPiece = destinationSquare->occupyingPiece;

    RemovePiece(removedPiece);

    sourceSquare->occupyingPiece = nullptr;
    destinationSquare->occupyingPiece = curPiece;
    curPiece->x = x;
    curPiece->y = y;

    CalculateMoves(curPiece);

    std::list<PieceMovement*> sourceMovements = sourceSquare->movements;
    for (auto movementIterator = sourceMovements.begin();
    movementIterator != sourceMovements.end(); ++movementIterator) {
        PieceMovement* curPieceMovement = *movementIterator;
        CalculateMoves(curPieceMovement->piece, curPieceMovement->movement->mobility, curPieceMovement->movement);
    }

    std::list<PieceMovement*> destinationMovements = destinationSquare->movements;
    for (auto movementIterator = destinationMovements.begin();
    movementIterator != destinationMovements.end(); ++movementIterator) {
        PieceMovement* curPieceMovement = *movementIterator;
        CutMovement(curPieceMovement);
    }
}

void Board::PerformMove(int x1, int y1, int x2, int y2)
{
    Square* curSquare = squares[y1][x1];
    Piece* curPiece = curSquare->occupyingPiece;

    MovePiece(curPiece, x2, y2);
}

void Board::PrintBoard()
{
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

void Board::PrintMoves()
{
    for (auto whitePieceIterator = whitePieces.begin();
    whitePieceIterator != whitePieces.end(); ++whitePieceIterator) {
        Piece *whitePiece = *whitePieceIterator;
        PrintMoves(whitePiece);
    }

    for (auto blackPieceIterator = blackPieces.begin();
    blackPieceIterator != blackPieces.end();++blackPieceIterator) {
        Piece *blackPiece = *blackPieceIterator;
        PrintMoves(blackPiece);
    }
}

void Board::PrintMoves(Piece* curPiece)
{
    std::cout << "Printing moves of piece: " << curPiece->name << "\n";

    char pieceMoves[ROWS][COLUMNS];
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            pieceMoves[cur_y][cur_x] = ' ';
        }
    }
    pieceMoves[curPiece->y][curPiece->x] = '0';

    std::list<Movement*> availableMoves = curPiece->availableMoves;
    for (auto movementIterator = availableMoves.begin();
    movementIterator != availableMoves.end(); ++movementIterator) {
        Movement* curMove = *movementIterator;

        while (curMove != nullptr) {
            if (curMove->legal)
                pieceMoves[curMove->y][curMove->x] = 'X';
            
            curMove = curMove->next;
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