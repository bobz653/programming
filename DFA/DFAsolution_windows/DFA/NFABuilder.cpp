// NFABuilder.cpp: implementation of the CNFABuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "NFABuilder.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNFABuilder::CNFABuilder()
{
	m_nNo=-1;
	m_nLen=0;
	m_nDFANo=-1;
}

CNFABuilder::~CNFABuilder()
{
	
}

bool CNFABuilder::LoadRuleFile(char *sInfile)
{
	FILE *fprReg=fopen(sInfile,"rb");
	if(fprReg==NULL) {
		printf("can't open reg file:%s\n",sInfile);
		return  false;
	}
	char szBuffer[1024];
	int nCur=0,nLine=0;
	while(fgets(szBuffer,1024,fprReg)!=NULL){
		nCur=0;
		nLine++;
		int nlen=strlen(szBuffer);
		if(nlen>=1000){
			printf("line:%d too long\n",nLine);
			return  false;
		}
		//去除尾部无用的符号，空格，TAB键
		while(szBuffer[nlen-1]=='\r'||szBuffer[nlen-1]=='\n'||
			(szBuffer[nlen-1]==' '&&szBuffer[nlen-1]!='\\')||(szBuffer[nlen-1]=='\t'&&szBuffer[nlen-1]!='\\'))
			szBuffer[--nlen]=0;
		if(nlen==0) continue;
		//去除头部无用的符号，空格，TAB键
		while(szBuffer[nCur]==' '||szBuffer[nCur]=='\t') nCur++;
		//注释行
		if(szBuffer[nCur]=='/'&&szBuffer[nCur+1]=='/') continue;
		if(szBuffer[nCur]=='$'){
			//处理变量行
			int nPos=nCur+1;
			while(szBuffer[nCur]!='='&&nCur<nlen) nCur++;
			if(nCur>=nlen-1){
				printf("line:%d syntax error!missing '=' \n",nLine);
				return false;
			}
			//变量名
			char svar[1024];
			strncpy(svar,&szBuffer[nPos],nCur-nPos);
			svar[nCur-nPos]=0;
			
			//加上标识
			int nMark=svar[0]=='@'?1:0;
			string out="(";
			if(!Expandline(&szBuffer[nCur+1],nMark,out)){
				return false;
			}
			out+=")";
			m_mapQu[svar]=out;
			
		}else if(szBuffer[nCur]=='('){
			if(szBuffer[nlen-1]!=')'){
				printf("line:%d syntax error!missing ')' \n",nLine);
				return false;
			}
			//m_mapNFA
			int nType=0;
			//得到最后的终结点变量
			vector<string> svec;
			string final="";
			for(int i=nCur+1;i<nlen;i++){
				if(szBuffer[i]==' '||szBuffer[i]=='\t') continue;
				if(szBuffer[i]=='$'){
					nType=1;
					final="";
				}else if(szBuffer[i]==':'||szBuffer[i]==')'){
					if(final.length()!=0){
						svec.push_back(final);
					}
					nType=2;
					final="";
				}else if(szBuffer[i]=='|'){
					if(final.length()!=0){
						svec.push_back(final);
					}
					nType=3;
				}else{
					final+=szBuffer[i];
				}
			}
			if(svec.size()%2!=0){
				printf("line:%d syntax error!missing elements\n",nLine);
				return false;
			}
			for(int i=0;i<svec.size();i++){
				string out=m_mapQu[svec[i]];
				if(out.size()==0){
					printf("variable %s need declaration\n",svec[i].c_str());
					return false;
				}

				int nRetVal=atoi(svec[i+1].c_str());
				FSA_TABLE *queue=NULL;
				printf("C:%s\n",out.c_str());
				if(!getqueue(out.c_str(),queue,nLine))
					return false;
				//TravelNFA(queue);
				m_mapNFA[nRetVal]=queue;
				i++;
			}
			
		}else{
			printf("line:%d Position:%d syntax error!\nmissing '$' or '('\n",nLine,nCur);
			return false;
		}
	}
	fclose(fprReg);
	if(m_mapNFA.size()==0){
		printf("no command line !\n");
		return false;
	}
	return true;	
}

void CNFABuilder::BuildqueForYuan(unsigned short nword,FSA_TABLE* &newque)
{
	PSNODE node=new SNODE;
	node->nodeNo=++m_nNo;
	node->nVal=nword;
	node->bmark=-1;
	newque =new FSA_TABLE;
	newque->push_back(node);
	m_vecNode.push_back(node);
	//填充符号库
	m_mapsym[nword]=1;
}

