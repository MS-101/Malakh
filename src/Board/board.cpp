#include "board.h"
#include <iostream>
#include <algorithm>

Board::Board()
{
    for (int cur_y = 0; cur_y < ROWS; cur_y++)
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++)
            squares[cur_y][cur_x] = new Square(cur_x, cur_y);
}

Board::Board(EssenceConfig EssenceConfig) : Board()
{
    initBoard(essenceConfig);
}

Board::Board(Board* board) : Board()
{
    essenceConfig = board->essenceConfig;

    Ghost* ghost = board->curGhost;
    Piece* ghostParent = nullptr;

    for each (Piece* whitePiece in board->whitePieces) {
        Piece* newPiece = copyPiece(whitePiece);

        if (ghost != nullptr && (newPiece->x == ghost->parent->x && newPiece->y == ghost->parent->y))
            ghostParent = newPiece;
    }

    for each (Piece* blackPiece in board->blackPieces) {
        Piece* newPiece = copyPiece(blackPiece);

        if (ghost != nullptr && (newPiece->x == ghost->parent->x && newPiece->y == ghost->parent->y))
            ghostParent = newPiece;
    }

    for each (PieceMovement* check in board->checks) {
        Square* square = squares[check->movement->y][check->movement->x];
        auto checkIt = std::find(square->movements.begin(), square->movements.end(), check);

        PieceMovement* newCheck = *checkIt;
        checks.push_back(newCheck);
    }

    whiteCheck = board->whiteCheck;
    blackCheck = board->blackCheck;

    curTurn = board->curTurn;
    if (curTurn == Black)
        hash ^= zobrist.turn;

    if (ghost != nullptr)
        curGhost = new Ghost(ghost->x, ghost->y, ghostParent);

    curPhase = board->curPhase;
    matEval = board->matEval;
    mg_pcsqEval = board->mg_pcsqEval;
    eg_pcsqEval = board->eg_pcsqEval;

    initialized = true;
}

Board::~Board()
{
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            Square* curSquare = squares[cur_y][cur_x];
            if (curSquare != nullptr)
                delete curSquare;
        }
    }

    whitePieces.clear();
    blackPieces.clear();
    checks.clear();

    if (curGhost != nullptr)
        delete curGhost;
}

void Board::initBoard(EssenceConfig essenceConfig)
{
    this->essenceConfig = essenceConfig;

    for each (Piece* piece in whitePieces)
        removePiece(piece);

    for each (Piece* piece in blackPieces)
        removePiece(piece);

    int pawnCount = 8;

    // White pieces
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        addPiece(new Piece(Pawn, White, essenceConfig.whitePawnEssence), pawnIterator, 6);

    addPiece(new Piece(Rook, White, essenceConfig.whiteRookEssence), 0, 7);
    addPiece(new Piece(Knight, White, essenceConfig.whiteKnightEssence), 1, 7);
    addPiece(new Piece(Bishop, White, essenceConfig.whiteBishopEssence), 2, 7);
    addPiece(new Piece(Queen, White, Classic), 3, 7);
    addPiece(new Piece(King, White, Classic), 4, 7);
    addPiece(new Piece(Bishop, White, essenceConfig.whiteBishopEssence), 5, 7);
    addPiece(new Piece(Knight, White, essenceConfig.whiteKnightEssence), 6, 7);
    addPiece(new Piece(Rook, White, essenceConfig.whiteRookEssence), 7, 7);

    // Black pieces
    for (int pawnIterator = 0; pawnIterator < pawnCount; pawnIterator++)
        addPiece(new Piece(Pawn, Black, essenceConfig.blackPawnEssence), pawnIterator, 1);

    addPiece(new Piece(Rook, Black, essenceConfig.blackRookEssence), 0, 0);
    addPiece(new Piece(Knight, Black, essenceConfig.blackKnightEssence), 1, 0);
    addPiece(new Piece(Bishop, Black, essenceConfig.blackBishopEssence), 2, 0);
    addPiece(new Piece(Queen, Black, Classic), 3, 0);
    addPiece(new Piece(King, Black, Classic), 4, 0);
    addPiece(new Piece(Bishop, Black, essenceConfig.blackBishopEssence), 5, 0);
    addPiece(new Piece(Knight, Black, essenceConfig.blackKnightEssence), 6, 0);
    addPiece(new Piece(Rook, Black, essenceConfig.blackRookEssence), 7, 0);

    calculateMoves();
    initialized = true;
}

bool Board::addPiece(Piece* curPiece, int x, int y)
{
    Square* curSquare = squares[y][x];
    if (curPiece == nullptr || curSquare->occupyingPiece != nullptr)
        return false;

    curPiece->x = x;
    curPiece->y = y;

    int y_pcsq = Evaluation::getY_pcsq(curPiece);

    hash ^= getZobrist(curPiece);
    curPhase += Evaluation::piecePhaseValues[curPiece->type];
    matEval[curPiece->color] += Evaluation::pieceMatValues[curPiece->type];
    mg_pcsqEval[curPiece->color] += Evaluation::mg_pcsq[curPiece->type][y_pcsq][x];
    eg_pcsqEval[curPiece->color] += Evaluation::eg_pcsq[curPiece->type][y_pcsq][x];

    curSquare->occupyingPiece = curPiece;
    switch (curPiece->color) {
        case::White:
            whitePieces.push_back(curPiece);
            break;
        case::Black:
            blackPieces.push_back(curPiece);
            break;
    }

    if (initialized) {
        // cut movements that were blocked by adding this piece
        for each (PieceMovement* curMovement in curSquare->movements)
            cutMovement(curMovement);

        blockPins(curSquare);
    }

    if (curPiece->type == King) {
        if (curPiece->color == White)
            whiteKing = curPiece;
        else
            blackKing = curPiece;
    }

    return true;
}

Piece* Board::copyPiece(Piece* piece)
{
    Piece* newPiece = new Piece(piece->type, piece->color, piece->essence);
    newPiece->hasMoved = piece->hasMoved;
    addPiece(newPiece, piece->x, piece->y);

    for each (Movement* move in piece->movements) {
        Movement* prevMove = nullptr;
        Movement* curMove = move;
        bool isFirst = true;

        while (curMove != nullptr) {
            Movement* newMovement = new Movement(curMove->x, curMove->y, curMove->moveCounter, curMove->legal, curMove->mobility, nullptr);
            if (isFirst) {
                newPiece->movements.push_back(newMovement);
                isFirst = false;
            }

            PieceMovement* newPieceMovement = new PieceMovement(newPiece, newMovement);
            Square* square = squares[newMovement->y][newMovement->x];
            square->movements.push_back(newPieceMovement);

            if (prevMove != nullptr)
                prevMove->next = newMovement;
            prevMove = newMovement;
            curMove = curMove->next;
        }
    }

    for each (Movement* virtualMove in piece->virtualMovements) {
        Movement* prevMove = nullptr;
        Movement* curMove = virtualMove;
        bool isFirst = true;

        while (curMove != nullptr) {
            Movement* newMovement = new Movement(curMove->x, curMove->y, curMove->moveCounter, curMove->legal, curMove->mobility, nullptr);
            if (isFirst) {
                newPiece->virtualMovements.push_back(newMovement);
                isFirst = false;
            }

            PieceMovement* newPieceMovement = new PieceMovement(newPiece, newMovement);
            Square* square = squares[newMovement->y][newMovement->x];
            square->virtualMovements.push_back(newPieceMovement);

            if (prevMove != nullptr)
                prevMove->next = newMovement;
            prevMove = newMovement;
            curMove = curMove->next;
        }
    }

    return newPiece;
}

