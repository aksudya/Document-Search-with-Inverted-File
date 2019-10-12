#include "iodelegator.h"

#include "scriptparser.h"
#include "cleaner.h"
#include "vstring.h"

#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <map>

using namespace std;

float *IODelegator::loadMatrix(const char *fn, unsigned int &row, unsigned int &col)
{
    assert(fn);
    float vals[2];

    ifstream *in_strm = new ifstream();
    in_strm->open(fn, ios::in);
    if(in_strm->fail())
    {
        cout<<"Fail to read "<<fn<<endl;
        delete in_strm;
        exit(0);
    }

    (*in_strm)>>vals[0];
    (*in_strm)>>vals[1];
    row = (unsigned int)round(vals[0]);
    col = (unsigned int)round(vals[1]);
    float *mat = new float[row*col];
    unsigned int irow, idim, loc;

    for(irow = 0; irow < row; irow++)
    {
        loc = irow*col;
        for(idim = 0; idim < col; idim++)
        {
            (*in_strm) >>mat[loc + idim];
        }
    }
    in_strm->close();
    delete in_strm;
    return mat;
}

float *IODelegator::loadMatrix_in_Row(const char *fn, unsigned int &row, unsigned int &col)
{
    assert(fn);
    float vals[2];

    ifstream *in_strm = new ifstream();
    in_strm->open(fn, ios::in);
    if(in_strm->fail())
    {
        cout<<"Fail to read "<<fn<<endl;
        delete in_strm;
        exit(0);
    }

    (*in_strm)>>vals[0];
    (*in_strm)>>vals[1];
    row = (unsigned int)round(vals[0]);
    col = (unsigned int)round(vals[1]);
    float *mat = new float[row*col];
    unsigned int irow, idim, loc;

    for(irow = 0; irow < row; irow++)
    {
        loc = irow*col;
        for(idim = 0; idim < col; idim++)
        {
            (*in_strm) >>mat[loc + idim];
        }
    }
    in_strm->close();
    delete in_strm;
    return mat;
}

float *IODelegator::loadMatrix_in_Col(const char *fn, unsigned int &row, unsigned int &col)
{
    assert(fn);
    float vals[2];

    ifstream *in_strm = new ifstream();
    in_strm->open(fn, ios::in);
    if(in_strm->fail())
    {
        cout<<"Fail to read "<<fn<<endl;
        delete in_strm;
        exit(0);
    }

    (*in_strm)>>vals[0];
    (*in_strm)>>vals[1];
    row = (unsigned int)round(vals[0]);
    col = (unsigned int)round(vals[1]);
    float *mat = new float[row*col];
    unsigned int irow, icol, loc;

    for(irow = 0; irow < row; irow++)
    {
       for(icol = 0; icol < col; icol++)
       {
            loc = row*icol;
            (*in_strm) >>mat[loc + irow];
            ///cout<<icol<<" "<<irow<<" "<<mat[loc + irow]<<"\n";
        }
        ///exit(0);
    }
    in_strm->close();
    delete in_strm;
    return mat;
}


void IODelegator::loadMultiMap(const char *srcfn, vector<vector<unsigned int> > &sup_map)
{
    unsigned int val, size, numb, j = 0;
    ifstream *inStrm = new ifstream(srcfn);

    (*inStrm)>>numb;
    for(j = 0; j < numb; j++)
    {
        vector<unsigned int> row;
        sup_map.push_back(row);
    }

    while(!inStrm->eof())
    {
        (*inStrm)>>size;
        (*inStrm)>>j;
        vector<unsigned> &crntVect = sup_map[j];

        for(unsigned int i = 0; i < size; i++)
        {
            (*inStrm)>>val;
            crntVect.push_back(val);
        }
    }
    inStrm->close();

    return ;
}

