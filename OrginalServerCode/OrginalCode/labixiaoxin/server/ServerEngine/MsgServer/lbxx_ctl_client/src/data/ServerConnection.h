/********************************************************************
	����ʱ�䣺	2014/08/07  10:58:17 
	�ļ�����    ServerConnection
	���ߣ�		лС��
	
	���ܣ�		1.ͨ�Žӿ���
				2.
				
	˵����	    1. �����������ӵĿͻ���������Ϣ
				2. 
*********************************************************************/
#ifndef _SERVERCONNECTION_H_
#define _SERVERCONNECTION_H_
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include "..\tools\MySingleton.h"
#include "MyClient.h"


//ͨ�Žӿ�
class CServerConnectionData:public MySingleton<CServerConnectionData>
{
public:
	typedef boost::shared_ptr<ClientSession> Session_ptr;
public:	
	boost::shared_mutex m_rw_mutex;
	std::vector<Session_ptr> m_verConnection;
	void AddOneConnection(const Session_ptr & _ptr);
	bool DeleteOneConnection(Session_ptr & _ptr);

	void SendAllMessage( MsgInfo & info );//������Ϣ
};

#endif