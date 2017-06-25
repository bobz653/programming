#include "NFABuilder.h"
#include <time.h>
int main(int args,char**argv){
	if(args!=3){
		printf("usage:%s <netinfile> <infile> \n",argv[0]);
		printf("arguments error\n");
		return false;
	}
	
    CNFABuilder NFAB;
/*	if(!NFAB.LoadRuleFile(argv[1])){
		printf("init file error!\n");
		return 0;
	}
*/	
//	NFAB.CreateNFA2DFA();

	/****DFA test*****/
	/*
	unsigned short *key=L"°Ë¾ÅÊ®11.11.1.11s";
	char *sStruct=new char[100];
	int myLen=0;
	if(NFAB.Search(key,sStruct,myLen)!=-1){
		printf("info:%s,len:%d\n",sStruct,myLen);
	}
	NFAB.SearchDFA(key,sStruct,myLen);
	printf("info:%s,len:%d\n",sStruct,myLen);
	NFAB.SaveDFAtoFile("dfa.net");
	NFAB.Release();
	NFAB.LoadFileToDFA("dfa.net");
	NFAB.SearchForDFA(key,sStruct,myLen);
	*/
	NFAB.LoadFileToDFA(argv[1]);

//	unsigned short *key=L"bob653@blcu.cn";
//	unsigned short *key=L"202.112.5.188";
	
	FILE *fp=fopen(argv[2],"rb");
	if(fp==NULL) return -1;
	unsigned short nBuf[20480];
	char sBuf[10240];
	char *sStruct=new char[100];
	int myLen=0;
	time_t start,end;
	time(&start);
	int nFindTotal=0;
	int line=0;
	while(fgets(sBuf,10240,fp)!=NULL){
		if(++line%10000==0){
			printf("line :%d\n",line);
		}
		int nlen=strlen(sBuf);
		while(sBuf[nlen-1]=='\r'||sBuf[nlen-1]=='\n')
				sBuf[--nlen]=0;
		
		int nCovlen=bytesToUni(sBuf, nlen, nBuf, 20480, "GBK");
		nBuf[nCovlen]=0;
		
		
		for(int j=0;j<nCovlen;j++){
			 int type=NFAB.SearchForDFA(&nBuf[j],sStruct,myLen);
		   if(type!=-1){
		   	     char myByte[1024];
		   	     int nlen=uniToBytes(&nBuf[j],myLen,myByte,1024,"GBK");
		   	     myByte[nlen]=0;
		   	     
          	//printf("word:%s len:%d type:%d\n",myByte,myLen,type);
          	printf("word:%s len:%d\n",myByte,myLen);
						nFindTotal++;
				     j+=myLen-1; 
		   }
			 
		   		   
		}
	
	}
	fclose(fp);
	time(&end);
	printf("Total number:%d\n",nFindTotal);
	printf("time spends %.5f\n",difftime(end,start));
	
	/*
	unsigned short *key=L"µÚ£¹Áã°Ùs";
	
		int nLen=0;
		if(NFAB.Search(key,sStruct,nLen)!=-1){
			printf("info:%s,len:%d\n",sStruct,nLen);
		}
		*/
	
	
	return 1;
}
