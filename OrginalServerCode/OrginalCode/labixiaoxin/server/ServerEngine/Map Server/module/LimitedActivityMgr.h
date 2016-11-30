#ifndef __LIMITEDACTIVITYMGR_H_
#define __LIMITEDACTIVITYMGR_H_

#include "ace/ace.h"
#include "BaseModule.h"
#include "../../GameBase/res/RechargeRes.h"

class LimitedActivityMgr : public BaseModule
{
public:
// 	
// 
// 	bool AddTask(ACE_UINT32 unNewTaskID);
// 	bool RefreshTask(ACE_UINT32 unTaskType, ACE_UINT32 unAttachID);
// 	bool GetReward(ACE_UINT32 unTaskID, RewardInfo &oRewardInfo);
// 
// 	void BuildTaskListPkg(MsgTaskList &response);
public:
	void init(MsgDBLimitedActivityList * list);//��ʼ��

	int GetReward(ACE_UINT32 unLAMissionID, RewardInfo &oRewardInfo);//��ȡ����Ʒ
	LimitedActivityMissionConfig FindLAmissionItem(ACE_UINT32 unLAMissionID);//������ʱ�����id
	LimitedActivityConfig FindLAItem(ACE_UINT32 unLAID);//������ʱ�����id
	void Refresh();//����
	void RefreshNew();//�������ļ��и��½���
	void RefreshOld();//ɾ��������

	int GetLACurrentList(MsgGMLimitedActivityListResponse & response);//��ȡ�б�

	int ChickLARecharge(const RechargeRes * res);//�����ʱ��Ƿ���֮��

	void Cost_basic_info(int nCoin, int nMoney, int nExp, int nPowerPoint, int nHonour, int nFriendship);//������ ������Ϣ
	bool DoLATypeLinkcost(ACE_UINT32 unLAMissionType,CostInfoList & costList/*,int times*/);//��������,������������Ҫ��¼�Ⱦ��������ﵽ֮�����û��������뽱Ʒ
	bool DoLATypeLuck(ACE_UINT32 unLAMissionType,const CostInfoList & costList,ACE_UINT32 unMissionType = 1/*,int times*/);//��������,�齱������Ҫ��¼�Ⱦ��������ﵽ֮�����û��������뽱Ʒ
	bool DoLATypeSecret(ACE_UINT32 unLAMissionType,CostInfoList & costList/*,int times*/);//��������,�����̵���ۣ������ۿ�
	bool DoLATypeMapCount(ACE_UINT32 unMapID,ACE_UINT32 type,ACE_UINT32 & nMult);//�ؿ����ޱ���
	bool DoLATypeLeveUP(ACE_UINT32 unLAtype,ACE_UINT32 & nMult);//ǿ���ۿ�
	bool DoLATypeLinkRecharge(ACE_UINT32 unLAMissionType,ACE_UINT32 nGold/*,int times*/);//��������,��ֵ��������Ҫ��¼�Ⱦ��������ﵽ֮�����û��������뽱Ʒ
	bool DoLATypeLinkRechargeGift(ACE_UINT32 unProductId);//��ʱ����
	bool DoLATypeLogin();//������¼
private:
	//ͨ�����Ͳ��Ҳ�ִ�лص�
	template<class T>
	bool DoActionFindByType(ACE_UINT32 unType,ACE_INT32 unLAType,T &t,/*int times,*/bool(LimitedActivityMgr::*CallFun)(ACE_INT32,LimitedActivityInfo &,T &/*,int*/));

	bool CallFun_Cost(ACE_INT32 unLAMissionType,LimitedActivityInfo & info,CostInfoList & costList/*,int times*/);
	bool CallFun_Luck(ACE_INT32 unLAMissionType,LimitedActivityInfo & info,ACE_UINT32 & unMissionType/*,int times*/);
	bool CallFun_Secret(ACE_INT32 unLAMissionType,LimitedActivityInfo & info,CostInfoList & costList/*,int times*/);
	bool CallFun_MapCount(ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & nMult/*,int times*/);
	bool CallFun_LeveUp(ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & nMult/*,int times*/);
	bool CallFun_Recharge(ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & nGold/*,int times*/);
	bool callFun_RechargeGift(ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & unProductId);
	bool callFun_Login(ACE_INT32 unLAMissionType,LimitedActivityInfo & info,ACE_UINT32 & unTimes);

public://���//
	bool m_isHaveNew;
	void isDown(ACE_UINT32 unLAMissionID);//�Ƿ��Ѿ�������//
private:
	bool ChickCanGetReward(ACE_UINT32 unLAMissionID,ACE_INT32 & unProgress,LimitedActivityInfo *& pLa);
	bool ChickTimeIsInActivity(LimitedActivityConfig & laconfig );//�жϵ�ǰʱ���Ƿ��ڻ��Χ��
	void update_2_db();
	void update_2_db(const LimitedActivityInfo & laconfig);
	std::map<ACE_UINT32, LimitedActivityInfo> m_mapCurrentLA;//��ǰ������б�
};

template<class T>
bool LimitedActivityMgr::DoActionFindByType( ACE_UINT32 unType,ACE_INT32 unLAType,T & t/*,int times*/,bool(LimitedActivityMgr::*CallFun)(ACE_INT32,LimitedActivityInfo &,T &/*,int*/) )
{
	Refresh();//�¼�ˢ��
	std::map<ACE_UINT32, LimitedActivityInfo>::iterator it = m_mapCurrentLA.begin();//��ǰ������б�
	while(it != m_mapCurrentLA.end())
	{
		if (it->second.unLinkType == unType)
		{
			if ((this->*CallFun)(unLAType,it->second,t/*,times*/))
			{
				return true;
			}
		}
		it++;
	}
	return false;
}

#endif
