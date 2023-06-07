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
    int pawnCount = 8;

    /*
    // White pieces
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        AddPiece(new Pawn(White, whitePawnEssence), pawnIterator, 6);

    AddPiece(new Rook(White, whiteRookEssence), 0, 7);
    AddPiece(new Knight(White, whiteKnightEssence), 1, 7);
    AddPiece(new Bishop(White, whiteBishopEssence), 2, 7);
    AddPiece(new Queen(White), 3, 7);
    AddPiece(new King(White), 4, 7);
    AddPiece(new Bishop(White, whiteBishopEssence), 5, 7);
    AddPiece(new Knight(White, whiteKnightEssence), 6, 7);
    AddPiece(new Rook(White, whiteRookEssence), 7, 7);

    // Black pieces
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        AddPiece(new Pawn(Black, blackPawnEssence), pawnIterator, 1);

    AddPiece(new Rook(Black, blackRookEssence), 0, 0);
    AddPiece(new Knight(Black, blackKnightEssence), 1, 0);
    AddPiece(new Bishop(Black, blackBishopEssence), 2, 0);
    AddPiece(new Queen(Black), 3, 0);
    AddPiece(new King(Black), 4, 0);
    AddPiece(new Bishop(Black, blackBishopEssence), 5, 0);
    AddPiece(new Knight(Black, blackKnightEssence), 6, 0);
    AddPiece(new Rook(Black, blackRookEssence), 7, 0);
    */

    AddPiece(new King(White), 4, 0);
    AddPiece(new Rook(White, Classic), 3, 1);
    AddPiece(new Bishop(White, Classic), 5, 3);
    AddPiece(new Rook(Black, Classic), 5, 2);

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

// KING SAFETY PROBLEM

/*
* Problem 1: King should not be allowed to move to unsafe squares
* When kings moves are being calculated, set the movement to illegal if there is opponent attack on that square otherwise it is legal.
* When other piece moves are being calculated, set the oposing king moves as ilegal when attacking their square.
* When other piece moves are being removed, set the oposing king moves as legal when no other hostile piece is attacking that square.
*/

/*
* Problem 2: Pinned pieces should not be allowed to move in a way that endangers the king.
* Before calculating any moves of piece other than king check if removing this piece from current square endangers the king.
* If that is the case check for each specific movement if your king is in danger after moving that piece to that square.
* After moving piece check if it pins any other piece and if it is the case recalculate its moves.
*/

/*
* Problem 3: When king is in check it has to be resolved otherwise its checkmate.
* When a newly calculated move attacks a kings square recalculate validity of all opponent moves.
* Only the following moves are valid:
* - King moves to a safe square
* This is valid only if one piece is attacking king:
* - Piece other than king moves and blocks attack on king. (this cannot reveal an attack on king)
* - Piece other than king removes attacking piece. (this cannot reveal an attack on king)
* After player resolves a checkmate recalculate validity of all their moves.
*/

void Board::CalculateMoves()
{
    for (auto whitePieceIterator = whitePieces.begin();
        whitePieceIterator != whitePieces.end(); ++whitePieceIterator) {
        Piece* whitePiece = *whitePieceIterator;
        CalculateMoves(whitePiece);
    }

    for (auto blackPieceIterator = blackPieces.begin();
        blackPieceIterator != blackPieces.end(); ++blackPieceIterator) {
        Piece* blackPiece = *blackPieceIterator;
        CalculateMoves(blackPiece);
    }
}

void Board::CalculateMoves(Piece* curPiece)
{
    RemoveMoves(curPiece);

    PieceMovement* pin = GetPin(curPiece);

    std::list<Mobility*> mobilities = curPiece->mobilities;
    for (auto mobilityIterator = mobilities.begin();
        mobilityIterator != mobilities.end(); ++mobilityIterator) {
        Mobility* curMobility = *mobilityIterator;
        CalculateMoves(curPiece, curMobility, nullptr, pin);
    }
}

void Board::CalculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin)
{
    ValidateMove(curPiece, prevMove, pin);

    if (curMobility->limit == 0) {
        while ((prevMove = CalculateMove(curPiece, curMobility, prevMove, pin)) != nullptr);
    } else {
        // if move has a limit we need to find out how many moves can be performed
        int curMoveIndex = 0;
        if (prevMove != nullptr)
            curMoveIndex = prevMove->moveCounter;

        for (int i = curMoveIndex; i < curMobility->limit; i++)
            if ((prevMove = CalculateMove(curPiece, curMobility, prevMove, pin)) == nullptr)
                break;
    }
}

