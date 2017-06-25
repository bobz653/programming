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
	int nodeNo;//����
	char bmark;	//-1����ԭʼ�ڵ� 0 �����ڲ���ǵ� 1 �����ս�� 2�����Ǻ��ս��
	//map<unsigned short,int> map_edge;//״̬�ڵ�
	//unsigned short edge[65536];
	unsigned short  *myedge;
}SEARCHNODE,*PSEARCHNODE;


typedef struct s_dfanode{
	int nodeNo;//����
	unsigned short nVal;//unicodeֵ,0����սڵ�
	int bmark;	//-1����ԭʼ�ڵ� 0 �����ڲ���ǵ� 1 �����ս�� 2�����Ǻ��ս��
	map<int,int> map_status;//״̬�ڵ�
	vector<int> veclink;//���ӵĽڵ�
}SDFANODE,*PSDFANODE;


typedef struct s_node{
	int nodeNo;//����
	unsigned short nVal;//unicodeֵ,0����սڵ�
	int bmark;	//-1����ԭʼ�ڵ� 0 �����ڲ���ǵ� 1 �����ս��
	vector<int> veclink;//���ӵĽڵ�
}SNODE,*PSNODE;

typedef deque<PSNODE> FSA_TABLE;

typedef struct s_code{
	char op;
	FSA_TABLE *codequeue;//������ʽ
	int ntype;//��������:0����ԭʼ 1��������� 2������� 3����0-9a-z֮�����д
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
	bool MarkBrace();//char(1) and char(2) ������
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
	
	int m_nNo;//NFA�ڵ���
	map<string,string> m_mapQu;
	map<int ,FSA_TABLE*> m_mapNFA;//����������NFA
	stack<FSA_TABLE*> m_QuStack;//��Ŷ��ж�ջ
	stack<char> m_OpStack;//��Ų�������ջ
	vector<PSNODE> m_vecNode;
	
	map<int,string> m_resultPath;//��Ž��
	int m_curNo;//��ǰ������Զ�������ֵ
	int m_retNo;//��������
	int m_nLen;//���س���

	map<int ,int > m_record;//����

	int m_nDFANo;//DFA�ڵ���
	vector<PSDFANODE> m_vecDFANode;
	map<string,int> m_mapStatusExist;//��¼�Ѿ����ɵĽڵ㣬���ִ���ڵ����


	map<int ,int> m_mapDFA;//����������retNo DFANode
	vector<int> m_vecDFApath;
	map<unsigned short,int> m_mapsym;
	
	/*****/
	vector<PSEARCHNODE> m_vecSearchNode;//DFA ���
	vector<int> m_vecStart;//DFA ��ʼ���
	vector<int> m_vecReturn;//DFA ����ֵ
	map<unsigned short,int> m_mapInitVal;//��ų�ʼֵ
	/****/
	unsigned short m_allsym[65536];
};

#endif // !defined(AFX_NFABUILDER_H__919EC030_9416_4C97_A4B1_DB8D7DB8D9F7__INCLUDED_)
