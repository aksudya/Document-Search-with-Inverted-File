#include "invtflloader.h"

#include "scriptparser.h"
#include "iodelegator.h"
#include "cleaner.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

/** load BoW structures into the memory **/
unsigned long InvtFLLoader::loadInvtFL(HdrInvtBlock *invtTable, const char *bowFn)
{
    ///cout<<"Building inverted file .......... VQ\n";
    unsigned int wId = 0, i = 0, docID = 0, tmpID = 0;
    InvtBlock *crntBlock = NULL;
    InvtUnit  *crntUnit  = NULL;

    unsigned int  trmNum = 0, nline = 0, dim = 0;
    unsigned long tltNum = 0;
    unsigned int  tf;

    ifstream *instrm = NULL;
    instrm = new ifstream(bowFn);
    if(instrm->fail())
    {
        cout<<"\nRead BoW '"<<bowFn<<"' fail!"<<endl;
        delete instrm;
    }
    tmpID  = 1;
    trmNum = 0;
    (*instrm)>>nline; (*instrm)>>dim;
    while(!instrm->eof())
    {
	    for (int i = 0; i < nline; ++i)
	    {
			docID = i;
			(*instrm) >> trmNum;
			for (int j = 0; j < trmNum; ++j)
			{
				(*instrm) >> wId;
				(*instrm) >> tf;
				invtTable[wId].tnum++;
				crntBlock = &invtTable[wId];
				crntBlock = crntBlock->next;
				/*for (int k = 0; k < invtTable[wId].bIdx; ++k)
				{
					crntBlock = crntBlock->next;
				}*/
				if(invtTable[wId].tnum>BLK_SIZE * invtTable[wId].bIdx)
				{
					InvtBlock* tmpBlock = new InvtBlock(BLK_SIZE);
					invtTable[wId].next = tmpBlock;
					tmpBlock->next = crntBlock;
					crntBlock = tmpBlock;
					/*crntBlock->next = tmpBlock;
					crntBlock = crntBlock->next;*/
					invtTable[wId].bIdx++;
				}
				crntUnit = &crntBlock->block[crntBlock->uIdx];
				crntUnit->Id = docID;
				crntUnit->tf = tf;
				crntBlock->uIdx++;
				tltNum++;
			}
	    }
    }
    instrm->close();
    delete instrm;
    cout<<endl;
    cout<<"\nTotal num. of features ........... "<<tltNum<<endl;
    return docID;
}


void InvtFLLoader::test()
{
    ///do unit test with your code in this class
}

