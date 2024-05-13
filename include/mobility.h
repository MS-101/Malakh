#pragma once

/*****************************************************************//**
 * \file   mobility.h
 * \brief  Definícia mobilít figúrok.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#include <piece.h>
#include <vector>
#include <string>


/**
 * Enumerátor reprezentujúci typ mobility.
 */
enum MovementType {
    /**
     * Tento pohyb umožňuje len presunúť sa na prázdnu pozíciu.
     */
    Move,
    /**
     * Tento pohyb umožňuje len napadnúť protivníkovu figúrku.
     */
    Attack,
    /**
     * Tento pohyb umožňuje presunúť sa na prádznu pozíciu a napadnúť protivníkovu figúrku.
     */
    AttackMove
};

/**
 * Enumerátor reprezentujúci typ rošády.
 */
enum Castling {
    /**
     * Pohyb nie je rošáda.
     */
    none,
    /**
     * Pohyb je rošáda z kráľovej strany.
     */
    kingSide,
    /**
     * Pohyb je rošáda z kráľovninej strany.
     */
    queenSide
};

/**
 * Štruktúra s mobilitnými flagmi. Sú potrebné na definíciu špeciálnych pohybov ako En Passant.
 */
struct MobilityFlags {
    /**
     * Pohyb s týmto flagom je možné vykonať len vtedy, keď figúrka sa ešte nepohla.
     */
    bool initiative = false;

    /**
     * Pohyb s týmto flagom po sebe na zanecháva figúrku o 1 pohyb pred cieľovým políčkom. Túto figúrku je možné napadnúť útokom s 'vigilant' flagom.
     */
    bool hasty = false;

    /**
     * Tento pohyb je možné vykonať len na finálnom políčku pohybu.
     */
    bool uninterruptible = false;

    /**
     * Pohyb s týmto flagom môže napadnúť figúrku zanechanú pohybom s 'hasty' flagom.
     */
    bool vigilant = false;
};


/**
 * Štruktúra pomocou ktorej môžeme dynamicky definovať pohyby figúrok.
 */
struct Mobility {
    /**
     * Typ pohybu.
     */
    MovementType type = Move;

    /**
     * Relatívny smer pohybu na osi X prvého pohybu.
     */
    int start_x = 0;

    /**
    * Relatívny smer pohybu na osi Y prvého pohybu.
    */
    int start_y = 0;

    /**
    * Relatívny smer pohybu na osi X.
    */
    int direction_x = 0;

    /**
    * Relatívny smer pohybu na osi Y.
    */
    int direction_y = 0;

    /**
     * Limit pohybu v danom smere. Hodnota 0 reprezentuje pohyb bez obmedzení.
     */
    int limit = 0;

    /**
     * Flagy pohybu pre špeciálne .
     */
    MobilityFlags flags{};
};

/**
 * Štruktúra reprezentujúca legálny pohyb.
 */
struct LegalMove {
    /**
     * Predvolený konštruktor.
     */
    LegalMove();

    /**
     * Konštruktor pre dané hodnoty.
     * 
     * \param x1 Koordinát X zdrojovej pozície.
     * \param y1 Koordinát Y zdrojovej pozície.
     * \param x2 Koordinát X cieľovej pozície.
     * \param y2 Koordinát Y cieľovej pozície.
     * \param mobility Pravidlá pohybu.
     */
    LegalMove(int x1, int y1, int x2, int y2, Mobility mobility);

    /**
     * Konštuktor pre algebraickú notáciu.
     * 
     * \param value Algebraická notácia pohybu.
     */
    LegalMove(std::string value);

    /**
     * Koordinát X zdrojovej pozície.
     */
    int x1 = 0;

    /**
     * Koordinát Y zdrojovej pozície.
     */
    int y1 = 0;

    /**
     * Koordinát X cieľovej pozície.
     */
    int x2 = 0;

    /**
     * Koordinát Y cieľovej pozície.
     */
    int y2 = 0;

    /**
     * Pravidlá pohybu.
     */
    Mobility mobility;

    /**
     * Typ figúrky po vykonaní promócie.
     */
    PieceType promotion = Pawn;

    /**
     * Typ rošády.
     */
    Castling castling = none;

    /**
     * Konverzia pohybu do algebraickej notácie.
     * 
     * \return Algebraická notácia pohybu.
     */
    std::string toString();

    /**
     * Konverzia pohybu do algebraickej notácie s flagmi pohybu.
     * 
     * \param color Vlastník figúrky.
     * \return Algebraická notácia pohybu.
     */
    std::string toStringWithFlags(PieceColor color);
};

/**
 * Trieda zodpovedná za konfiguráciu mobilít figúrok.
 */
class Mobilities {
public:
    /**
     * Zoznam konfigurácií mobilít figúrok.
     */
    static std::vector<Mobility> mobilityConfig[6][3];

    /**
     * Vypíše mobility pre danú figúrku.
     * 
     * \param type Typ figúrky.
     * \param essence Esencia figúrky.
     */
    static void printMobilities(PieceType type, PieceEssence essence);
};
