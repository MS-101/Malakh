#pragma once

/*****************************************************************//**
 * \file   hashing.h
 * \brief  Implementácia hashovanie šachovnice.
 *
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#include <piece.h>


/**
 * Trieda zodpovedná za implementáciu zobristovho hashovania.
 */
class ZobristHashing {
public:
    /**
     * Hodnoty reprezentujúce konkrétne figúrky na danej pozícii.
     */
    static unsigned long long squares[64*2*6];

    /**
     * Hodnoty reprezentujúce nepohnuté figúrky na danej pozícii.
     */
    static unsigned long long notMoved[64];

    /**
     * Hodnoty reprezentujúce špeciálnu figúrku na danej pozícii.
     */
    static unsigned long long ghosts[64*64];

    /**
     * Hodnota reprezentujúca hráča na rade.
     */
    static unsigned long long turn;

    /**
     * Pomocou tejto funkcie inicializujeme hodnoty zobristovho hashovania.
     */
    static void init();
private:
    /**
     * Bit pomocou ktorého vieme či sme vykonali inicializáciu.
     */
    static bool isInitalized;

    /**
     * Generácia pseudonáhodného čísla.
     * 
     * \return 64-bitové číslo
     */
    static unsigned long long rand64();
};

/**
 * Štruktúra reprezentujúca hash šachovnice.
 */
struct BoardHash {
    /**
     * 64-bitové číslo reprezentujúce identikikátor šachovnice.
     */
    unsigned long long value;

    /**
     * Touto funkciou upravíme hash šachovnice po premiestnení figúrky z alebo do danej pozície.
     * 
     * \param piece Presunutá figúrka.
     * \param x Koordinát X zmeneného políčka.
     * \param y Koordinát Y zmeneného políčka.
     */
    void switchSquare(Piece piece, char x, char y);

    /**
     * Touto funkciou upravíme hash šachovnice po pridaní alebo odstránení nepohnutej figúrky.
     * 
     * \param x Koordinát X zmeneného políčka.
     * \param y Koordinát Y zmeneného políčka.
     */
    void switchNotMoved(char x, char y);

    /**
     * Touto funkciou upravíme hash šachovnice po pridaní alebo odstránení špeciálnej figúrky (vytvórená 'hasty' pohybom).
     * 
     * \param ghost Špeciálna figúrka.
     */
    void switchGhost(Ghost ghost);

    /**
     * Touto funkciou upravíme hash šachovnice po zmenený aktívneho hráča.
     */
    void switchTurn();
};