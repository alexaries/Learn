#pragma once
//��־���߳�
#include "ace\ACE.h"
#include "ace\Singleton.h"
#include "ace\Recursive_Thread_Mutex.h"
#include <queue>
#include "cppconn\prepared_statement.h"
#include "..\GameBase\Protocol\DBProtocol.h"
#include "cppconn\connection.h"
#define  MAX_LIST_QUE 30
#define  MAX_LIVEL_TIME_INDEX 86400
#define  MAX_ONLINE_TIME_INDEX 300

#define  MAX_MSG_QUE_SIZE 1024   //Ĭ�Ͻ��Ͷ���
#define  LIMIT_MSG_QUE_SIZE 20480 //������

using namespace sql;

class DBLogMgr
{
public:
	DBLogMgr();
	~DBLogMgr();

	int init(void * pCall);
	void fini();

	int	 write_db_log(const LogItemInfo * msg,ACE_INT32 len);//д��
	int	 write_admin_db_log(MsgDBAdminLogInfoRequest * msg);//д��php����������
	bool insert_db_message();//����һ�����ݵ����ݿ�
	bool insert_list_to_File();
	bool read_db_message(Connection *db_conn, const MsgDBAdminLogInfoRequest &request);//��ѯ���ݵ����ݿ�
	int m_level_time_index;
	//Connection *create_db_connection();
	//void destroy_db_res();
	//bool ReadLogItems(const MsgDBAdminLogInfoRequest * request,MsgDBAdminLogInfoResponse & response);
	int m_online_time_index;
private:
	std::string IntToString(ACE_INT32 nv);
	bool Empty();
	bool Read_db_log();//��ȡһ������
	int set_name(SQLString str, char *dest, int len);
	
	std::queue<LogItemInfo> m_queueDBLog;
	std::queue<MsgDBAdminLogInfoRequest> m_queueDBadminLog;

	static ACE_THR_FUNC_RETURN do_execute(void *opParam);

	int m_nThreadGroupID;

	ACE_Recursive_Thread_Mutex m_DBLogLock;
	ObjAdr m_srcAdr;
	void * m_dbuserHandlerCallBack;
	LogItemInfo m_dealList[MAX_LIST_QUE];
	int m_len;
	ACE_UINT32 m_index;
	FILE * m_fp;
	bool OpenFile();
	void CloseFile();
	void DealLevelDistributed();//�ֲ�
	void DealOnlineDistributed();//�ֲ�
};

typedef ACE_Singleton<DBLogMgr, ACE_Thread_Mutex> DB_LOG_MGR_INSTANCE;