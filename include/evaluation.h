#pragma once

/*****************************************************************//**
 * \file   evaluation.h
 * \brief  Evaluácia šachovnice.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#include <piece.h>


/**
 * Evaluačné argumenty na výpočet evaluácie šachovnice.
 */
struct EvalArgs {
    /**
     * Materiálová evaluácia.
     */
    int matEval[2];

    /**
     * Aktuálna fáza hry. S ubúdajúcim materiálom sa táto hodnota znižuje.
     */
    int curPhase = 0;

    /**
     * Hodnoty piece-square tabuliek pre strednú fázu hry.
     */
    int mg_pcsqEval[2];

    /**
     * Hodnoty piece-square tabuliek pre koncovú fázu hry.
     */
    int eg_pcsqEval[2];

    /**
     * Počítadla mobilít pre jednotlivé typy figúrok.
     */
    int mobCount[2*6];

    /**
     * Počítadlá útokov na kráľa.
     */
    int attCount[2];

    /**
     * Hodnoty útokov na kráľa.
     */
    int attWeight[2];
};

/**
 * Trieda zodpovedná za konfigurácia evaluácie šachových pozícií.
 */
class Evaluation {
public:
    /**
     * Materiálové hodnoty figúrok.
     */
    static const int pieceMatValues[6];

    /**
     * Fázové hodnoty figúrok.
     */
    static const int piecePhaseValues[6];

    /**
     * Počiatočná fáza hry.
     */
    static const int startPhase;

    /**
     * Piece-square tabuľky strednej fázy hry.
     */
    static const int mg_pcsq[6][64];

    /**
     * Piece-square tabuľky koncovej fázy hry.
     */
    static const int eg_pcsq[6][64];

    /**
     * Váhy mobilít strednej fázy hry.
     */
    static const int mg_pieceMobWeights[6];

    /**
     * Váhy mobilít koncovej fázy hry.
     */
    static const int eg_pieceMobWeights[6];

    /**
     * Penalty mobilít figúrok.
     */
    static const int pieceMobPenalties[6];

    /**
     * Váhy tropizmu strednej fázy hry.
     */
    static const int mg_pieceTropismWeights[5];

    /**
    * Váhy tropizmu koncovej fázy hry.
    */
    static const int eg_pieceTropismWeights[5];

    /**
     * Váhy útokov figúrok.
     */
    static const int pieceAttWeights[5];

    /**
     * Bezpečnostná tabuľka potrebná na výpočet bezpečnosti kráľa.
     */
    static const int safetyTable[100];

    /**
     * Výpočet piece-square hodnoty danej figúrky na určitej pozícii v strednej fáze hry.
     * 
     * \param color Vlastník danej figúrky.
     * \param type Typ danej figúrky.
     * \param x Koordinát X danej pozície.
     * \param y Koordinát Y danej pozície.
     * \return Požadovaná hodnota z piece-square tabuľky.
     */
    static int get_mg_pcsq(PieceColor color, PieceType type, char x, char y);

    /**
     * Výpočet piece-square hodnoty danej figúrky na určitej pozícii v koncovej fáze hry.
     * 
     * \param color Vlastník danej figúrky.
     * \param type Typ danej figúrky.
     * \param x Koordinát X danej pozície.
     * \param y Koordinát Y danej pozície.
     * \return Požadovaná hodnota z piece-square tabuľky.
     */
    static int get_eg_pcsq(PieceColor color, PieceType type, char x, char y);
};