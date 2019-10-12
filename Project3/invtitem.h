#ifndef INVTITEM_H
#define INVTITEM_H

#include <iostream>
#include <cassert>

using namespace std;

/**
*@author:  Wanlei Zhao
*@date:    Dec.-3-2008
*
*
* definition of structures used in inverted file, for BoW based retrieval
*
*
**/

/***************unit definition region*******************/

class InvtUnit
{
public:
    unsigned int Id;
	float tf;
} __attribute__((__packed__));  ///all the members of the struct are packed without the aligned padding

/***************block definition region*******************/

class AbstractBlock
{
public:
    unsigned int length;
    unsigned short uIdx;

    AbstractBlock(const unsigned int size)
    {
        length = size;
        uIdx = 0;
    }
};

class AbstractHead
{
public:
    unsigned int Id;
    unsigned long tnum;
    unsigned int bIdx;
};

class InvtBlock : public AbstractBlock
{

public:
    InvtBlock *next;
    InvtUnit *block;

public:
    InvtBlock(const unsigned int size):AbstractBlock(size)
    {
        if(size > 0)
        {
            block = new InvtUnit[size];
        }
        else
        {
            block = NULL;
        }
        uIdx = 0;
        next = NULL;
    }
    inline InvtUnit* operator[](const unsigned int i)
    {
        return &block[i];
    }
    ~InvtBlock()
    {
        if(block != NULL)
        {
            delete [] block;
            block = NULL;
        }
        next = NULL;
    }
};


/***************header pointer definition region*******************/

class HdrInvtBlock: public InvtBlock, public AbstractHead
{

public:
    HdrInvtBlock(unsigned int i):InvtBlock(0)
    {
        Id = i;
        next = NULL;
        tnum = length = bIdx = uIdx = 0;
    }
    HdrInvtBlock():InvtBlock(0)
    {
        Id = 0;
        next = NULL;
        tnum = length = bIdx = uIdx = 0;
    }
    //sort in ascending order
    static int comparer(const HdrInvtBlock *a,const HdrInvtBlock *b)
    {
        return (a->tnum < b->tnum);
    }

    //sort in descending order
    static int lgcomparer(const HdrInvtBlock *a,const HdrInvtBlock *b)
    {
        return (a->tnum > b->tnum);
    }
};



#endif
