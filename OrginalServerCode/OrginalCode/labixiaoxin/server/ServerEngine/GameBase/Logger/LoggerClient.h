#pragma once

/*
1.�������е�sub adapter(ע�ᣬע��)
2.����sub adater�µ�obj��Ӻ�ɾ����Ϣ
*/
#include "ace\ace.h"
#include "ace\Singleton.h"

class LoggerClient
{
public:
	LoggerClient();
	~LoggerClient();

	void init();

	void log(char *fmtStr, ...);



private:
	ACE_UINT32 m_unGameServerID;

	void send_2_server(char *str);

};

typedef ACE_Singleton<LoggerClient, ACE_Thread_Mutex> LOGGER_CLIENT_INSTANCE;