void Board::changePiece(Piece* piece, PieceType type, Essence essence)
{
    int y_pcsq = Evaluation::getY_pcsq(piece);

    curPhase -= Evaluation::piecePhaseValues[piece->type];
    curPhase += Evaluation::piecePhaseValues[type];
    matEval[piece->color] -= Evaluation::pieceMatValues[piece->type];
    matEval[piece->color] += Evaluation::pieceMatValues[type];
    mg_pcsqEval[piece->color] -= Evaluation::mg_pcsq[piece->type][y_pcsq][piece->x];
    mg_pcsqEval[piece->color] += Evaluation::mg_pcsq[type][y_pcsq][piece->x];
    eg_pcsqEval[piece->color] -= Evaluation::eg_pcsq[piece->type][y_pcsq][piece->x];
    eg_pcsqEval[piece->color] += Evaluation::eg_pcsq[type][y_pcsq][piece->x];

    removeMoves(piece);
    piece->change(type, essence);
    calculateMoves(piece);
}

void Board::removePiece(Piece* curPiece)
{
    if (curPiece == nullptr)
        return;

    int y_pcsq = Evaluation::getY_pcsq(curPiece);

    hash ^= getZobrist(curPiece);
    curPhase -= Evaluation::piecePhaseValues[curPiece->type];
    matEval[curPiece->color] -= Evaluation::pieceMatValues[curPiece->type];
    mg_pcsqEval[curPiece->color] -= Evaluation::mg_pcsq[curPiece->type][y_pcsq][curPiece->x];
    eg_pcsqEval[curPiece->color] -= Evaluation::eg_pcsq[curPiece->type][y_pcsq][curPiece->x];

    removeMoves(curPiece);
    removeInspiringMoves(curPiece);

    Square* curSquare = squares[curPiece->y][curPiece->x];
    curSquare->occupyingPiece = nullptr;

    switch (curPiece->color) {
        case::White:
            whitePieces.remove(curPiece);
            break;
        case::Black:
            blackPieces.remove(curPiece);
            break;
    }

    delete curPiece;
}

std::vector<LegalMove> Board::getLegalMoves(PieceColor color)
{
    std::list<LegalMove> decisions;
    if (color == White) {
        for each (Piece* whitePiece in whitePieces)
            decisions.splice(decisions.end(), getLegalMoves(whitePiece));
    } else {
        for each (Piece* blackPiece in blackPieces)
            decisions.splice(decisions.end(), getLegalMoves(blackPiece));
    }

    // vectors are faster for iteration, which is important for fast minimax function
    std::vector<LegalMove> result(decisions.begin(), decisions.end());

    return result;
}

std::list<LegalMove> Board::getLegalMoves(Piece* curPiece)
{
    std::list<LegalMove> decisions;

    for each (Movement* move in curPiece->movements) {
        Movement* curMove = move;

        while (curMove != nullptr) {
            if (curMove->legal) {
                if (curPiece->type == Pawn && ((curPiece->color == White && curMove->y == 0) || (curPiece->color == Black && curMove->y == 7))) {
                    decisions.push_back(LegalMove{ curPiece->x, curPiece->y, curMove->x, curMove->y, Queen, curMove->mobility });
                    decisions.push_back(LegalMove{ curPiece->x, curPiece->y, curMove->x, curMove->y, Rook, curMove->mobility });
                    decisions.push_back(LegalMove{ curPiece->x, curPiece->y, curMove->x, curMove->y, Bishop, curMove->mobility });
                    decisions.push_back(LegalMove{ curPiece->x, curPiece->y, curMove->x, curMove->y, Knight, curMove->mobility });
                } else {
                    decisions.push_back(LegalMove{ curPiece->x, curPiece->y, curMove->x, curMove->y, Pawn, curMove->mobility });
                }
            }

            curMove = curMove->next;
        }
    }

    return decisions;
}

// KING SAFETY PROBLEM

// Problem 1: King should not be allowed to move to unsafe squares
// When kings moves are being calculated, set the movement to illegal if there is opponent attack on that square otherwise it is legal.
// When other piece moves are being calculated, set the oposing king moves as ilegal when attacking their square.
// When other piece moves are being removed, set the oposing king moves as legal when no other hostile piece is attacking that square.


// Problem 2: Pinned pieces should not be allowed to move in a way that endangers the king.
// Before calculating any moves of piece other than king check if removing this piece from current square endangers the king.
// If that is the case check for each specific movement if your king is in danger after moving that piece to that square.
// After moving piece check if it pins any other piece and if it is the case recalculate its moves.

// Problem 3: When king is in check it has to be resolved otherwise its checkmate.
// When a newly calculated move attacks a kings square recalculate validity of all opponent moves.
// Only the following moves are valid:
// - King moves to a safe square
// This is valid only if one piece is attacking king:
// - Piece other than king moves and blocks attack on king. (this cannot reveal an attack on king)
// - Piece other than king removes attacking piece. (this cannot reveal an attack on king)
// After player resolves a checkmate recalculate validity of all their moves.

// EN PASSANT

// When pawn has not moved, can move forward 2 squares.
// Oponent pawns can attack the pawn as if it has moved 1 square forward on the following move.

// CASTLING 

// For queenside castling the left rook has not moved, there are no pieces on squares b1, c1, d1, king has not moved and no hostile piece is attacking the king or squares b1, c1, d1
// For kingside castling the right rook has not moved, there are no pieces on squares f1, g1, king has not moved and no hostile piece is attacking the king or squares f1, g1

void Board::calculateMoves()
{
    for each (Piece* whitePiece in whitePieces)
        calculateMoves(whitePiece);

    for each (Piece* blackPiece in blackPieces)
        calculateMoves(blackPiece);
}

void Board::calculateMoves(Piece* curPiece)
{
    removeMoves(curPiece);

    PieceMovement* pin = getPin(curPiece);

    for each (Mobility* curMobility in curPiece->getMobilities()) {
        calculateVirtualMoves(curPiece, curMobility);
        calculateMoves(curPiece, curMobility, nullptr, pin);
    }
}

void Board::calculateVirtualMoves(Piece* curPiece, Mobility* curMobility)
{
    if (curMobility->type == Move)
        return;

    PinArgs pinArgs{};

    Movement* prevMove = nullptr;
    if (curMobility->limit == 0) {
        while ((prevMove = calculateVirtualMove(curPiece, curMobility, prevMove, &pinArgs)) != nullptr);
    } else {
        for (int i = 0; i < curMobility->limit; i++)
            if ((prevMove = calculateVirtualMove(curPiece, curMobility, prevMove, &pinArgs)) == nullptr)
                break;
    }

    if (pinArgs.kingHit && pinArgs.pinnedCount == 1 && pinArgs.pinnedPiece->color != curPiece->color)
        validateMoves(pinArgs.pinnedPiece, pinArgs.pin);
}

