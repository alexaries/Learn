#ifndef __RECHARGERES_H_
#define __RECHARGERES_H_
#include "BaseRes.h"

#define RechargeRes_type_general 1//��ͨ��ֵ
#define RechargeRes_type_monthcard 2//�¿�
#define RechargeRes_type_LIMITED_ACTIVITY 3//��ʱ�

class RechargeRes : public BaseRes
{
public:
	RechargeRes(ACE_UINT32 unType);
	virtual ~RechargeRes();

	virtual int parse_xml_node(xmlNodePtr node);
	
	//ACE_UINT32 m_unPrice;//��Ҫ֧����ʵ�ʻ���ֵ
	//ACE_UINT32 m_buy_card;//�𿨵Ĺ�������
	ConfigReward m_reward;
	ACE_UINT32 m_gift_card;//�𿨵���������
	//ACE_UINT32 m_is_monthcard;//�Ƿ�Ϊ�¿�
	//ACE_UINT32 m_type;//"���͡�1������ͨ��ֵ��2�����¿���3������ʱ�" m_unSubType

};

#endif