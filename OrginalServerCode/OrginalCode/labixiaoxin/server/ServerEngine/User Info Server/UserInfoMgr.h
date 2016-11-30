#pragma once
//���������������

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

#include "PlayerPoolMgr.h"

#include "TeamRankMgr.h"
#include "PVPSeasonMgr.h"
#include <vector>
#include "../GameBase/Protocol/ControlProtocol.h"
#include <queue>
#include "HonorLevel.h"

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT32, PublicPlayerInfo *, ACE_Hash<ACE_UINT32>, 
ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex> CHARACTER_INFO_MAP;
typedef CHARACTER_INFO_MAP::ITERATOR CHARACTER_INFO_MAP_ITERATOR;

using namespace sql;

struct MailType
{
	MailType(ACE_UINT32 cid,ACE_UINT32 mid)
	{
		charact_id = cid;
		mail_id = mid;
	}
	ACE_UINT32 charact_id;
	ACE_UINT32 mail_id;
};

//ս�������£����µ����ݿ⣬����Ƿ����ս�������а�������룬ֱ�Ӹ�����ҵ����������û�н��룬�����ϴζ����ݿ��ʱ�䣬���ڵ���10���ӵ�����£��첽�������ݿ⣬����Ҹ��µ���Ӧ��ս������
//��һ�ȡս����������ֱ�Ӷ�ȡ�����أ�Ȼ������ϴζ����ݿ��ʱ�䣬���ڵ���10���ӵ�����£��첽�������ݿ�
//��һ�ȡ���������������û�л�ȡ���ƺźͽ���ID�������ݿ��첽��ȡ����������������Ѿ���ȡ��ֱ�ӻ�ã����������㷨ͬս����
//���ֻ��ࣨս���������֣�
//���а���ࣨս���������֣�
//��ҳػ���
//�����û���������

#define MAX_ARENA_POOL_NODE 512

class UserInfoMgr : public BaseAdapter
{
public:
	UserInfoMgr(ACE_UINT32 unID);
	virtual ~UserInfoMgr();

	bool init_timer();

	void uninit_timer();

	static void EmailtimerCallback(void *p, long lTimerID);
	void DoEmailtimer();

	//virtual BaseObj *new_child_obj(ACE_UINT32 unID){return new PhpProxySession(unID, this);};

	virtual int deal_msg(MessageInfo &msgInfo);
	virtual int obj_init(void *arg = NULL, ACE_UINT32 unArgLen = 0);
	virtual void obj_fini();
	virtual void obj_timeout();

	int get_user_count(){return m_characterMap.current_size();};

	//void reset_season();

	TeamRankMgr m_teamRankMgr;
	PVPSeasonMgr m_pvpSeasonMgr;
//	Pvp100Group  m_pvp100List[MAX_PVP100_STAGE_COUNT];

	//long m_lTimerID;
	BaseTimerHandler *m_serviceEmailTimerHandler;
public://pvp ���а����
	CHonorLevel m_honorLevel;
	void Init_Honour_Level();//��������
	void DoLoadHL(std::vector<HLInfo> & ver);
	void DoClearHL();//һ�����������������
	std::map<ACE_UINT32,HLInfo> m_nowHLinfoMap;
	HLInfo GetNowHLinfo(ACE_UINT32 characterID);//�õ���ɫ��pvp��Ϣ
	void UpdateHLinfo(HLInfo & info);//���½�ɫ��pvp��Ϣ
	bool GetHLInfoArrayFromDB(HLInfoArray & _array,ACE_UINT32 characterID);//�����ݿ��ȡս���б�
	bool UpdateHLInfoArrayToDB(const HLInfoArray & _array,ACE_UINT32 characterID);//����ս���б����ݿ�
	void UpdateFightRecordInfoToDB(FightRecordInfo & info,bool isRoobt);//����һ��ս�������ݿ�
	bool GetFightRecordInfoList(FightRecordInfoList & info,ACE_UINT32& winLost,ACE_UINT32 characterID);
	bool UpdateFightRecordInfoList(FightRecordInfoList & info,ACE_UINT32& winLost,ACE_UINT32 characterID);
public:
	void insert_mail_info(MailInfo& roMailInfo);//���һ���ʼ�
	PublicPlayerInfo *getFriendInfo(ACE_UINT32 cid);
private:
	CHARACTER_INFO_MAP m_characterMap;

	Connection *m_dbConn;//���ܳ��ڳ���
	ACE_UINT32 m_db_time_heart;//���ݿ��������� ����m_dbConn

	PreparedStatement *m_prep_stmt;
	ResultSet *m_db_res;

	ACE_UINT32 m_dwMaxMailID;

	Pvp100ListCfg m_pvp100Cfg;

	void initDBConnection();
	void finiDBConnection();

	void destroy_db_res();

	int deal_client_msg(MessageInfo &msgInfo);

	int readFriendDetailFromDB(PublicPlayerInfo *info);
	void readAllFriendDetailFromDB();

	void init_config();
	void init_team_rank();
	void init_pvp_season();

	void update_team_rank_info(ACE_UINT32 dwCharacterID, PlayerTeamInfo *info);

	void init_mail_list();

	//int get_statistical_data(const ACE_UINT32 dwType);

	void init_gift_bag_code();

	void update_gift_bag_code(const char *strCode, const ACE_UINT32 dwUsed);

	void update_season();