unsigned short CNFABuilder::ToUni(const char* sc,int len)
{
	LPWSTR a;
	unsigned short wide[2];

	int len_uni = MultiByteToWideChar(CP_ACP,NULL,sc, len,(LPWSTR)&wide[0],2);
  //int len_uni = bytesToUni(sc, len, wide, 1, "GBK");
	return wide[0];
}

void CNFABuilder::BuildqueForSuo(const char *words, FSA_TABLE *&newque)
{
	if(strlen(words)!=3) return;
	newque =new FSA_TABLE;
	
	PSNODE startnode=new SNODE;
	startnode->nodeNo=++m_nNo;
	startnode->nVal=0;
	startnode->bmark=-1;

	m_vecNode.push_back(startnode);
	
	PSNODE endnode=new SNODE;
	endnode->nodeNo=++m_nNo;
	endnode->nVal=0;
	endnode->bmark=-1;
	m_vecNode.push_back(endnode);
	
	for(char i=words[0];i<=words[2];i++){
		PSNODE node=new SNODE;
		node->nodeNo=++m_nNo;
		node->nVal=ToUni(&i,1);
		node->bmark=-1;

		m_vecNode.push_back(node);
		node->veclink.push_back(endnode->nodeNo);
		startnode->veclink.push_back(m_nNo);
		newque->push_back(node);
		//填充符号库
		m_mapsym[node->nVal]=1;
	}
	newque->push_back(endnode);
	newque->push_front(startnode);
	
}
//返回true，就会操作栈，返回false,压栈
bool CNFABuilder::CountPrior(char a, char b)
{
	if(a=='}'&&b!='{')		
		return true;
	if(a==')'&&b!='(')		
		return true;
	if(a==']'&&b!='[')
		return true;
	if(a=='>'&&b!='<')
		return true;
	if(a==2&&b!=1)
		return true;
	if((a=='}'&&b=='{')||(a==']'&&b=='[')||(a=='>'&&b=='<')||(a==')'&&b=='(')||(a==2&&b==1)){
		Eval();
		return false;
	}
	if(b=='('||b=='['||b=='{'||b=='<'||b==1)
		return false;
	if(b=='.')
		return true;
	if(b=='|'&&a=='.')
		return false;
	return true;
}

bool CNFABuilder::Eval()
{
	// First pop the operator from the stack
	if(m_OpStack.size()>0)
	{
		char chOperator = m_OpStack.top();
		m_OpStack.pop();
		// Check which operator it is
		switch(chOperator)
		{
		case  1:
			return MarkBrace();
			break;
		case  '{':
			return CurlyBrace();
			break;
		case  '(':
			return BracetBrace();
			break;
		case '[':
			return SquareBrace();
			break;
		case  '<':
			return AngleBrace();
			break;
		case  '|':
			return Union();
			break;
		case  '.':
			return Concat();
			break;
		}
		return true;
	}
	
	return false;
}

bool CNFABuilder::CurlyBrace()
{
	if(m_QuStack.size()==0) return false;
	FSA_TABLE *A=m_QuStack.top();
	m_QuStack.pop();
	
	int nSize=A->size();
	int nTouId=(*A)[0]->nodeNo;
	(*A)[nSize-1]->veclink.push_back(nTouId);
	
	
	PSNODE startnode=new SNODE;
	startnode->nodeNo=++m_nNo;
	startnode->nVal=0;
	startnode->bmark=-1;

	m_vecNode.push_back(startnode);
	
	
	PSNODE endnode=new SNODE;
	endnode->nodeNo=++m_nNo;
	endnode->nVal=0;

	endnode->bmark=-1;

	m_vecNode.push_back(endnode);
	
	startnode->veclink.push_back(nTouId);
	startnode->veclink.push_back(m_nNo);
	(*A)[nSize-1]->veclink.push_back(m_nNo);
	
	A->push_back(endnode);
	A->push_front(startnode);
	
	m_QuStack.push(A);
//	printf("CurlyBrace count done\n");
	return true;
}

bool CNFABuilder::SquareBrace()
{
	
	if(m_QuStack.size()==0) return false;
	FSA_TABLE *A=m_QuStack.top();
	m_QuStack.pop();
	
	int nSize=A->size();
	int nTouId=(*A)[0]->nodeNo;
	
	PSNODE startnode=new SNODE;
	startnode->nodeNo=++m_nNo;
	startnode->nVal=0;
	startnode->bmark=-1;

	m_vecNode.push_back(startnode);
	
	PSNODE endnode=new SNODE;
	endnode->nodeNo=++m_nNo;
	endnode->nVal=0;
	endnode->bmark=-1;
	
	m_vecNode.push_back(endnode);

	startnode->veclink.push_back(nTouId);
	startnode->veclink.push_back(m_nNo);
	(*A)[nSize-1]->veclink.push_back(m_nNo);
	
	A->push_back(endnode);
	A->push_front(startnode);
	
	m_QuStack.push(A);
//	printf("Squarebrace count done\n");

	return true;
}

