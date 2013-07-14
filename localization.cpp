#include "localization.h"
#include <algorithm>
#include "pavl.h"

/**
 * @brief Metoda pro porovnání dvou Y souřadnic u dvouch boduu.
 * @param a První bod.
 * @param b Druhý bod.
 * @return True jestli je první Y souřadnice větší.
 */
inline bool bodYCompare(Bod *a,Bod *b) { return a->y > b->y ; }

void Localization::preprocess(std::istream &input)
{
    //Nahrani vrcholu a hran
    loadFile(input);

    //Sort vrcholu podle Y
    std::sort(body.begin(),body.end(),bodYCompare);

    //Vytvoreni pasuu
    for(size_t i = 0; i < body.size() - 1; ++i)
    {
        pasy.push_back(Pas(i,body[i],body[i+1]));
    }

    int counter = 0;
    double half = 0;
    //Vytvoreni stromu pro kazdy pas
    for(std::vector<Bod*>::iterator it1 = body.begin();
        it1 != body.end(); ++it1, ++counter)
    {
        if(counter == body.size() - 1)
            half = (*it1)->y;
        else
            half = pasy[counter].getHalfY();

        for(std::vector<Bod*>::iterator it2 = (*it1)->sousedi.begin();
            it2 != (*it1)->sousedi.end(); ++it2)
        {
            if((*it2)->y > (*it1)->y)
            {
                Usecka tmp(*it2,*it1);
                avl.deleteCell(&tmp);
            }
            else if((*it2)->y < (*it1)->y)
            {
                Polygons *p = &((*it1)->polygony[int(it2 - (*it1)->sousedi.begin())]);
                Usecka tmp(*it1, *it2, p);
                avl.insertCell(&tmp,half);
            }
        }
        pasy[counter].treeVer = avl.getLatestVersion();
    }


    processed = true;
}

void Localization::loadFile(std::istream &input)
{
    //Nacteni poctu vstupnich bodu
    unsigned long n = 0;
    input >> n;
    double x = 0,y = 0;
    unsigned long m = 0,s = 0, p1 = 0, p2 = 0;

    //Nacte body a ulozi je do vektoru
    for(size_t i = 0; i < n; ++i)
    {
        input >> x;
        input >> y;
        Bod *tmp = new Bod(x,y);
        body.push_back(tmp);
    }

    //Do ulozenych bodu zapise hrany
    for(size_t i = 0; i < n; ++i)
    {
        input >> m; //pocet sousedu
        for(size_t j = 0; j < m; ++j)
        {
            input >> s; //soused
            input >> p1; //polygon1 ktery ohranicuje hrana (i,s)
            input >> p2; //polygon2 ktery ohranicuje hrana (i,s)
            body[i]->sousedi.push_back(body[s]);
            body[i]->polygony.push_back( Polygons(p1,p2) );
        }
    }
}

int Localization::pasYCompare(Pas a,double b)
{
    if(a.dolni->y > b)
        return -1;
    else if(a.horni->y < b)
        return 1;
    else
        return 0;
}

Pas Localization::pasyBinaryFind(double y)
{
    int lb = 0, ub = pasy.size() - 1, mid;

    while(lb<ub)
    {
        mid=(lb+ub)/2;
        if(pasYCompare(pasy[mid],y) == 1)
            ub=mid-1;
        else if(pasYCompare(pasy[mid],y) == -1)
            lb=mid+1;
        else if(pasYCompare(pasy[mid],y) == 0)
            break;
    }
    if(pasYCompare(pasy[mid],y) == 0)
        return pasy[mid];
    else if(ub > -1 && pasYCompare(pasy[ub],y) == 0)
        return pasy[ub];
    else if(lb < pasy.size() && pasYCompare(pasy[lb],y) == 0)
        return pasy[lb];
    else
        return Pas();
}

unsigned long Localization::polygonsIntersection(std::pair<Vrchol*,Vrchol*> val)
{
    if(!val.second)
        return 0;

    unsigned long a,b,c,d;
    a = val.first->value.polygony->first;
    b = val.first->value.polygony->second;
    c = val.second->value.polygony->first;
    d = val.second->value.polygony->second;
    if(a == c || a == d)
        return a;
    else if(b == c || b == d)
        return b;
}

long int Localization::findPoint(double x, double y)
{
    if(!processed)
        return -1;

    Pas p = pasyBinaryFind(y);

    if(p.index == -1)
        return 0;
    else
    {
        std::pair<Vrchol*,Vrchol*> vysledek = avl.findPair(p.treeVer,Bod(x,y));
        return polygonsIntersection(vysledek);
    }
}
