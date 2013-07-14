#ifndef STRUKTURY_H
#define STRUKTURY_H
#include <vector>
#include <math.h>

//Slouží ke zkrácení kódu
typedef std::pair < unsigned long, unsigned long > Polygons;

/**
 * @brief Struktura pro uložení Bodu a jeho sousedů.
 *          (jediná instance pro celý výpočet.)
 */
struct Bod
{
    /**
     * @brief Defaultní konstruktor Bodu.
     */
    Bod()  :x(0),y(0) { }
    /**
     * @brief Konstruktor Bodu.
     * @param _x    X souřadnice.
     * @param _y    Y souřadnice.
     */
    Bod(double _x, double _y) : x(_x),y(_y) { }

    double x;
    double y;
    std::vector< Bod* > sousedi;
    std::vector< Polygons > polygony;
};

/**
 * @brief Struktura pro uložení jedné úsečky (hrany) jako vrchol do stromu.
 */
struct Usecka
{
    /**
     * @brief Defaultní konstruktor Usecky.
     */
    Usecka() : a(0),b(0) { }
    /**
     * @brief Konstruktor Usecky.
     * @param _A    První ukazatel bodu.
     * @param _B    Druhý ukazatel bodu.
     */
    Usecka(Bod *_A, Bod *_B) : a(_A),b(_B) { }
    /**
     * @brief Konstruktor Usecky.
     * @param _A    První ukazatel bodu.
     * @param _B    Druhý ukazatel bodu.
     * @param p     Ukazatel na pár polygonů, který tato Usecka ohraničuje.
     */
    Usecka(Bod *_A, Bod *_B, Polygons* p) : a(_A),b(_B), polygony(p) { }
    /**
     * @brief Kopirovaci konstruktor Usecky.
     * @param u Usecka ke zkopírování.
     */
    Usecka(const Usecka &u) : a(u.a),b(u.b),polygony(u.polygony) { }

    Bod *a;
    Bod *b;
    Polygons* polygony;

    /**
     * @brief   Porovnávací funkce pro navigaci ve stromě. S vestavěnou
     *          tolerancí, kvůli zaokrouhlovacím chybám.
     * @param other     Bod ke kterému se hledá na jakou stranu se vydat.
     * @param equals    True pokud chceme zjistit jestli dany bod lezi na teto
     *                  usecce.
     * @return  -1: Když po výpočtu podle Y leží vstupní X nalevo od X ležící
     *              na úsečce.
     *          0:  Kdyz equals = true a když bod leží na úsečce (bráno i se
     *              zaokrouhlovací tolerancí, která mi v testech vyšla řádově
     *              10^-15)
     *          +1: Když po výpočtu podle Y leží vstupní X napravo od X ležící
     *              na úsečce.
     */
    inline int compare(Bod other, bool equals = false)
    {
        double dx = other.x -
                    (a->x + ((b->x - a->x)*(other.y - a->y))/(b->y - a->y));

        if(isRightOfTolerance(dx))
            return 1;
        else if(isLeftOfTolerance(dx))
            return -1;
        else if(equals && isInTolerance(dx))
            return 0;
        else if(!equals && isInTolerance(dx))
            return -1;
    }

    /**
     * @brief   Vypočítá X souřadnici ke vstupní Y souřadnici takovou, která
     *          leží na této úsečce.
     * @param y Vstupní Y souřadnice.
     * @return  Vrací X souřadnici, ležící na úsečce.
     */
    inline double getXfromY(double y)
    {
        return a->x + ((b->x - a->x)*(y - a->y))/(b->y - a->y);
    }
private:
    /**
     * @brief  Metoda na zjištění jestli danná diference je nula s jistou
     *         zaokrouhlovací tolerancí.
     * @param a Vstupní diference.
     * @return  Vrací true pokud je diference v toleranci.
     */
    inline bool isInTolerance(double a)
    {
        if(a > pow(-10,-15) || a < pow(10,-15))
            return true;
        else
            return false;
    }

    /**
     * @brief   Metoda na zjištění jestli danná diference leží nalevo od nuly,
     *          až za zaokrouhlovací tolerancí.
     * @param a Vstupní diference.
     * @return  Vrací true, pokud je diference nalevo od nuly a tolerance.
     */
    inline bool isLeftOfTolerance(double a)
    {
        if(a < pow(-10,-15))
            return true;
        else
            return false;
    }

    /**
     * @brief   Metoda na zjištění jestli danná diference leží napravo od nuly,
     *          až za zaokrouhlovací tolerancí.
     * @param a Vstupní diference.
     * @return  Vrací true, pokud je diference napravo od nuly a tolerance.
     */
    inline bool isRightOfTolerance(double a)
    {
        if(a > pow(10,-15))
            return true;
        else
            return false;
    }
};

/**
 * @brief   Pomocná struktura pro tvorbu perzistentního AVL stromu. Uchovávající
 *          úsečku.
 */
struct Vrchol
{
    /**
     * @brief Defaultní konstruktor Vrcholu.
     */
    Vrchol() : value(Usecka()), sign(0), parent(0), right(0), left(0) {}
    /**
     * @brief Konstruktor Vrcholu.
     * @param u Usecka jako value.
     */
    Vrchol(Usecka *u) : value(*u), sign(0), parent(0), right(0), left(0) {}
    /**
     * @brief Kopírovací konstruktor Vrcholu.
     * @param v Vrchol ke zkopírování.
     */
    Vrchol(const Vrchol &v) : value(v.value), sign(v.sign), parent(v.parent),
        right(v.right), left(v.left) {}

    Usecka value;
    int sign;
    Vrchol *parent;
    Vrchol *right;
    Vrchol *left;
};

/**
 * @brief Struktura pro uložení pásu.
 */
struct Pas
{
    /**
     * @brief Defaultní konstruktor Pasu.
     */
    Pas() : index(-1),horni(0),dolni(0) { }
    /**
     * @brief Konstruktor Pasu.
     * @param i Index pásu.
     * @param h Horní okraj pásu.
     * @param d Dolní okraj pásu.
     */
    Pas(int i, Bod* h, Bod* d) : index(i),horni(h),dolni(d) { }

    int index;
    Bod *horni;
    Bod *dolni;
    Vrchol *treeVer;

    /**
     * @brief   Slouží pro vyhledávání ve stromu pásu.
     * @return  Vrací hodnotu Y ve středu pásu.
     */
    inline double getHalfY()
    {
        return ((horni->y - dolni->y) / 2) + dolni->y;
    }

};

#endif // STRUKTURY_H
