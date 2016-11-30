#pragma once

#include "BaseRes.h"
#include <map>
//����ϵͳ

class BadgeRes : public BaseRes
{
public:
	BadgeRes(ACE_UINT32 unType);
	virtual ~BadgeRes();

	virtual int parse_xml_node(xmlNodePtr node);
	ACE_UINT32 m_equip_id;//��Ӧ��װ��ID
	ACE_UINT32 m_gaint_to[3];//"�ӳ�Ŀ�꣨16���ƣ�1.ȫ��2.�ӳ�characterID.ָ����ɫ�����3�ˣ����ŷָ"
	ACE_UINT32 m_gaint_ype[2];//"�������ͣ��ӳ�����(1.�����ӳ�/2.�����ӳ�/3.�����ӳ�/4.���Լӳ�/5.��������/6.�Ͻ�����)����ֵ����(1.�̶�ֵ/2.�ٷֱ�)"
	float m_gain_value[2];//"����ֵ��ʼֵ,�ɳ�"
	ACE_UINT32 m_need_role;


	//ACE_UINT32 m_unQuality;Ʒ��
	//ACE_UINT32 m_unSubType;
	/*
		"��������
			1.��������
			2.��������
			3.��������
			4.��������
			5.��������
			6.������
			7.���������
			8.������
			101.��ָ����
			102.��������
			103.��������"
		*/

	//virtual void process_res_id_by_name();
public:
	int Parse_info(ACE_UINT32 * des, char *str);
	int Parse_infoEx(float * des, char *str);
	int Parse_res_info(ACE_UINT32 * des, char *str);

	static std::map<ACE_UINT32,ACE_UINT32> m_equipmentMap;//��Ӧ��װ��ID<equipmentid,badgeid>
	static ACE_UINT32 GetBadgeID(ACE_UINT32 equipmentid);
};