Movement* Board::CalculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin)
{
    Movement* newMove = CreateMove(curPiece, curMobility, prevMove);
    if (newMove == nullptr)
        return nullptr;

    ValidateMove(curPiece, newMove, pin);

    Square* targetSquare = squares[newMove->y][newMove->x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    // add new movement to current piece
    if (prevMove == nullptr)
        curPiece->availableMoves.push_back(newMove);
    else
        prevMove->next = newMove;

    // add new movement to target square
    PieceMovement* newPieceMove = new PieceMovement(curPiece, newMove);
    targetSquare->movements.push_back(newPieceMove);

    // when this movement is attacking move, make opponent king moves on that square ilegal
    auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
    {
        return curPieceMovement->piece->name._Equal("King") && curPieceMovement->piece->owner != curPiece->owner;
    };

    auto it = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findPieceMovement);
    if (it != targetSquare->movements.end())
    {
        PieceMovement* kingPieceMovement = *it;
        kingPieceMovement->movement->legal = false;
    }

    if (targetPiece != nullptr)
    {
        if (targetPiece->owner != curPiece->owner && (curMobility->type == Attack || curMobility->type == AttackMove))
        {
            if (targetPiece->name._Equal("King"))
            {
                switch (targetPiece->owner)
                {
                    case::White:
                        whiteCheck = true;
                        break;
                    case::Black:
                        blackCheck = true;
                        break;
                }
                checks.push_front(newPieceMove);

                // when targeting opponent king revalidate all attacked player moves
                ValidateMoves(targetPiece->owner);
            }
            else
            {
                // when targeting opponent piece other than king with hostile move revalidate attacked piece moves if it results in pin
                PieceMovement* pin = GetPin(targetPiece);
                if (pin != nullptr)
                    ValidateMoves(targetPiece, pin);
            }
        }

        return nullptr;
    }

    return newMove;
}

// this method is called when check is made and when check is resolved
void Board::ValidateMoves(Owner owner)
{
    switch (owner)
    {
        case White:
            for each (Piece * whitePiece in whitePieces)
            {
                ValidateMoves(whitePiece, nullptr);
            }
            break;
        case Black:
            for each (Piece * blackPiece in blackPieces)
            {
                ValidateMoves(blackPiece, nullptr);
            }
            break;
    }
}

void Board::ValidateMoves(Piece* curPiece, PieceMovement* pin)
{
    for each (Movement *curMovement in curPiece->availableMoves)
    {
        while (curMovement != nullptr)
        {
            ValidateMove(curPiece, curMovement, pin);
            curMovement = curMovement->next;
        }
    }
}

void Board::ValidateMove(Piece* curPiece, Movement* curMovement, PieceMovement* pin)
{
    if (curMovement == nullptr)
        return;

    bool legal = true;

    Square* targetSquare = squares[curMovement->y][curMovement->x];

    if (curPiece->name._Equal("King"))
    {
        // if we are validating kings move we cannot move to squares that are attacked by our opponent
        auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
        {
            return curPieceMovement->piece->owner != curPiece->owner && (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove);
        };

        auto it = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findPieceMovement);
        if (it != targetSquare->movements.end())
            legal = false;
    }
    else
    {
        // if we are validating move of piece other than king - must capture the pinning piece or move along the pins vector
        if (pin != nullptr)
        {
            if (targetSquare->occupyingPiece != pin->piece)
            {
                Movement* curPinMovement = pin->movement;
                while (curPinMovement != nullptr)
                {
                    if (curPinMovement->x == targetSquare->x && curPinMovement->y == targetSquare->y)
                        break;

                    curPinMovement = curPinMovement->next;
                }

                if (curPinMovement == nullptr)
                    legal = false;
            }
        }

        // if we are validating move of piece other than king and we are in check - must capture the piece attacking the king (only if one attacker) or block all attacker moves
        if (curPiece->owner == White && whiteCheck || curPiece->owner == Black && blackCheck)
        {
            // king is attacked by one piece and we are moving to its square
            if (checks.size() == 1 && targetSquare->occupyingPiece != nullptr && targetSquare->occupyingPiece == checks.front()->piece)
            {
                legal = true;
            }
            // king is attacked by any amount of pieces and we are blocking all of these attacks on this square (e.g. eagle + rook can be blocked simultaneously)
            else
            {
                for each (PieceMovement* check in checks)
                {
                    auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
                    {
                        return curPieceMovement->piece == check->piece && check->movement->mobility == curPieceMovement->movement->mobility;
                    };

                    auto it = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findPieceMovement);
                    if (it == targetSquare->movements.end())
                    {
                        legal = false;
                        break;
                    }
                }
            }
        }
    }

    if (legal)
    {
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
    }

    curMovement->legal = legal;
}

