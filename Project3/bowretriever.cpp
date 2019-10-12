#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <list>

#include "scriptparser.h"
#include "invtflloader.h"
#include "bowretriever.h"
#include "iodelegator.h"
#include "indexmeta.h"
#include "vstring.h"
#include "cleaner.h"
#include "timer.h"

using namespace std;

/******definition for constant**/
const int BoWRetriever::topk0   = 50;
/****end of definition for constant**/

BoWRetriever::BoWRetriever(const char *conf)
{
    const char* taboptions[3] = {"doctab", "bowtab", "wghtab"};
    map<string, const char*> crntMap;

    cout<<"Input data type ................. BoW\n";
    unsigned int i = 0;
    this->i2wMap   = NULL;
    bool loaded    = false;

    this->init(conf);
    this->ranks = NULL;

    cout<<"VQ dim .......................... "<<this->wrdNum<<endl;

    this->i2wMap = new map<unsigned int, float>;

    ScriptParser::getParas(crntMap, itmTabFn);
    ScriptParser::verifyParaMap(taboptions, 3, crntMap);

    ///docID starts from 1 by setting offset to 1
    IODelegator::loadWghs(crntMap["wghtab"],    *i2wMap,      1);
    IODelegator::load_i2kmap(crntMap["doctab"], this->i2kMap, 1);

    this->simThresh = AbstractRetriever::sim0;
    cout<<"Distance threshold .............. cos = "<<this->simThresh<<endl;

    ///word ID starts from 1
    this->invertTable = new HdrInvtBlock[this->wrdNum+2];
    for(i = 1; i < this->wrdNum; i++)
    {
        this->invertTable[i].Id   = i;
        this->invertTable[i].next = NULL;
    }

    ///cout<<crntMap["bowtab"]<<endl;

    cout<<"Loading BoWs as Inverted File ... \n";
    loaded = InvtFLLoader::loadInvtFL(this->invertTable, crntMap["bowtab"]);
    if(!loaded)
    {
        exit(0);
    }

    ///docID starts from 1
    docNum = this->i2kMap.size() + 1;

    this->ranks    = new float[docNum];
    this->rankList = new list<IndexMeta*>;
}

int BoWRetriever::bowSrch(map<unsigned int, unsigned int> bowVect, const unsigned int qryId)
{
    multimap<unsigned int, unsigned int> ::iterator mit;
    InvtBlock *blckIter = NULL, *crntBlck = NULL;
    unsigned int wId = 0, i = 0, docId = 0;
    float qryWgh = 0, refWgh = 0, sim = 0;
    InvtUnit  *crntUnit = NULL;
    IndexMeta *crntMeta = NULL;

    memset(this->ranks, 0, sizeof(float)*docNum);
    ///filling your codes here

    rankList->sort(IndexMeta::LGcomparer);
    /**/

    return true;
}

BoWRetriever::~BoWRetriever()
{
   /**/
    cout<<"Release Inverted File ............ ";
    Cleaner::releaseInvtList(invertTable, this->wrdNum);
    cout<<"done\n";

    if(this->i2kMap.size() != 0)
    {
        Cleaner::clear_i2kMap(this->i2kMap);
    }

    if(this->i2wMap != NULL)
    {
        this->i2wMap->clear();
        delete this->i2wMap;
        this->i2wMap = NULL;
    }
    /**/

}

void BoWRetriever::test()
{
    const char *conf  = "/root/projects/Project3/etc/srch.conf";
    const char *dstFn = "/root/projects/dataset/bow_rslt1.txt";
    const char *qryFn = "/root/projects/dataset/query1.mat";

    BoWRetriever *bowsrch = new BoWRetriever(conf);
    bowsrch->runSrch(qryFn, dstFn);
    delete bowsrch;
    return ;
}
