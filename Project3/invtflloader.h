#ifndef INVTFLLOADER_H
#define INVTFLLOADER_H

#include "invtitem.h"

#include <map>

using namespace std;

/**
*@author:  Wanlei Zhao
*date:     Apr.-1-2014
*
In charge of loading inverted file from the
disc. The inverted file structure is built on the fly.
*
**/

class InvtFLLoader
{

public:
    static const unsigned int BLK_SIZE = 500;
    static const unsigned int FNLEN    = 1024;

    static unsigned long loadInvtFL(HdrInvtBlock *invtTable, const char *bowFn);
             static void test();
};
#endif
