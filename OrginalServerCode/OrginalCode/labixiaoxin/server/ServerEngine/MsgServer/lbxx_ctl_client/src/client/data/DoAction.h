/********************************************************************
����ʱ�䣺    2015/07/15  10:35:16 
�ļ�����      DoAction.h
���ߣ�        xxj

����:		1.�����߼�
2.

˵��:		1. 
2. 
*********************************************************************/
#ifndef _DOACTION_H_
#define _DOACTION_H_
#include <string>

#define ZeroVar(p) ZeroMemory( &p, sizeof( p ) )

typedef struct tagBitfield
{
	int stop:1;
	int start:1;
}Bitfield;

class CDoAction
{
public:
	void OnMessage(std::string & str);
	bool OnDealMsg(int tag);
private:
	int KillProcess(const char * pszClassName,const char * pszWindowTitle); 
	int CreateNewProcess(const char * pszExeName);  
};
#endif