Movement* Board::calculateVirtualMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PinArgs* pinArgs)
{
    // movement calculation fails if this move can only be performed on first turn
    if (curMobility->flags.initiative && curPiece->hasMoved)
        return nullptr;

    // movement calculation fails if it cannot inspire a piece
    if (curMobility->flags.inspiring && curPiece->hasMoved) {
        int inspired_x = curPiece->x + curMobility->flags.affected_x;
        int inspired_y = curPiece->y;

        switch (curPiece->color) {
        case::White:
            inspired_y -= curMobility->flags.affected_y;
            break;
        case::Black:
            inspired_y += curMobility->flags.affected_y;
            break;
        }

        if (inspired_x < 0 || inspired_x > COLUMNS - 1 || inspired_y < 0 || inspired_y > ROWS - 1)
            return nullptr;

        Square* inspiredSquare = squares[inspired_y][inspired_x];
        Piece* inspiredPiece = inspiredSquare->occupyingPiece;

        if (inspiredPiece == nullptr || inspiredPiece->hasMoved || inspiredPiece->color != curPiece->color)
            return nullptr;
    }

    Movement* newMove = createMove(curPiece, curMobility, prevMove);
    if (newMove == nullptr)
        return nullptr;

    Square* targetSquare = squares[newMove->y][newMove->x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    if (targetPiece != nullptr && !pinArgs->kingHit) {
        if (targetPiece->type == King && targetPiece->color != curPiece->color) {
            pinArgs->kingHit = true;
        } else {
            pinArgs->pinnedPiece = targetPiece;
            pinArgs->pinnedCount++;
        }
    }

    // add new movement to current piece
    if (prevMove == nullptr)
        curPiece->virtualMovements.push_back(newMove);
    else
        prevMove->next = newMove;

    // add new movement to target square
    PieceMovement* newPieceMove = new PieceMovement(curPiece, newMove);
    targetSquare->virtualMovements.push_back(newPieceMove);
    if (prevMove == nullptr)
        pinArgs->pin = newPieceMove;

    return newMove;
}

void Board::calculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin)
{
    validateMove(curPiece, prevMove, pin);

    if (curMobility->limit == 0) {
        while ((prevMove = calculateMove(curPiece, curMobility, prevMove, pin)) != nullptr);
    } else {
        // if move has a limit we need to find out how many moves can be performed
        int curMoveIndex = 0;
        if (prevMove != nullptr)
            curMoveIndex = prevMove->moveCounter;

        for (int i = curMoveIndex; i < curMobility->limit; i++)
            if ((prevMove = calculateMove(curPiece, curMobility, prevMove, pin)) == nullptr)
                break;
    }
}

Movement* Board::calculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin)
{
    // movement calculation fails if this move can only be performed on first turn
    if (curMobility->flags.initiative && curPiece->hasMoved)
        return nullptr;

    // movement calculation fails if it cannot inspire a piece
    if (curMobility->flags.inspiring && !curPiece->hasMoved) {
        int inspired_x = curPiece->x + curMobility->flags.affected_x;
        int inspired_y = curPiece->y;

        switch (curPiece->color) {
        case::White:
            inspired_y -= curMobility->flags.affected_y;
            break;
        case::Black:
            inspired_y += curMobility->flags.affected_y;
            break;
        }

        if (inspired_x < 0 || inspired_x > COLUMNS - 1 || inspired_y < 0 || inspired_y > ROWS - 1)
            return nullptr;

        Square* inspiredSquare = squares[inspired_y][inspired_x];
        Piece* inspiredPiece = inspiredSquare->occupyingPiece;

        if (inspiredPiece == nullptr || inspiredPiece->hasMoved || inspiredPiece->color != curPiece->color)
            return nullptr;
    }

    // movement calculation fails if previous move was interrupted cowardly move
    if (curMobility->flags.cowardly && prevMove != nullptr) {
        Square* prevSquare = squares[prevMove->y][prevMove->x];
        for each (PieceMovement* curPieceMovement in prevSquare->movements) {
            if ((curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove) && curPieceMovement->piece->color != curPiece->color)
                return nullptr;
        }
    }

    Movement* newMove = createMove(curPiece, curMobility, prevMove);
    if (newMove == nullptr)
        return nullptr;

    validateMove(curPiece, newMove, pin);

    Square* targetSquare = squares[newMove->y][newMove->x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    // add new movement to current piece
    if (prevMove == nullptr)
        curPiece->movements.push_back(newMove);
    else
        prevMove->next = newMove;

    // add new movement to target square
    PieceMovement* newPieceMove = new PieceMovement(curPiece, newMove);
    targetSquare->movements.push_back(newPieceMove);

    if (curMobility->type == Attack || curMobility->type == AttackMove) {
        // invalidate king moves
        auto kingPieceMovementIt = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
            return curPieceMovement->piece->type == King && curPieceMovement->piece->color != curPiece->color;
        });

        if (kingPieceMovementIt != targetSquare->movements.end()) {
            PieceMovement* kingPieceMovement = *kingPieceMovementIt;
            if (kingPieceMovement->movement->legal) {
                kingPieceMovement->movement->legal = false;
            }
        }

        if (targetPiece != nullptr) {
            if (targetPiece->color != curPiece->color) {
                if (targetPiece->type == King) {
                    // when targeting opponent king revalidate all attacked player moves
                    switch (targetPiece->color) {
                    case::White:
                        whiteCheck = true;
                        break;
                    case::Black:
                        blackCheck = true;
                        break;
                    }
                    checks.push_front(newPieceMove);

                    validateMoves(targetPiece->color);
                }
                
                PieceMovement* targetPin = getPin(targetPiece);

                // cut cowardly moves of targeted piece
                for each (Movement* targetMovement in targetPiece->movements) {
                    if (targetMovement->mobility->flags.cowardly) {
                        validateMove(targetPiece, targetMovement, targetPin);

                        Movement* nextTargetMovement = targetMovement->next;
                        if (nextTargetMovement != nullptr) {
                            targetMovement->next = nullptr;
                            cutMovement(targetPiece, nextTargetMovement);
                        }
                    }
                }
            }
        }

        // cut cowardly moves on targeted square
        for each (PieceMovement* curPieceMovement in targetSquare->movements) {
            if (curPieceMovement->movement->mobility->flags.cowardly && curPieceMovement->piece->color != curPiece->color)
                cutMovement(curPieceMovement);
        }
    }

    if (targetPiece != nullptr)
        return nullptr;
    return newMove;
}

void Board::calculateInspiringMoves(Piece* inspiredPiece)
{
    std::list<Piece*> pieces;
    if (inspiredPiece->color == White)
        pieces = whitePieces;
    else
        pieces = blackPieces;

    for each (Piece* piece in pieces) {
        for each (Mobility* mobility in piece->getMobilities()) {
            if (mobility->flags.inspiring) {
                int inspiring_x = piece->x + mobility->flags.affected_x;
                int inspiring_y = piece->y;

                if (piece->color == White)
                    inspiring_y -= mobility->flags.affected_y;
                else
                    inspiring_y += mobility->flags.affected_y;

                if (inspiring_x == inspiredPiece->x && inspiring_y == inspiredPiece->y) {
                    PieceMovement* pin = getPin(piece);
                    calculateMoves(piece, mobility, nullptr, pin);
                }
            }
        }
    }
}

void Board::removeInspiringMoves(Piece* inspiredPiece)
{
    std::list<Piece*> pieces;
    if (inspiredPiece->color == White)
        pieces = whitePieces;
    else
        pieces = blackPieces;

    for each (Piece* piece in pieces) {
        std::list<Movement*> ilegalMoves;

        for each (Movement* movement in piece->movements) {
            if (movement->mobility->flags.inspiring) {
                int inspiring_x = piece->x + movement->mobility->flags.affected_x;
                int inspiring_y = piece->y;

                if (piece->color == White)
                    inspiring_y -= movement->mobility->flags.affected_y;
                else
                    inspiring_y += movement->mobility->flags.affected_y;

                if (inspiring_x == inspiredPiece->x && inspiring_y == inspiredPiece->y) {
                    cutMovement(piece, movement);
                    ilegalMoves.push_back(movement);
                }
            }
        }

        piece->movements.remove_if([&](Movement* movement) -> bool {
            return std::find(ilegalMoves.begin(), ilegalMoves.end(), movement) != ilegalMoves.end();
        });
    }
}

