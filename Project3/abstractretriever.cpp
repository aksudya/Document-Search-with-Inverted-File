#include "abstractretriever.h"
#include "scriptparser.h"
#include "iodelegator.h"
#include "cleaner.h"
#include "vstring.h"

#include <cstdio>

const float AbstractRetriever::sim0  = 0.00f;

AbstractRetriever::AbstractRetriever()
{
    this->ranks    = NULL;
    strcpy(itmTabFn, "");
    strcpy(tmLogFn,  "");
    this->rankList = NULL;
}

bool AbstractRetriever::init(const char *conf)
{
    const char* options[2] = {"dim", "refer"};
    unsigned int i = 0;

    cout<<"Initializing .................... ";

    ScriptParser::getParas(paras, conf);

    for(i = 0; i < 2; i++)
    {
        if(paras.find(options[i]) == paras.end())
        {
            cout<<"\nOption '"<<options[i]<<"' is missing!\n";
            exit(1);
        }
    }

    this->wrdNum = atoi(paras["dim"]);
    strcpy(itmTabFn, paras["refer"]);


    cout<<"succeed\n";
    this->_idf_wght_ = false;
    if(paras.find("idf") != paras.end())
    {
        cout<<"Weighting with IDF .............. ";
        IODelegator::load_IDF(paras["idf"], idfMap);
        cout<<idfMap.size()<<endl;
        this->_idf_wght_ = true;
    }

    return true;
}

bool AbstractRetriever::runSrch(const char *qryFn, const char *dstFn)
{
   unsigned int i = 0, wrd = 0, tf = 0, qryId = 1;
   unsigned int docNum = 0, dim = 0, nTerm = 0;
   map<unsigned int, unsigned int> oneQry;
   char qryStr[32] = "";

   ifstream inStrm;
   inStrm.open(qryFn, ios::in);
   if(!inStrm.is_open())
   {
      cout<<"File '"<<qryFn<<"' cannot open for read!\n";
      exit(0);
   }

   ofstream outStrm;
   outStrm.open(dstFn, ios::out);
   if(!outStrm.is_open())
   {
      cout<<"File '"<<dstFn<<"' cannot open for write!\n";
      exit(0);
   }

   inStrm>>docNum;
   inStrm>>dim;
   cout<<"Start BoW search ................. \n";
   while(!inStrm.eof())
   {
       inStrm>>nTerm;

       if(nTerm == 0)
       break;

       for(i = 0; i < nTerm; i++)
       {
           inStrm>>wrd;
           inStrm>>tf;
           oneQry.insert(pair<unsigned int, unsigned int>(wrd, tf));
       }
       this->bowSrch(oneQry, qryId);
       sprintf(qryStr, "qry_%d", qryId);
       this->saveSrchRank(&outStrm, qryStr, 8);
       this->rankList->clear();
       Cleaner::clearRankList(this->rankList);
       oneQry.clear();
       nTerm = 0;
       qryId++;
       cout<<"\r\r\r\t\t"<<qryId-1;
   }
   cout<<endl;
   inStrm.close();
   outStrm.close();

   return 0;
}

bool AbstractRetriever::saveSrchRank(ostream *outStrm, const char *qryID, int topk)
{
  if(outStrm == NULL)
  {
     return false;
  }

  list<IndexMeta *>::iterator lit;
  IndexMeta* crntMeta = NULL;
  int num = 0;

  for(lit = this->rankList->begin(); lit != this->rankList->end(); lit++)
  {
     crntMeta = *lit;
     if(strcmp(qryID, i2kMap[crntMeta->pp]))
     {
        (*outStrm)<<qryID<<" "<<i2kMap[crntMeta->pp]<<" "<<crntMeta->dst<<endl;
        num++;
        if(num == topk)
           break;
     }
  }

  return true;
}

AbstractRetriever::~AbstractRetriever()
{
  /**/
    if(this->rankList != NULL)
    {
        Cleaner::clearRankList(this->rankList);
        delete this->rankList;
        this->rankList = NULL;
    }

    if(this->ranks != NULL)
    {
        delete [] this->ranks;
        this->ranks = NULL;
    }

    if(this->i2kMap.size() != 0)
    {
        Cleaner::clear_i2kMap(this->i2kMap);
    }
    idfMap.clear();
    /**/
}
