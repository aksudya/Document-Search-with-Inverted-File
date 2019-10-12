#ifndef ABSTRACTRETRIEVER_H
#define ABSTRACTRETRIEVER_H

#include "indexmeta.h"
#include "invtitem.h"

#include <unordered_map>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;


class AbstractRetriever
{
    protected:
        static const unsigned int FNLEN = 1024;
        static const float sim0;

        char tmLogFn[FNLEN], itmTabFn[FNLEN];
        bool _idf_wght_;

        map<unsigned int, map<string, const char *>* > itmMaps;
        unordered_map<unsigned int, float> idfMap;
        map<unsigned int, const char*> i2kMap;
        map<string, const char*> paras;

        unsigned int wrdNum, docNum;
        float *ranks, simThresh;
        bool _timeon_;

        unsigned int getDim(){return this->wrdNum;}
        map<string, unsigned char> fltMap;
        vector<const char*> qryImgLst;
        list<IndexMeta *> *rankList;

    public:
        bool init(const char *conf);
        bool runSrch(const char *qryFn, const char *dstFn);
        virtual int bowSrch(map<unsigned int, unsigned int> bow, const unsigned int qryId) = 0;

        bool saveSrchRank(ostream *outStrm, const char *qryID, int topk);
        bool printSrchRank(list<IndexMeta *> *rankList, vector<const char*> &qkflst);
        list<IndexMeta *>* getRankList() {return rankList;}
        const char* getDocID(const unsigned int Id) {return i2kMap[Id];}
        AbstractRetriever();
        virtual ~AbstractRetriever();
};

#endif
