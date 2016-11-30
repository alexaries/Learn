#pragma once

#include "..\GameBase\Adapter\SubAdapter.h"

#include "Map.h"

/*
�������еļ�,ͨ�����������ԭ�򣬶�̬���ؼҵ����ݣ��ҵ�������һ�����ֵ,�ڴ��еļ���Ϊcache������߷����ٶ�
*/
//#define MAX_HOME_IN_CACHE 300

class MapMgr : public SubAdapter
{
public:
	MapMgr(ACE_UINT32 unID);
	virtual ~MapMgr();

	virtual BaseObj *new_child_obj(ACE_UINT32 unID);
	virtual ACE_UINT32 get_register_service_id(){return m_unGameServerID;};
	virtual ACE_UINT32 get_register_adapter_id(){return ADAPTER_GAME_SERVER_MGR;};

	//virtual int deal_msg(MessageInfo &msgInfo);

	bool init_timer();

	void uninit_timer();

	static void DoubleDropTimerCallback(void *p, long lTimerID);
	void DoDoubleDropTimer();


	BaseTimerHandler *m_doubleDropTimerHandler;
private:
	ACE_UINT32 m_unGameServerID;
};