Movement* Board::CreateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove)
{
    int cur_x, cur_y;

    if (prevMove != nullptr)
    {
        // calculate position of next move
        cur_x = prevMove->x + curMobility->direction_x;
        cur_y = prevMove->y;

        switch (curPiece->owner)
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

        switch (curPiece->owner)
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
    if (cur_x < 0 || cur_x > COLUMNS - 1 || cur_y < 0 || cur_y > ROWS - 1)
        return nullptr;

    int moveCounter = 1;
    if (prevMove != nullptr)
        moveCounter = prevMove->moveCounter;

    // create new movement
    return new Movement(cur_x, cur_y, moveCounter, false, curMobility, nullptr);
}

// We are creating a movement attacking the king that would be possible if this piece were to be removed
PieceMovement* Board::GetPin(Piece* curPiece)
{
    if (curPiece->name._Equal("King"))
        return nullptr;

    // iterate through every blocked movement that will be unblocked after removing this piece
    Square* curSquare = squares[curPiece->y][curPiece->x];
    for each (PieceMovement * attackerPieceMovement in curSquare->movements)
    {
        Piece* attackerPiece = attackerPieceMovement->piece;
        Movement* attackerMovement = attackerPieceMovement->movement;

        // ignore movement of friendly pieces and non-hostile moves
        if (attackerPiece->owner == curPiece->owner || attackerMovement->mobility->type == Move)
            continue;

        Movement* unblockedPin = nullptr;
        Piece* targetPiece = nullptr;

        Movement* newMove;
        Movement* prevMove = attackerMovement;
        // generate continuation of unblocked movement
        if (attackerMovement->mobility->limit == 0) {
            // unlimited movement generation
            while ((newMove = CreateMove(attackerPiece, attackerMovement->mobility, prevMove)) != nullptr)
            {
                if (unblockedPin == nullptr)
                    unblockedPin = newMove;
                else
                    prevMove->next = newMove;

                Square* targetSquare = squares[newMove->y][newMove->x];
                targetPiece = targetSquare->occupyingPiece;
                if (targetPiece != nullptr)
                    break;

                prevMove = newMove;
            }
        }
        else {
            // limited movement generation
            for (int i = attackerMovement->moveCounter; i < attackerMovement->mobility->limit; i++)
            {
                if ((newMove = CreateMove(attackerPiece, attackerMovement->mobility, prevMove)) == nullptr)
                    break;

                if (unblockedPin == nullptr)
                    unblockedPin = newMove;
                else
                    prevMove->next = newMove;

                Square* targetSquare = squares[newMove->y][newMove->x];
                targetPiece = targetSquare->occupyingPiece;
                if (targetPiece != nullptr)
                    break;

                prevMove = newMove;
            }
        }

        // if generated pin ended attacks your king create copy of existing pin movements and connect it to the already generated unblocked movement
        Movement* pinStart = nullptr;
        if (targetPiece != nullptr && targetPiece->name._Equal("King") && targetPiece->owner == curPiece->owner)
        {
            auto findMovement = [&](Movement* curMovement) -> bool
            {
                return curMovement->mobility == attackerMovement->mobility;
            };

            auto it = std::find_if(attackerPiece->availableMoves.begin(), attackerPiece->availableMoves.end(), findMovement);
            if (it != attackerPiece->availableMoves.end())
            {
                attackerMovement = *it;

                while (attackerMovement != nullptr)
                {
                    newMove = new Movement(attackerMovement->x, attackerMovement->y, attackerMovement->moveCounter, attackerMovement->legal, attackerMovement->mobility, nullptr);

                    if (pinStart == nullptr)
                        pinStart = newMove;
                    else
                        prevMove->next = newMove;

                    prevMove = newMove;
                    attackerMovement = attackerMovement->next;
                }

                newMove->next = unblockedPin;

                return new PieceMovement(attackerPiece, pinStart);
            }
        }
    }

    return nullptr;
}

void Board::RemoveMoves(Piece* curPiece)
{
    if (curPiece == nullptr)
        return;

    std::list<Movement*> movements = curPiece->availableMoves;
    for (auto movementIterator = movements.begin();
    movementIterator != movements.end(); ++movementIterator) {
        Movement* curMovement = *movementIterator;
        CutMovement(curPiece, curMovement);
    }

    curPiece->availableMoves.clear();
}

void Board::CutMovement(PieceMovement* curPieceMovement)
{
    Piece* curPiece = curPieceMovement->piece;
    Movement* curMovement = curPieceMovement->movement;

    ValidateMove(curPiece, curMovement, GetPin(curPiece));

    Movement* nextMovement = curMovement->next;
    curMovement->next = nullptr;

    CutMovement(curPiece, curMovement);
}

void Board::CutMovement(Piece* curPiece, Movement* curMovement)
{
    while (curMovement != nullptr)
    {
        Square* curSquare = squares[curMovement->y][curMovement->x];

        // remove this movement from the targeted square
        auto removePieceMovements = [&](PieceMovement* pieceMovement) -> bool
        {
            return pieceMovement->piece == curPiece && pieceMovement->movement == curMovement;
        };
        curSquare->movements.remove_if(removePieceMovements);

        // after removing this movement, revalidate opponent king moves on this square
        auto findPieceMovement = [&](PieceMovement* targetPieceMovement) -> bool
        {
            return targetPieceMovement->piece->name._Equal("King") && targetPieceMovement->piece->owner != curPiece->owner;
        };

        auto it = std::find_if(curSquare->movements.begin(), curSquare->movements.end(), findPieceMovement);
        if (it != curSquare->movements.end())
        {
            PieceMovement* kingPieceMovement = *it;
            ValidateMove(kingPieceMovement->piece, kingPieceMovement->movement, nullptr);
        }

        // continue with next movement
        Movement* prevMovement = curMovement;
        curMovement = curMovement->next;
        //delete prevMovement;
    }
}

void Board::MovePiece(Piece* curPiece, int x, int y)
{
    if (curPiece == nullptr || (curPiece->x == x && curPiece->y == y))
        return;

    Square* sourceSquare = squares[curPiece->y][curPiece->x];
    Square* destinationSquare = squares[y][x];
    Piece* removedPiece = destinationSquare->occupyingPiece;

    auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
    {
        return curPieceMovement->piece == curPiece && curPieceMovement->movement->legal;
    };

    auto it = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), findPieceMovement);
    if (it == destinationSquare->movements.end())
        return;

    // if move was succesfuly performed while in check it means that check was resolved and all moves need to be revalidated
    if (curPiece->owner == White && whiteCheck)
    {
        whiteCheck = !whiteCheck;
        checks.clear();
        ValidateMoves(White);
    }
    else if (curPiece->owner == Black && blackCheck)
    {
        blackCheck = !blackCheck;
        checks.clear();
        ValidateMoves(Black);
    }

    RemovePiece(removedPiece);

    sourceSquare->occupyingPiece = nullptr;
    destinationSquare->occupyingPiece = curPiece;
    curPiece->x = x;
    curPiece->y = y;

    CalculateMoves(curPiece);

    // continue calculations of movements that were unblocked by moving this piece
    std::list<PieceMovement*> sourceMovements = sourceSquare->movements;
    for (auto movementIterator = sourceMovements.begin();
    movementIterator != sourceMovements.end(); ++movementIterator) {
        PieceMovement* curPieceMovement = *movementIterator;
        if (curPieceMovement->piece != curPiece)
        {
            PieceMovement* pin = GetPin(curPieceMovement->piece);
            CalculateMoves(curPieceMovement->piece, curPieceMovement->movement->mobility, curPieceMovement->movement, pin);
        }
    }

    // cut movements that were blocked by moving this piece
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
    std::string ownerName;
    switch (curPiece->owner)
    {
        case::White:
            ownerName = "White";
            break;
        case::Black:
            ownerName = "Black";
            break;
    }

    std::string essenceName;
    switch (curPiece->essence)
    {
        case::Classic:
            essenceName = "Classic";
            break;
        case::Red:
            essenceName = "Red";
            break;
        case::Blue:
            essenceName = "Blue";
            break;
    }

    std::string pieceName = curPiece->name;
    
    std::cout << "Printing moves of piece: " << ownerName << " " << essenceName << " " << pieceName << "\n";

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