#pragma 

#include "..\GameBase\Adapter\MainAdapter.h"
#define MAX_GameServer_SESSION_LIVE  10
/*
1.����login
2.����gateway
3.����map server
4.����db server
5.�����¼����
*/

class GameServerMgr : public MainAdapter
{
public:
	GameServerMgr(ACE_UINT32 unID);
	virtual ~GameServerMgr();
// 	//������״̬�µ��õ�timeout
// 	virtual void obj_timeout();
// 	ACE_UINT32 m_unLiveCounter;

	//virtual BaseObj *new_child_obj(ACE_UINT32 unID);

	//virtual int deal_msg(MessageInfo &msgInfo);

private:
};