// this method is called when check is made and when check is resolved
void Board::validateMoves(PieceColor owner)
{
    std::list<Piece*> pieces;
    if (owner == White)
        pieces = whitePieces;
    else
        pieces = blackPieces;

    for each (Piece* piece in pieces) {
        PieceMovement* pin = getPin(piece);
        validateMoves(piece, pin);
    }
}

void Board::validateMoves(Piece* curPiece, PieceMovement* pin)
{
    for each (Movement* curMovement in curPiece->movements) {
        while (curMovement != nullptr) {
            validateMove(curPiece, curMovement, pin);
            curMovement = curMovement->next;
        }
    }
}

void Board::validateMove(Piece* curPiece, Movement* curMovement, PieceMovement* pin)
{
    if (curMovement == nullptr)
        return;

    curMovement->legal = getValidity(curPiece, curMovement, pin);
}

bool Board::getValidity(Piece* curPiece, Movement* curMovement, PieceMovement* pin)
{
    Square* targetSquare = squares[curMovement->y][curMovement->x];
    Piece* targetPiece = targetSquare->occupyingPiece;

    // if this movement is uninterruptible set it to ilegal if it is not on the end of the movement chain
    if (curMovement->mobility->flags.uninterruptible && curMovement->moveCounter != curMovement->mobility->limit)
        return false;

    // if this movement is cowardly set it to ilegal if target square is attacked by hostile piece
    if (curMovement->mobility->flags.cowardly) {
        if (std::any_of(targetSquare->movements.begin(), targetSquare->movements.end(), [curPiece](PieceMovement* curPieceMovement) {
            return (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove) && curPieceMovement->piece->color != curPiece->color;
        })) {
            return false;
        }
    }

    // movement type validation
    switch (curMovement->mobility->type) {
    case::Move:
        // when moving you cant attack any pieces
        if (targetPiece != nullptr)
            return false;
        break;
    case::Attack:
        // when attacking you cant move to empty squares or attack your own pieces
        if (targetPiece == nullptr || targetPiece->color == curPiece->color)
            // not attacking the ghost as a vigilant move
            if (!(curMovement->mobility->flags.vigilant && curGhost != nullptr && curMovement->x == curGhost->x && curMovement->y == curGhost->y))
                return false;
        break;
    case::AttackMove:
        // when attack moving you cant attack your own pieces
        if (targetPiece != nullptr && targetPiece->color == curPiece->color)
            return false;
        break;
    }

    // king safety validation
    if (curPiece->type == King) {
        // if we are validating kings move we cannot move to squares that are attacked by our opponent
        if (std::any_of(targetSquare->movements.begin(), targetSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
            return curPieceMovement->piece->color != curPiece->color && (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove);
        })) {
            return false;
        }
    } else {
        // if we are validating move of piece other than king - must capture the pinning piece or move along the pins vector
        if (pin != nullptr) {
            if (targetSquare->occupyingPiece != pin->piece) {
                Movement* curPinMovement = pin->movement;
                while (curPinMovement != nullptr) {
                    if (curPinMovement->x == targetSquare->x && curPinMovement->y == targetSquare->y)
                        break;

                    curPinMovement = curPinMovement->next;
                }

                if (curPinMovement == nullptr)
                    return false;
            }
        }

        // if we are validating move of piece other than king and we are in check - must capture the piece attacking the king (only if one attacker) or block all attacker moves
        if (curPiece->color == White && whiteCheck || curPiece->color == Black && blackCheck) {
            // check if king is attacked by one piece and we are moving to its square
            if (!(checks.size() == 1 && targetSquare->occupyingPiece != nullptr && targetSquare->occupyingPiece == checks.front()->piece)) {
                // king is attacked by any amount of pieces and we are blocking all of these attacks on this square (e.g. eagle + rook can be blocked simultaneously)
                for each (PieceMovement * check in checks) {
                    if (!std::any_of(targetSquare->movements.begin(), targetSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
                        return curPieceMovement->piece == check->piece && check->movement->mobility == curPieceMovement->movement->mobility;
                    })) {
                        return false;
                    }    
                }
            }
        }
    }

    return true;
}

void Board::setGhost(Ghost* newGhost)
{
    if (curGhost != nullptr) {
        Square* curSquare = squares[curGhost->y][curGhost->x];

        delete curGhost;
        curGhost = newGhost;

        for each (PieceMovement* curPieceMovement in curSquare->movements) {
            if (curPieceMovement->movement->mobility->flags.vigilant) {
                PieceMovement* pin = getPin(curPieceMovement->piece);
                validateMove(curPieceMovement->piece, curPieceMovement->movement, pin);
            }
        }
    }
    
    curGhost = newGhost;

    if (newGhost != nullptr) {
        Square* newSquare = squares[newGhost->y][newGhost->x];

        for each (PieceMovement* curPieceMovement in newSquare->movements) {
            if (curPieceMovement->movement->mobility->flags.vigilant) {
                PieceMovement* pin = getPin(curPieceMovement->piece);
                validateMove(curPieceMovement->piece, curPieceMovement->movement, pin);
            }
        }
    }
}

