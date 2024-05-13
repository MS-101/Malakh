#pragma once

/*****************************************************************//**
 * \file   board.h
 * \brief  Definícia šachovnice a generácia pohybov.
 *
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#include <bitboard.h>
#include <piece.h>
#include <mobility.h>
#include <evaluation.h>
#include <hashing.h>
#include <vector>


/**
 * Enumerátor reprezentujúci výsledok hry.
 */
enum GameResult {
    /**
     * Hra ešte nebola ukončená.
     */
    Unresolved,
    /**
     * Hra bola terminovaná výhrou bieleho hráča.
     */
    WhiteWin,
    /**
    * Hra bola terminovaná výhrou čierneho hráča.
    */
    BlackWin,
    /**
     * Hra bola terminovaná remízou.
     */
    Stalemate
};

/**
 * Herná konfigurácia esencií.
 */
struct EssenceArgs {
    /**
     * Esencia bieleho pešiaka.
     */
    PieceEssence whitePawn = Classic;

    /**
     * Esencia bielej veže.
     */
    PieceEssence whiteRook = Classic;

    /**
     * Esencia bieleho rytiera.
     */
    PieceEssence whiteKnight = Classic;

    /**
     * Esencia bieleho strelca.
     */
    PieceEssence whiteBishop = Classic;

    /**
     * Esencia čierneho pešiaka.
     */
    PieceEssence blackPawn = Classic;

    /**
     * Esencia čiernej veže.
     */
    PieceEssence blackRook = Classic;

    /**
     * Esencia čierneho rytiera.
     */
    PieceEssence blackKnight = Classic;

    /**
     * Esencia čierneho strelca.
     */
    PieceEssence blackBishop = Classic;
};

/**
 * Táto štruktúra reprezentuje šachovnicu so všetkými relevantnými informáciami.
 */
class Board {
public:
    /**
     * Konfigurácia esencií fúgurok.
     */
    PieceEssence essenceConfig[2*6] = {};
    
    /**
     * Počítadlo herných esencií.
     */
    int essenceCounts[3];
    
    /**
     * Bitboardy reprezentujúce konkrétny typ figúrok.
     */
    BitBoard pieces[2*6];
    
    /**
     * Bitboardy reprezentujúce všetky figúrky konkrétneho hráča.
     */
    BitBoard colors[2];
    
    /**
     * Bitboardy reprezentujúce napadnuté políčka konkrétneho hráča.
     */
    BitBoard attacks[2];
	
	/**
	 * Bitboard reprezentujúci všetky figúrky na šacchovnici.
	 */
	BitBoard allPieces;

    /**
     * Bitboard reprezentujúci figúrky, ktoré sa ešte nepohli (pre En Passant a rošádu).
     */
    BitBoard notMoved;

    /**
     * Figúrka zanechaná po vykonaní špeciálneho pohybu pešiaka na začiatku hry.
     */
    Ghost ghost;

    /**
     * Počítadla figúrok.
     */
    int pieceCounts[2*6] = {};

    /**
     * Reprezentácia šachovnice pre vstup do modelu CNN.
     */
    int inputArray[2*8*8];

    /**
     * Zoznamy dostupných pohybov pre konkrétneho hráča.
     */
    std::vector<LegalMove> moves[2];

    /**
     * Bit pomocou ktorého si pamätáme či sme zvalidovali vygenerované pseudolegálne pohyby.
     */
    bool movesValidated = false;

    /**
     * Aktuálny hráč na rade.
     */
    PieceColor curTurn = White;

    /**
     * Priebežne aktualizované argumenty evaluačnej funkcie.
     */
    EvalArgs eval{};

    /**
     * Hash šachovej pozície pomocou ktorého vieme identifikovať šachovnicu.
     */
    BoardHash hash{};

    /**
     * Počítadlo polovičných pohybov. Toto počítadlo sa resetuje po pohybu pešiaka alebo obsadení figúrky.
     */
    int halfmoveClock = 0;

    /**
     * Počítadlo všetkých plných pohybov. Inkrementované po každom pohybe čierneho hráča.
     */
    int fullmoveClock = 1;
    
	/**
	 * Inicializácia počiatočného stavu šachovnice.
	 * 
	 * \param essenceArgs Herná konfigurácia esencií.
	 */
	void initBoard(EssenceArgs essenceArgs);

    /**
     * Inicializáccia šachovnice podľa argumentov.
     * 
     * \param essenceArgs Herná konfigurácia esencií.
     * \param pieces Bitboardy reprezentujúce konkrétny typ figúrok.
     * \param curTurn Aktuálny hráč na rade.
     */
    void initBoard(EssenceArgs essenceArgs, BitBoard pieces[12], PieceColor curTurn);

    /**
     * Táto funkcia vypíše umiestnenie figúrok na šachovnicí.
     */
    void printBoard();

    /**
    * Táto funkcia vypíše dostupné pohyby figúrok.
    */
    void printMoves();

    /**
     * Táto funkcia vypočíta číselnú hodnotu aktuálnej pozície podľa daného hráča.
     * 
     * \param color Hráč pre ktorého je vypočítaná hodnota pozície.
     * \return Hodnota pozície.
     */
    int evalBoard(PieceColor color);

    /**
     * Overenie či je daná pozícia považovaná za tichú.
     * 
     * \return Vráti 1 ak je pozícia tichá, 0 ak pozícia nie je tichá.
     */
    bool isQuiet();

