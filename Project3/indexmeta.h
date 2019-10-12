#ifndef INDEXMETA_H
#define INDEXMETA_H

/**
*@author:  Wanlei Zhao
*date:     Apr.-1-2014
*
*definition of structure in keeping retrieved images and the rank
*
*
**/

class IndexMeta
{
public:
    IndexMeta(unsigned int Id,float w)
    {
        this->Id = Id;
        this->dst = w;
        this->pp = 0;
    }
public:
    unsigned int Id;
    unsigned int pp;
    float dst;

    static bool LGcomparer(const IndexMeta *a,const IndexMeta *b)
    {
        return (a->dst > b->dst);
    }

    static bool LLcomparer(const IndexMeta *a,const IndexMeta *b)
    {
        return (a->dst < b->dst);
    }
};

#endif
