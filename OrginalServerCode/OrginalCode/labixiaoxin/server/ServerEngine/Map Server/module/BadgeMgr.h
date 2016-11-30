#ifndef __BADGE_MGR_H_
#define __BADGE_MGR_H_
#include "BaseModule.h"
#include <vector>

struct tagGaintFlag
{
	tagGaintFlag()
	{
		ZeroVar(*this);
	}
	ACE_UINT32 flag[2];//1.�̶�ֵ/2.�ٷֱ�
};
struct ChargeBeginInfo
{
	ChargeBeginInfo()
	{
		ZeroVar(*this);
	}
	BadgeInfo m_old;
	BadgeInfo m_new;
};
class CBadgeMgr : public BaseModule
{
public:
	CBadgeMgr();
	~CBadgeMgr();
public:
	void init(MsgDBBadgeList * list);//��ʼ��
	bool TryAddBadge(ACE_UINT32 equipmentID);//��������һ������
	
	void send_Gaint_info(ACE_UINT32 unSN);
	void DoCharge_Add_Begin();
	bool DoCharge_Add(const ItemInfo *itemInfo,ACE_UINT32 & powerPoint);//���� װ��
	bool DoCharge_Add(ACE_UINT32 tuzhiID,ACE_UINT32 & powerPoint);//���� ͼֽ
	void DoCharge_Add_After();//����
	int GetGaintPage(MsgGETBadgeResponse & response,ACE_UINT32 unPage,bool isClear);
	double GetCoinGaint();
	double GetPowerPointGaint();
	bool isUnlock();//�Ƿ��Ѿ�����
private:
	std::map<ACE_UINT32, BadgeInfo> m_mapBadge;//��ǰ�����б�
	//�ӳ�Ŀ��ӳ�Ŀ�꣨16���ƣ�1.ȫ��2.�ӳ�characterID.ָ����ɫ�����3�ˣ����ŷָ
	//�������ͣ��ӳ�����(1.�����ӳ�/2.�����ӳ�/3.�����ӳ�/4.���Լӳ�/5.��������/6.�Ͻ�����)����ֵ����(1.�̶�ֵ/2.�ٷֱ�)
	std::map<ACE_UINT32,std::map<ACE_UINT32,tagGaintFlag> >m_mapGaint;//<�ӳ�Ŀ��<��������,�ٷֱ�>>
	bool HaveBadgeByID(ACE_UINT32 badgeID);//�ж��Ƿ��Ѿ��д˻�����
	BadgeInfo * GetBadgeByID(ACE_UINT32 badgeID);//��ȡ�˻���
	void UpdateGaint();//���¼ӳ�Ŀ�꣬��������
	//void UpdateGaint(const BadgeInfo & old,const BadgeInfo & _new);
	void update_2_db(ACE_UINT32 badgeID);
	void GetGaintInfo(std::vector<MsgGetBadgeGaintResponse::tagGetBadgeGaintInfo> & ver);
	ACE_UINT32 GetLeve(ACE_UINT32 unID,ACE_UINT32 chargeNum);//�õ��ȼ�
	std::map<ACE_UINT32,ChargeBeginInfo> m_chargeVer;

	ACE_UINT32 m_scheme_badge;//�����ȼ�

private:
	std::map<ACE_UINT32,int> m_playerListMap;
	void UpdatePlayerListMap();
	bool CanUse(ACE_UINT32 BadgeID);//�Ƿ��ܻ�ȡ
};

#endif
