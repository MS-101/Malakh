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

    AddPiece(new Pawn(White, Classic), 0, 7);
    AddPiece(new Pawn(Black, Classic), 1, 5);

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

// EN PASSANT

/*
* When pawn has not moved, can move forward 2 squares.
* Oponent pawns can attack the pawn as if it has moved 1 square forward on the following move.
*/

// CASTLING 

/*
* For queenside castling the left rook has not moved, there are no pieces on squares b1, c1, d1, king has not moved and no hostile piece is attacking the king or squares b1, c1, d1
* For kingside castling the right rook has not moved, there are no pieces on squares f1, g1, king has not moved and no hostile piece is attacking the king or squares f1, g1
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
    // movement calculation fails if this move can only be performed on first turn
    if (curMobility->flags.initiative && curPiece->hasMoved)
        return nullptr;

    // movement calculation fails if it cannot inspire a piece
    if (curMobility->flags.inspiring)
    {
        int inspired_x = curPiece->x + curMobility->flags.affected_x;
        int inspired_y = curPiece->y;

        switch (curPiece->owner)
        {
        case::White:
            inspired_y -= curMobility->flags.affected_y;
            break;
        case::Black:
            inspired_y += curMobility->flags.affected_y;
        }

        if (inspired_x < 0 || inspired_x > COLUMNS - 1 || inspired_y < 0 || inspired_y > ROWS - 1)
            return nullptr;

        Square* inspiredSquare = squares[inspired_y][inspired_x];
        Piece* inspiredPiece = inspiredSquare->occupyingPiece;

        if (inspiredPiece == nullptr)
            return nullptr;

        if (inspiredPiece->hasMoved || inspiredPiece->owner != curPiece->owner)
            return nullptr;
    }

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

    if (curMobility->type == Attack || curMobility->type == AttackMove)
    {
        // invalidate king moves
        auto findKingPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
        {
            return curPieceMovement->piece->name._Equal("King") && curPieceMovement->piece->owner != curPiece->owner;
        };

        auto findKingPieceMovementIterator = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findKingPieceMovement);
        if (findKingPieceMovementIterator != targetSquare->movements.end())
        {
            PieceMovement* kingPieceMovement = *findKingPieceMovementIterator;
            kingPieceMovement->movement->legal = false;
        }

        if (targetPiece != nullptr)
        {
            if (targetPiece->owner != curPiece->owner)
            {
                if (targetPiece->name._Equal("King"))
                {
                    // when targeting opponent king revalidate all attacked player moves
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

                    ValidateMoves(targetPiece->owner);
                }
                else
                {
                    // when targeting opponent piece other than king with hostile move revalidate attacked piece moves if it results in pin
                    PieceMovement* pin = GetPin(targetPiece);
                    if (pin != nullptr)
                        ValidateMoves(targetPiece, pin);
                }

                // cut cowardly moves of targeted piece
                for each (Movement * targetMovement in targetPiece->availableMoves)
                {
                    if (targetMovement->mobility->flags.cowardly)
                    {
                        CutMovement(targetPiece, targetMovement);
                    }
                }

                auto removeCowardlyMovements = [&](Movement* movement) -> bool
                {
                    return movement->mobility->flags.cowardly;
                };
                targetPiece->availableMoves.remove_if(removeCowardlyMovements);
            }
        }

        // cut cowardly moves on targeted square
        for each (PieceMovement* curPieceMovement in targetSquare->movements)
        {
            if (curPieceMovement->movement->mobility->flags.cowardly && curPieceMovement->piece->owner != curPiece->owner)
                CutMovement(curPieceMovement);
        }
    }

    if (targetPiece != nullptr)
        return nullptr;

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

    curMovement->legal = GetValidity(curPiece, curMovement, pin);
}

bool Board::GetValidity(Piece* curPiece, Movement* curMovement, PieceMovement* pin)
{
    Square* targetSquare = squares[curMovement->y][curMovement->x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    // if this movement is uninterruptible set it to ilegal if it is not on the end of the movement chain
    if (curMovement->mobility->flags.uninterruptible && curMovement->moveCounter != curMovement->mobility->limit)
        return false;

    // if this movement is cowardly set it to ilegal if target square is attacked by hostile piece
    if (curMovement->mobility->flags.cowardly)
    {
        auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
        {
            return (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove) && curPieceMovement->piece->owner != curPiece->owner;
        };

        auto it = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findPieceMovement);
        if (it != targetSquare->movements.end())
            return false;
    }

    // movement type validation
    switch (curMovement->mobility->type)
    {
    case::Move:
        // when moving you cant attack any pieces
        if (targetPiece != nullptr)
            return false;
        break;
    case::Attack:
        // when attacking you cant move to empty squares or attack your own pieces
        if (targetPiece == nullptr || targetPiece->owner == curPiece->owner)
            // not attacking the ghost as a vigilant move
            if (!(curMovement->mobility->flags.vigilant && curGhost != nullptr && curMovement->x == curGhost->x && curMovement->y == curGhost->y))
                return false;
        break;
    case::AttackMove:
        // when attack moving you cant attack your own pieces
        if (targetPiece != nullptr && targetPiece->owner == curPiece->owner)
            return false;
        break;
    }

    // king safety validation
    if (curPiece->name._Equal("King"))
    {
        // if we are validating kings move we cannot move to squares that are attacked by our opponent
        auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
        {
            return curPieceMovement->piece->owner != curPiece->owner && (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove);
        };

        auto it = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findPieceMovement);
        if (it != targetSquare->movements.end())
            return false;
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
                    return false;
            }
        }

        // if we are validating move of piece other than king and we are in check - must capture the piece attacking the king (only if one attacker) or block all attacker moves
        if (curPiece->owner == White && whiteCheck || curPiece->owner == Black && blackCheck)
        {
            // check if king is attacked by one piece and we are moving to its square
            if (!(checks.size() == 1 && targetSquare->occupyingPiece != nullptr && targetSquare->occupyingPiece == checks.front()->piece))
            {
                // king is attacked by any amount of pieces and we are blocking all of these attacks on this square (e.g. eagle + rook can be blocked simultaneously)
                for each (PieceMovement * check in checks)
                {
                    auto findPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
                    {
                        return curPieceMovement->piece == check->piece && check->movement->mobility == curPieceMovement->movement->mobility;
                    };

                    auto it = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findPieceMovement);
                    if (it == targetSquare->movements.end())
                    {
                        return false;
                        break;
                    }
                }
            }
        }
    }

    return true;
}

void Board::SetGhost(Ghost* newGhost)
{
    if (curGhost != nullptr)
    {
        Square* curSquare = squares[curGhost->y][curGhost->x];

        curGhost = newGhost;

        for each (PieceMovement * curPieceMovement in curSquare->movements)
        {
            if (curPieceMovement->movement->mobility->flags.vigilant)
                ValidateMove(curPieceMovement->piece, curPieceMovement->movement, GetPin(curPieceMovement->piece));
        }
    }
    
    curGhost = newGhost;

    if (newGhost != nullptr)
    {
        Square* newSquare = squares[newGhost->y][newGhost->x];

        for each (PieceMovement * curPieceMovement in newSquare->movements)
        {
            if (curPieceMovement->movement->mobility->flags.vigilant)
                ValidateMove(curPieceMovement->piece, curPieceMovement->movement, GetPin(curPieceMovement->piece));
        }
    }
}

Movement* Board::CreateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove)
{
    int cur_x, cur_y;

    if (prevMove != nullptr)
    {
        // calculate position of next move
        cur_x = prevMove->x;
        cur_y = prevMove->y;

        switch (curPiece->owner)
        {
        case::White:
            cur_x += curMobility->direction_x;
            cur_y -= curMobility->direction_y;
            break;
        case::Black:
            cur_x -= curMobility->direction_x;
            cur_y += curMobility->direction_y;
            break;
        }
    }
    else
    {
        // calculate positon of first move
        cur_x = curPiece->x;
        cur_y = curPiece->y;

        switch (curPiece->owner)
        {
        case::White:
            cur_x += curMobility->start_x;
            cur_y -= curMobility->start_y;
            break;
        case::Black:
            cur_x -= curMobility->start_x;
            cur_y += curMobility->start_y;
            break;
        }
    }

    // when new move is out of bounds return null
    if (cur_x < 0 || cur_x > COLUMNS - 1 || cur_y < 0 || cur_y > ROWS - 1)
        return nullptr;

    int moveCounter = 1;
    if (prevMove != nullptr)
        moveCounter = prevMove->moveCounter + 1;

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

    CutMovement(curPiece, nextMovement);
}

void Board::CutMovement(Piece* curPiece, Movement* curMovement)
{
    while (curMovement != nullptr)
    {
        Square* targetSquare = squares[curMovement->y][curMovement->x];
        Piece* targetPiece = targetSquare->occupyingPiece;

        // remove this movement from the targeted square
        auto removePieceMovements = [&](PieceMovement* pieceMovement) -> bool
        {
            return pieceMovement->piece == curPiece && pieceMovement->movement == curMovement;
        };
        targetSquare->movements.remove_if(removePieceMovements);

        if (curMovement->mobility->type == Attack || curMovement->mobility->type == AttackMove)
        {
            // after removing this hostile movement, revalidate opponent king moves on this square
            auto findKingPieceMovement = [&](PieceMovement* targetPieceMovement) -> bool
            {
                return targetPieceMovement->piece->name._Equal("King") && targetPieceMovement->piece->owner != curPiece->owner;
            };

            auto findKingPieceMovementIterator = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), findKingPieceMovement);
            if (findKingPieceMovementIterator != targetSquare->movements.end())
            {
                PieceMovement* kingPieceMovement = *findKingPieceMovementIterator;
                ValidateMove(kingPieceMovement->piece, kingPieceMovement->movement, nullptr);
            }

            // continue movement calculation of cowardly moves
            for each (PieceMovement* curPieceMovement in targetSquare->movements)
            {
                if (curPieceMovement->movement->mobility->flags.cowardly)
                    CalculateMoves(curPieceMovement->piece, curPieceMovement->movement->mobility, curPieceMovement->movement, GetPin(curPieceMovement->piece));
            }

            if (targetPiece != nullptr)
            {
                PieceMovement* pin = GetPin(targetPiece);
                for each (Mobility* targetMobility in targetPiece->mobilities)
                {
                    if (targetMobility->flags.cowardly)
                        CalculateMoves(targetPiece, targetMobility, nullptr, pin);
                }
            }
        }

        // continue with next movement
        Movement* prevMovement = curMovement;
        curMovement = curMovement->next;
        //delete prevMovement;
    }
}

void Board::MovePiece(Piece* curPiece, int x, int y)
{
    Square* sourceSquare = squares[curPiece->y][curPiece->x];
    Square* destinationSquare = squares[y][x];
    Piece* removedPiece = destinationSquare->occupyingPiece;

    if (!curPiece->hasMoved)
    {
        curPiece->hasMoved = true;
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

    // cancel inspiring movements targeting that were previously targeting this piece
    switch (curPiece->owner)
    {
    case::White:
        for each (Piece* whitePiece in whitePieces)
        {
            std::list<Movement*> ilegalMoves;

            for each (Movement* movement in whitePiece->availableMoves)
            {
                if (movement->mobility->flags.inspiring)
                {
                    int inspiring_x = whitePiece->x + movement->mobility->flags.affected_x;
                    int inspiring_y = whitePiece->y - movement->mobility->flags.affected_y;

                    if (inspiring_x == sourceSquare->x && inspiring_y == sourceSquare->y)
                    {
                        CutMovement(whitePiece, movement);
                        ilegalMoves.push_back(movement);
                    }
                }
            }

            auto removeIlegalMovements = [&](Movement* movement) -> bool
            {
                return std::find(ilegalMoves.begin(), ilegalMoves.end(), movement) != ilegalMoves.end();
            };
            whitePiece->availableMoves.remove_if(removeIlegalMovements);
        }
        break;
    case::Black:
        for each (Piece * blackPiece in blackPieces)
        {
            std::list<Movement*> ilegalMoves;

            for each (Movement * movement in blackPiece->availableMoves)
            {
                if (movement->mobility->flags.inspiring)
                {
                    int inspiring_x = blackPiece->x + movement->mobility->flags.affected_x;
                    int inspiring_y = blackPiece->y + movement->mobility->flags.affected_y;

                    if (inspiring_x == sourceSquare->x && inspiring_y == sourceSquare->y)
                    {
                        CutMovement(blackPiece, movement);
                    }
                }
            }

            auto removeIlegalMovements = [&](Movement* movement) -> bool
            {
                return std::find(ilegalMoves.begin(), ilegalMoves.end(), movement) != ilegalMoves.end();
            };
            blackPiece->availableMoves.remove_if(removeIlegalMovements);
        }
        break;
    }
}

void Board::PerformMove(int x1, int y1, int x2, int y2)
{
    Square* sourceSquare = squares[y1][x1];
    Piece* sourcePiece = sourceSquare->occupyingPiece;

    Square* destinationSquare = squares[y2][x2];

    if (sourcePiece == nullptr || (x1 == x2 && y1 == y2))
        return;

    // only perform this move if it is a valid movement of this piece
    auto findLegalPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
    {
        return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal;
    };

    auto legalPieceMovementIterator = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), findLegalPieceMovement);
    if (legalPieceMovementIterator == destinationSquare->movements.end())
        return;

    // remove parent of ghost if ghost is present on destination square and vigilant offensive move is found
    if (curGhost != nullptr && curGhost->x == x2 && curGhost->y == y2)
    {
        auto findVigilantPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
        {
            return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal && curPieceMovement->movement->mobility->flags.vigilant 
                && (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove);
        };

        auto vigilantPieceMovementIterator = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), findVigilantPieceMovement);
        if (vigilantPieceMovementIterator != destinationSquare->movements.end())
            RemovePiece(curGhost->parent);
    }

    // create ghost piece if hasty movement is found, delete ghost if it is not found
    auto findHastyPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
    {
        return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal && curPieceMovement->movement->mobility->flags.hasty;
    };

    auto hastyPieceMovementIterator = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), findHastyPieceMovement);
    if (hastyPieceMovementIterator != destinationSquare->movements.end())
    {
        PieceMovement* hastyPieceMovement = *hastyPieceMovementIterator;

        int ghost_x = x2;
        int ghost_y = y2;

        switch (sourcePiece->owner)
        {
        case::White:
            ghost_x -= hastyPieceMovement->movement->mobility->direction_x;
            ghost_y += hastyPieceMovement->movement->mobility->direction_y;
            break;
        case::Black:
            ghost_x += hastyPieceMovement->movement->mobility->direction_x;
            ghost_y -= hastyPieceMovement->movement->mobility->direction_y;
            break;
        }

        SetGhost(new Ghost(ghost_x, ghost_y, sourcePiece));
    }
    else
    {
        SetGhost(nullptr);
    }

    // find inspiring pieceMovement and move it one square behind the destination square
    auto findInspiringPieceMovement = [&](PieceMovement* curPieceMovement) -> bool
    {
        return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal && curPieceMovement->movement->mobility->flags.inspiring;
    };

    PieceMovement* inspiringPieceMovement = nullptr;
    auto inspiringPieceMovementIterator = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), findInspiringPieceMovement);
    if (inspiringPieceMovementIterator != destinationSquare->movements.end())
        inspiringPieceMovement = *inspiringPieceMovementIterator;
    
    MovePiece(sourcePiece, x2, y2);

    if (inspiringPieceMovement != nullptr)
    {
        int inspired_x1 = x1 + inspiringPieceMovement->movement->mobility->flags.affected_x;
        int inspired_y1 = y1;
        int inspired_x2 = x2;
        int inspired_y2 = y2;

        switch (sourcePiece->owner)
        {
        case::White:
            inspired_y1 -= inspiringPieceMovement->movement->mobility->flags.affected_y;
            inspired_x2 -= inspiringPieceMovement->movement->mobility->direction_x;
            inspired_y2 += inspiringPieceMovement->movement->mobility->direction_y;
            break;
        case::Black:
            inspired_y1 += inspiringPieceMovement->movement->mobility->flags.affected_y;
            inspired_x2 += inspiringPieceMovement->movement->mobility->direction_x;
            inspired_y2 -= inspiringPieceMovement->movement->mobility->direction_y;
        }

        Square* inspiredSquare = squares[inspired_y1][inspired_x1];
        Piece* inspiredPiece = inspiredSquare->occupyingPiece;
        MovePiece(inspiredPiece, inspired_x2, inspired_y2);
    }

    // if move was succesfuly performed while in check it means that check was resolved and all moves need to be revalidated
    if (sourcePiece->owner == White && whiteCheck)
    {
        whiteCheck = !whiteCheck;
        checks.clear();
        ValidateMoves(White);
    }
    else if (sourcePiece->owner == Black && blackCheck)
    {
        blackCheck = !blackCheck;
        checks.clear();
        ValidateMoves(Black);
    }
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
            } else if (curGhost != nullptr && cur_x == curGhost->x && cur_y == curGhost->y) {
                std::cout << "GGG";
            } else {
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