bool CNFABuilder::AngleBrace()
{
	if(m_QuStack.size()==0) return false;
	FSA_TABLE *A=m_QuStack.top();
	
	int nSize=A->size();
	int nTouId=(*A)[0]->nodeNo;
	(*A)[nSize-1]->veclink.push_back(nTouId);
	
//	printf("AngleBrace count done\n");
	return true;
}

bool CNFABuilder::Union()
{
	if(m_QuStack.size()<2) return false;
	FSA_TABLE *A=m_QuStack.top();
	m_QuStack.pop();
	FSA_TABLE *B=m_QuStack.top();
	m_QuStack.pop();
	

	int nSize=A->size();
//	if((*A)[0]->nVal!=0){
		PSNODE startnode=new SNODE;
		startnode->nodeNo=++m_nNo;
		startnode->nVal=0;
		startnode->bmark=-1;
		startnode->veclink.push_back((*A)[0]->nodeNo);

		m_vecNode.push_back(startnode);
		A->push_front(startnode);
//	}
	nSize=A->size();
//	if((*A)[nSize-1]->nVal!=0){
		PSNODE endnode=new SNODE;
		endnode->nodeNo=++m_nNo;
		endnode->nVal=0;
		endnode->bmark=-1;

		(*A)[nSize-1]->veclink.push_back(m_nNo);
		m_vecNode.push_back(endnode);
		A->push_back(endnode);
//	}
	nSize=A->size();
	
	int nBsize=B->size();
	int nBTou=(*B)[0]->nodeNo;
	int nBWei=(*B)[nBsize-1]->nodeNo;
	int nATou=(*A)[0]->nodeNo;
	int nAWei=(*A)[nSize-1]->nodeNo;
	
	(*A)[0]->veclink.push_back(nBTou);
	(*B)[nBsize-1]->veclink.push_back(nAWei);

	PSNODE tmpnode=(*A)[nSize-1];
	A->pop_back();
	
	B->push_back(tmpnode);

	A->insert(A->end(), B->begin(), B->end());
	
	m_QuStack.push(A);
	
//	printf("Union count done\n");
	return true;
}

bool CNFABuilder::Concat()
{
	if(m_QuStack.size()<2) return false;
	FSA_TABLE *A=m_QuStack.top();
	m_QuStack.pop();
	FSA_TABLE *B=m_QuStack.top();
	m_QuStack.pop();
	
	int nBsize=B->size();
	int nATou=(*A)[0]->nodeNo;
	(*B)[nBsize-1]->veclink.push_back(nATou);
	
	B->insert(B->end(), A->begin(), A->end());
	m_QuStack.push(B);
//	printf("Concat count done\n");
	
	return true;
}
bool CNFABuilder::MarkBrace()
{
	if(m_QuStack.size()==0) return false;
	FSA_TABLE *A=m_QuStack.top();
	m_QuStack.pop();

	AddMark(A);
	int nSize=A->size();

	PSNODE startnode=new SNODE;
	startnode->nodeNo=++m_nNo;
	startnode->nVal=0;
	startnode->bmark=0;
	
	m_vecNode.push_back(startnode);
	startnode->veclink.push_back((*A)[0]->nodeNo);
	
	PSNODE endnode=new SNODE;
	endnode->nodeNo=++m_nNo;
	endnode->nVal=0;
	endnode->bmark=0;
	
	(*A)[nSize-1]->veclink.push_back(m_nNo);
	m_vecNode.push_back(endnode);
	
	A->push_front(startnode);
	A->push_back(endnode);

	m_QuStack.push(A);
//	printf("MarkBrace count done\n");
	return true;	
}