Movement* Board::createMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove)
{
    int cur_x, cur_y;

    if (prevMove != nullptr) {
        // calculate position of next move
        cur_x = prevMove->x + curMobility->direction_x;
        cur_y = prevMove->y;

        switch (curPiece->color) {
        case::White:
            cur_y -= curMobility->direction_y;
            break;
        case::Black:
            cur_y += curMobility->direction_y;
            break;
        }
    } else {
        // calculate positon of first move
        cur_x = curPiece->x + curMobility->start_x;
        cur_y = curPiece->y;

        switch (curPiece->color) {
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
        moveCounter = prevMove->moveCounter + 1;

    // create new movement
    return new Movement(cur_x, cur_y, moveCounter, false, curMobility, nullptr);
}

PieceMovement* Board::getPin(Piece* curPiece)
{
    if (curPiece->type == King)
        return nullptr;

    Square* curSquare = squares[curPiece->y][curPiece->x];
    for each (PieceMovement* virtualMovement in curSquare->virtualMovements) {
        Piece* attackerPiece = virtualMovement->piece;
        if (attackerPiece->color != curPiece->color) {
            auto it = std::find_if(attackerPiece->virtualMovements.begin(), attackerPiece->virtualMovements.end(), [&](Movement* curMovement) -> bool {
                return curMovement->mobility == virtualMovement->movement->mobility;
            });

            if (it != attackerPiece->virtualMovements.end()) {
                PinArgs pinArgs{};
                Movement* curMovement = *it;

                Square* pinOrigin = squares[curMovement->y][curMovement->x];
                auto pinIt = std::find_if(pinOrigin->virtualMovements.begin(), pinOrigin->virtualMovements.end(), [&](PieceMovement* curMovement) -> bool {
                    return curMovement->piece == virtualMovement->piece && curMovement->movement->mobility == virtualMovement->movement->mobility;
                });
                PieceMovement* pin = *pinIt;
                
                while (curMovement != nullptr) {
                    Square* targetSquare = squares[curMovement->y][curMovement->x];
                    Piece* targetPiece = targetSquare->occupyingPiece;

                    if (targetPiece != nullptr) {
                        if (targetPiece->type == King && targetPiece->color == curPiece->color) {
                            pinArgs.kingHit = true;
                            break;
                        } else if (targetPiece == curPiece) {
                            pinArgs.pinnedCount++;
                        } else {
                            break;
                        }
                    }

                    curMovement = curMovement->next;
                }

                if (pinArgs.kingHit) {
                    if (pinArgs.pinnedCount == 1)
                        return pin;
                }
            }
        }
    }

    return nullptr;
}

void Board::removeMoves(Piece* curPiece)
{
    if (curPiece == nullptr)
        return;

    for each (Movement* curMovement in curPiece->movements)
        cutMovement(curPiece, curMovement);
    curPiece->movements.clear();

    for each (Movement* curVirtualMovement in curPiece->virtualMovements)
        removeVirtualMoves(curPiece, curVirtualMovement);
    curPiece->virtualMovements.clear();
}

void Board::cutMovement(PieceMovement* curPieceMovement)
{
    Piece* curPiece = curPieceMovement->piece;
    Movement* curMovement = curPieceMovement->movement;

    PieceMovement* pin = getPin(curPiece);
    validateMove(curPiece, curMovement, pin);

    Movement* nextMovement = curMovement->next;
    curMovement->next = nullptr;

    cutMovement(curPiece, nextMovement);
}

void Board::removeVirtualMoves(Piece* curPiece, Movement* curMovement)
{
    PinArgs pinArgs{};

    while (curMovement != nullptr) {
        Square* targetSquare = squares[curMovement->y][curMovement->x];
        Piece* targetPiece = targetSquare->occupyingPiece;

        // remove this movement from the targeted square
        auto targetPieceMovementIt = std::find_if(targetSquare->virtualMovements.begin(), targetSquare->virtualMovements.end(), [&](PieceMovement* pieceMovement) -> bool {
            return pieceMovement->piece == curPiece && pieceMovement->movement == curMovement;
        });

        if (targetPieceMovementIt != targetSquare->virtualMovements.end()) {
            PieceMovement* targetPieceMovement = *targetPieceMovementIt;
            targetSquare->virtualMovements.remove(targetPieceMovement);

            delete targetPieceMovement;
        }

        if (targetPiece != nullptr && !pinArgs.kingHit) {
            if (targetPiece->type == King && targetPiece->color != curPiece->color) {
                pinArgs.kingHit = true;
            } else {
                pinArgs.pinnedPiece = targetPiece;
                pinArgs.pinnedCount++;
            }
        }

        // continue with next movement
        Movement* prevMovement = curMovement;
        curMovement = curMovement->next;
        delete prevMovement;
    }

    // pinned piece is no longer pinned by this movement, but may be pinned by another piece
    if (pinArgs.kingHit && pinArgs.pinnedCount == 1 && pinArgs.pinnedPiece->color != curPiece->color)
        validateMoves(pinArgs.pinnedPiece, getPin(pinArgs.pinnedPiece));
}

void Board::cutMovement(Piece* curPiece, Movement* curMovement)
{
    while (curMovement != nullptr) {
        Square* targetSquare = squares[curMovement->y][curMovement->x];
        Piece* targetPiece = targetSquare->occupyingPiece;

        // remove this movement from the targeted square
        auto targetPieceMovementIt = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), [&](PieceMovement* pieceMovement) -> bool {
            return pieceMovement->piece == curPiece && pieceMovement->movement == curMovement;
        });

        if (targetPieceMovementIt != targetSquare->movements.end()) {
            PieceMovement* targetPieceMovement = *targetPieceMovementIt;
            targetSquare->movements.remove(targetPieceMovement);

            if (targetPiece != nullptr && targetPiece->type == King && targetPiece->color != curPiece->color && (curMovement->mobility->type == Attack || curMovement->mobility->type == AttackMove)) {
                checks.remove(targetPieceMovement);

                if (checks.size() == 0) {
                    if (whiteCheck) {
                        whiteCheck = false;
                        validateMoves(targetPiece->color);
                    } else {
                        blackCheck = false;
                        validateMoves(targetPiece->color);
                    }
                }
            }

            delete targetPieceMovement;
        }

        if (curMovement->mobility->type == Attack || curMovement->mobility->type == AttackMove) {
            // after removing this hostile movement, revalidate opponent king moves on this square
            auto kingPieceMovementIt = std::find_if(targetSquare->movements.begin(), targetSquare->movements.end(), [&](PieceMovement* targetPieceMovement) -> bool {
                return targetPieceMovement->piece->type == King && targetPieceMovement->piece->color != curPiece->color;
            });

            if (kingPieceMovementIt != targetSquare->movements.end()) {
                PieceMovement* kingPieceMovement = *kingPieceMovementIt;
                validateMove(kingPieceMovement->piece, kingPieceMovement->movement, nullptr);
            }

            // continue movement calculation of cowardly moves on previously attacked square
            if (targetPiece == nullptr) {
                for each (PieceMovement * targetPieceMovement in targetSquare->movements) {
                    if (targetPieceMovement->piece->color != curPiece->color && targetPieceMovement->movement->mobility->flags.cowardly) {
                        if (!std::any_of(targetSquare->movements.begin(), targetSquare->movements.end(), [targetPieceMovement](PieceMovement* curPieceMovement) {
                            return (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove) && curPieceMovement->piece->color != targetPieceMovement->piece->color;
                        })) {
                            PieceMovement* pin = getPin(targetPieceMovement->piece);
                            calculateMoves(targetPieceMovement->piece, targetPieceMovement->movement->mobility, targetPieceMovement->movement, pin);
                        }
                    }
                }
            }

            if (targetPiece != nullptr && targetPiece->color != curPiece->color) {
                // continute movement calculation of cowardly moves if I was previously attacking opponent piece
                if (!std::any_of(targetSquare->movements.begin(), targetSquare->movements.end(), [targetPiece](PieceMovement* curPieceMovement) {
                    return (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove) && curPieceMovement->piece->color != targetPiece->color;
                })) {
                    PieceMovement* targetPin = getPin(targetPiece);
                    for each (Mobility* targetMobility in targetPiece->getMobilities()) {
                        if (targetMobility->flags.cowardly)
                            calculateMoves(targetPiece, targetMobility, nullptr, targetPin);
                    }
                }
            }
        }

        // continue with next movement
        Movement* prevMovement = curMovement;
        curMovement = curMovement->next;
        delete prevMovement;
    }
}

void Board::movePiece(Piece* curPiece, int x, int y, bool hasMoved)
{
    int y_pcsq = Evaluation::getY_pcsq(curPiece);
    hash ^= getZobrist(curPiece);
    mg_pcsqEval[curPiece->color] -= Evaluation::mg_pcsq[curPiece->type][y_pcsq][curPiece->x];
    eg_pcsqEval[curPiece->color] -= Evaluation::eg_pcsq[curPiece->type][y_pcsq][curPiece->x];

    // cancel inspiring movements targeting that were previously targeting this piece
    removeInspiringMoves(curPiece);

    Square* sourceSquare = squares[curPiece->y][curPiece->x];
    Square* destinationSquare = squares[y][x];
    Piece* removedPiece = destinationSquare->occupyingPiece;

    removePiece(removedPiece);

    sourceSquare->occupyingPiece = nullptr;
    destinationSquare->occupyingPiece = curPiece;
    curPiece->x = x;
    curPiece->y = y;
    curPiece->hasMoved = hasMoved;

    y_pcsq = Evaluation::getY_pcsq(curPiece);
    hash ^= getZobrist(curPiece);
    mg_pcsqEval[curPiece->color] += Evaluation::mg_pcsq[curPiece->type][y_pcsq][curPiece->x];
    eg_pcsqEval[curPiece->color] += Evaluation::eg_pcsq[curPiece->type][y_pcsq][curPiece->x];

    if (curPiece->type == King) {
        removePins(sourceSquare);
        makePins(destinationSquare);

        for each (PieceMovement* pieceMovement in destinationSquare->movements) {
            Piece* attacker = pieceMovement->piece;
            Movement* movement = pieceMovement->movement;
            if (attacker->color != curPiece->color && (movement->mobility->type == Attack || movement->mobility->type == AttackMove)) {
                checks.push_back(pieceMovement);
            }
        }

        if (checks.size() > 0) {
            if (curPiece->color == White)
                whiteCheck = true;
            else
                blackCheck = true;

            validateMoves(curPiece->color);
        }
    }

    unblockPins(sourceSquare);
    blockPins(destinationSquare);

    calculateMoves(curPiece);

    // continue calculations of movements that were unblocked by moving this piece
    for each (PieceMovement* sourceMovement in sourceSquare->movements) {
        if (sourceMovement->piece != curPiece) {
            PieceMovement* pin = getPin(sourceMovement->piece);
            calculateMoves(sourceMovement->piece, sourceMovement->movement->mobility, sourceMovement->movement, pin);
        }
    }

    // cut movements that were blocked by moving this piece
    for each (PieceMovement* destinationMovement in destinationSquare->movements)
        cutMovement(destinationMovement);
}