bool IODelegator::loadWghs(const char *weightfn, map<unsigned int,float> &i2wMap,
                           const unsigned int offset)
{
    assert(weightfn);
    FILE *fp = fopen(weightfn,"r");
    if(fp== NULL)
    {
        cout<<"Open file "<<weightfn<<" error!\n";
        exit(1);
    }

    char oneline[LONGCHAR];
    unsigned int docID;
    int num = 0;
    float weight = 0;
    ///cout<<weightfn<<endl;
    char *strPt = NULL;

    while(!feof(fp))
    {
        strPt = fgets(oneline, LONGCHAR, fp);

        if(strPt == NULL)
        continue;

        if(!strcmp(strPt, ""))
        continue;

        if(strcmp(oneline, ""))
        {
            sscanf(oneline,"%d %f %d",&docID, &weight, &num);
            docID = docID + offset;
            i2wMap.insert(pair<unsigned int, float>(docID, weight));
            strcpy(oneline, "");
        }
    }

    fclose(fp);

    return true;
}

bool IODelegator::loadWghs(const char *weightfn, map<unsigned int,float> &i2wMap, map<unsigned int, unsigned int> &pmap,
                           map<unsigned int, unsigned int>  &_pmap, map<unsigned int, int>  &tmap, const unsigned int kf_offset)
{
    assert(weightfn);
    FILE *fp = fopen(weightfn,"r");
    if(fp== NULL)
    {
        cout<<"Open file "<<weightfn<<" error!\n";
        exit(1);
    }

    ///cout<<"i am here\n";

    char oneline[LONGCHAR];
    unsigned int docID, crnt_vid, pre_vid, tm_code;
    float weight;
    pre_vid = 0;
    unsigned vid_offset =  pmap.size();
    char *strPt = NULL;

    while(!feof(fp))
    {
        strPt = fgets(oneline, LONGCHAR, fp);

        if(strPt == NULL)
        continue;

        if(!strcmp(strPt, ""))
        continue;

        if(strcmp(oneline, ""))
        {
            sscanf(oneline,"%d %d %d %f",&docID, &crnt_vid, &tm_code, &weight);
            docID = docID + kf_offset;
            _pmap.insert(pair<unsigned int,unsigned int>(docID, crnt_vid+vid_offset));
            if(crnt_vid != pre_vid)
            {
                pmap.insert(pair<unsigned int,unsigned int>(crnt_vid+vid_offset, docID));
            }
            i2wMap.insert(pair<unsigned int,float>(docID,weight));
            tmap.insert(pair<unsigned int,int>(docID,tm_code));
            strcpy(oneline,"");
            pre_vid = crnt_vid;
        }
    }

    fclose(fp);

    return true;
}

void IODelegator::load_k2imap(const char *srcfn, map<string, unsigned int> &referTab)
{
    FILE *fp = fopen(srcfn, "r");

    if(fp == NULL)
        return ;

    char key[256];
    char *crnt_key = NULL;
    int klen = 0, label = 0, nargc = 0;

    while(!feof(fp))
    {
        nargc = fscanf(fp,"%s %d",key, &label);

        if(nargc != 2)
         continue;

        klen = strlen(key)+1;
        crnt_key = new char[klen];

        strcpy(crnt_key,key);

        referTab.insert(pair<const char*, unsigned int>(crnt_key, label));
    }
    fclose(fp);
    return ;
}

void IODelegator::load_k2imap(const char *srcfn, map<string, int> &referTab)
{
    FILE *fp = fopen(srcfn, "r");

    if(fp == NULL)
        return ;

    char key[256];
    char *crnt_key;
    int klen, label, nargc;

    while(!feof(fp))
    {
        nargc = fscanf(fp,"%s %d", key, &label);

        if(nargc != 2)
        continue;

        klen = strlen(key)+1;
        crnt_key = new char[klen];

        strcpy(crnt_key,key);

        referTab.insert(pair<const char*, int>(crnt_key, label));
    }
    fclose(fp);

    return ;
}

