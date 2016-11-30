#pragma once

#include "BaseRes.h"

#define VIP_HEAD 0x05000000

class VipRes : public BaseRes
{
public:
	VipRes(ACE_UINT32 unType);
	virtual ~VipRes();

	virtual int parse_xml_node(xmlNodePtr node);
	virtual void process_res_id_by_name();

	ACE_UINT32 m_unVipLevel;//vip�ȼ�
	ACE_UINT32 m_unRechargeRequired;//��ֵ���
	ACE_UINT32 m_unGift_id;//Vip���ID,����behavior��
	ACE_UINT32 m_gold_profit;//���ر�������(�ٷֱ�)
	ACE_UINT32 m_package_limit;//������������
	ACE_UINT32 m_friends_limit;//������������
	ACE_UINT32 m_energy_limit;//������������
	ACE_UINT32 m_buyenergy_limit;//��������������
	ACE_UINT32 m_reset_level_limit;//�ؿ����ô���
	ACE_UINT32 m_reset_dngeon_limit;//�������ô���
	ACE_UINT32 m_golden_profit;//����ֱ�������ٷֱ�
	ACE_UINT32 m_buy_gold_limit;//���ǹ������
	ACE_UINT32 m_buy_pvp_limit;//pvp�������
	
	//������ ����
public:
	ACE_UINT32 m_unBenefitCount;
	ACE_UINT32 m_benifitList[MAX_VIP_BENIFIT_COUNT];
private:
	int parse_benifit_list(ACE_UINT32 list[], char *str);
};