/********************************************************************
    ����ʱ�䣺    2015/08/08  16:22:27 
    �ļ�����      GameDefine.h
    ���ߣ�        xxj
    
    ����:		1.��Ϣ����
				2.
                
    ˵��:		1. 
                2. 
*********************************************************************/
#ifndef __GAMEDEFINE_H_
#define __GAMEDEFINE_H_

#include <string>

typedef unsigned int MY_UINT32;
//"ID"
#define MSG_SEND_RESULT_ID 0 //������Ϣ

#define MSG_LOGIN_ID 1//��¼
#define MSG_CHANGE_NAME_ID 2//������
#define MSG_CHANGE_LEVEL_ID 3//������
#define MSG_SEND_FROM_MSG_ID 4//������Ϣ

#define MSG_SEND_ALL_MSG_ID 10//����Ϣ (�㲥)
#define MSG_SEND_TO_MSG_ID 11//����Ϣ (˽����Ϣ)

typedef struct tagMsgBaseHead
{
	MY_UINT32 unMsgID;
	int nResult;

	tagMsgBaseHead()
	{
		Clear();
	}
	void Clear()
	{
		unMsgID = 0;
		nResult = 0;
	}
}MsgBaseHead;

typedef struct tagUserInfo
{
	MY_UINT32 m_id;//˭˵
	MY_UINT32 m_level;
	MY_UINT32 m_vipLevel;

	std::vector<char> m_name;
	//std::string m_name;

	tagUserInfo()
	{
		Clear();		
	}
	void Clear()
	{
		m_id = 0;
		m_level = 0;
		m_vipLevel = 0;
		m_name.clear();
		//m_name.swap(std::vector<char>());
	}
}UserInfo;

typedef struct tagMsgInfo
{
	MsgBaseHead m_head;//��Ϣͷ
	UserInfo m_user;//˭˵��

	MY_UINT32 toID;//��˭˵
	std::vector<char> text;
	
	tagMsgInfo()
	{
		Clear();
	}
	void Clear()
	{
		m_head.Clear();
		m_user.Clear();
		toID = 0;
		text.clear();
	}
}MsgInfo;
//"uID"


#endif
