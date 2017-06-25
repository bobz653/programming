#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fromuni.h"
#include "touni.h"
#include "SegRule.hpp"

int main(int args,char**argv){
	if(args!=3){
		printf("usage:%s <netinfile> <infile> \n",argv[0]);
		printf("arguments error\n");
		return false;
	}
	CSegRule * NFAB=CreateSegRuleInstance();
 NFAB->Init(argv[1]);
 printf("init finished\n");
 FILE *fp=fopen(argv[2],"rb");
	if(fp==NULL) return -1;
	unsigned short nBuf[20480];
	char sBuf[10240];
	
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
		
		int k=0;
		for(;k<nCovlen;k++){
			 regnode retnode;
			 int len=-1;
				retnode.retstruct[0]=0;
			 len=NFAB->Search(&nBuf[k],nCovlen-k, &retnode);
			 //int len=-1;
		   if(len!=-1){
		   	     char myByte[1024];
		   	     int nlen=uniToBytes(&nBuf[k],len,myByte,1024,"GBK");
		   	     myByte[nlen]=0;
          	 printf("word:%s len:%d type:%d ",myByte,len,retnode.type);
          	 
          	 if(retnode.retstruct[0]!=0){
          	 	printf("struct:");
          	 }
          	 for(int i=0;retnode.retstruct[i];i++){
          	 	printf("%d ",retnode.retstruct[i]);
          	 }
          	 printf("\n");
						 nFindTotal++;
				     k+=len-1; 
		   }
			 //delete retnode;
		   		   
		}
	}
	
	fclose(fp);
	
	Release(NFAB);
	time(&end);
	printf("Total number:%d\n",nFindTotal);
	printf("time spends %.5f\n",difftime(end,start));
	
	return 1;
}