bool CNFABuilder::getqueue(const char *szBuffer, FSA_TABLE *&queue,int nLine)
{
	//变量内容
	//连续的内容增加连接符号
	int nlen=strlen(szBuffer);
	 vector<PZSCODE> veccode;
	 int nV=0;
	 
		 for(int i=0;i<nlen;i++){
		 if(szBuffer[i]==' '||szBuffer[i]=='\t') continue;
		 //转义字符处理
		 if(szBuffer[i]=='\\'){
			 if(szBuffer[i+1]==0){
				 printf("line:%d Position:%d syntax error!\nmissing end\n",nLine,i);
				 return false;
			 }
			 if(nV==1){
				 PZSCODE val=new ZSCODE;
				 val->op='.';
				 val->ntype=1;
				 veccode.push_back(val);
			 }
			 if(nV==0) nV=1;
			 PZSCODE val=new ZSCODE;
			 FSA_TABLE *que=NULL;
			 BuildqueForYuan(ToUni(&szBuffer[i+1],1),que);
			 val->codequeue=que;
			 val->ntype=0;
			 veccode.push_back(val);
			 i++;
			 continue;
		 }
		 //缩写符号处理
		 if(i+2<nlen&&(unsigned char)szBuffer[i]<=255&&szBuffer[i+1]=='-'&&(unsigned char)szBuffer[i+2]<=255){
			 if(nV==1){
				 PZSCODE val=new ZSCODE;
				 val->op='.';
				 val->ntype=1;
				 veccode.push_back(val);
			 }
			 if(nV==0) nV=1;
			 PZSCODE val=new ZSCODE;
			 char value[1024];
			 strncpy(value,&szBuffer[i],3);
			 value[3]=0;
			 FSA_TABLE *que=NULL;
			 BuildqueForSuo(value,que);
			 val->codequeue=que;
			 val->ntype=3;
			 veccode.push_back(val);
			 i+=2;
			 continue;
		 }
		 //操作符处理
		 if(szBuffer[i]=='{'||szBuffer[i]=='}'||szBuffer[i]=='['||szBuffer[i]==']'||
			 szBuffer[i]=='<'||szBuffer[i]=='>'||szBuffer[i]=='|'||
			 szBuffer[i]=='('||szBuffer[i]==')'||
			 szBuffer[i]==1||szBuffer[i]==2){
			 if(nV==1&&(szBuffer[i]=='['||szBuffer[i]=='<'||szBuffer[i]=='{'||szBuffer[i]=='('||szBuffer[i]==1)){
				 PZSCODE val=new ZSCODE;
				 val->op='.';
				 val->ntype=1;
				 veccode.push_back(val);
			 }
			 PZSCODE val=new ZSCODE;
			 val->op=szBuffer[i];
			 val->ntype=1;
			 veccode.push_back(val);
			 if(szBuffer[i]==']'||szBuffer[i]=='>'||szBuffer[i]=='}'||szBuffer[i]==')'||szBuffer[i]==2) nV=1;
			 else
			  nV=0;
			 continue;
		 }
		 //原始中文或字符
		 if(szBuffer[i]&0x80){
			 if(nV==1){
				 PZSCODE val=new ZSCODE;
				 val->op='.';
				 val->ntype=1;
				 veccode.push_back(val);
			 }
			 if(nV==0) nV=1;
			 char value[1024];
			 strncpy(value,&szBuffer[i],2);
			 value[2]=0;
			 PZSCODE val=new ZSCODE;
			 FSA_TABLE *que=NULL;
			 BuildqueForYuan(ToUni(&szBuffer[i],2),que);
			 val->codequeue=que;
			 val->ntype=0;
			 veccode.push_back(val);
			 i++;
			 continue;
		 }else{
			 if(nV==1){
				 PZSCODE val=new ZSCODE;
				 val->op='.';
				 val->ntype=1;
				 veccode.push_back(val);
			 }
			 if(nV==0) nV=1;
			 PZSCODE val=new ZSCODE;
			 FSA_TABLE *que=NULL;
			 BuildqueForYuan(ToUni(&szBuffer[i],1),que);
			 val->codequeue=que;
			 val->ntype=0;
			 veccode.push_back(val);
			 continue;
		 }
	 }//读行结束
	 //生成自动机,开始压栈
	 for(int j=0;j<veccode.size();j++){
		 if(veccode[j]->ntype==1){
			 char c=veccode[j]->op;
			 if(m_OpStack.empty()){
				 m_OpStack.push(c);
			 }else{
				 if(c=='{'||c=='<'||c=='['||c=='('||c==1){
					 m_OpStack.push(c);
					 continue;
				 }
				 while(m_OpStack.size()!=0&&CountPrior(c, m_OpStack.top())){
					 if(!Eval()){
						 printf("Eval error\n");
						 return false;
					 }
				 }
				 if(c=='}'||c=='>'||c==']'||c==')'||c==2){
					 //m_OpStack.pop();	//错误没有控制
				 }else
					 m_OpStack.push(c);
			 }
		 }else{
			 m_QuStack.push(veccode[j]->codequeue);
		 }
		 delete veccode[j];
	 }
	 if(!m_OpStack.empty()){
		 if(!Eval()){
			 printf("Eval error\n");
			 return false;
		 }
	 }
	queue=m_QuStack.top();
	m_QuStack.pop();
	if(m_QuStack.empty()&&m_OpStack.empty()){
		int nSize=queue->size();
		//添加起始节点
		PSNODE startnode=new SNODE;
		startnode->nodeNo=++m_nNo;
		startnode->nVal=0;
		startnode->bmark=-1;
		m_vecNode.push_back(startnode);

		PSNODE finalnode=new SNODE;
		finalnode->nodeNo=++m_nNo;
		finalnode->nVal=0;
		finalnode->bmark=1;
		m_vecNode.push_back(finalnode);
		

		startnode->veclink.push_back((*queue)[0]->nodeNo);
		(*queue)[nSize-1]->veclink.push_back(m_nNo);

		queue->push_back(finalnode);
		queue->push_front(startnode);
		return true;
	}
	return false;
}

