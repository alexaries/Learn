/********************************************************************
	����ʱ�䣺	2015/04/25  0:37:22
	�ļ�����	MyClient.h
	���ߣ�		лС��
	
	���ܣ�		1. ��Ҫ�߼��࣬�����ﴦ�����Ծ�����Ϣ
				2.
				
	˵����	    1. 
				2. 
*********************************************************************/
#ifndef __MYCLIENT_H_
#define __MYCLIENT_H_

#include "..\asio\ClientSession.h"

class CMyClient:public ClientSession
{
public:
	CMyClient(boost::asio::io_service & ioservice);
	~CMyClient();
public:
	//��ȡ�����ļ�
	virtual void OnRead(const std::vector<MsgInfo> & ver);

	//�����¼�
	virtual bool OnSend(const boost::scoped_array<char> & str,MY_UINT32 len);

	//���ӳɹ�����
	virtual void OnConnect();
	//�Ͽ�
	virtual void DisConnect();
private:

};

#endif // !__MYCLIENT_H_