void Board::makeMove(int x1, int y1, int x2, int y2)
{
    makeMove(x1, y1, x2, y2, Queen);
}

void Board::makeMove(int x1, int y1, int x2, int y2, PieceType promotionType)
{
    Square* sourceSquare = squares[y1][x1];
    Piece* sourcePiece = sourceSquare->occupyingPiece;
    Square* destinationSquare = squares[y2][x2];
    Piece* destinationPiece = destinationSquare->occupyingPiece;

    if (sourcePiece == nullptr || sourcePiece->color != curTurn || (x1 == x2 && y1 == y2))
        return;

    MoveRecord newMoveRecord{};
    newMoveRecord.x1 = x1;
    newMoveRecord.y1 = y1;
    newMoveRecord.x2 = x2;
    newMoveRecord.y2 = y2;

    if (curGhost != nullptr) {
        newMoveRecord.hadGhost = true;
        newMoveRecord.ghostX =  curGhost->x;
        newMoveRecord.ghostY = curGhost->y;
        newMoveRecord.ghostParentX = curGhost->parent->x;
        newMoveRecord.ghostParentY = curGhost->parent->y;
    }

    if (destinationPiece != nullptr) {
        newMoveRecord.performedCapture = true;
        newMoveRecord.capturedType = destinationPiece->type;
        newMoveRecord.capturedHasMoved = destinationPiece->hasMoved;
    }

    // only perform this move if it is a valid movement of this piece
    if (!std::any_of(destinationSquare->movements.begin(), destinationSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
        return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal;
    })) {
        return;
    }
    
    // remove parent of ghost if ghost is present on destination square and vigilant offensive move is found
    if (curGhost != nullptr && curGhost->x == x2 && curGhost->y == y2) {
        auto vigilantPieceMovementIterator = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
            return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal && curPieceMovement->movement->mobility->flags.vigilant
                && (curPieceMovement->movement->mobility->type == Attack || curPieceMovement->movement->mobility->type == AttackMove);
        });

        if (vigilantPieceMovementIterator != destinationSquare->movements.end()) {
            newMoveRecord.performedCapture = true;
            newMoveRecord.capturedType = curGhost->parent->type;
            newMoveRecord.capturedHasMoved = true;
           
            removePiece(curGhost->parent);
        }
    }

    // create ghost piece if hasty movement is found, delete ghost if it is not found
    auto hastyPieceMovementIterator = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
        return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal && curPieceMovement->movement->mobility->flags.hasty;
    });

    if (hastyPieceMovementIterator != destinationSquare->movements.end()) {
        PieceMovement* hastyPieceMovement = *hastyPieceMovementIterator;

        int newGhostX = x2;
        int newGhostY = y2;

        switch (sourcePiece->color) {
            case::White:
                newGhostX -= hastyPieceMovement->movement->mobility->direction_x;
                newGhostY += hastyPieceMovement->movement->mobility->direction_y;
                break;
            case::Black:
                newGhostX += hastyPieceMovement->movement->mobility->direction_x;
                newGhostY -= hastyPieceMovement->movement->mobility->direction_y;
                break;
        }

        setGhost(new Ghost(newGhostX, newGhostY, sourcePiece));
    } else {
        setGhost(nullptr);
    }

    // move inspired piece one square behind the destination square
    auto inspiringPieceMovementIt = std::find_if(destinationSquare->movements.begin(), destinationSquare->movements.end(), [&](PieceMovement* curPieceMovement) -> bool {
        return curPieceMovement->piece == sourcePiece && curPieceMovement->movement->legal && curPieceMovement->movement->mobility->flags.inspiring;
    });
    
    if (inspiringPieceMovementIt != destinationSquare->movements.end()) {
        PieceMovement* inspiringPieceMovement = *inspiringPieceMovementIt;

        int inspiredX1 = x1 + inspiringPieceMovement->movement->mobility->flags.affected_x;
        int inspiredY1 = y1;
        int inspiredX2 = x2 - inspiringPieceMovement->movement->mobility->direction_x;
        int inspiredY2 = y2;

        switch (sourcePiece->color) {
            case::White:
                inspiredY1 -= inspiringPieceMovement->movement->mobility->flags.affected_y;
                inspiredY2 += inspiringPieceMovement->movement->mobility->direction_y;
                break;
            case::Black:
                inspiredY1 += inspiringPieceMovement->movement->mobility->flags.affected_y;
                inspiredY2 -= inspiringPieceMovement->movement->mobility->direction_y;
                break;
        }

        Square* inspiredSquare = squares[inspiredY1][inspiredX1];
        Piece* inspiredPiece = inspiredSquare->occupyingPiece;
        movePiece(inspiredPiece, inspiredX2, inspiredY2, true);

        newMoveRecord.performedInspiration = true;
        newMoveRecord.inspiredX1 = inspiredX1;
        newMoveRecord.inspiredY1 = inspiredY1;
        newMoveRecord.inspiredX2 = inspiredX2;
        newMoveRecord.inspiredY2 = inspiredY2;
    }

    movePiece(sourcePiece, x2, y2, true);

    // pawn promotions
    if (sourcePiece->type == Pawn && ((sourcePiece->color == White && sourcePiece->y == 0) || (sourcePiece->color == Black && sourcePiece->y == 7))) {
        Essence promotionEssence;
        switch (sourcePiece->color) {
        case::White:
            switch (promotionType) {
            case Pawn:
                promotionEssence = essenceConfig.whitePawnEssence;
                break;
            case Knight:
                promotionEssence = essenceConfig.whiteKnightEssence;
                break;
            case Bishop:
                promotionEssence = essenceConfig.whiteBishopEssence;
                break;
            case Rook:
                promotionEssence = essenceConfig.whiteRookEssence;
                break;
            default:
                promotionEssence = Classic;
                break;
            }
            break;
        case::Black:
            switch (promotionType) {
            case Pawn:
                promotionEssence = essenceConfig.blackPawnEssence;
                break;
            case Knight:
                promotionEssence = essenceConfig.blackKnightEssence;
                break;
            case Bishop:
                promotionEssence = essenceConfig.blackBishopEssence;
                break;
            case Rook:
                promotionEssence = essenceConfig.blackRookEssence;
                break;
            default:
                promotionEssence = Classic;
                break;
            }
            break;
        }

        changePiece(sourcePiece, promotionType, promotionEssence);
        newMoveRecord.performedPromotion = true;
    }

    // if move was succesfuly performed while in check it means that check was resolved and all moves need to be revalidated
    if (sourcePiece->color == White && whiteCheck) {
        whiteCheck = !whiteCheck;
        checks.clear();
        validateMoves(White);
    } else if (sourcePiece->color == Black && blackCheck) {
        blackCheck = !blackCheck;
        checks.clear();
        validateMoves(Black);
    }

    if (curTurn == White)
        curTurn = Black;
    else
        curTurn = White;
    hash ^= zobrist.turn;

    moveHistory.push(newMoveRecord);
}

