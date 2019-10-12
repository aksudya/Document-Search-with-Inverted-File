#ifndef BOWRETRIEVER_H
#define BOWRETRIEVER_H

#include "abstractretriever.h"
#include "iodelegator.h"
#include "invtitem.h"

#include <vector>
#include <list>

using namespace std;

class BoWRetriever: public AbstractRetriever
{
private:
    static const int topk0;
    HdrInvtBlock *invertTable;
    map<unsigned int, float> *i2wMap;
    float *refRanks, *qryRanks;
    unsigned int topk;

public:

    BoWRetriever(const char *conf);
    int bowSrch(map<unsigned int, unsigned int> bow, const unsigned int qryId);
    void batchRun(const char *qryFn, const char *dstfn);

public:
    static void test();

    static bool loadBoW(map<unsigned int, unsigned int> &bowVect, ifstream *bowStrm);
    ~BoWRetriever();
};
#endif
