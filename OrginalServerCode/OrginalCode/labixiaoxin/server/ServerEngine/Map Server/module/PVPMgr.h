#pragma once
/********************************************************************
    ����ʱ�䣺    2015/08/18  15:20:51 
    �ļ�����      PVPMgr.h
    ���ߣ�        xxj
    
    ����:		1.pvp�����
				2.
                
    ˵��:		1. honour = -1��ʾ������δ�μ�
                2. 
*********************************************************************/

#include "ace\ace.h"
#include "BaseModule.h"
#include "..\..\Base\Event\EventListener.h"

class PVPMgr:public BaseModule//, public EventListener
{
public:
	PVPMgr();
	void init(PVPInfo & info);
	void update();
	bool DoFreashen();//ˢ��
	void OnDo();
	virtual void update(ACE_UINT32 unCurrentTime);

	void update_pvp_Info(PVPInfoEx & info);
	void DoDayChange();
	void GetQuitList(HLInfo *list);
public:
	void EnterPVPFight(HLInfoArray & _array);//��ʼս��
	void update_TimesMax(ACE_UINT32 nMax,bool is);
	int BuyTimes();//�����
	int EnterMap();//�����ͼ
	int AccountsMap(FightRecordInfo & info,ACE_INT32 & score);//����
	
	//���»�����Ϣ
	void update_Info_2_db(bool isToUserServer);
	void update_honour(HLInfo & info);
public:
	PVPInfo m_pvpInfo;
	HLInfoArray m_array;
	ACE_UINT32 m_scheme_arena;//�����������ȼ�

private:
	ACE_INT32 m_unRestoreInterval;
	ACE_UINT32 m_unRestoreAmount;
	ACE_INT32 m_oldHonour;
	ACE_UINT32 m_buy_pvp_limit;
	
	bool isEnter;
};