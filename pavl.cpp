#include "pavl.h"

void PAVL::replaceChild(Vrchol *otec, Vrchol *syn, Vrchol *replika)
{
    //Hledani orientace syna
    if(otec->left == syn)
        otec->left = replika;
    else if(otec->right == syn)
        otec->right = replika;
}

void PAVL::zasobnikClear()
{
    while(!zasobnik.empty())
        zasobnik.pop();
}

Vrchol* PAVL::findCell(Vrchol *cell, Bod a, bool equals)
{
    zasobnikClear();
    while(true)
    {
        if(!cell)
            return 0;

        int cmp = cell->value.compare(a,equals);
        if(cmp == 0)
        {
            return cell;
        }
        else if(cmp == -1)
        {
            if(!cell->left && !equals)
                return cell;
            zasobnik.push(cell);
            cell = cell->left;
        }
        else if(cmp == 1)
        {
            if(!cell->right && !equals)
                return cell;
            zasobnik.push(cell);
            cell = cell->right;
        }
    }
}

std::pair<Vrchol*,Vrchol*> PAVL::findPair(Vrchol *koren, Bod a)
{
    Vrchol *tmp = findCell(koren,a,false);
    int where = tmp->value.compare(a,false);
    if(where == -1)
        return std::pair<Vrchol*,Vrchol*>(tmp,findBefore(tmp));
    else
        return std::pair<Vrchol*,Vrchol*>(tmp,findNext(tmp));
}

Vrchol* PAVL::findMax(Vrchol *cell)
{
    if(!cell)
        return 0;
    while(cell->right)
        cell = cell->right;
    return cell;
}

Vrchol* PAVL::findMin(Vrchol *cell)
{
    if(!cell)
        return 0;
    while(cell->left)
        cell = cell->left;
    return cell;
}

Vrchol* PAVL::findRoot(Vrchol *cell)
{
    if(!cell)
        return 0;
    while(cell->parent)
        cell = cell->parent;
    return cell;
}

Vrchol* PAVL::findNext(Vrchol *cell)
{
    if(cell->right)
        return findMin(cell->right);
    else
    {
        while(true)
        {
            Vrchol *parent = zasobnik.empty() ? 0 : zasobnik.top();

            if(!parent)
                return 0;

            if(parent->left == cell)
                return parent;
            if(parent)
            {
                cell = parent;
                zasobnik.pop();
            }
        }
    }
}

Vrchol* PAVL::findBefore(Vrchol *cell)
{
    if(cell->left)
        return findMax(cell->left);
    else
    {
        while(true)
        {
            Vrchol *parent = zasobnik.empty() ? 0 : zasobnik.top();

            if(!parent)
                return 0;

            if(parent->right == cell)
                return parent;
            if(parent)
            {
                cell = parent;
                zasobnik.pop();
            }
        }
    }
}

void PAVL::rotateRight(Vrchol *cell)
{
    Vrchol *l, *r, *m;
    l = cell->left;
    m = l->right;
    r = cell;

    l->parent = r->parent;
    if(l->parent)
        PAVL::replaceChild(l->parent,r,l);
    l->right = r;
    r->parent = l;
    r->left = m;
    if(m)
        m->parent = r;
}
void PAVL::rotateLeft(Vrchol *cell)
{
    Vrchol *l, *r, *m;
    l = cell;
    r = l->right;
    m = r->left;

    r->parent = l->parent;
    if(r->parent)
        PAVL::replaceChild(r->parent,l,r);
    r->left = l;
    l->parent = r;
    l->right = m;
    if(m)
        m->parent = l;
}

void PAVL::balanceAdd(Vrchol *cell)
{
    Vrchol *t;
    while(true)
    {
        t=cell->parent;
        if(!t)
            return;

        if(cell == t->left)
            t->sign--;
        else
            t->sign++;

        if(t->sign == 0)
            return;
        else if((t->sign == -1) || (t->sign == 1))
        {
            cell=t;
            continue;
        }
        else if(t->sign == -2)
        {
            if(cell->sign == -1)
            {
                cell->sign = t->sign = 0;
                PAVL::rotateRight(t);
            }
            else
            {
                cell->sign = t->sign = 0;
                if(cell->right->sign == -1)
                    t->sign = 1;
                else if(cell->right->sign == 1)
                    t->sign = -1;
                cell->right->sign = 0;
                PAVL::rotateLeft(cell);
                PAVL::rotateRight(t);
            }
        }
        else if(t->sign == 2)
        {
            if(cell->sign == 1)
            {
                cell->sign = t->sign = 0;
                PAVL::rotateLeft(t);
            }
            else
            {
                cell->sign = t->sign = 0;
                if(cell->left->sign == 1)
                    t->sign = -1;
                else if(cell->left->sign == -1)
                    t->sign = 1;
                cell->left->sign = 0;
                PAVL::rotateRight(cell);
                PAVL::rotateLeft(t);
            }
        }
        return;
    }
}

int PAVL::getDeleteSign(Vrchol *cell)
{
    if (!cell->parent) return 0;
    else if (cell->parent->left == cell) return 1;
    else if (cell->parent->right == cell) return -1;
}