void IODelegator::load_i2kmap(const char *nmapfn, map<unsigned int, const char*> &i2kMap,
                              const unsigned int offset)
{
    assert(nmapfn);

    FILE *fp = fopen(nmapfn, "r");

    if(fp == NULL)
    {
        cout<<"Open file '"<<nmapfn<<"' error!\n";
        exit(1);
    }

    char oneline[LONGCHAR];
    char fname[FNLEN];

    unsigned int docID;
    char *name = NULL, *strPt = NULL;

    while(!feof(fp))
    {
        strPt = fgets(oneline, LONGCHAR, fp);

        if(strPt == NULL)
        continue;

        if(!strcmp(strPt, ""))
        continue;

        if(!strcmp(oneline, "\n")||!strcmp(oneline, ""))
            continue;

        if(oneline[0] == '#')
            continue;

        sscanf(oneline,"%d %s\n",&docID, fname);

        name = new char[strlen(fname)+1];
        strcpy(name, fname);
        name[strlen(fname)] = '\0';
        docID = docID +  offset;

        i2kMap.insert(pair<unsigned int, const char*>(docID, name));
        strcpy(oneline, "");
    }
    fclose(fp);
    return ;
}

vector<const char*> IODelegator::loadNameList(const char *kflstfn, const char *msg)
{
    assert(kflstfn);
    FILE *fp = fopen(kflstfn,"r");

    vector<const char*> namelst;

    if(fp == NULL)
    {
        cout<<"Open file "<<kflstfn<<" error!\n";
        exit(1);
    }

    cout<<msg;

    char oneline[LONGCHAR];
    char fname[FNLEN];

    unsigned int docID = 0, i = 0;
    char *name, *strPt = NULL;

    while(!feof(fp))
    {
        strPt = fgets(oneline, LONGCHAR, fp);

        if(strPt == NULL)
        continue;

        if(!strcmp(strPt, ""))
        continue;

        if(!strcmp(oneline, "\n")||!strcmp(oneline, ""))
        {
            continue;
        }

        if(oneline[0] != '#' || oneline[0] != '\n')
        {
            sscanf(oneline,"%d %s\n",&docID,fname);
            name = new char[strlen(fname)+1];
            strcpy(name,fname);

            name[strlen(fname)] = '\0';
            namelst.push_back(name);
            i++;
        }
        strcpy(oneline,"");
    }
    fclose(fp);
    cout<<i<<endl;

    return namelst;
}

void IODelegator::load_IDF(const char *idfn, unordered_map<unsigned int, float> &idf)
{
    assert(idfn);
    int idx = -1;
    unsigned int wId;
    float wgh = 0;
    ifstream *inStrm = new ifstream(idfn, ios::in);

    if(!inStrm->is_open())
    {
        cout<<"File '"<<idfn<<"' cannot open for read!\n";
        return ;
    }

    while(!inStrm->eof())
    {
        (*inStrm)>>idx;
        (*inStrm)>>wgh;

        if(idx >= 0)
        {
            wId = idx;
            idf.insert(pair<unsigned int, float>(wId, wgh));

        }
        idx = -1;
    }
    inStrm->close();
    return ;
}

vector<const char*> IODelegator::loadStrings(const char *kflstfn)
{
    assert(kflstfn);
    FILE *fp = fopen(kflstfn,"r");

    vector<const char*> namelst;

    if(fp == NULL)
    {
        cout<<"Open file "<<kflstfn<<" error!\n";
        exit(1);
    }

    cout<<"Loading Name List ......... ";

    char oneline[LONGCHAR];
    char fname[FNLEN];

    char *name = NULL, *strPt = NULL;
    int i = 0;

    while(!feof(fp))
    {
        strPt = fgets(oneline, LONGCHAR, fp);

        if(strPt == NULL)
        continue;

        if(!strcmp(strPt, ""))
        continue;

        if(!strcmp(oneline, "\n")||!strcmp(oneline, ""))
        {
            continue;
        }

        if(oneline[0] != '#' || oneline[0] != '\n')
        {
            sscanf(oneline,"%s\n",fname);
            name = new char[strlen(fname)+1];
            strcpy(name,fname);

            name[strlen(fname)] = '\0';
            namelst.push_back(name);
            i++;
        }
        strcpy(oneline,"");
    }
    fclose(fp);
    cout<<i<<endl;
    return namelst;
}

