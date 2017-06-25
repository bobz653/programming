#if !defined(ZHANGBO_SEGRULE_INCLUDED_)
#define ZHANGBO_SEGRULE_INCLUDED_ 

#define STRUCTLEN 100
typedef struct {
	int type;
	unsigned char retstruct[STRUCTLEN+1];
}regnode;

/*
struct regnode{
	int type;
	char *retstruct;
};
//*/

class CSegRule
{

public:
	virtual int Init(const char * filename) = 0;
	virtual int Search(unsigned short *pStart, int iLen, regnode* retnode) = 0;

}; 

extern "C" CSegRule * CreateSegRuleInstance();
extern "C" int Release(CSegRule *pRule);

#endif // !defined(AFX_NFABUILDER_H__919EC030_9416_4C97_A4B1_DB8D7DB8D9F7__INCLUDED_)

