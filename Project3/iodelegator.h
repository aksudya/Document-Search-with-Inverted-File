#ifndef IODELEGATOR_H
#define IODELEGATOR_H

#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>


using namespace std;

/**
*
*@author: Wanlei Zhao
*@date:   Mar-22-2014
*
*in charge of IO operations
**/

class IODelegator
{
private:
    static const int LONGCHAR  = 2000;
    static const int FNLEN     = 1024;
    static const int precision = 3;

public:
    IODelegator();
    static float  *loadMatrix(const char *fn, unsigned int &num, unsigned int &dim);
    static float  *loadMatrix_in_Col(const char *fn, unsigned int &row, unsigned int &col);
    static float  *loadMatrix_in_Row(const char *fn, unsigned int &row, unsigned int &col);


    static bool loadWghs(const char *weightfn, map<unsigned int,float> &i2wMap, const unsigned int offset);
    static bool loadWghs(const char *weightfn, map<unsigned int,float> &i2wMap, map<unsigned int, unsigned int>  &pmap,
                         map<unsigned int, unsigned int>  &_pmap, map<unsigned int, int>  &tmap, const unsigned int offset);

    static void loadMultiMap(const char *srcfn, vector<vector<unsigned int> > &supmap);
    static void load_k2imap(const char *srcfn,  map<string, unsigned int> &referTab);
    static void load_k2imap(const char *srcfn,  map<string, int> &referTab);
    static void load_i2kmap(const char *nmapfn, map<unsigned int, const char*> &i2kMap,
                            const unsigned int offset);

    static void load_map(const char *srcfn, map<string, unsigned char> &referTab);
    static void load_IDF(const char *idfn, unordered_map<unsigned int, float> &idf);

    static vector<const char*> loadNameList(const char *kflstfn, const char *msg);
    static vector<const char*> loadStrings(const char *kflstfn);

    static void parse(const char *oneline, list<float> &vals, const char splitter);
    static void parse(const char *oneline, list<unsigned int> &vals,const char splitter);
    static void parseLine(vector<char> &oneline, vector<float> &vals, const char splitter);
    static void parseLine(vector<char> &oneline, vector<float> &vals, const char splitters[]);
    static void printDocID(const unsigned int num, const char *dstFn);
    static void test();

    ~IODelegator();
};

#endif
