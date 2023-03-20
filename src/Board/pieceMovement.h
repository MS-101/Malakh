#include "piece.h"
#include "movement.h"

class PieceMovement {
    public:
        PieceMovement(Piece* piece, Movement* movement);

        Piece* piece;
        Movement* movement;
};
