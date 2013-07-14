#ifndef PAVL_H
#define PAVL_H
#include "struktury.h"
#include <stack>

class PAVL
{
private:
    /**
     * @brief Vymění syna otce za jiného syna.
     * @param otec
     * @param syn
     * @param replika
     */
    static void replaceChild(Vrchol *otec, Vrchol *syn, Vrchol *replika);

    /**
     * @brief Hledá vrchol ve stromě podle bodu.
     * @param cell      Kořen stromu.
     * @param a         Hledaný bod.
     * @param equals    Pokud dojde k nalezení bodu na přímce, tak podle equals
     *                  se rozhoduje Compare jestli vrátí 0 (pro True) resp.
     *                  -1 (pro False).
     * @return Vrací nejbližší vrchol (úsečku) u kterého se nachází daný bod.
     */
    Vrchol* findCell(Vrchol *cell, Bod a, bool equals);

    /**
     * @brief Hledá hodnotově největší vrchol od daného.
     * @param cell  Daný vrchol.
     * @return      Vrací nejpravější vrchol od daného.
     */
    Vrchol* findMax(Vrchol *cell);

    /**
     * @brief Hledá hodnotově nejmenší vrchol od daného.
     * @param cell  Daný vrchol.
     * @return      Vrací nejlevější vrchol od daného.
     */
    Vrchol* findMin(Vrchol *cell);

    /**
     * @brief Hledá kořen od daného vrcholu.
     * @param cell  Daný vrchol.
     * @return      Vrací kořen daného vrcholu.
     */
    Vrchol* findRoot(Vrchol *cell);

    /**
     * @brief   Hledá následující vrchol ve stromě. Tak, že se nejdřív podívá
     *          jestli existuje jeho pravý syn a hledá minimální vrchol na tomto
     *          synovi nebo jde ke kořeni a dívá se, jestli je synem levým.
     * @param cell  Vrchol odkud se hledá.
     * @return  Vrací následující vrchol.
     */
    Vrchol* findNext(Vrchol *cell);

    /**
     * @brief   Hledá předchozí vrchol ve stromě. Tak, že se nejdřív podívá
     *          jestli existuje jeho levy syn a hledá maximální vrchol na tomto
     *          synovi nebo jde ke kořeni a dívá se, jestli je synem pravým.
     * @param cell  Vrchol odkud se hledá.
     * @return  Vrací předchozí vrchol.
     */
    Vrchol* findBefore(Vrchol *cell);

    /**
     * @brief Jednoduchá AVL rotace vpravo přes vstupní vrchol.
     * @param cell  Vstupní vrchol.
     */
    void rotateRight(Vrchol *cell);

    /**
     * @brief Jednoduchá AVL rotace vlevo přes vstupní vrchol.
     * @param cell  Vstupní vrchol.
     */
    void rotateLeft(Vrchol *cell);

    /**
     * @brief Funkce na vyvážení stromu po insertu.
     * @param cell  Vrchol, který byl právě přidán.
     */
    void balanceAdd(Vrchol *cell);

    /**
     * @brief   Funkce na zjistění jakou změnu znaménka udělá snížení podstromu
     *          daného vrcholu jeho otci.
     * @param cell  Vrchol jakožto odebíraný syn.
     * @return      Vrací vyvážení.
     */
    int getDeleteSign(Vrchol *cell);

    /**
     * @brief Funkce na vyvážení stromu po deletu.
     * @param cell  Rodič smazaného vrcholu.
     * @param s     Znaménko po smazání syna.
     */
    void balanceDelete(Vrchol *cell, int s);

    /**
     * @brief   Zkopíruje cestu od daného vrcholu až ke kořeni a přepojí všechny
     *          předchozí verze na tu novou. Čili ve starých verzích se nedá
     *          navigovat podle parent proměnné.
     * @param cell  Daný vrchol.
     * @return  Vrací kopii daného vrcholu.
     */
    Vrchol* copyPath(Vrchol *cell);

    /**
     * @brief Vyčistí pomocný zásobník.
     */
    void zasobnikClear();

    std::vector<Vrchol*> verze;
    std::stack<Vrchol*> zasobnik; //Slouzi pouze pro vyhledavani

public:
    /**
     * @brief Defaultní konstruktor, který vytvoří první nulový kořen.
     */
    PAVL()
    {
        Vrchol *tmp = 0;
        verze.push_back(tmp);
    }

    /**
     * @brief Pro vyhledání poslední verze.
     * @return Vrátí ukazatel na kořen verze, která je ve vektoru poslední.
     */
    inline Vrchol* getLatestVersion() { return verze.empty() ? 0 : verze.back(); }

    /**
     * @brief Hledá dvojici vrcholů ve stromě mezi kterýma leží hledaný bod.
     * @param cell  Kořen stromu.
     * @param a     Hledaný bod.
     * @return      Vrací nejbližší vrchol u kterého se bod nachází a vrchol
     *              těsně před resp. za prvním nalezeným podle toho jestli
     *              hledaný bod byl nalevo resp. napravo od prvního nalezeného
     *              vrcholu.
     */
    std::pair<Vrchol*,Vrchol*> findPair(Vrchol *cell, Bod a);

    /**
     * @brief Funkce na přidání vrcholu do stromu.
     * @param a     Vrchol, který se má přidat.
     * @param y     Double hodnota, podle které se ve stromě vyhledává.
     *              (nějaká Y souřadnice bodu z pásu, který se právě zpracovává)
     *
     *      Vytvoří novou verzi stromu s tím, že udělá kopii cesty ke kořenu.
     */
    void insertCell(Usecka *a, double y);

    /**
     * @brief Funkce pro smazání vrcholu ze stromu.
     * @param a     Vrchol ke smazání.
     *
     *      Vytvoří novou verzi stromu s tím, že udělá kopii cesty ke kořenu.
     */
    void deleteCell(Usecka *a);

    /**
     * @brief Zničí strom.
     * @param cell  Kořen stromu.
     */
    void destroyTree(Vrchol *cell);

};

#endif // PAVL_H
