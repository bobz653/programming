#if !defined(AFX_NFABUILDER_H__919EC030_9416_4C97_A4B1_DB8D7DB8D9F7__INCLUDED_)
#define AFX_NFABUILDER_H__919EC030_9416_4C97_A4B1_DB8D7DB8D9F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning (disable:4786)
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <stack>
#include <windows.h>

using namespace std;

typedef struct s_searchnode{
	int nodeNo;//号码
	char bmark;	//-1代表原始节点 0 代表内部标记点 1 代表终结点 2代表标记和终结点
	//map<unsigned short,int> map_edge;//状态节点
	//unsigned short edge[65536];
	unsigned short  *myedge;
}SEARCHNODE,*PSEARCHNODE;


typedef struct s_dfanode{
	int nodeNo;//号码
	unsigned short nVal;//unicode值,0代表空节点
	int bmark;	//-1代表原始节点 0 代表内部标记点 1 代表终结点 2代表标记和终结点
	map<int,int> map_status;//状态节点
	vector<int> veclink;//链接的节点
}SDFANODE,*PSDFANODE;


typedef struct s_node{
	int nodeNo;//号码
	unsigned short nVal;//unicode值,0代表空节点
	int bmark;	//-1代表原始节点 0 代表内部标记点 1 代表终结点
	vector<int> veclink;//链接的节点
}SNODE,*PSNODE;

typedef deque<PSNODE> FSA_TABLE;

typedef struct s_code{
	char op;
	FSA_TABLE *codequeue;//规则形式
	int ntype;//规则类型:0代表原始 1代表操作符 2代表变量 3代表0-9a-z之类的缩写
}ZSCODE,*PZSCODE;



class CNFABuilder  
{
public:
	void releaseDFA();
	void MergeToWholeNFA();
	int SearchForDFA(unsigned short *words, char *&sStruct, int &nlen);
	void LoadFileToDFA(char *psInfile);
	void SaveDFAtoFile(char *psFileout);
	void AddMark(FSA_TABLE *queue);
	/**search DFA functions**/
	int searchDFAPath(unsigned short *words,int nStartnode);
	int SearchDFA(unsigned short *words, char *&sStruct,int &nlen);
	
	/**DFA functions**/
	void TravelDFA(int nstart);
	void CreateNFA2DFA();
	void PrintDFANodeinfo(PSDFANODE node);
	map<int,int> Move(map<int,int>& map_status,unsigned short sym,int &flag);
	int EpsilonClosure(map<int,int>& map_status);
	int CreateDFANode(map<int,int>& map_status,unsigned short val=0);

	bool BracetBrace();
	void PrintNodeinfo(PSNODE node);
	void TravelNFA(FSA_TABLE *queue);
	bool SearchPath(unsigned short *words,int nStatus,string path="",int nrecivelen=0);
	int Search(unsigned short *words,char *&sStruct,int &nlen);
	bool MarkBrace();//char(1) and char(2) 代表标记
	bool Expandline(char *sBuf,int nMark,string &sout);
	void Release();
	bool getqueue(const char *sBuf,FSA_TABLE *&queue,int nLine);
	bool Concat();
	bool Union();
	bool AngleBrace();
	bool SquareBrace();
	bool CurlyBrace();
	bool Eval();
	bool CountPrior(char a,char b);
	void BuildqueForSuo(const char *words,FSA_TABLE* &newque);
	unsigned short ToUni(const char* sc,int len);
	void BuildqueForYuan(unsigned short nword,FSA_TABLE* &newque);
	bool LoadRuleFile(char *sInfile);
	CNFABuilder();
	virtual ~CNFABuilder();
private:
	
	int m_nNo;//NFA节点编号
	map<string,string> m_mapQu;
	map<int ,FSA_TABLE*> m_mapNFA;//存放最后结果的NFA
	stack<FSA_TABLE*> m_QuStack;//存放队列堆栈
	stack<char> m_OpStack;//存放操作符堆栈
	vector<PSNODE> m_vecNode;
	
	map<int,string> m_resultPath;//存放结果
	int m_curNo;//当前处理的自动机返回值
	int m_retNo;//返回类型
	int m_nLen;//返回长度

	map<int ,int > m_record;//遍历

	int m_nDFANo;//DFA节点编号
	vector<PSDFANODE> m_vecDFANode;
	map<string,int> m_mapStatusExist;//记录已经生成的节点，数字代表节点号码


	map<int ,int> m_mapDFA;//存放最后结果的retNo DFANode
	vector<int> m_vecDFApath;
	map<unsigned short,int> m_mapsym;
	
	/*****/
	vector<PSEARCHNODE> m_vecSearchNode;//DFA 结点
	vector<int> m_vecStart;//DFA 开始结点
	vector<int> m_vecReturn;//DFA 返回值
	map<unsigned short,int> m_mapInitVal;//存放初始值
	/****/
	unsigned short m_allsym[65536];
};

#endif // !defined(AFX_NFABUILDER_H__919EC030_9416_4C97_A4B1_DB8D7DB8D9F7__INCLUDED_)
