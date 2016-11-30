#pragma once

#include "..\GameBase\Common\GameService.h"

/*
�����������ͼ��ά��ÿһ����ͼ�ļҵ����ݣ������Ƿ�����ռ�죬���ռ��Ļ�������һЩ������Ϣ

*/

class MapService : public GameService
{
public:
	MapService();
	~MapService();

	virtual int obj_init(void *arg = NULL, ACE_UINT32 unArgLen = 0);

	virtual int get_service_info_msg(char *buff, int nMaxSize);

	virtual int start_running();
	virtual int deal_msg(MessageInfo &msgInfo);
	virtual void obj_timeout();

private:
	void get_character_id_list();
	void do_character_db_update(ACE_UINT32 unCount, ACE_UINT32 *list);

	
	ACE_UINT32 m_unLastUpdateID;
	ACE_UINT32 m_unCurrentUpdateID;
	bool m_bUpdateDone;
	ACE_UINT32 m_unUserInfoServerID;
	
};