	void update_player_pvp_info(PublicPlayerInfo *player);

	int set_name(SQLString str, char *dest, int len);

	ACE_UINT32 m_unDealID;

	//ACE_UINT32 m_arenaPoolCount[MAX_ARENA_COUNT];
	//ArenaPoolNode *m_arenaPoolList[MAX_ARENA_COUNT][MAX_ARENA_POOL_NODE];
	
	PublicPlayerInfo *alloc_character_detail();

	int update_db_connection();
	//Connection *create_db_connection();


	int get_random_player_in_guidance(PublicPlayerInfo *list[], int iLevel, int nCount,bool bFlag = false);

	int get_special_player(PublicPlayerInfo *list[],ACE_UINT32 unMapID);//����

	//CharacterPool m_characterPool;
	PlayerPoolMgr m_levelPool;
	PlayerPoolMgr m_teamRankPool;
	PlayerPoolMgr m_pvpRankPool;

	
	void update_mail_info(MailInfo& roMailInfo);//�����ʼ�
	void delete_mail_info(ACE_UINT32 dwCharacterID, ACE_UINT32 dwMailID);//ɾ���ʼ�
	bool Get_mail_info(MailInfo& roMailInfo, ACE_UINT32 dwCharacterID, ACE_UINT32 dwMailID);//�����ʼ�
	int get_reward_from_email(ACE_UINT32 dwCharacterID, MailInfo& roMailInfo, ACE_UINT32 dwReadMailRequestSN);
	
	void insert_gift_bag_code(AdminGiftBagCode &stCode);

	ACE_UINT32 get_character_id_list(ACE_UINT32 unCurrentID, ACE_UINT32 unMaxCount, ACE_UINT32 *dest);

	bool is_pvp_team_valid(PublicPlayerInfo *info);
	void update_pvp_pool(PublicPlayerInfo *info);

	void init_team_rank_pvp100();
	int get_pvp100_rank(ACE_UINT32 point);
	static ACE_UINT32 m_unPhpProxyID;
	ACE_UINT32 m_unCurrentTime;

	int LoadStatistics(const ACE_UINT32 dwType, const char *strDate,ACE_UINT32 * paramList);//ͳ��
	//gm ����
	int Set_GM_info( GMInfo *request);//����GM����
	int Delete_GM_Info(ACE_UINT32 dwtype);//ɾ��һ�����͵�����
	void Init_GM_Info();//����ϵͳ����
	int Get_GM_info(MsgGMRequest * request,MsgGMRsponse & response);
	void Deal_recharge_vip_result(OrderInfo *request);
	void GetProductList(ACE_UINT32 unCharacterID,ProductList & list);
	void UpdateProductList(ACE_UINT32 unCharacterID,ACE_UINT32 unProductID,ProductList & list);
	bool GetVipInfo(ACE_UINT32 unCharacterID,VipInfo & vip);//����vip��ֵ
	void UpdateVipInfo(ACE_UINT32 unCharacterID,ACE_UINT32 unMoney,VipInfo & vip);//����vip��ֵ�����ݿ�
	void UpdateVip(VipInfo & vip);//����vip��ֵ
	void SendMonthCard(MsgSendMonthCardRequest * request);
private:
	static int get_pool_id(ACE_UINT32 unPoolType, PublicPlayerInfo *playerInfo);
	//void add_2_pvp_rank_pool(PublicPlayerInfo *playerInfo);
	//void update_pvp_rank(PublicPlayerInfo *playerInfo);

	void request_rank_from_db();
	int fill_leaderboard_info(PVPTeamInfo *dest, PublicPlayerInfo *list[], ACE_UINT32 unCount, bool isPVPTeam,ACE_INT32 unPage);
	void deal_admin_op( MsgAdminOPRequest * param1, ObjAdr& srcAdr );

	void deal_admin_buffer_op(MsgAdminOPBufferRequest *pRequest, ObjAdr &strAdr);
	int get_all_reward_from_email( ACE_UINT32 dwCharacterID, ACE_UINT32 dwGetAllMailRewardRequestSN );
	ACE_UINT32 m_unCurrentRankUpdateID;
	bool m_bRankUpdateDone;
	bool m_bProcessGetRank;

	std::map<ACE_UINT32, std::map<ACE_UINT32, MailInfo> > m_mapCharacter2Mail;

	//ACE_Double_Linked_List<ACE_DLList_Node> m_arenaPoolList[MAX_ARENA_COUNT];

	//ACE_UINT32 m_unGlobleGiftBagCodeCount;
	//GiftBagCodeDBItem m_globleGiftBagCodeList[MAX_GLOBLE_GIFT_BAG_CODE_COUNT];
	std::map<std::string, GiftBagCodeInfo> m_mapGiftBagCodeInfo;
	std::map<ACE_UINT32, std::vector<GMInfo>> m_mapSysGMInfo;//ϵͳGM������Ϣ
	std::queue<GMInfo> m_queUserGMInfo;//��ҫ��Ϣ������Ҫ�������ݿ�

	ACE_Recursive_Thread_Mutex m_mutex;

	void update_BS_info(BadgeShuxingS& infos,ACE_UINT32 unCharacterID);//���»�������
	void Get_BS_info(BadgeShuxingS& infos,ACE_UINT32 unCharacterID);//�õ���������
	void CheckSetupMax(PublicPlayerInfo *info);
};