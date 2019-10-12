#include "convert.h"

#include <iostream>
#include <fstream>
#include <ostream>

#include <cmath>
#include <map>

using namespace std;

Convert::Convert()
{
    //ctor
}

Convert::~Convert()
{
    //dtor
}


void Convert::bow2nbow(const char *srcFn, const char *dstFn, const char *wghFn)
{
    unsigned int wrdNum = 0, nterm = 0, docNum = 0;
    unsigned int iterm = 0, oterm = 0, nTerms = 0;
    map<unsigned int, float>::iterator mit;
    map<unsigned int, float> bow;
    int idoc = -1;
    float tf = 0.0f, wgh = 0.0;

    ifstream inStrm;
    inStrm.open(srcFn, ios::in);
    ofstream dstStrm;
    dstStrm.open(dstFn, ios::out);
    ofstream wghStrm;
    wghStrm.open(wghFn, ios::out);
    if(!inStrm.is_open())
    {
        cout<<"File '"<<srcFn<<"' cannot open for read!\n";
        exit(0);
    }

    if(!dstStrm.is_open())
    {
        cout<<"File '"<<dstFn<<"' cannot open for write!\n";
        exit(0);
    }

    if(!wghStrm.is_open())
    {
        cout<<"File '"<<wghFn<<"' cannot open for write!\n";
        exit(0);
    }

    inStrm>>docNum;
    inStrm>>wrdNum;
    inStrm>>nterm;
    oterm = wrdNum + 1;

    dstStrm<<docNum<<" "<<wrdNum<<endl;

    while(!inStrm.eof())
    {
        inStrm>>iterm;

        if(iterm == 0)
            break;

        if(iterm < oterm)
        {
            if(bow.size () > 0)
            {
                idoc  = idoc + 1;
                dstStrm<<bow.size();
                wgh = 0.0f;
                for(mit = bow.begin(); mit != bow.end(); mit++)
                {
                    dstStrm<<" "<<mit->first<<" "<<mit->second;
                    wgh += mit->second*mit->second;
                }
                dstStrm<<endl;
                wgh = sqrt(wgh);
                wghStrm<<idoc<<"\t"<<wgh<<"\t"<<bow.size()<<endl;
                nTerms += bow.size();
                bow.clear();
            }
        }
        inStrm>>tf;
        bow.insert(pair<unsigned int, float>(iterm, tf));
        oterm = iterm;
        iterm = 0;
        tf = 0.0f;
        cout<<"\r\r\r\t\t"<<idoc;
    }

    if(bow.size () > 0)
    {
        idoc  = idoc + 1;
        dstStrm<<bow.size();
        wgh = 0.0f;
        for(mit = bow.begin(); mit != bow.end(); mit++)
        {
            dstStrm<<" "<<mit->first<<" "<<mit->second;
            wgh += mit->second*mit->second;
        }
        dstStrm<<endl;
        wgh = sqrt(wgh);
        wghStrm<<idoc<<"\t"<<wgh<<endl;
        nTerms += bow.size();
        bow.clear();
    }

    cout<<endl<<endl;
    cout<<nTerms<<endl;

    inStrm.close();
    dstStrm.close();
    wghStrm.close();
}

void Convert::test()
{
    const char *srcFn = "/home/wlzhao/lectures/mir/project1/dataset/new3_qry.mat";
    const char *wghFn = "/home/wlzhao/lectures/mir/project1/dataset/qry_wgh.txt";
    const char *dstFn = "/home/wlzhao/lectures/mir/project1/dataset/query1.mat";

    Convert::bow2nbow(srcFn, dstFn, wghFn);
}
