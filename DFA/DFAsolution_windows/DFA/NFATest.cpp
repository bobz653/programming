#include "NFABuilder.h"
#include <time.h>

#define TRAIN false

int main(int args,char**argv){
	if(args!=3){
		printf("usage:%s <netinfile> <infile> \n",argv[0]);
		printf("arguments error\n");
		getchar();
		return false;
	}

	char *sStruct=new char[100];
	int myLen=0;

	if(TRAIN ){
		CNFABuilder NFAB;

		if(!NFAB.LoadRuleFile(argv[1])){
			printf("init file error!\n EXE <infile_GBK>");
			return 0;
		}

		NFAB.CreateNFA2DFA();

		NFAB.SaveDFAtoFile("dfa.net");
		NFAB.Release();
	}else{

		CNFABuilder NFAB;
		NFAB.LoadFileToDFA("dfa.net");

		FILE *fp=fopen(argv[2],"rb");
		if(fp==NULL) return -1;
		unsigned short nBuf[20480];
		char sBuf[10240];

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

			int nCovlen=MultiByteToWideChar(CP_ACP,NULL,sBuf,nlen,(LPWSTR)nBuf,20480);

			nBuf[nCovlen]=0;


			for(int j=0;j<nCovlen;j++){
				int type=NFAB.SearchForDFA(&nBuf[j],sStruct,myLen);
				if(type!=-1){
					char myByte[1024];
					int nlen=WideCharToMultiByte(CP_ACP,NULL,(LPWSTR)&nBuf[j],myLen,myByte,1024,NULL,NULL);
					myByte[nlen]=0;

					printf("type:%d word:%s len:%d\n",type,myByte,myLen);
					nFindTotal++;
					j+=myLen-1; 
				}


			}

		}
		fclose(fp);
		time(&end);
		printf("Total number:%d\n",nFindTotal);
		printf("time spends %.5f\n",difftime(end,start));
	}



	return 1;
}