void PAVL::balanceDelete(Vrchol *cell, int s)
{
    while(true)
    {
        if(!cell)
            return;
        cell->sign += s;

        if((cell->sign == 1) || (cell->sign == -1))
            return;
        if(cell->sign == 0)
        {
            s = getDeleteSign(cell);
            cell = cell->parent;
            continue;
        }
        if(cell->sign == 2)
        {
            if(cell->right->sign == 1)
            {
                cell->sign = cell->right->sign = 0;
                rotateLeft(cell);
                cell = cell->parent;
                s = getDeleteSign(cell);
                cell = cell->parent;
                continue;
            }
            if(cell->right->sign == 0)
            {
                cell->right->sign = -1;
                cell->sign = 1;
                rotateLeft(cell);
                return;
            }
            if(cell->right->sign == -1)
            {
                cell->sign = cell->right->sign = 0;
                if(cell->right->left->sign == 1)
                    cell->sign = -1;
                if(cell->right->left->sign == -1)
                    cell->right->sign = 1;
                cell->right->left->sign = 0;
                rotateRight(cell->right);
                rotateLeft(cell);
                cell = cell->parent;
                s = getDeleteSign(cell);
                cell = cell->parent;
                continue;
            }
        }
        if(cell->sign == -2)
        {
            if(cell->left->sign == -1)
            {
                cell->sign = cell->left->sign = 0;
                rotateRight(cell);
                cell = cell->parent;
                s = getDeleteSign(cell);
                cell = cell->parent;
                continue;
            }
            if(cell->left->sign == 0)
            {
                cell->left->sign = 1;
                cell->sign = -1;
                rotateRight(cell);
                return;
            }
            if(cell->left->sign == 1)
            {
                cell->sign = cell->left->sign = 0;
                if(cell->left->right->sign == -1)
                    cell->sign = 1;
                if(cell->left->right->sign == 1)
                    cell->left->sign = -1;
                cell->left->right->sign = 0;
                rotateLeft(cell->left);
                rotateRight(cell);
                cell = cell->parent;
                s = getDeleteSign(cell);
                cell = cell->parent;
                continue;
            }
        }
    }
}

void PAVL::destroyTree(Vrchol *cell)
{
    if(!cell)
        return;
    destroyTree(cell->right);
    destroyTree(cell->left);
    cell->~Vrchol();
}

Vrchol* PAVL::copyPath(Vrchol *cell)
{
    Vrchol *ret = new Vrchol(*cell);
    if(ret->parent)
    {
        ret->parent = copyPath(ret->parent);
        if(ret->parent->left == cell)
            ret->parent->left = ret;
        if(ret->parent->right == cell)
            ret->parent->right = ret;
    }

    if(ret->left)
        ret->left->parent = ret;
    if(ret->right)
        ret->right->parent = ret;


    return ret;
}

void PAVL::insertCell(Usecka *a, double y)
{
    Vrchol *p = getLatestVersion();
    Vrchol *novy = new Vrchol();
    novy->value = *a;
    if(!p)
    {
        p = novy;
        verze.push_back(novy);
        return;
    }
    while(true)
    {
        int cmp = p->value.compare(Bod(a->getXfromY(y),y), true);
        //Pridani existujici usecky
        if(cmp == 0)
        {
            delete novy;
            return;
        }

        else if(cmp == -1)
        {
            if(!p->left)
            {
                p = copyPath(p);
                p->left = novy;
                novy->parent = p;
                break;
            }
            else
                p = p->left;
        }
        else if(cmp == 1)
        {
            if(!p->right)
            {
                p = copyPath(p);
                p->right = novy;
                novy->parent = p;
                break;
            }
            else
                p = p->right;
        }
    }
    balanceAdd(novy);
    verze.push_back(findRoot(p));
    //*root = find_Root(p);
}

void PAVL::deleteCell(Usecka *a)
{
    Vrchol *p, *t;
    int s;
    p = PAVL::findCell(getLatestVersion(),*a->b,true);
    p = copyPath(p);

    if(!p)
        return;

    if(p->left && p->right)
    {
        t = findMax(p->left);
        p->value = t->value;
        p = t;
    }

    if(p->left && (!p->right))
    {
        if(p->parent)
        {
            s = getDeleteSign(p);
            replaceChild(p->parent,p,p->left);
            p->left->parent = p->parent;
            balanceDelete(p->parent,s);
            verze.push_back(findRoot(p->parent));
            //*root = find_Root(p->parent);
            p->~Vrchol();
        }
        else
        {
            p->left->parent = 0;
            verze.push_back(findRoot(p->left));
            //*root = find_Root(p->left);
            p->~Vrchol();
        }
    }
    else if((!p->left) && p->right)
    {
        if(p->parent)
        {
            s = getDeleteSign(p);
            replaceChild(p->parent,p,p->right);
            p->right->parent = p->parent;
            balanceDelete(p->parent,s);
            verze.push_back(findRoot(p));
            //*root = find_Root(p);
            p->~Vrchol();
        }
        else
        {
            p->right->parent = 0;
            verze.push_back(findRoot(p->right));
            //*root = find_Root(p->right);
            p->~Vrchol();
        }
    }
    else
    {
        if(p->parent)
        {
            s = getDeleteSign(p);
            replaceChild(p->parent,p,0);
            balanceDelete(p->parent,s);
            verze.push_back(findRoot(p));
            //*root = find_Root(p);
            p->~Vrchol();
        }
        else
        {
            p->~Vrchol();
            verze.push_back(0);
            //*root = 0;
        }
    }
}