void CNFABuilder::Release()
{
	map<int ,FSA_TABLE*>::iterator iter=m_mapNFA.begin();
	while(iter!=m_mapNFA.end()){
		FSA_TABLE *deque=iter->second;
		for(int i=0;i<deque->size();i++){
			PSNODE pval=(*deque)[0];
			deque->pop_front();
			if(pval!=NULL)
				delete pval;
		}
		delete deque;
		iter++;
	}
	m_mapNFA.clear();

	for(int i=0;i<m_vecDFANode.size();i++){
		delete m_vecDFANode[i];
	}
	m_vecDFANode.clear();
}

//nMark 0代表没有标记 1代表有标记
bool CNFABuilder::Expandline(char *szBuffer, int nMark,string &sOut)
{
	if(nMark==1)
		sOut+=(char)1;
	int nlen=strlen(szBuffer);
	for(int i=0;i<nlen;i++){
		 //变量处理
		 if(szBuffer[i]=='$'){
			 int nPos=i+1;
			 while(szBuffer[nPos]!=' '&&nPos<nlen) nPos++;
			 char value[1024];
			 strncpy(value,&szBuffer[i+1],nPos-i-1);
			 value[nPos-i-1]=0;
			 if(m_mapQu[value].size()==0){
				 printf("last line Position:%d syntax error!\nundefined variable %s\n",i,value);
				 return false;
			 }
			 i=nPos;
			 sOut+=m_mapQu[value];
			 continue;
		 }
		 sOut+=szBuffer[i];
	 }//
	if(nMark==1)
		sOut+=(char)2;
	return true;
}

//返回-1代表不匹配
int CNFABuilder::Search(unsigned short *words, char *&sStruct,int &nlen)
{
	return 1;	
}
bool CNFABuilder::SearchPath(unsigned short *words,int nStatus,string path,int nrecivelen)
{
	if(m_vecNode[nStatus]->bmark==1){
		if(nrecivelen>m_nLen){
			   m_nLen=nrecivelen;
		       m_resultPath[-nrecivelen]=path;
			   m_retNo=m_curNo;
		}
	}
	int nlinknum=m_vecNode[nStatus]->veclink.size();
	for(int i=0;i<nlinknum;i++){
		    int nNum=m_vecNode[nStatus]->veclink[i];
			unsigned short val=m_vecNode[nNum]->nVal;
			if(val!=0&&words[0]==val){
				char sBuf[10];
				//itoa(nNum,sBuf,10);
				sprintf(sBuf,"%d",nNum);
				string mypath=path+" ";
				SearchPath(&words[1],nNum,mypath+sBuf,++nrecivelen);
			}else if(val==0){
				char sBuf[10];
				sprintf(sBuf,"%d",nNum);
				string mypath=path+" ";
				SearchPath(&words[0],nNum,mypath+sBuf,nrecivelen);
			}
	}
	if(m_resultPath.size()==0)
		return false;
	return true;
}

		
void CNFABuilder::TravelNFA(FSA_TABLE *queue)
{
	m_record.clear();
	int headNode=(*queue)[0]->nodeNo;
	deque<int> myque;
	myque.push_back(headNode);
	while(myque.size()!=0){	
		
		if(m_record[myque[0]]==1){
			myque.pop_front();
			continue;
		}
		PrintNodeinfo(m_vecNode[myque[0]]);
		for(int i=0;i<m_vecNode[myque[0]]->veclink.size();i++){
			myque.push_back(m_vecNode[myque[0]]->veclink[i]);
		}
		myque.pop_front();
	}
}

