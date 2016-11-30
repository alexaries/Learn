#pragma once

#include "ace\ace.h"
#include "BaseModule.h"

class GoldenTouchMgr : public BaseModule
{
public:
	ACE_UINT32 m_buy_gold_limit;
public:
	GoldenTouchMgr();
	~GoldenTouchMgr();

	void init(ACE_UINT32 unUsedTouchCount);//���Ӵ�����ˢ�´������ϴ�ˢ��ʱ�䣬��Ʒ�б� 

	void Reset()
	{
		m_unUsedTouchCount = 0;
		update_2_db();
	}
	ACE_UINT32 m_unUsedTouchCount;

	void update_2_db();

	void update_GoldMax(ACE_UINT32 nMax,bool is);


};