#pragma once

/*
1.�������е�sub adapter(ע�ᣬע��)
2.����sub adater�µ�obj��Ӻ�ɾ����Ϣ
*/

#include "..\..\Base\Service\BaseAdapter.h"

class LoggerServerMgr : public BaseAdapter
{
public:
	LoggerServerMgr(ACE_UINT32 unID);
	virtual ~LoggerServerMgr();

	virtual int deal_msg(MessageInfo &msgInfo);

	virtual int obj_init(void *arg = NULL, ACE_UINT32 unArgLen = 0);

private:

	void close_log_file();
	int open_log_file();
	void write_log(char *strLog);

	ACE_UINT32 m_unLastFileTime;

	FILE *m_fp;

	//virtual void on_create_sub_adapter_obj_done(ACE_UINT32 unObjID, int result) = 0;
	//virtual void on_destroy_sub_adapter_obj_done(ACE_UINT32 unObjID, int result) = 0;
};