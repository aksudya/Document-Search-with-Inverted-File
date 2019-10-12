#include "scriptparser.h"
#include "vstring.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <list>

using namespace std;

void ScriptParser::getParas(map<string, const char *> &paras, const char *fn)
{
    assert(fn);
    FILE *fp = fopen(fn, "r");
	if(fp == NULL)
	{
		printf("Configure '%s' cannot be open!\n", fn);
		exit(0);
	}

    string fst_str = "", sec_str = "";
    char *pairf = NULL, *pairs = NULL, result[STRLEN];
    char *strPt = NULL;

	while(!feof(fp))
	{
		strPt = fgets(result, STRLEN, fp);

		if(strPt == NULL)
		continue;

		if(!strcmp(strPt, ""))
		continue;

		if(result[0] == '#')
		continue;

		VString::trimEnd(result, '\n');
		VString::trimEnd(result, '\r');
		VString::parsePair(result, fst_str, sec_str, '=');

		pairf = new char[fst_str.length()+1];
		strcpy(pairf, fst_str.c_str());

		pairs = new char[sec_str.length()+1];
		strcpy(pairs, sec_str.c_str());

		paras.insert(pair<const char *, const char*>(pairf, pairs));

		fst_str.erase(fst_str.begin(), fst_str.end());
		sec_str.erase(sec_str.begin(), sec_str.end());
		strcpy(result, "");
	}
	fclose(fp);

	return ;
}

map<unsigned int, map<string, const char*>* > ScriptParser::getItmMaps(const char *fn)
{
    assert(fn);

    unsigned int index = 0;
    FILE *fp = fopen(fn, "r");
	if(fp == NULL)
	{
		printf("Configure '%s' cannot be open!\n", fn);
		exit(0);
	}

	char result[STRLEN];

	map<unsigned int, map<string, const char*>* > data;
	map<string, const char*> *crnt_para_map;
    string fst_str = "", sec_str = "";
    char *pairf = NULL, *pairs = NULL;
    char *strPt = NULL;

	while(!feof(fp))
	{
		strPt = fgets(result, STRLEN, fp);

		if(strPt == NULL)
		continue;

		if(!strcmp(strPt, ""))
		continue;

		VString::trimEnd(result, '\n');
        VString::trimEnd(result, '\r');

		if(!strcmp(result, "<item>"))
		{
		    crnt_para_map = new map<string, const char*>;

		    strPt = fgets(result, STRLEN, fp);

		    if(strPt == NULL)
		     continue;

            if(!strcmp(strPt, ""))
		     continue;

		    if(!strcmp(result, ""))
		    continue;

		    if(result[0] == '#')
		    continue;

		    VString::trimEnd(result, '\n');
		    VString::trimEnd(result, '\r');
		    while(strcmp(result, "</item>"))
		    {
		        VString::parsePair(result, fst_str, sec_str, '=');
		        pairf = new char[fst_str.length()+1];
		        strcpy(pairf, fst_str.c_str());

		        pairs = new char[sec_str.length()+1];
		        strcpy(pairs, sec_str.c_str());
		        crnt_para_map->insert(pair<const char*,const char*>(pairf,pairs));
		        fst_str.erase(fst_str.begin(),fst_str.end());
		        sec_str.erase(sec_str.begin(),sec_str.end());

		        strPt = fgets(result, STRLEN, fp);

		        if(strPt == NULL)
		        continue;

		        if(!strcmp(strPt, ""))
		        continue;

		        VString::trimEnd(result, '\n');
		        VString::trimEnd(result, '\r');
		    }
		    data.insert(pair<int,map<string,const char*>* >(index, crnt_para_map));
		    index++;
		}
		strcpy(result, "");
	}
	fclose(fp);
	return data;
}

void ScriptParser::getStrPairs(const char *srcfn, vector<const char*> &strlst1,
                                     vector<const char*> &strlst2)
{
    assert(srcfn);
    printf("Read string pairs ........... ");
	FILE *fp = fopen(srcfn, "r");
	if(fp == NULL)
	{
		printf("File '%s' cannot be open!\n", srcfn);
		return ;
	}

	char result[STRLEN];
	char former[STRLEN];
	char latter[STRLEN];

	char *first = NULL, *second = NULL;
	unsigned int i = 0, len1 = 0, len2 = 0, nargs = 0;
	while(!feof(fp))
	{
		nargs = fscanf(fp, "%s\n", result);

		if(nargs != 1)
		continue;

		if((!strcmp(result, "NULL"))||(!strcmp(result, "")))
		{
		      continue;
		}
		else
		{
		    VString::split_twin(former, latter, result, '^');
		    len1   = strlen(former) + 1;
		    len2   = strlen(latter) + 1;
		    first  = new char[len1];
		    second = new char[len2];

		    strcpy(first,  former);
		    strcpy(second, latter);

		    strlst1.push_back(first);
		    strlst2.push_back(second);
		    i++;
		}
		strcpy(result, "");
	}
	printf("%d\n", i);
	fclose(fp);
	return ;
}

vector<const char*> ScriptParser::getNamelst(const char *fname)
{
    assert(fname);
    vector<const char*> kflst;

	FILE *fp = fopen(fname, "r");
	if(fp == NULL)
	{
		printf("File '%s' cannot be open!\n",fname);
		return kflst;
	}

	char result[ScriptParser::STRLEN];
	char *crntKeyFrm = NULL;
	int i = 0, len = 0, nargs = 0;

	while(!feof(fp))
	{
		nargs = fscanf(fp, "%s\n", result);

		if(nargs != 1)
		continue;

		len = strlen(result) + 1;
		if((!strcmp(result, "NULL"))||(!strcmp(result, "")))
		{
		      continue;
		}
		else
		{
             crntKeyFrm = new char[len];
             strcpy(crntKeyFrm, result);
		     kflst.push_back(crntKeyFrm);
		     i++;
		}
		strcpy(result, "");
	}
	fclose(fp);
	return kflst;
}


bool ScriptParser::verifyParaMap(const char *options[], const unsigned int n,
                                map<string, const char*> &paras)
{
    unsigned int i;
    for(i = 0; i < n; i++)
    {
        if(paras.find(options[i]) == paras.end())
        {
            cout<<"Option '"<<options[i]<<"' has not been set!\n";
        }
    }
    return true;
}

void ScriptParser::clearParaMap(map<string, const char *> &paras)
{
    string key;
    const char *val;
    map<string, const char *>::iterator mit;

    for(mit = paras.begin(); mit != paras.end(); mit++)
    {
        key = mit->first;
        key.clear();
        val = mit->second;
        delete [] val;
    }
    paras.clear();
}