void Board::unmakeMove()
{
    MoveRecord lastMoveRecord = moveHistory.top();
    moveHistory.pop();

    Square* sourceSquare = squares[lastMoveRecord.y2][lastMoveRecord.x2];
    Piece* sourcePiece = sourceSquare->occupyingPiece;

    if (sourcePiece->color == White && whiteCheck) {
        whiteCheck = !whiteCheck;
        checks.clear();

        validateMoves(White);
    } else if (sourcePiece->color == Black && blackCheck) {
        blackCheck = !blackCheck;
        checks.clear();

        validateMoves(Black);
    }

    movePiece(sourcePiece, lastMoveRecord.x1, lastMoveRecord.y1, lastMoveRecord.hasMoved);
    calculateInspiringMoves(sourcePiece);

    if (lastMoveRecord.performedPromotion) {
        if (curTurn == White)
            changePiece(sourcePiece, Pawn, essenceConfig.whitePawnEssence);
        else
            changePiece(sourcePiece, Pawn, essenceConfig.blackPawnEssence);
    }

    if (lastMoveRecord.performedCapture) {
        Piece* capturedPiece;
        Essence essence = Classic;

        if (curTurn == White) {
            switch (lastMoveRecord.capturedType) {
            case Pawn:
                essence = essenceConfig.whitePawnEssence;
                break;
            case Rook:
                essence = essenceConfig.whiteRookEssence;
                break;
            case Knight:
                essence = essenceConfig.whiteKnightEssence;
                break;
            case Bishop:
                essence = essenceConfig.whiteBishopEssence;
                break;
            }
        } else {
            switch (lastMoveRecord.capturedType) {
            case Pawn:
                essence = essenceConfig.blackPawnEssence;
                break;
            case Rook:
                essence = essenceConfig.blackRookEssence;
                break;
            case Knight:
                essence = essenceConfig.blackKnightEssence;
                break;
            case Bishop:
                essence = essenceConfig.blackBishopEssence;
                break;
            }
        }

        capturedPiece = new Piece(lastMoveRecord.capturedType, curTurn, essence);
        capturedPiece->hasMoved = lastMoveRecord.capturedHasMoved;

        if (lastMoveRecord.hadGhost && lastMoveRecord.x2 == lastMoveRecord.ghostX && lastMoveRecord.y2 == lastMoveRecord.ghostY)
            addPiece(capturedPiece, lastMoveRecord.ghostParentX, lastMoveRecord.ghostParentY);
        else
            addPiece(capturedPiece, lastMoveRecord.x2, lastMoveRecord.y2);

        calculateMoves(capturedPiece);
        calculateInspiringMoves(capturedPiece);
    }

    if (lastMoveRecord.performedInspiration) {
        Square* inspiredSquare = squares[lastMoveRecord.inspiredY2][lastMoveRecord.inspiredX2];
        Piece* inspiredPiece = inspiredSquare->occupyingPiece;

        movePiece(inspiredPiece, lastMoveRecord.inspiredX1, lastMoveRecord.inspiredY1, false);
        calculateInspiringMoves(inspiredPiece);
    }

    if (lastMoveRecord.hadGhost) {
        Square* ghostParentSquare = squares[lastMoveRecord.ghostParentY][lastMoveRecord.ghostParentX];
        Piece* ghostParent = ghostParentSquare->occupyingPiece;

        if (ghostParent == nullptr)
            int foo = 0;

        setGhost(new Ghost(lastMoveRecord.ghostX, lastMoveRecord.ghostY, ghostParent));
    } else {
        setGhost(nullptr);
    }

    if (curTurn == White)
        curTurn = Black;
    else
        curTurn = White;
    hash ^= zobrist.turn;
}

