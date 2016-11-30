#pragma once

#include "ace\ace.h"
#include "BaseModule.h"

class WingMgr:public BaseModule
{
public:
	WingMgr();
	~WingMgr();

	WingItemInfo m_list[MAX_WING_LIST];
	ACE_UINT32 m_unCount;

	void init(ACE_UINT32 unCount, WingItemInfo *list);//���Ӵ�����ˢ�´������ϴ�ˢ��ʱ�䣬��Ʒ�б� 

	virtual void send_info(ObjAdr &targetAdr, ACE_UINT32 unSN,int result = 0);

	void update_2_db();

};