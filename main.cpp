#include <iostream>
#include <fstream>
#include "localization.h"

using namespace std;

int main(int argc, char **argv)
{
    vector<string> params(argv, argv+argc);

    ifstream inV, _inB;
    ofstream _out;

    for(vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        if(!(*it).compare("-inV"))
        {
            ++it;
            inV.open((*it).data(),ifstream::in);
        }
        else if(!(*it).compare("-inB"))
        {
            ++it;
            _inB.open((*it).data(),ifstream::in);
        }
        else if(!(*it).compare("-out"))
        {
            ++it;
            _out.open((*it).data(),ifstream::in);
        }
    }

    Localization test;

    if(inV.is_open())
    {
        test.preprocess(inV);
        inV.close();
    }
    else
        test.preprocess(cin);

    istream& inB = _inB.is_open() ? _inB : cin;
    ostream& out = _out.is_open() ? _out : cout;

    unsigned long n = 0;
    double x = 0,y = 0;
    inB >> n;
    for(unsigned long i = 0; i < n; ++i)
    {
        inB >> x;
        inB >> y;
        out << "Bod: [ " << x << " ; " << y << " ] Polygon: 0"
            << test.findPoint(x,y) << endl;
    }
    if(_inB.is_open())
        _inB.close();
    if(_out.is_open())
        _out.close();

    return 0;
}