void Board::printBoard()
{
    std::cout << "Printing current state of board:" << std::endl;

    for (int cur_y = 0; cur_y < COLUMNS; cur_y++) {
        for (int cur_x = 0; cur_x < ROWS; cur_x++) {
            if (cur_x > 0)
                std::cout << "|";

            Square* curSquare = squares[cur_y][cur_x];
            Piece* curPiece = curSquare->occupyingPiece;

            if (curPiece != nullptr) {
                char ownerTag = '?';
                switch (curPiece->color) {
                case::White:
                    ownerTag = 'W';
                    break;
                case::Black:
                    ownerTag = 'B';
                    break;
                }

                char essenceTag = '?';
                switch (curPiece->essence) {
                case::Classic:
                    essenceTag = 'C';
                    break;
                case::Red:
                    essenceTag = 'R';
                    break;
                case::Blue:
                    essenceTag = 'B';
                    break;
                }

                char pieceTag = '?';
                switch (curPiece->type) {
                case::Pawn:
                    pieceTag = 'P';
                    break;
                case::Rook:
                    pieceTag = 'R';
                    break;
                case::Knight:
                    pieceTag = 'N';
                    break;
                case::Bishop:
                    pieceTag = 'B';
                    break;
                case::Queen:
                    pieceTag = 'Q';
                    break;
                case::King:
                    pieceTag = 'K';
                    break;
                }

                std::cout << ownerTag << essenceTag << pieceTag;
            } else if (curGhost != nullptr && cur_x == curGhost->x && cur_y == curGhost->y) {
                std::cout << "GGG";
            } else {
                std::cout << "XXX";
            }
        }

        if (cur_y < 7)
            std::cout << std::endl << "---|---|---|---|---|---|---|---" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void Board::printMoves()
{
    for each (Piece* whitePiece in whitePieces)
        printMoves(whitePiece);

    for each (Piece* blackPiece in blackPieces)
        printMoves(blackPiece);
}

void Board::printVirtualMoves()
{
    for each (Piece* whitePiece in whitePieces)
        printVirtualMoves(whitePiece);

    for each (Piece* blackPiece in blackPieces)
        printVirtualMoves(blackPiece);
}

void Board::printVirtualMoves(Piece* curPiece)
{
    std::string ownerName;
    switch (curPiece->color) {
    case::White:
        ownerName = "White";
        break;
    case::Black:
        ownerName = "Black";
        break;
    }

    std::string essenceName;
    switch (curPiece->essence) {
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

    std::string pieceName;
    switch (curPiece->type) {
    case::Pawn:
        pieceName = "Pawn";
        break;
    case::Knight:
        pieceName = "Knight";
        break;
    case::Bishop:
        pieceName = "Bishop";
        break;
    case::Rook:
        pieceName = "Rook";
        break;
    case::Queen:
        pieceName = "Queen";
        break;
    case::King:
        pieceName = "King";
        break;
    }

    std::cout << "Printing virtual moves of piece: " << ownerName << " " << essenceName << " " << pieceName << std::endl;

    char pieceMoves[ROWS][COLUMNS];
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            pieceMoves[cur_y][cur_x] = ' ';
        }
    }
    pieceMoves[curPiece->y][curPiece->x] = '0';

    for each (Movement* move in curPiece->virtualMovements) {
        Movement* curMove = move;

        while (curMove != nullptr) {
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
            std::cout << std::endl << "-|-|-|-|-|-|-|-" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void Board::makePins(Square* curSquare)
{
    for each (PieceMovement* virtualMovement in curSquare->virtualMovements) {
        Piece* attackerPiece = virtualMovement->piece;

        auto it = std::find_if(attackerPiece->virtualMovements.begin(), attackerPiece->virtualMovements.end(), [&](Movement* curMovement) -> bool {
            return curMovement->mobility == virtualMovement->movement->mobility;
        });

        if (it != attackerPiece->virtualMovements.end()) {
            PinArgs pinArgs{};
            Movement* curMovement = *it;

            Square* pinOrigin = squares[curMovement->y][curMovement->x];
            auto pinIt = std::find_if(pinOrigin->virtualMovements.begin(), pinOrigin->virtualMovements.end(), [&](PieceMovement* curMovement) -> bool {
                return curMovement->piece == virtualMovement->piece && curMovement->movement->mobility == virtualMovement->movement->mobility;
            });
            pinArgs.pin = *pinIt;

            while (curMovement != nullptr) {
                Square* pinnedSquare = squares[curMovement->y][curMovement->x];
                Piece* pinnedPiece = pinnedSquare->occupyingPiece;

                if (pinnedPiece != nullptr) {
                    if (pinnedPiece->type == King && pinnedPiece->color != attackerPiece->color) {
                        pinArgs.kingHit = true;
                        break;
                    }

                    if (pinArgs.pinnedCount < 1)
                        pinArgs.pinnedCount++;
                    else
                        break;

                    pinArgs.pinnedPiece = pinnedPiece;
                }

                curMovement = curMovement->next;
            }

            if (pinArgs.kingHit && pinArgs.pinnedCount == 1 && attackerPiece->color != pinArgs.pinnedPiece->color) {
                validateMoves(pinArgs.pinnedPiece, pinArgs.pin);
            }
        }
    }
}

void Board::removePins(Square* curSquare)
{
    for each (PieceMovement* virtualMovement in curSquare->virtualMovements) {
        Piece* attackerPiece = virtualMovement->piece;

        auto it = std::find_if(attackerPiece->virtualMovements.begin(), attackerPiece->virtualMovements.end(), [&](Movement* curMovement) -> bool {
            return curMovement->mobility == virtualMovement->movement->mobility;
        });

        if (it != attackerPiece->virtualMovements.end()) {
            PinArgs pinArgs{};
            Movement* curMovement = *it;

            while (curMovement != nullptr) {
                Square* pinnedSquare = squares[curMovement->y][curMovement->x];
                Piece* pinnedPiece = pinnedSquare->occupyingPiece;

                if (pinnedPiece != nullptr) {
                    if (pinArgs.pinnedCount < 1)
                        pinArgs.pinnedCount++;
                    else
                        break;

                    pinArgs.pinnedPiece = pinnedPiece;
                }

                curMovement = curMovement->next;
            }

            if (pinArgs.pinnedCount == 1 && attackerPiece->color != pinArgs.pinnedPiece->color) {
                validateMoves(pinArgs.pinnedPiece, nullptr);
            }
        }
    }
}

void Board::unblockPins(Square* curSquare)
{
    for each (PieceMovement* virtualMovement in curSquare->virtualMovements) {
        Piece* attackerPiece = virtualMovement->piece;

        auto it = std::find_if(attackerPiece->virtualMovements.begin(), attackerPiece->virtualMovements.end(), [&](Movement* curMovement) -> bool {
            return curMovement->mobility == virtualMovement->movement->mobility;
        });

        if (it != attackerPiece->virtualMovements.end()) {
            PinArgs pinArgs{};
            Movement* curMovement = *it;

            Square* pinOrigin = squares[curMovement->y][curMovement->x];
            auto pinIt = std::find_if(pinOrigin->virtualMovements.begin(), pinOrigin->virtualMovements.end(), [&](PieceMovement* curMovement) -> bool {
                return curMovement->piece == virtualMovement->piece && curMovement->movement->mobility == virtualMovement->movement->mobility;
            });
            pinArgs.pin = *pinIt;

            while (curMovement != nullptr) {
                Square* pinnedSquare = squares[curMovement->y][curMovement->x];
                Piece* pinnedPiece = pinnedSquare->occupyingPiece;

                if (pinnedPiece != nullptr) {
                    if (pinnedPiece->type == King && pinnedPiece->color != attackerPiece->color) {
                        pinArgs.kingHit = true;
                        break;
                    }

                    if (pinArgs.pinnedCount < 1)
                        pinArgs.pinnedCount++;
                    else
                        break;

                    pinArgs.pinnedPiece = pinnedPiece;
                }

                curMovement = curMovement->next;
            }

            if (pinArgs.kingHit && pinArgs.pinnedCount == 1 && attackerPiece->color != pinArgs.pinnedPiece->color) {
                validateMoves(pinArgs.pinnedPiece, pinArgs.pin);
            }
        }
    }
}

void Board::blockPins(Square* curSquare)
{
    for each (PieceMovement* virtualMovement in curSquare->virtualMovements) {
        Piece* attackerPiece = virtualMovement->piece;

        auto it = std::find_if(attackerPiece->virtualMovements.begin(), attackerPiece->virtualMovements.end(), [&](Movement* curMovement) -> bool {
            return curMovement->mobility == virtualMovement->movement->mobility;
        });

        if (it != attackerPiece->virtualMovements.end()) {
            PinArgs pinArgs{};
            Movement* curMovement = *it;

            while (curMovement != nullptr) {
                Square* pinnedSquare = squares[curMovement->y][curMovement->x];
                Piece* pinnedPiece = pinnedSquare->occupyingPiece;

                if (pinnedPiece != nullptr) {
                    if (pinnedPiece->type == King && pinnedPiece->color != attackerPiece->color) {
                        pinArgs.kingHit = true;
                        break;
                    }

                    if (pinArgs.pinnedCount < 2)
                        pinArgs.pinnedCount++;
                    else
                        break;

                    if (pinnedPiece != curSquare->occupyingPiece)
                        pinArgs.pinnedPiece = pinnedPiece;
                }

                curMovement = curMovement->next;
            }

            if (pinArgs.kingHit) {
                if (pinArgs.pinnedCount == 2 && attackerPiece->color != pinArgs.pinnedPiece->color)
                    validateMoves(pinArgs.pinnedPiece, nullptr);
            }
        }
    }
}

void Board::printMoves(Piece* curPiece)
{
    std::string ownerName;
    switch (curPiece->color) {
        case::White:
            ownerName = "White";
            break;
        case::Black:
            ownerName = "Black";
            break;
    }

    std::string essenceName;
    switch (curPiece->essence) {
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

    std::string pieceName;
    switch (curPiece->type) {
        case::Pawn:
            pieceName = "Pawn";
            break;
        case::Knight:
            pieceName = "Knight";
            break;
        case::Bishop:
            pieceName = "Bishop";
            break;
        case::Rook:
            pieceName = "Rook";
            break;
        case::Queen:
            pieceName = "Queen";
            break;
        case::King:
            pieceName = "King";
            break;
    }
    
    std::cout << "Printing moves of piece: " << ownerName << " " << essenceName << " " << pieceName << std::endl;

    char pieceMoves[ROWS][COLUMNS];
    for (int cur_y = 0; cur_y < ROWS; cur_y++) {
        for (int cur_x = 0; cur_x < COLUMNS; cur_x++) {
            pieceMoves[cur_y][cur_x] = ' ';
        }
    }
    pieceMoves[curPiece->y][curPiece->x] = '0';

    for each (Movement* move in curPiece->movements) {
        Movement* curMove = move;

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
            std::cout << std::endl << "-|-|-|-|-|-|-|-" << std::endl;
    }
    std::cout << std::endl << std::endl;
}