    /**
     * Vykoná sa pohyb figúrky bez nastavenej promócie pešiaka (len ak je pohyb legálny).
     * 
     * \param x1 Koordinát X zdrojovej pozície.
     * \param y1 Koordinát Y zdrojovej pozície.
     * \param x2 Koordinát X cieľovej pozície.
     * \param y2 Koordinát Y cieľovej pozície.
     * \return Bit reprezentujúci či bol pohyb úspešne vykonaný.
     */
    bool makeMove(char x1, char y1, char x2, char y2);

    /**
     * Vykoná sa pohyb figúrky s nastevenou promóciou pešiaka (len ak je pohyb legálny).
     * 
     * \param x1 Koordinát X zdrojovej pozície.
     * \param y1 Koordinát Y zdrojovej pozície.
     * \param x2 Koordinát X cieľovej pozície.
     * \param y2 Koordinát Y cieľovej pozície.
     * \param promotion Typ figúrky na ktorý sa presunutý pešiak zmení.
     * \return Bit reprezentujúci či bol pohyb úspešne vykonaný.
     */
    bool makeMove(char x1, char y1, char x2, char y2, PieceType promotion);

    /**
     * Vykoná sa konkrétny pseudolegálny pohyb figúrky.
     * 
     * \param move Vykonávaný pseudolegálny pohyb.
     * \return Vráti 1 ak je vykonaný pohyb legálny, vráti 0 ak je vykonaný pohyb pseudolegálny.
     */
    bool makeMove(LegalMove move);

    /**
     * Nájde sa figúrka na konkrétnej pozícii na šachovnici.
     * 
     * \param x Koordinát X danej pozície.
     * \param y Koordinát Y danej pozície.
     * \return Vráti pár bitu reprezentujúci úšpešné/neúspešné nájdenie figúrky a nájdenú figúrku.
     */
    std::pair<bool, Piece> getPiece(char x, char y);

    /**
     * Vráti legálne pohyby aktuálneho hráča. Vykoná validáciu pohybov ak ešte nebola vykonaná.
     * 
     * \return Zoznam legálnych pohybov hráča, ktorý je na rade.
     */
    std::vector<LegalMove> getLegalMoves();

    /**
     * Zapíše našu šachovnicu do FEN stringu.
     * 
     * \return FEN string reprezentujúci našu šachovnicu.
     */
    std::string toString();

    /**
    * Vráti aktuálny výsledok hry.
    * 
    * \return Enumerátor reprezentujúci aktuálny výsledok hry.
    */
    GameResult getResult();
private:
    /**
     * Nastaví konfigurácia esencií na šachovnici.
     * 
     * \param essenceArgs Nová konfigurácia esencií.
     */
    void setEssenceConfig(EssenceArgs essenceArgs);

    /**
     * Odstráni všetky figúrky zo šachovnice a resetuje evaluačné argumenty.
     */
    void clearBoard();

    /**
     * Pridá figúrku na šachovnicu a aktualizuje relevantné evaluačné argumenty.
     * 
     * \param color Vlastník novej figúrky.
     * \param type Typ novej figúrky.
     * \param x Koordinát X novej figúrky.
     * \param y Koordinát Y novej figúrky.
     * \param isNew Ak je figúrka nová, tak sa nastaví aj notMoved bitboard.
     */
    void addPiece(PieceColor color, PieceType type, char x, char y, bool isNew);

    /**
     * Odstráni zo šachovnice figúrku a aktualizuje relevantné evaluačné argumenty.
     * 
     * \param x Koordinát X odstránenej figúrky.
     * \param y Koordinát Y odstránenej figúrky.
     * \return Vráti pár bitu, ktorý reprezentuje úspešné/neúspešné odstránenie figúrky a odstránenú figúrku.
     */
    std::pair<bool, Piece> removePiece(char x, char y);

    /**
     * Aktualizuje agregácie bitboardov - colors, allPieces.
     */
	void refreshAggregations();

    /**
     * Nastaví figúrku na prvom kanáli vstupnej matice pre CNN.
     * 
     * \param pieceIndex Identifikátor figúrky na danej pozícii.
     * \param x Koordinát X danej pozície.
     * \param y Koordinát Y danej pozície.
     */
    void setInputArrayPiece(char pieceIndex, char x, char y);

    /**
     * Nastaví farbu hráča na druhom kanáli vstupnej matice pre CNN.
     * 
     * \param color Farba hráča na rade
     */
    void setInputArrayColor(PieceColor color);
};

/**
 * Pomocná trieda na generáciu pohybov šachovnice
 */
class MoveGenerator {
public:
    /**
     * Táto funkcia odstráni všetký vygenerované pohyby a resetuje všetky relevantné evaluačné argumenty.
     * 
     * \param board Šachovnica kde odstraňujeme vygenerované pohyby.
     */
    static void clearMoves(Board* board);

    /**
     * Táto funkcia vygeneruje všetky pseudolegálne pohyby.
     * 
     * \param board Šachovnica kde generujeme pohyby.
     */
    static void generateMoves(Board* board);

    /**
     * Táto funkcia vygeneruje všetky pseudolegálne pohyby pre figúrku na konkrétnej pozícii.
     * 
     * \param board Šachovnica kde generujeme pohyby.
     * \param piece Figúrka ktorej generujeme pohyby.
     * \param x Koordinát X kde sa nachádza naša figúrka.
     * \param y Koordinát Y kde sa nachádza naša figúrka.
     */
    static void generateMoves(Board* board, Piece piece, char x, char y);
};