void CNFABuilder::PrintNodeinfo(PSNODE node)
{
	
}

void CNFABuilder::PrintDFANodeinfo(PSDFANODE node)
{
	
	
}
bool CNFABuilder::BracetBrace()
{
	return true;	
}

int CNFABuilder::EpsilonClosure(map<int,int>& map_status)
{
	deque<int> myqueue;
	map<int,int>::iterator myiter=map_status.begin();
	while(myiter!=map_status.end()){
		myqueue.push_back(myiter->first);
		myiter++;
	}
	
	while(myqueue.size()!=0){
		//printf("%d\t",myqueue[0]);
		map_status[myqueue[0]]=1;
		int size=m_vecNode[myqueue[0]]->veclink.size();
		for(int i=0;i<size;i++){
			int nNo=m_vecNode[myqueue[0]]->veclink[i];
			if(m_vecNode[nNo]->nVal==0){
				map_status[nNo]=1;
				myqueue.push_back(nNo);
			}
		}
		myqueue.pop_front();
	}	
	//printf("\n");
	return 1;
}

map<int,int> CNFABuilder::Move(map<int,int>& map_status,unsigned short sym,int &flag)

{
	map<int,int> mymap;
	map<int,int>::iterator myiter=map_status.begin();
	while(myiter!=map_status.end()){
		int size=m_vecNode[myiter->first]->veclink.size();
		for(int i=0;i<size;i++){
			int nNo=m_vecNode[myiter->first]->veclink[i];
			if(m_vecNode[nNo]->nVal==sym){
				mymap[nNo]=1;
				flag=1;//存在边
			}
		}
		myiter++;
	}
	return mymap;
}

int CNFABuilder::CreateDFANode(map<int,int>& map_status,unsigned short val)
{
  string sval="";
	int nMark=-1;
	bool biaoji=false,final=false;
	map<int,int>::iterator iter=map_status.begin();
	char retNo=0;
	while(iter!=map_status.end()){
		if(m_vecNode[iter->first]->bmark==0&&m_vecNode[iter->first]->nVal!=0){
			biaoji=true;
		}
		if(m_vecNode[iter->first]->bmark>1){
			final=true;
			retNo=m_vecNode[iter->first]->bmark;
		}
		char sBuf[100];
		sprintf(sBuf,"%d",iter->first);
		sval+=" ";
		sval+=sBuf;
		iter++;
	}
	//printf("%d\t%s\n",val,sval.c_str());
	if(m_mapStatusExist[sval]==0){
		PSDFANODE pDFANode=new SDFANODE;
		pDFANode->nodeNo=++m_nDFANo;
		pDFANode->map_status=map_status;
		pDFANode->nVal=val;
		if(biaoji&&final){
			//pDFANode->bmark=2;
			pDFANode->bmark=retNo;
		}else if(!biaoji&&final){
			//pDFANode->bmark=1;
			pDFANode->bmark=retNo;
		}else if(biaoji&&!final){
			pDFANode->bmark=0;
		}else{
			pDFANode->bmark=-1;
		}
		m_vecDFANode.push_back(pDFANode);
		m_mapStatusExist[sval]=m_nDFANo;
	    return -1;//新节点返回-1
	}
	return m_mapStatusExist[sval];//返回DFA的结点号码
}

void CNFABuilder::CreateNFA2DFA()
{
	MergeToWholeNFA();
	FSA_TABLE* queue=m_QuStack.top();
	m_QuStack.pop();
	m_mapNFA.clear();
	m_mapNFA[1]=queue;

	map<int ,FSA_TABLE*>::iterator t_iter=m_mapNFA.begin();
	while(t_iter!=m_mapNFA.end()){
		//初始化结点
		int reNo=t_iter->first;
		FSA_TABLE* val=t_iter->second;
		int headNode=(*val)[0]->nodeNo;
		m_mapDFA[reNo]=m_nDFANo+1;
		int nstartstate=headNode;
		map<int,int> mymap;	
		mymap[nstartstate]=1;
		EpsilonClosure(mymap);
		CreateDFANode(mymap,0);
		deque<int> dfaque;
		dfaque.push_back(m_nDFANo);
		while(dfaque.size()!=0){
			PSDFANODE tmp=m_vecDFANode[dfaque[0]];
			map<unsigned short,int>::iterator iter=m_mapsym.begin();

			while(iter!=m_mapsym.end()){
				int flag=0;
				map<int,int> mvmap=Move(tmp->map_status,iter->first,flag);
				if(flag==1){
					EpsilonClosure(mvmap);
					int nNewDFANo=CreateDFANode(mvmap,iter->first);
					if(nNewDFANo!=-1){
						tmp->veclink.push_back(nNewDFANo);
					}else{
						tmp->veclink.push_back(m_nDFANo);
						dfaque.push_back(m_nDFANo);
					}
				}
				iter++;
			}
			dfaque.pop_front();
		}
		t_iter++;
	}
}

