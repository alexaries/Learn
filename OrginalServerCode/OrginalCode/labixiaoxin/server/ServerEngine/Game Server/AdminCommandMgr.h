#pragma 

#include "..\Base\Service\BaseAdapter.h"
#include "AdminCommandSession.h"
/*
1.����login
2.����gateway
3.����map server
4.����db server
5.�����¼����
*/

class AdminCommandMgr : public BaseAdapter
{
public:
	AdminCommandMgr(ACE_UINT32 unID);
	virtual ~AdminCommandMgr();

	virtual BaseObj *new_child_obj(ACE_UINT32 unID);

	virtual int deal_msg(MessageInfo &msgInfo);

	bool is_valid_key(const char *key);

private:
	int init_key();
	void free_key();
	bool is_same_key(const char *key, int cursor);
	void print_key(int cursor);
	char *m_key;
};