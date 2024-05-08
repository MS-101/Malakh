# Malakh

Tento projekt je šachový engine, ktorý je schopný hrať s neštandardnými šachovými pravidlami. Tento engine sa dá ovládať pomocou UCI príkazov.

## AI režimy

Tento engine má implementované nasledujúce AI režimy:

- **basic** - využíva staticku evaluačnú funkciu na evaluáciu šachovej pozície
- **ensemble** - využíva súborové učenie na evaluáciu šachovej pozície. Modely súborového učenia boli vytrénované v skripte [Malakh Trainer](https://github.com/MS-101/Malakh-Trainer).

## UCI príkazy

### Engine -> GUI 

Zoznam štandardných UCI príkazov:

- **id** - pomocou tohto príkazu sa engine identifikuje po prijatí uci príkazu
	- **id name [name]** - engine odošle enginu svoje meno (id name Malakh)
	- **id author [author]** - engine odošle enginu svojho autora (id author Martin Svab)
- **option name [name] type [type] ...** - pomocou tohto príkazu engine odošle nastaviteľnú konfiguráciu po prijatí uci príkazu (napr. option name WhitePawn type combo default Classic var Classic var Red var Blue)
- **uciok** - po odoslaní identifikácie, konfigurovateľných nastavení a mobilít sa odošle tento príkaz
- **readyok** - tento príkaz je odoslaný ako odpoveď na 'isready' správu
- **bestmove [bestmove]** - tento príkaz slúži ako odpoveď na 'go' správu. Po ukončení prehľadávania sa odošle najlepší možný ťah hráča. (napr. bestmove d2d4\_Hd3)

Zoznam dodatočných UCI príkazov:

- **mobility [piece] [essence] [type] [start\_x] [start\_y] [direction\_x] [direction\_y] [limit]** - týmito príkazmi sa odošle zoznam mobilít konkrétnej figúrky s určitou esenciou (napr. mobility Pawn Classic Move 0 1 0 0 1).
- **legalmoves [legalmove1] [legalmove2] ...** - týmto príkazom sa odošle zoznam legálnych pohybov hráča, ktorý je aktuálne na ťahu (napr. legalmoves a7a6 a7a5\_Ha6 b7b6 b7b5\_Hb6 ...)
- **check** - po príkaze 'position' ak v novej pozícii je kráľ napadnutý, tak engine na to upozorní GUI pomocou tohto príkazu
- **result [result]** - po príkaze 'legalmoves' a po príkaze 'go' ak legálne pohyby neexistujú tak engine odošle GUI výsledok hry týmto príkazom
	- **result White** - hra bola ukončená výhrou bieleho hráča
	- **result Black** - hra bola ukončená výhrou čierneho hráča
	- **result Stalemate** - hra bola ukončená remízou

### GUI -> Engine 

Zoznam štandardných UCI príkazov:

- **uci** - engine sa prepne do UCI režimu a na túto správu odpovie svojou identifikáciou, odoslaním konfigurovateľných nastavení a mobilít figúrok. Po odoslaní všetkých potrebných informácii enginu odošle 'uciok' správu.
- **isready** - pred prvým prehľadávaním pozície je nutné poslať tento príkaz aby si Engine inicializoval hodnoty ako zobrist kľúče. Engine po dokončení inicializácie odpovie správou 'readyok'.
- **setoption name [name] value [value]** - pomocou tohto príkazu sa zmení konfigurácia enginu. Aktuálne sa tento príkaz využíva pred 'ucinewgame' príkazom na nastavenie esencií figúrok (napr. setoption name WhitePawn value Red)
- **ucinewgame** - pre spustenie novej hry sa použije tento príkaz. Engine spustí novú hru s esenciami, ktoré boli predtým nastavené pomocou 'setoption' príkazov.
- **position [fen | startpos | curpos] moves ...** - týmto príkazom môžeme nastaviť aktuálnu pozíciu v hre (napr. position curpos moves e2e4). Štandardný protokol UCI nepodporuje pozíciu 'curpos'. Tento príkaz bol pre účely tejto implementácie modifikovaný. V aktuálnej verzii implementácia nepodporuje fen stringy.
- **go** - týmto príkazom povieme enginu aby vyhľadal najlepší ťah v aktuálnej pozícii, ktorá bola nastavená príkazom 'position'. V aktuálnej verzii je podporovaný tento príkaz len s parametrom 'depth' (napr. go depth 2).
- **quit** - engine sa po prijatí tejto správy vypne

Zoznam dodatočných UCI príkazov:

- **legalmoves** - týmto príkazom si GUI vypýta od enginu zoznam legálnych pohybov hráča, ktorý je aktuálne na ťahu. Engine na to odpovie vlastnou 'legalmoves' správou.