void CNFABuilder::TravelDFA(int nstart)
{
	printf("/******DFA travel*****/\n");
	if(m_vecDFANode.size()==0)
		return;
	m_record.clear();
	deque<int> dfaque;
	dfaque.push_back(nstart);
	map<int,int> mapexsit;
	while(dfaque.size()!=0){
		if(m_record[dfaque[0]]==1){
			dfaque.pop_front();
			continue;
		}
		PrintDFANodeinfo(m_vecDFANode[dfaque[0]]);
		m_record[dfaque[0]]=1;
		int nsize=m_vecDFANode[dfaque[0]]->veclink.size();
		for(int i=0;i<nsize;i++){
				
			int nNo=m_vecDFANode[dfaque[0]]->veclink[i];
			dfaque.push_back(nNo);
		}
		dfaque.pop_front();
	}
}

int CNFABuilder::SearchDFA(unsigned short *words, char *&sStruct, int &nlen)
{
	return 1;
}

int CNFABuilder::searchDFAPath(unsigned short *words,int nStartnode)
{
	m_vecDFApath.clear();
	deque<int> nodeque;
	nodeque.push_back(nStartnode);
	int nCur=0;
	bool bmatch=false;
	while(nodeque.size()!=0){
		int childrennum=m_vecDFANode[nodeque[0]]->veclink.size();
		for(int i=0;i<childrennum;i++){
			int nNo=m_vecDFANode[nodeque[0]]->veclink[i];
			if(m_vecDFANode[nNo]->nVal==words[nCur]&&words[nCur]!=0){
				int mark=m_vecDFANode[nNo]->bmark;
				if(mark>=1)
					bmatch=true;
				if(mark==0||mark==2)
					mark=3;
				else
					mark=0;
				m_vecDFApath.push_back(mark);

				nodeque.push_back(nNo);
				nCur++;
				break;
			}
		}
		nodeque.pop_front();
	}
	if(bmatch)
		return nCur;
	return 0;//不匹配
}

void CNFABuilder::AddMark(FSA_TABLE *queue)
{
	m_record.clear();
	int headNode=(*queue)[0]->nodeNo;
	deque<int> myque;
	myque.push_back(headNode);
	while(myque.size()!=0){	
		if(m_record[myque[0]]==1){
			myque.pop_front();
			continue;
		}
		m_vecNode[myque[0]]->bmark=0;
		m_record[myque[0]]=1;
		for(int i=0;i<m_vecNode[myque[0]]->veclink.size();i++){
			myque.push_back(m_vecNode[myque[0]]->veclink[i]);
		}
		myque.pop_front();
	}	
}

void CNFABuilder::SaveDFAtoFile(char *psFileout)
{
	FILE *fpw=fopen(psFileout,"wb");
	if(fpw==NULL)
		return;
	/***write head nodes****/
	map<int ,int>::iterator iter=m_mapDFA.begin();
	while(iter!=m_mapDFA.end()){

		fwrite(&iter->second,sizeof(int),1,fpw);
		fwrite(&iter->first,sizeof(int),1,fpw);
		iter++;
	}
	int end=-1;
	fwrite(&end,sizeof(int),1,fpw);
	/****start to write DFA nodes****/
	for(int nCur=0;nCur<m_vecDFANode.size();nCur++){
		int num=m_vecDFANode[nCur]->nodeNo;
		fwrite(&num,sizeof(int),1,fpw);
		char mark=m_vecDFANode[nCur]->bmark;
		fwrite(&mark,sizeof(char),1,fpw);
		int childrennum=m_vecDFANode[num]->veclink.size();
		fwrite(&childrennum,sizeof(int),1,fpw);
		for(int i=0;i<childrennum;i++){
			int nNo=m_vecDFANode[num]->veclink[i];
			unsigned short nVal=m_vecDFANode[nNo]->nVal;
			fwrite(&nVal,sizeof(unsigned short),1,fpw);
			fwrite(&nNo,sizeof(int),1,fpw);
		}
	}
	fclose(fpw);
}

