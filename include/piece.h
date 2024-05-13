#pragma once

/*****************************************************************//**
 * \file   piece.h
 * \brief  Definícia šachových figúrok.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#define NOMINMAX
#include <string>


/**
 * Vlastník figúrky.
 */
enum PieceColor {
    /**
     * Biely hráč.
     */
    White,
    /**
     * Čierny hráč.
     */
    Black
};

/**
 * Typ figúrky.
 */
enum PieceType {
    /**
     * Pešiak.
     */
    Pawn,
    /**
     * Veža.
     */
    Rook,
    /**
     * Rytier.
     */
    Knight,
    /**
     * Strelec.
     */
    Bishop,
    /**
     * Kráľovná.
     */
    Queen,
    /**
     * Kráľ.
     */
    King
};

/**
 * Esencia figúrky.
 */
enum PieceEssence {
    /**
     * Klasická esencia.
     */
    Classic,
    /**
     * Červená esencia.
     */
    Red,
    /**
     * Modrá esencia.
     */
    Blue
};

/**
 * Štruktúra reprezentujúca šachovú figúrku.
 */
struct Piece {
    /**
     * Vlastník figúrky.
     */
    PieceColor color = White;

    /**
     * Typ figúrky.
     */
    PieceType type = Pawn;

    /**
     * Esencia figúrky.
     */
    PieceEssence essence = Classic;

    /**
     * Konverzia figúrky do znaku.
     * 
     * \return Znak reprezentujúci figúrku
     */
    char toChar();
};

/**
 * Figúrka zanechaná po vykonaní 'hasty' pohybu.
 */
struct Ghost {
    /**
     * Koordinát X tejto figúrky.
     */
    int x = -1;

    /**
     * Koordinát Y tejto figúrky.
     */
    int y = -1;

    /**
     * Koordinát X rodičovskej figúrky.
     */
    int parentX = -1;

    /**
     * Koordinát Y rodičovskej figúrky.
     */
    int parentY = -1;
};

/**
* Zoznam, ktorý umožňuje konverziu na protivníkovu farbu figúrky.
*/
extern PieceColor opponent[];

/**
* Zoznam znakov reprezentujúci farbu figúrky.
*/
extern char colorChars[];

/**
* Zoznam znakov reprezentujúci esenciu figúrky.
*/
extern char essenceChars[];

/**
* Zoznam znakov reprezentujúci typ figúrky.
*/
extern char typeChars[];

/**
 * Táto funkcia vráti id figúrky.
 * 
 * \param color Farba šachovej figúrky.
 * \param type Typ šachovej figúrky.
 * \return Identifikátor šachovej figúrky
 */
char getPieceIndex(char color, char type);

/**
 * Konverzia stringu na farbu figúrky.
 * 
 * \param value Slovo reprezentujúce farbu figúrky.
 * \return Farba šachovej figúrky.
 */
PieceColor stringToColor(std::string value);

/**
 * Konverzia stringu na typ figúrky.
 *
 * \param value Slovo reprezentujúce typ figúrky.
 * \return Typ šachovej figúrky.
 */
PieceType stringToType(std::string value);

/**
 * Konverzia stringu na esenciu figúrky.
 *
 * \param value Slovo reprezentujúce esenciu figúrky.
 * \return Esencia šachovej figúrky.
 */
PieceEssence stringToEssence(std::string value);

/**
 * Konverzia farby figúrky na slovo.
 *
 * \param value Farba šachovej figúrky.
 * \return Slovo reprezentujúce farbu figúrky.
 */
std::string colorToString(PieceColor value);

/**
 * Konverzia typu figúrky na slovo.
 *
 * \param value Typ šachovej figúrky.
 * \return Slovo reprezentujúce typ figúrky.
 */
std::string typeToString(PieceType value);

/**
 * Konverzia esencie figúrky na slovo.
 *
 * \param value Esencia šachovej figúrky.
 * \return Slovo reprezentujúce esenciu figúrky.
 */
std::string essenceToString(PieceEssence value);
