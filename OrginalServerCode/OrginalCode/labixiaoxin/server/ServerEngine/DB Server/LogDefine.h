/********************************************************************
    ����ʱ�䣺    2015/04/26  16:33:54 
    �ļ�����      LogDefine.h
    
    ����:		1.��־�ļ������Ϣ����
				2.
                
    ˵��:		1. ��Ҫ������־�ļ����ݽṹ��ö�ٶ���
                2. 
*********************************************************************/
#ifndef __LOGDEFINE_H_
#define __LOGDEFINE_H_
#include "..\GameBase\Common\GameDefine.h"

#define ZeroVar(p) ZeroMemory( &p, sizeof( p ) )

//��־����
enum enum_LOG_TYPE
{
	LOG_TYPE_DEFALT = 0,//Ĭ��ֵ
	LOG_TYPE_money_add,//������
	LOG_TYPE_exp_add,//����ֵ����
	LOG_TYPE_coin_add,//��������
	LOG_TYPE_energy_add,//��������
	LOG_TYPE_pvpenergy_add,//��������
	LOG_TYPE_PowerPoint_add,//�Ͻ�����
	LOG_TYPE_Friendship_add,//���������
	LOG_TYPE_honour_add,//����ֵ����
	LOG_TYPE_handBook_add,//ͼ������
	LOG_TYPE_player_add,//��������
	LOG_TYPE_player_chip_add, //��ɫ��Ƭ����
	LOG_TYPE_equipment_add, //�豸��װ������
	LOG_TYPE_function_add,	//���ܽ���
	LOG_TYPE_gift_add,		//������
	LOG_TYPE_extra_skill_add, //���ܻ��
	LOG_TYPE_material_add,	 //���ϣ���Ƭ���
	LOG_TYPE_map_start_add,		//�ؿ����� (���Ǹ���)
	LOG_TYPE_equipment_up, //�豸��װ������
	LOG_TYPE_player_up,		//�������
	LOG_TYPE_skill_up,		//��������
	LOG_TYPE_step,			//����
	LOG_TYPE_money_lower,//��Ҽ���
	LOG_TYPE_coin_lower,//�����½�
	LOG_TYPE_energy_lower,//�����½�
	LOG_TYPE_handBook_lower,//ͼ���½�
	LOG_TYPE_PowerPoint_lower,//�Ͻ��½�
	LOG_TYPE_honour_lower,//����ֵ�½�
	LOG_TYPE_pvp_lower,//pvp�½�
	LOG_TYPE_Friendship_lower,	//������½�
	LOG_TYPE_material_lower,	//���ϣ���Ƭ����
	LOG_TYPE_equipment_lower,	//�ֽ�
	LOG_TYPE_props_add,		//���ӵ���
	LOG_TYPE_props_lower,	//���ٵ���
	LOG_TYPE_Dynamic_Energy_lower,	//��������
	LOG_TYPE_Dynamic_Energy_add,	//��������
	LOG_TYPE_RECHARGE,	//��ֵ
	LOG_TYPE_Statistics,	//ͳ��
	LOG_TYPE_props,		//����
	LOG_TYPE_POWER,		//ս��

	
	LOG_TYPE_FAIL,
};

//��������ԭ��
enum enum_LOG_ACTION
{
	LOG_ACTION_DEFALT = 0,//Ĭ��ֵ

