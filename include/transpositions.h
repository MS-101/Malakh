/*****************************************************************//**
 * \file   transpositions.h
 * \brief  Implementácia transpozičných tabuliek pre zrýchlenie prehľadávania.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#pragma once

#include <unordered_map>
#include <mutex>


/**
 * Štruktúra reprezentujúca transpozíciu.
 */
struct Transposition {
    /**
     * Evaluácia danej šachovej pozície.
     */
    int value = 0;

    /**
     * Hĺbka do ktorej bola táto pozícia prehľadaná.
     */
    int depth = -1;
};

/**
 * Trieda reprezentujúca LRU cache v ktorej ukladáma transpozície.
 */
class TranspositionCache {
public:
    /**
     * Konštruktor inicializuje cache danej veľkosti.
     * 
     * \param capacity Kapacita našej cache.
     */
    TranspositionCache(int capacity);

    /**
     * Pomocou tejto metódy z cache vyberieme transpozíciu. Ak ju nájdeme, tak ju presunieme na vrch nášho zoznamu.
     * 
     * \param key Hash hľadanej transpozície.
     * \return Nájdená transpozícia. Ak žiadna nebola nájdená tak hĺbka prehľadávania vrátenej transpozície je -1.
     */
    Transposition get(unsigned long long key);

    /**
     * Pomocou tejto metódy do cache vložíme novú transpozíciu. Ak je naše cache plné, tak z cache odstránime transpozícia podľa LRU princípu.
     * 
     * \param key Hash novej transpozície.
     * \param value Hodnota novej transpozície.
     */
    void put(unsigned long long key, Transposition value);
private:
    /**
     * Hashmapa v ktorej ukladáme všetky transpozície. Kľúčom tejto mapy je hash šachovej pozície.
     */
    std::unordered_map<unsigned long long, Transposition> cache;

    /**
     * Zoznam reprezentuje poradie transpozícii na základe LRU princípu.
     */
    std::list<unsigned long long> order;

    /**
     * Maximálna kapacita našej cache.
     */
    int capacity;

    /**
     * Mutex pre úpravu cache v paralelnom vyhľadávaní.
     */
    std::mutex* mutex;
};