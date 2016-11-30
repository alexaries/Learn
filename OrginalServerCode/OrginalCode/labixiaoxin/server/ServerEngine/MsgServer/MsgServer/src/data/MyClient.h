/********************************************************************
	����ʱ�䣺	2015/04/25  0:37:22
	�ļ�����	MyClient.h
	���ߣ�		лС��
	
	���ܣ�		1. ��Ҫ�߼��࣬�����ﴦ�����о�����Ϣ
				2.
				
	˵����	    1. 
				2. 
*********************************************************************/
#ifndef __MYCLIENT_H_
#define __MYCLIENT_H_

#include "..\asio\ClientSession.h"
#include "..\tools\DefineBase.h"
#include "..\json\MyJson.h"

class CMyClient:public ClientSession
{
public:
	CMyClient(boost::asio::io_service & ioservice);
	~CMyClient();
public://IGameBase �ӿ�
	//��ȡ�����ļ�
	virtual void OnRead(std::vector<MsgInfo> & ver);

	//�����¼�
	virtual bool OnSend(const boost::scoped_array<char> & str,MY_UINT32 len);

	//���ӳɹ�����
	virtual void OnConnect();
	//�Ͽ�
	virtual void DisConnect();
public://IModelBase�ӿ�
	virtual void OnInit();
	virtual bool OnTimeOut();
public://������Ϣ
	bool DealMsg(MsgInfo & Msg);//��Ϣ��������ʼ
private:
	MY_UINT32 m_loginTime;
	//CMyJson m_myJsonOp;
	void OnDestroye();//�����Լ�
	bool OnLogin(const MsgInfo & json);//��¼
	bool OnChangeName(const MsgInfo & json);//������
	bool OnChangeLevel(const MsgInfo & json);//�ĵȼ�
	bool OnSendMsg(MsgInfo & json);//������Ϣ

};

#endif // !__MYCLIENT_H_
