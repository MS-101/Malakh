#include "evaluation.h"

std::unordered_map<PieceType, int> Evaluation::piecePhaseValues = {
    {Pawn, 0},
    {Knight, 1},
    {Bishop, 1},
    {Rook, 2},
    {Queen, 4},
};

int Evaluation::startPhase = 16 * Evaluation::piecePhaseValues[Pawn] + 4 * Evaluation::piecePhaseValues[Knight]
+ 4 * Evaluation::piecePhaseValues[Bishop] + 4 * Evaluation::piecePhaseValues[Rook] + 2 * Evaluation::piecePhaseValues[Queen];

std::unordered_map<PieceType, int> Evaluation::pieceMatValues = {
    {Pawn, 100},
    {Knight, 320},
    {Bishop, 330},
    {Rook, 500},
    {Queen, 900},
};

std::unordered_map<PieceType, std::array<std::array<int, 8>, 8>> Evaluation::mg_pcsq = {
    {Pawn, {{
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-6, -4, 1, 1, 1, 1, -4, -6},
            {-6, -4, 1, 2, 2, 1, -4, -6},
            {-6, -4, 2, 8, 8, 2, -4, -6},
            {-6, -4, 5, 10, 10, 5, -4, -6},
            {-4, -4, 1, 5, 5, 1, -4, -4},
            {-6, -4, 1, -24, -24, 1, -4, -6},
            {-6, -4, 1, -24, -24, 1, -4, -6}
        }}
    },
    {Knight, {{
            {-8, -8, -8, -8, -8, -8, -8, -8},
            {-8, 0, 0, 0, 0, 0, 0, -8},
            {-8, 0, 4, 6, 6, 4, 0, -8},
            {-8, 0, 6, 8, 8, 6, 0, -8},
            {-8, 0, 6, 8, 8, 6, 0, -8},
            {-8, 0, 4, 6, 6, 4, 0, -8},
            {-8, 0, 1, 2, 2, 1, 0, -8},
            {-16, -12, -8, -8, -8, -8, -12, -16}
        }}
    },
    {Bishop, {{
            {-4, -4, -4, -4, -4, -4, -4, -4},
            {-4, 0, 0, 0, 0, 0, 0, -4},
            {-4, 0, 2, 4, 4, 2, 0, -4},
            {-4, 0, 4, 6, 6, 4, 0, -4},
            {-4, 0, 4, 6, 6, 4, 0, -4},
            {-4, 1, 2, 4, 4, 2, 1, -4},
            {-4, 2, 1, 1, 1, 1, 2, -4},
            {-4, -4, -12, -4, -4, -12, -4, -4}
        }}
    },
    {Rook, {{
            {5, 5, 5, 5, 5, 5, 5, 5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {0, 0, 0, 2, 2, 0, 0, 0}
        }}
    },
    {Queen, {{
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 2, 2, 1, 0, 0},
            {0, 0, 2, 3, 3, 2, 0, 0},
            {0, 0, 2, 3, 3, 2, 0, 0},
            {0, 0, 1, 2, 0, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 0, 0},
            {-5, -5, -5, -5, -5, -5, -5, -5}
        }}
    },
    {King, {{
            {-40, -30, -50, -70, -70, -50, -30, -40},
            {-30, -20, -40, -60, -60, -40, -20, -30},
            {-20, -10, -30, -50, -50, -30, -10, -20},
            {-10, 0, -20, -40, -40, -20, 0, -10},
            {0, 10, -10, -30, -30, -10, 10, 0},
            {10, 20, 0, -20, -20, 0, 20, 10},
            {30, 40, 20, 0, 0, 20, 40, 30},
            {40, 50, 30, 10, 10, 30, 50, 40}
        }}
    }
};

std::unordered_map<PieceType, std::array<std::array<int, 8>, 8>> Evaluation::eg_pcsq = {
    {Pawn, {{
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-6, -4, 1, 1, 1, 1, -4, -6},
            {-6, -4, 1, 2, 2, 1, -4, -6},
            {-6, -4, 2, 8, 8, 2, -4, -6},
            {-6, -4, 5, 10, 10, 5, -4, -6},
            {-4, -4, 1, 5, 5, 1, -4, -4},
            {-6, -4, 1, -24, -24, 1, -4, -6},
            {-6, -4, 1, -24, -24, 1, -4, -6}
        }}
    },
    {Knight, {{
            {-8, -8, -8, -8, -8, -8, -8, -8},
            {-8, 0, 0, 0, 0, 0, 0, -8},
            {-8, 0, 4, 6, 6, 4, 0, -8},
            {-8, 0, 6, 8, 8, 6, 0, -8},
            {-8, 0, 6, 8, 8, 6, 0, -8},
            {-8, 0, 4, 6, 6, 4, 0, -8},
            {-8, 0, 1, 2, 2, 1, 0, -8},
            {-16, -12, -8, -8, -8, -8, -12, -16}
        }}
    },
    {Bishop, {{
            {-4, -4, -4, -4, -4, -4, -4, -4},
            {-4, 0, 0, 0, 0, 0, 0, -4},
            {-4, 0, 2, 4, 4, 2, 0, -4},
            {-4, 0, 4, 6, 6, 4, 0, -4},
            {-4, 0, 4, 6, 6, 4, 0, -4},
            {-4, 1, 2, 4, 4, 2, 1, -4},
            {-4, 2, 1, 1, 1, 1, 2, -4},
            {-4, -4, -12, -4, -4, -12, -4, -4}
        }}
    },
    {Rook, {{
            {5, 5, 5, 5, 5, 5, 5, 5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {0, 0, 0, 2, 2, 0, 0, 0}
        }}
    },
    {Queen, {{
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 2, 2, 1, 0, 0},
            {0, 0, 2, 3, 3, 2, 0, 0},
            {0, 0, 2, 3, 3, 2, 0, 0},
            {0, 0, 1, 2, 0, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 0, 0},
            {-5, -5, -5, -5, -5, -5, -5, -5}
        }}
    },
    {King, {{
            {-72, -48, -36, -24, -24, -36, -48, -72},
            {-48, -24, -12, 0, 0, -12, -24, -48},
            {-36, -12, 0, 12, 12, 0, -12, -36},
            {-24, 0, 12, 24, 24, 12, 0, -24},
            {-36, -12, 0, 24, 24, 12, 0, -24},
            {-36, -12, 0, 24, 24, 12, 0, -24},
            {-48, -24, -12, 0, 0, -12, -24, -48},
            {-72, -48, -36, -24, -24, -36, -48, -72}
        }}
    }
};