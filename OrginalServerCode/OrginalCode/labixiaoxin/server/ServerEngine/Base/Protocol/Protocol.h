#pragma once

#include "ace\ACE.h"
#include "ace\Message_Block.h"

/*
#define MESSAGE_TYPE_TASK 0x01
#define MESSAGE_TYPE_KERNAL 0x02
#define MESSAGE_TYPE_TIMER 0x03
#define MESSAGE_TYPE_USER 0x04
*/

extern char public_send_buff[81920];

//#define MAX_MESSAGE_BODY_SIZE 4096

enum
{
	SERVICE_MESSAGE_ACT_READ = 0,
	SERVICE_MESSAGE_ACT_WRITE
};

enum
{
	SERVICE_MESSAGE_TYPE_CTRL = 0,
	SERVICE_MESSAGE_TYPE_TASK,//����
	SERVICE_MESSAGE_TYPE_TIMER,//��ʱ��
	SERVICE_MESSAGE_TYPE_OBJ_LOCATION,//
	SERVICE_MESSAGE_TYPE_DB_REQUEST,//db ����
	SERVICE_MESSAGE_TYPE_DB_RESPONSE,//db server ����
	SERVICE_MESSAGE_TYPE_DB_REQUEST_ACK,//db server Ӧ��ȷ����Ϣ
	SERVICE_MESSAGE_TYPE_SYSTEM,//ϵͳ��Ϣ
	SERVICE_MESSAGE_TYPE_USER,//UserInfoMgr
};

#define SERVICE_MESSAGE_CTRL_NONE 0x00000000
#define SERVICE_MESSAGE_CTRL_NEED_QUERY_LOCATION 0x00000001
#define SERVICE_MESSAGE_CTRL_TO_INTERNET 0x40000000
#define SERVICE_MESSAGE_CTRL_FROM_INTERNET 0x80000000

#define MAKE_MSG_TYPE(g, t) ((g << 16) | t)

typedef struct tagObjAdr
{
	ACE_UINT32 unServiceID;
	ACE_UINT32 unAdapterID;
	ACE_UINT32 unObjID;
	ACE_UINT32 unTaskID;
}ObjAdr;

typedef struct tagPacketHead
{
	ACE_UINT32 unType;
	//ACE_UINT32 unSessionID;
	ACE_UINT32 unCtrlCode;
	//ACE_UINT32 unSN;
}PacketHead;

typedef struct tagServiceHead
{
	ObjAdr desAdr;
	ObjAdr srcAdr;
}ServiceHead;

/*
typedef struct tagClientHead
{
	ACE_UINT32 unMsgCount;
}ClientHead;
*/

typedef struct tagMsgInfoHead
{
	ACE_UINT32 unSig;
	ServiceHead s_head;
}MsgInfoHead;;


typedef struct tagMessageHead
{
	ACE_UINT32 unType;
	ACE_UINT32 unPacketNO;
}MessageHead;

typedef struct tagMessageInfo
{
	ACE_Message_Block *msg;
	PacketHead *p_head;
	MsgInfoHead *i_head;
	ServiceHead *s_head;
	//ClientHead *c_head;
	MessageHead *m_head;
	void *body;
	int nBodySize;
}MessageInfo;

extern int allocMessageWithInfo(MessageInfo &info, ACE_UINT32 unBodySize, ACE_UINT32 unType, ACE_UINT32 unDesServiceID, 
								ACE_UINT32 unDesAdapterID, ACE_UINT32 unDesObjID, ACE_UINT32 unSrcServiceID, ACE_UINT32 unSrcAdapterID, ACE_UINT32 unSrcObjID, ACE_UINT32 unMsgType);
extern int parseMsg(MessageInfo &info, ACE_Message_Block *msg);
extern ACE_Message_Block *allocServiceMessage(int nAct, ACE_UINT32 unBodySize = 4096);
extern void resetServiceMessage(ACE_Message_Block &msg, int nAct);

typedef struct tagMsgCommonRequest
{
	ACE_UINT32 unType;
	ACE_UINT32 unSize;
	char data[1];
}MsgCommonRequest;

typedef struct tagMsgCommonResponse
{
	ACE_UINT32 unType;
	ACE_UINT32 unSize;
	ACE_INT32 nRet;
	char data[1];
}MsgCommonResponse;

typedef struct tagMsgDBBaseResponse
{
	int nResult;
}MsgDBBaseResponse;

#define PACKET_HEAD_SIZE (sizeof(PacketHead) + sizeof(MsgInfoHead) + sizeof(MessageHead))

//������Ϣ
#define ZeroVar(p) ZeroMemory( &p, sizeof( p ) )
#define RECHARGE_ORDERID_LEN 32 //��ֵorderid����
//#define  RECHARGE_STATE_TRUE 2 //����ȡ״̬
#define  RECHARGE_STATE_FLAG 1 //����ȡ״̬
#define  RECHARGE_STATE_FALSE 0//����״̬

#define  RECHARGE_ORDER_SUCCESS 0 //�ɹ�
#define  RECHARGE_ORDER_ERROR -1 //ʧ��
#define  RECHARGE_ORDER_NOT_EXIST -2//�����Ų�����
#define  RECHARGE_ORDER_COMPLETED -3//����ȡ
#define  RECHARGE_ORDER_USER_ID_ERROR -4//��ɫid��ƥ��
#define  RECHARGE_ORDER_PayState_ERROR -5//֧��״̬����1
#define  RECHARGE_ORDER_ProductId_ERROR -6//��Ʒid��һ��
#define  RECHARGE_ORDER_ProductCount_ERROR -7//��Ʒ������һ��
#define  RECHARGE_ORDER_Product_NOT_EXIST -8//��Ʒ������
#define  RECHARGE_ORDER_Renminbi_ERROR -9//��ֵ����ұ�ʵ����Ҫ����Ʒ�����
#define  RECHARGE_ORDER_ZONE_ERROR -10//���Ų���

#define  RECHARGE_CHANNEL_LEN 16

typedef struct tagOrderInfo
{
	tagOrderInfo()
	{
		ZeroVar(*this);
	}
	char orderID[RECHARGE_ORDERID_LEN+1];//��ֵͷ
	ACE_UINT32  unCharacterID;//��ֵ�û�id
	ACE_UINT32 creat_time;//��������ʱ��
	ACE_INT32	unProductId;//��Ʒid
	ACE_UINT32 unProductCount;//������Ʒ����
	ACE_INT32  state;//��ǰ״̬���Ƿ��Ѿ���ȡ��
	ACE_UINT32  unMoney;//��ֵ������
	ACE_UINT32  unFlagEx;//�͵�Ǯ
	ACE_UINT32 unRenminbi;
	char  strChannel[RECHARGE_CHANNEL_LEN + 1];//��ֵ����id
	ACE_UINT32 unbitfield;
}OrderInfo;

#define  RECHARGE_MAX_PRODUCT 32
typedef struct tagProductList
{
	tagProductList()
	{
		ZeroVar(*this);
	}
	ACE_UINT32 unCount;
	ACE_UINT32 list[RECHARGE_MAX_PRODUCT];
}ProductList;

typedef struct tagVipInfo
{
	ACE_UINT32 vip_level;
	ACE_UINT32 vip_amount;
}VipInfo;

typedef struct tagVipGift
{
	tagVipGift()
	{
		ZeroVar(*this);
	}
	ACE_UINT32 unCount;
	ACE_UINT32 list[RECHARGE_MAX_PRODUCT];
}VipGift;