#pragma once
//��־���߳�
#include "ace\ACE.h"
#include "ace\Singleton.h"
#include "ace\Recursive_Thread_Mutex.h"
#include <queue>
#include "cppconn\prepared_statement.h"
#include "..\GameBase\Protocol\DBProtocol.h"
#include "DBUserHandler.h"

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
	bool insert_db_message(Connection *db_conn, const LogItemInfo &msgInfo);//����һ�����ݵ����ݿ�
	bool read_db_message(Connection *db_conn, const MsgDBAdminLogInfoRequest &request);//��ѯ���ݵ����ݿ�
	//void destroy_db_res();
	//bool ReadLogItems(const MsgDBAdminLogInfoRequest * request,MsgDBAdminLogInfoResponse & response);
private:
	std::string IntToString(ACE_INT32 nv);
	bool Read_db_log(LogItemInfo & msg);//��ȡһ������
	bool Read_db_log(MsgDBAdminLogInfoRequest & msg);//��ȡһ������
	int set_name(SQLString str, char *dest, int len);
	PreparedStatement *m_prep_stmt;
	Connection *m_con;

	std::queue<LogItemInfo> m_queueDBLog;
	std::queue<MsgDBAdminLogInfoRequest> m_queueDBadminLog;

	static ACE_THR_FUNC_RETURN do_execute(void *opParam);

	int m_nThreadGroupID;

	ACE_Recursive_Thread_Mutex m_DBLogLock;
	ObjAdr m_srcAdr;
	void * m_dbuserHandlerCallBack;
};

typedef ACE_Singleton<DBLogMgr, ACE_Thread_Mutex> DB_LOG_MGR_INSTANCE;