	LOG_ACTION_use_gift = 1,//ʹ�õ��ߣ������
	LOG_ACTION_map_gift,	//�ؿ����� �ۻ����ǽ���
	LOG_ACTION_pvp_rank,	//pvp����
	LOG_ACTION_level_up,	//����
	LOG_ACTION_vip_up,		//vip ����
	LOG_ACTION_sign_up,		//ÿ��ǩ��
	LOG_ACTION_convert_gift, //�����һ�
	LOG_ACTION_mail_get,	//�ʼ���ȡ
	LOG_ACTION_mail_all,	//һ����ȡ���и���
	LOG_ACTION_map_get,		//���㽱��,�ܿ�
	LOG_ACTION_purchase_gold, //��ֵ
	LOG_ACTION_do_behavior,	 //�������磺��Ԫ������pve��pvp����//m_goodsID,0x16100019:��ˮ,0x16100020:ҡǮ,0x16000022:͵��,ÿ�յ�¼
	LOG_ACTION_get_luck,	//�齱
	LOG_ACTION_daily_Single_task,	//ÿ������ 
	LOG_ACTION_daily_task,	//ÿ������ 
	LOG_ACTION_task,		//����
	LOG_ACTION_secret_buy,	//�����̵�
	LOG_ACTION_do_compose,	//�ϳ�
	LOG_ACTION_secret_refresh,//ˢ�������̵�,0���Զ�ˢ�£�1����ͨԪ��ˢ�£�2����ͨ����ˢ�£�3�Ǹ߼�Ԫ��ˢ�£�4�Ǹ߼�����ˢ��//
	LOG_ACTION_secret_expand,//���������̵�ĸ���
	LOG_ACTION_melt,		//�ֽ�
	LOG_ACTION_step,		//����
	LOG_ACTION_map_start,	//�ؿ�����
	LOG_ACTION_Limited_Activity,	//��ʱ�����
	LOG_ACTION_MAP_RESET,			//��ͼ����
	LOG_ACTION_golden_touch,        //���������
	LOG_ACTION_buy_energy,        //������
	LOG_ACTION_map_cost,		//���㽱��,�ܿ�
	LOG_ACTION_piece_role,		//��Ƭת��ɫ
	LOG_ACTION_Unlock,		//����
	LOG_ACTION_user_level_up,//�˻�����
	LOG_ACTION_user_exp_up,//�˻�����ֵ����
	LOG_ACTION_Login,//��¼
	LOG_ACTION_REGISTER,//ע��
	LOG_ACTION_quit,//�뿪
	LOG_ACTION_shared,//����ý���
	LOG_ACTION_level_count,//�ȼ�ͳ��
	LOG_ACTION_level_Gift,//�ȼ����
	LOG_ACTION_badge_up,//��������
	LOG_ACTION_CREATE_CHARACT,//����
	LOG_ACTION_PVP_TIMES_up,//��PVPս������
	LOG_ACTION_RECHARGE,//��ֵ
	LOG_ACTION_use_props,//ʹ�õ���
	LOG_ACTION_FIRST_RECHARGE,//�׳�
	LOG_ACTION_Online_user_count,//ʱʱ��������ͳ��
	LOG_ACTION_POWER,		//ս�� (m_op_after ��ʾ��ǰս��ֵ)


	LOG_ACTION_TEST = 400,//����
};
/*
	ÿ������/������m_goodsID
	enum E_ACTIVE_BASE_EVENT
	{
	Event_Base_Type_Duplication = 1, //����
	Event_Base_Type_TreasureMountain = 2, //��ɽ
	Event_Base_Type_Trial = 3,             //
	Event_Base_Type_Lottery = 4,           //����
	Event_Base_Type_LevelUp = 5,		//����
	Event_Base_Type_EquipmentUpgrade = 6, //װ������
	Event_Base_Type_EnhanceSkill = 7,   //��������
	Event_Base_Type_SecretBuy = 8,              //�����̵�
	Event_Base_Type_SupermanPowerStone = 9,     //��������ʯ
	Event_Base_Type_SpecificDuplication = 10,	//���⸱��
	Event_Base_Type_ReachSpecificLevel = 11,	//�ȼ�
	Event_Base_Type_ReachSpecificStar = 12,		//����
	Event_Base_Type_EquipmentSpecificLevel = 13,//װ������ȼ�
	Event_Base_Type_EquipmentSpecificStar = 14,	//װ����������
	};
*/

typedef struct tagLogItemInfo
{
	//log_id ://����
	ACE_UINT32 m_unUserID;//�ʺ�id user_id
	ACE_UINT32 m_unLevel;//��ɫ�ȼ� level
	ACE_UINT32 m_unChannel;//���� channel_id
	enum_LOG_TYPE m_unType;//log��� log_type ---------------------------
	ACE_UINT32 m_goodsID;//װ��ID char m_goods_name[MAX_ITEM_NAME + 1];//װ������ goods_id
	ACE_UINT32 m_op_before;//����ǰ operate_before
	ACE_UINT32 m_op_after;//������ operate_after
	ACE_UINT32 m_op_involve;//�����漰 operate_involve (��phpû��ϵ��������Ϣ��ϵ)
	enum_LOG_ACTION m_op_action;//��������ԭ�� operate_action---------------------
	ACE_UINT32 m_op_time;//����ʱ�� operate_time
	char m_name[MAX_CHARACTER_LENGTH + 1];//��ɫ�� name
	char m_op_way[MAX_CHARACTER_LENGTH + 1];//;��(������Ϣ) operate_way   ---------------------------
	ACE_UINT32 m_charactID;//��ɫid
	void SetOp_Way(const char * str)
	{
		ZeroVar(m_op_way);
		int len = strlen(str);
		if (len > MAX_CHARACTER_LENGTH)
			len = MAX_CHARACTER_LENGTH;
		memcpy(m_op_way,str,len);
	}

	void SetOp_Way(const wchar_t * str)
	{
		ZeroVar(m_op_way);
		char rstrUtf8[MAX_CHARACTER_LENGTH + 1];
		ZeroVar(rstrUtf8);
		int dwLen = -1;
		WideCharToMultiByte(CP_UTF8, 0, str, dwLen, &rstrUtf8[0], MAX_CHARACTER_LENGTH, NULL, NULL);
		memcpy(m_op_way,rstrUtf8,MAX_CHARACTER_LENGTH + 1);
	}

	tagLogItemInfo()
	{
		ZeroVar((*this));
	}
}LogItemInfo;

