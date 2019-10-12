/**
*
*
*@author Wanlei Zhao
*@version 1.0
*
*load and parse configuration files
***/

#ifndef SCRIPTPARSER_H
#define SCRIPTPARSER_H

#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

class ScriptParser
{
public:
    static const int STRLEN = 512;
	ScriptParser(){}

	static void getParas(map<string, const char *> &paras, const char *fn);
	static vector<const char*> getNamelst(const char *fname);
	static map<unsigned int, map<string,const char*>* > getItmMaps(const char *fn);

	static void getStrPairs(const char* srcfn, vector<const char*> &strlst1,
                            vector<const char*> &strlst2);

	static void clearParaMap(map<string, const char *> &paras);
	static bool verifyParaMap(const char *options[], const unsigned int num,
                               map<string, const char *> &paras);

	virtual ~ScriptParser(){}
	///static void test();
};

#endif