void IODelegator::load_map(const char *srcfn, map<string,unsigned char> &referTab)
{
    FILE *fp = fopen(srcfn, "r");
    if(fp == NULL)
        return ;

    char key[256];
    char *crnt_key = NULL;
    int  klen = 0, nargc;

    while(!feof(fp))
    {
        nargc = fscanf(fp,"%s", key);

        if(nargc != 1)
        continue;

        klen = strlen(key)+1;
        crnt_key = new char[klen];

        strcpy(crnt_key,key);

        referTab.insert(make_pair(crnt_key,1));
    }
    fclose(fp);
}


void IODelegator::parseLine(vector<char> &oneline,vector<float> &vals,const char splitter)
{
    int size = 0;
    char word[30], ch =' ';
    float val;

    vector<char>::iterator it;
    for(it = oneline.begin(); it != oneline.end(); it++)
    {
        ch = *it;
        if(ch != splitter)
        {
            word[size] = ch;
            size++;
        }
        else
        {
            word[size] = '\0';
            val = atof(word);
            vals.push_back(val);
            size = 0;
        }
    }

    if(size > 0)
    {
        word[size]='\0';
        val=atof(word);
        vals.push_back(val);
    }
}

void IODelegator::parse(const char *oneline, list<float> &vals, const char splitter)
{
    assert(oneline);
    int len = strlen(oneline);
    int size = 0;
    char word[30];
    int i;
    float val;

    vector<char>::iterator it;
    for(i = 0; i < len; i++)
    {
        if(oneline[i] != splitter && oneline[i] != '\n')
        {
            word[size] = oneline[i];
            size++;
        }
        else
        {
            word[size] = '\0';
            if(size > 0)
            {
                val = atof(word);
                vals.push_back(val);
                size = 0;
            }
        }
    }

    if(size > 0)
    {
        word[size]='\0';
        val=atof(word);
        vals.push_back(val);
    }

    return ;
}

void IODelegator::parse(const char *oneline, list<unsigned int> &vals, const char splitter)
{
    assert(oneline);
    int len = strlen(oneline);
    int size = 0;
    char word[30];
    int i;
    unsigned int val;

    vector<char>::iterator it;
    for(i = 0; i < len; i++)
    {
        if(oneline[i] != splitter && oneline[i] != '\n' && oneline[i] != ',')
        {
            word[size] = oneline[i];
            size++;
        }
        else
        {
            word[size] = '\0';
            if(size > 0)
            {
                val = (unsigned int)atoi(word);
                vals.push_back(val);
                size = 0;
            }
        }
    }

    if(size > 0)
    {
        word[size]='\0';
        val=(unsigned int)atoi(word);
        vals.push_back(val);
    }

    return ;
}

void IODelegator::parseLine(vector<char> &oneline, vector<float> &vals, const char splitters[])
{
    char word[30], ch;
    float val;
    unsigned int i, size = 0;
    unsigned int len = strlen(splitters);

    vector<char>::iterator it;
    bool SPLIT = false;
    for(it = oneline.begin(); it != oneline.end(); it++)
    {
        ch = *it;
        SPLIT = false;
        for(i = 0; i < len; i++)
        {
            if(ch == splitters[i])
            {
                if(size > 0)
                {
                    word[size] = '\0';
                    val = atof(word);
                    vals.push_back(val);
                    size = 0;
                }
                SPLIT = true;
                break;
            }
        }

        if(!SPLIT)
        {
            word[size] = ch;
            size++;
        }
    }

    if(size > 0)
    {
        word[size] = '\0';
        val = atof(word);
        vals.push_back(val);
    }
    return ;
}


void IODelegator::printDocID(const unsigned int num, const char *dstFn)
{
    ofstream outStrm;
    unsigned int i = 0;
    outStrm.open(dstFn, ios::out);
    for(i = 0; i < num; i++)
    {
        outStrm<<i<<"\t"<<"doc_"<<i+1<<endl;
    }
    outStrm.close();
}

void IODelegator::test()
{
    const char *dstFn = "/home/wlzhao/lectures/mir/project2/dataset/labels.txt";
    IODelegator::printDocID(9557, dstFn);
}