// void tagLogItemInfo::ToJson( Json::Value &root )
// {
// 	root["m_unUserID"] = m_unUserID;
// 	root["m_name"] = m_name;
// 	root["m_unLevel"] = m_unLevel;
// 	root["m_unChannel"] = m_unChannel;
// 	root["m_goodsID"] = m_goodsID;
// 	root["m_op_action"] = (int)m_op_action;
// 	root["m_op_way"] = m_op_way;
// 	root["m_unType"] = (int)m_unType;
// 	root["m_op_before"] = m_op_before;
// 	root["m_op_after"] = m_op_after;
// 	root["m_op_involve"] = m_op_involve;
// 	root["m_op_time"] = m_op_time;
// }

#define _LOG_OP_WAY_use_gift			L"�����"
#define _LOG_OP_WAY_map_gift			L"�ۻ����ǽ���"
#define _LOG_OP_WAY_pvp_rank			L"pvp����"
#define _LOG_OP_WAY_level_up			L"level up"
#define _LOG_OP_WAY_vip_up				L"vip up"
#define _LOG_OP_WAY_sign_up				L"ÿ��ǩ��"
#define _LOG_OP_WAY_convert_gift		L"�����һ�"
#define _LOG_OP_WAY_mail_get			L"������ȡ"
#define _LOG_OP_WAY_mail_all			L"һ����ȡ���и���"
#define _LOG_OP_WAY_map_get				L"���㽱��"
#define _LOG_OP_WAY_map_cost			L"��ͼ����"
#define _LOG_OP_WAY_purchase_gold		L"��ֵ"
#define _LOG_OP_WAY_do_behavior			L"����"
#define _LOG_OP_WAY_get_luck			L"�齱"
#define _LOG_OP_WAY_daily_task			L"ÿ������"
#define _LOG_OP_WAY_daily_Single_task	L"ÿ�յ�������"
#define _LOG_OP_WAY_task				L"������"
#define _LOG_OP_WAY_secret_buy			L"�����̵�"
#define _LOG_OP_WAY_do_compose			L"�ϳ�"
#define _LOG_OP_WAY_secret_refresh		L"ˢ�������̵�"
#define _LOG_OP_WAY_secret_expand		L"���������̵�"
#define _LOG_OP_WAY_melt				L"�ֽ�"
#define _LOG_OP_WAY_step				L"����"
#define _LOG_OP_WAY_map_start			L"�ؿ�����"
#define _LOG_OP_WAY_Limited_Activity	L"��ʱ�"
#define _LOG_OP_WAY_MAP_RESET			L"��ͼ����"
#define _LOG_OP_WAY_SYSTEM				L"ϵͳ�ʼ�"
#define _LOG_OP_WAY_golden_touch		L"�����"
#define _LOG_OP_WAY_buy_energy			L"������"
#define _LOG_OP_WAY_piece_role			L"��Ƭת��ɫ"
#define _LOG_OP_WAY_Unlock				L"����"
#define _LOG_OP_WAY_user_level_up		L"�ʺ�����"
#define _LOG_OP_WAY_user_exp_up			L"����ֵ����"
#define _LOG_OP_WAY_login				L"��¼"
#define _LOG_OP_WAY_REGISTER			L"ע��"
#define _LOG_OP_WAY_quit				L"�뿪"
#define _LOG_OP_WAY_shared				L"������"
#define _LOG_OP_WAY_level_count			L"�ȼ�ͳ��"
#define _LOG_OP_WAY_level_Gift			L"�ȼ����"
#define _LOG_OP_WAY_badge_up			L"��������"
#define _LOG_OP_WAY_CREATE_CHARACT		L"����"
#define _LOG_OP_WAY_TEST				L"���Ե�½"
#define _LOG_OP_WAY_PVP_TIMES_up		L"�����������"
#define _LOG_OP_WAY_RECHARGE			L"��ֵ"
#define _LOG_OP_WAY_use_props			L"ʹ�õ���"
#define _LOG_OP_WAY_FIRST_RECHARGE		L"�׳�"
#define _LOG_OP_WAY_Online_user_count	L"��������ͳ��"
#define _LOG_OP_WAY_POWER				L"ս��"


#endif