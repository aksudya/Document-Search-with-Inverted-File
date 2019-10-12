#ifndef CLEANER_H
#define CLEANER_H

//#include "nnitem.h"
#include "invtitem.h"
#include "indexmeta.h"

#include <vector>
#include <list>
#include <map>

/**
In charge of memory recycling, acts as
a rubbish collector

@author:    Wan-Lei Zhao
@email:     stonescx@gmail.com
**/

using namespace std;

class Cleaner
{

private:
    static void eraseInvtList(InvtBlock    *header);

public:
    static void clearRankList(list<IndexMeta*> *rankList);
    static void clearRankList(vector<IndexMeta*> *rankList);

    static void clearMatrix(vector<vector<unsigned int> > &matrix);

    static void clearInvtList(InvtBlock    *header);

    static void clear_k2iMap(map<string, unsigned int> &refTab);
    static void clear_k2iMap(map<string, unsigned char> &refTab);
    static void clear_i2kMap(map<unsigned int, const char*> &i2kMap);

    static void releaseInvtList(HdrInvtBlock    *invertLst, const unsigned int vqNum);
    static void releaseInvtList(map<unsigned int, vector<unsigned int>* > &invtLst);
    static void freeItemMaps(map<unsigned int, map<string, const char*> *> &itmMaps);

    static void freeParaMap(map<string, const char*> &paras);
    static void freeStrVect(vector<const char*> &strVect);

};

#endif
