#pragma 

#include "..\Base\Service\BaseAdapter.h"
/*
1.����login
2.����gateway
3.����map server
4.����db server
5.�����¼����
*/

class TestMgr : public BaseAdapter
{
public:
	TestMgr(ACE_UINT32 unID);
	virtual ~TestMgr();

	virtual BaseObj *new_child_obj(ACE_UINT32 unID);

	virtual void service_obj_timer_callback();
};