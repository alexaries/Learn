#pragma once

#include "ace\ace.h"
#include "BaseModule.h"

class SecretMgr:public BaseModule
{
public:
	SecretMgr();
	~SecretMgr();

	SecretItemInfo m_list[MAX_SECRET_LIST];  //��Ʒ��id�͸���
	ACE_UINT32 m_unCellCount;//��ǰӵ�и�����
	ACE_UINT32 m_refreshTimes;//ˢ�´���
	ACE_UINT32 m_unLastRefreshTime;//�ϴ�ˢ�µ�ʱ���

	ACE_UINT32 m_unPaidRefreshPrice; //���θ���ˢ�¼۸�
	
	void init(ACE_UINT32 unCellCount,ACE_UINT32 refreshTimes,ACE_UINT32 lastRefreshTime,SecretItemInfo *list);//���Ӵ�����ˢ�´������ϴ�ˢ��ʱ�䣬��Ʒ�б� 
	virtual void send_info(ObjAdr &targetAdr, ACE_UINT32 unSN,int result = 0);


	void update_2_db();

};