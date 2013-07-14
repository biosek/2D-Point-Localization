#ifndef LOCALIZATION_H
#define LOCALIZATION_H
#include <iostream>
#include <vector>
#include "struktury.h"
#include "pavl.h"

/**
 * @brief Třída pro předzpracování a pro vyhledávání ve voronově diagramu.
 */
class Localization
{
private:
    bool processed;
    std::vector< Bod * > body;
    std::vector< Pas > pasy;

    /**
     * @brief       Načte a rozparsuje vstup.
     * @param input Odkud se bude načítat.
     */
    void loadFile(std::istream &input);

    /**
     * @brief   Vyhledá Pás, který obsahuje Y.
     * @param y Y souřadnice pro vyhledání.
     * @return  Vrací nalezený Pás nebo nulový pointer.
     */
    Pas pasyBinaryFind(double y);

    /**
     * @brief   Pomocná metoda pro porovnávání jestli daná double hodnota leží v
     *          daném pásu.
     * @param a Pás.
     * @param b Double hodnota.
     * @return  Vrací -1 resp. 0 resp. 1 podle toho jestli je nad resp. v resp.
     *          pod pásem.
     */
    int pasYCompare(Pas a,double b);

    /**
     * @brief   Vezme výsledek hledání Find_Pair a udělá průnik na polygony, které
     *          obsahuje.
     * @param val   Vstupní výsledek hledání.
     * @return  Vrací nalezený shodný polygon z obouch Usecek nebo defaultní
     *          nultý polygon.
     */
    unsigned long polygonsIntersection(std::pair<Vrchol*,Vrchol*> val);

public:
    /**
     * @brief Konstruktor lokalizace
     */
    Localization() : processed(false) {}

    PAVL avl;

    /**
     * @brief       Předzpracuje voronův diagram.
     *              1. Načte vstup do pole.
     *              2. Seřadí ho.
     *              3. Vytvoří z něj pásy.
     *              4. Postupně prochází seřazené body podle Y a prochází
     *              každého sousedy a přidává resp. odebírá je ze stromu pásu
     *              podle toho jestli soused leží pod resp. nad daným bodem.
     * @param input Odkud se bude načítat.
     */
    void preprocess(std::istream &input);

    /**
     * @brief   Vyhledá zadaný bod ve Voronově diagramu, pokud je již předzpracován.
     *          1. Najde pás podle Y souřadnice
     *          2. Pošle dotaz na strom pásu, který vrátí dvojici úseček.
     *          3. Zjištění polygonu, který je pro ty dvě úsečky společný.
     * @param x X-ová souřadnice pro vyhledání.
     * @param y Y-ová souřadnice pro vyhledání.
     */
    long int findPoint(double x, double y);
};

#endif // LOCALIZATION_H
