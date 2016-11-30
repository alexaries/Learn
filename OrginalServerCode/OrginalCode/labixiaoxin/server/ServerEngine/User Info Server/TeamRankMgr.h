#pragma once

//#include "PhpProxySession.h"
#include "..\Base\Service\BaseAdapter.h"
#include "..\GameBase\Common\GameDefine.h"
#include "ace\Null_Mutex.h"
#include "ace\Hash_Map_Manager.h"
//#include "ace\Containers.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "cppconn/statement.h"   
#include "cppconn/prepared_statement.h"   
#include "cppconn/metadata.h"   
#include "cppconn/exception.h" 
#include <map>
#include <set>
#include <vector>

//#include "PVPMatchMgr.h"

using namespace sql;

#define MAX_TEAM_RANK_COUNT 50
#define MAX_TEM_PAGE_COUNT 10

class TeamRankMgr
{
public:
	TeamRankMgr();
	~TeamRankMgr();

	void reset();

	void load_from_db();

	void add(PublicPlayerInfo *player);

	void update(PublicPlayerInfo *player, ACE_UINT32 unOldPoint);

	void UpdateList(PublicPlayerInfo *player);//����m_list����,������//

	void InsertionSort(PublicPlayerInfo *player,int len);

	ACE_UINT32 m_unCount;
	PublicPlayerInfo *m_list[MAX_TEAM_RANK_COUNT + 1];//��Ԥ��һ����������//

	Pvp100Group  m_pvp100List[MAX_PVP100_STAGE_COUNT];//����ն��ϣ��

	Pvp100ListCfg m_pvp100Cfg;
	
	int get_pvp100_rank(ACE_UINT32 point);

	void update_pvp100(PublicPlayerInfo *player,ACE_UINT32 unOldPoint);//���°���ն������Ϣ
	
	Pvp100_node* find_pvp100(ACE_UINT32 unId,ACE_UINT32 unOldPoint);


public://�·�ս�����а�
	bool m_isGetLa;
	ACE_UINT32 m_unNewPaihangbang[MAX_TEAM_RANK_COUNT];
	LimitedActivityConfig m_laConfig;
	void UpdateLAAction();
	void DoLAAction();//�·�ս�������а�
	bool DoLAActionTimeCallBack();//�·�ս�������а�
	void GetMailItems(std::vector<MailInfo> & ver);
	LimitedActivityMissionConfig FindLAmissionItem(ACE_UINT32 unLAMissionID);//������ʱ�����id
	static bool GetMailInfo(MailInfo & oMailInfo,const ConfigReward & reward,ACE_UINT32 unCharacterID,ACE_UINT32 unMailId);

public:
	std::map<int, std::set<ACE_UINT32> > m_PlayerSortedByStar;
	std::map<ACE_UINT32, int> m_PlayerID2StarCount;

private:

	//void update_cell(int idx, ACE_UINT32 unCharacterID, ACE_UINT32 unPoint, PlayerTeamInfo *teamInfo);

	//PVPMatchMgr m_matchPool;
};