void CNFABuilder::LoadFileToDFA(char *psInfile)
{
	for(int i=0;i<65536;i++)
	   m_allsym[i]=0;
	
	FILE *fp=fopen(psInfile,"rb");
	//得到所有的终结点
	while(1){
		int nStartnum=0,nretNo=0;
		fread(&nStartnum,sizeof(int),1,fp);
		if(nStartnum==-1){
			break;
		}
		fread(&nretNo,sizeof(int),1,fp);
		m_vecStart.push_back(nStartnum);
		m_vecReturn.push_back(nretNo);
	}
	int nNodeNo=0;
	int egenum=0;
	while(fread(&nNodeNo,sizeof(int),1,fp)!=0){
		char mark=0;
		fread(&mark,sizeof(char),1,fp);
		int nEdge=0;
		fread(&nEdge,sizeof(int),1,fp);
		for(int i=0;i<nEdge;i++){
			int nNo=0;

			unsigned short nVal=0;
			fread(&nVal,sizeof(unsigned short),1,fp);
			fread(&nNo,sizeof(int),1,fp);
			if(m_allsym[nVal]==0)
				 m_allsym[nVal]=++egenum;
			
		}
	}
	
	/*read nodes*/
	rewind(fp);
	printf("edge:%d\n",egenum);
	while(1){
		int nStartnum=0;
		fread(&nStartnum,sizeof(int),1,fp);
		if(nStartnum==-1){
			break;
		}
		
	}
	
	while(fread(&nNodeNo,sizeof(int),1,fp)!=0){
		PSEARCHNODE pval=new SEARCHNODE;
		pval->nodeNo=nNodeNo;
		char mark=0;
		fread(&mark,sizeof(char),1,fp);
		pval->bmark=mark;
		pval->myedge=new unsigned short[egenum+1];
		
		for(int i=0;i<egenum+1;i++)
			pval->myedge[i]=0;
			
		int nEdge=0;
		fread(&nEdge,sizeof(int),1,fp);
		for(int i=0;i<nEdge;i++){
			int nNo=0;
			unsigned short nVal=0;
			fread(&nVal,sizeof(unsigned short),1,fp);
			fread(&nNo,sizeof(int),1,fp);
			pval->myedge[m_allsym[nVal]]=nNo;
			
		}
		m_vecSearchNode.push_back(pval);
	}
	
	fclose(fp);
	/*
	for(int i=0;i<m_vecStart.size();i++){
		int no=m_vecStart[i];
		for(int j=0;j<65536;j++){
			if(m_vecSearchNode[no]->edge[j]!=0)
				m_mapInitVal[j]=1;
		}
	}
	*/
	printf("size %d\n",m_vecSearchNode.size());
}

int CNFABuilder::SearchForDFA(unsigned short *words, char *&sStruct, int &nlen)
{
	int nMtlen=0;
	int rNo=-1;
	nlen=0;

		int nCur=0;
		int nMatchlen=0;
		
		int nEdgenum=0;
		/****start search******/
		
		
		nEdgenum=m_vecSearchNode[nEdgenum]->myedge[m_allsym[words[nCur++]]];
		while(nEdgenum!=0){
			if(m_vecSearchNode[nEdgenum]->bmark>1){
				nMatchlen=nCur;
				rNo=m_vecSearchNode[nEdgenum]->bmark;
			}
			nEdgenum=m_vecSearchNode[nEdgenum]->myedge[m_allsym[words[nCur++]]];
		}
		if(nMatchlen>nlen){
			nlen=nMatchlen;
		}

	return rNo;	
}

void CNFABuilder::MergeToWholeNFA()
{
	if(m_QuStack.size()!=0){ 
		printf("stack is not empty!\n");
		return ;
	}
	map<int ,FSA_TABLE*>::iterator iter=m_mapNFA.begin();
	while(iter!=m_mapNFA.end()){
		if(iter->first<5||iter->first>127){
			printf("return value invalid!\n");
			return;
		}
		FSA_TABLE * p=iter->second;
		int nSize=p->size();
		(*p)[nSize-1]->bmark=iter->first;
		m_QuStack.push(p);
		iter++;
	}
	while(m_QuStack.size()!=1){
		Union();
	}
	
}

void CNFABuilder::releaseDFA(){
	  for(int i=0;i<m_vecSearchNode.size();i++){
	  	delete[] m_vecSearchNode[i]->myedge;
	  	delete m_vecSearchNode[i];
	  }
}
