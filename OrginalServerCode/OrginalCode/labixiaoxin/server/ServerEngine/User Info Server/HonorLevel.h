
/********************************************************************
    ����ʱ�䣺    2015/08/19  14:43:59 
    �ļ�����      HonorLevel.h
    ���ߣ�        xxj
    
    ����:		1.pvp ���а���� ������
				2.
                
    ˵��:		1. 
                2. 
*********************************************************************/
#pragma once
#include "..\ace\Basic_Types.h"
#include "..\GameBase\Common\GameDefine.h"
#include <set>
#include <map>
#include <vector>

//flag ���λ
#define HL_Flag_Clear 0			//���״̬
#define HL_Flag_init 1			//��ʼ��
#define HL_Flag_start 2			//������
#define HL_Flag_end 3			//��� ����
#define HL_Flag_end_ex 4		//��� ���ʼ�
#define HL_Flag_get_reword 5	//����ɷ���


class UserInfoMgr;

class CHonorLevel
{
public:
	CHonorLevel();
	void SetSuper(UserInfoMgr * pSuper);
public:
	void add(HLInfo &info);
	void update(HLInfo & info,ACE_INT32 oldHonour);
	void Clear();
	void DoOnTimerOut();
	int  GetRanking(ACE_INT32 honour);//��ȡ����
	bool ChoiceFightRoole(const HLInfo & info,HLInfoArray & _array,HLInfo * quitList);//ѡ���ɫ
	void ChoiceFightRooleEx(HLInfoArray & _array,ACE_UINT32 _characterID);//ѡ���ɫ

	ACE_UINT32 m_unCount;
	HLInfo m_list[MAX_HONOUR_LEVEL_COUNT + 1];//��Ԥ��һ����������//
	ACE_UINT32 GetSumNum();//��ȡ�����˲μ����������//
private:
	void InsertionSort(HLInfo & info);
	bool DoLoadHL();//���� ��������
	bool DoSendWeakReword();//����
	void DoSendDayReword();//����ÿ�ս���
	void DoSendRankReword();//������������
	void UpdateLevelMap(const HLInfo & honour,bool isAdd);//������������
	void UpdateHonorLevelMap(const HLInfo & honour,bool isAdd);//������������
	ACE_INT32 GetHLevel(ACE_INT32 honour);//�õ���λ
	bool TryAdd(std::set<ACE_UINT32> & quitList,ACE_UINT32 id);//���ż���

	void OnSendMail(const HLInfo & info,ACE_INT32 behaviorID,ACE_UINT32 unMailId,ACE_UINT32 parm);//���ʼ�

	std::vector<HLInfo> m_verHLinfo;//��������
	UserInfoMgr * m_pUserInfoCallback;//���ڴ�����Ϣ

	std::map<ACE_INT32,ACE_UINT32> m_levelMap;//ÿ������ֵ���û����� ���ڵõ���ǰ�������
	std::map<ACE_INT32,std::vector<HLInfo> > m_honorlevelMap;//ÿ����λ�ȼ��е��������
	std::set<ACE_UINT32> m_characterKey;//�Ѳμӵ��û��б� ���������Ƿ��Ѿ��μ�
	ACE_INT32 m_flag;//���λ
	bool m_isRestart;//�Ƿ�һ����������
	ACE_INT32 m_max;
	ACE_INT32 m_min;
};