#pragma once

#include "ace\ace.h"
#include "BaseModule.h"

class HandbookMgr : public BaseModule
{
public:
	HandbookMgr();
	~HandbookMgr();

	void init(ACE_UINT32 unCount, BaseItemInfo *list);

	void add(ACE_UINT32 unID);//��ǰӵ����Ʒֻ���и��б�����Ӧ�ý�������MaterialMgr��m_list���У��˴����������⴦����	

	void del(ACE_UINT32 unID);//added jinpan for del

	bool has_handbook(ACE_UINT32 unID);

	virtual void send_info(ObjAdr &targetAdr, ACE_UINT32 unSN);

	BaseItemInfo m_list[MAX_HANDBOOK_COUNT];


	ACE_UINT32 m_unCount;

private:
	void update_2_db();
};