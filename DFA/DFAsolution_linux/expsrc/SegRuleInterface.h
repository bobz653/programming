/*
* Copyright (c) 2006, �������з�����
* All rights reserved.
* 
*/


#ifndef SEGRULE_INTERFACE_H
#define SEGRULE_INTERFACE_H 

#ifdef __cplusplus
extern "C" {
#endif


//����һ��node
struct {


};

//
CSegRule* ConstructFwSegRule(const char * filename);

//
int LongForward(CSegRule* pRule, unsigned short *pStart, int iLen, node* );

//
int FreeSegRuleMem(CSegRule* pRule);


#ifdef __cplusplus
}
#endif

#endif

