#include "RechargeRes.h"
#include "RewardRes.h"

RechargeRes::RechargeRes( ACE_UINT32 unType )
:BaseRes(unType)
{
	//m_price = 0;//��Ҫ֧����ʵ�ʻ���ֵ
	//m_buy_card = 0;//�𿨵Ĺ�������
	ZeroVar(m_reward);
	m_gift_card = 0;//�𿨵���������
	m_unSubType = 0;//�Ƿ�Ϊ�¿�
}

RechargeRes::~RechargeRes()
{

}

int RechargeRes::parse_xml_node( xmlNodePtr node )
{
	BaseRes::parse_xml_node(node);

// 	if(xmlStrcasecmp(node->name, BAD_CAST"price") == 0)//xxj 2015/04/23 20:57:11  ��������
// 	{
// 		m_price = ACE_OS::atoi((char *)xmlNodeGetContent(node));
// 	}
// 	if(xmlStrcasecmp(node->name, BAD_CAST"buy_card") == 0)
// 	{
// 		m_buy_card = ACE_OS::atoi((char *)xmlNodeGetContent(node));
// 	}
	if(xmlStrcasecmp(node->name, BAD_CAST"commodity") == 0)
	{
		RewardRes::parse_reward_list(m_reward, (char *)xmlNodeGetContent(node));
	}
	else if (xmlStrcasecmp(node->name, BAD_CAST"gift_card") == 0)
	{
		m_gift_card = ACE_OS::atoi((char *)xmlNodeGetContent(node));
	}
// 	else if(xmlStrcasecmp(node->name, BAD_CAST"type") == 0)//xxj 2015/04/23 20:57:11  ��������
// 	{
// 		m_type = ACE_OS::atoi((char *)xmlNodeGetContent(node));
// 	}
	return 0;
}
