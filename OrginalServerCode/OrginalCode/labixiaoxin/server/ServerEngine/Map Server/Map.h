#pragma once

#include "..\GameBase\Adapter\SubAdapterObj.h"
#include "item\ItemContainer.h"
#include "..\GameBase\Common\IDMap.h"
#include "module\PlayerMgr.h"
#include "module\FriendMgr.h"
//#include "module\EquipmentMgr.h"
#include "module\EnergyMgr.h"
#include "module\PackageMgr.h"
#include "module\MapInfoMgr.h"
#include "module\SkillMgr.h"
//#include "module\WeaponSkillMgr.h"
//#include "module\TeamMgr.h"
#include "module\LootMgr.h"
#include "module\FunctionMgr.h"
#include "module\VipMgr.h"
#include "module\UnlockMgr.h"
#include "module\BenifitMgr.h"
#include "module\GiftMgr.h"
#include "module\CooldownMgr.h"
#include "module\HandbookMgr.h"
#include "module\MaterialMgr.h"
#include "module\PVPMgr.h"
#include "module\MoneyTreeMgr.h"
#include "module\SecretMgr.h"
#include "module\WingMgr.h"
#include "module\GiftBagCodeMgr.h"
#include "module\TaskMgr.h"
#include "task/DailyTaskMgr.h"
#include <vector>
#include "module/LimitedActivityMgr.h"
#include "module/GoldenTouchMgr.h"
#include "module/BadgeMgr.h"
#include "module/PropsMgr.h"
//#include "..\Base\Observer\Subject.h"

//#include "module\BuffMgr.h"
//#include "module\GameTimeMgr.h"
//#define MAX_CROPS_COUNT 64
//#define MAX_MONSTER_COUNT 5
#define MAX_MAP_SESSION_LIVE (10 * 60)

class Map : public SubAdapterObj
{
public:
	Map(ACE_UINT32 unID, BaseAdapter *parent);
	~Map();

	virtual int start_create();
	//virtual int start_destroy();
	virtual bool hasCreated(){return m_bLoadingDone;};

	virtual void obj_timeout();
	ACE_UINT32 m_unLiveCounter;
	virtual int deal_msg(MessageInfo &msgInfo);
	virtual int deal_db_msg(ACE_UINT32 unType, void *msg);

	void wanna_remove_from_lru(){};
	void cancel_remove_from_lru(){};

	void dispatch_map_event(char *strEventName, ACE_UINT32 unResID = 0, ACE_UINT32 unCount = 1);

	int send_msg_2_client(ACE_UINT32 unMsgType, ACE_UINT32 unBodyType, char *strBody, ACE_UINT32 unBodyLen);

	
	void get_reward_from_config_reward(RewardInfo &dest,const ConfigReward &configReward, int item_times, int nCount,bool isLuck = false);
	void add_reward_from_config_reward(RewardInfo &dest,const ConfigReward &configReward, int item_times, int nCount,bool isLuck = false);
	void get_reward(const RewardInfo &reward,LogItemInfo & logInfo);
	void SetRewardLogItem(const RewardInfo &reward,LogItemInfo & logInfo,std::vector<LogItemInfo> & ver);//添加货币日志信息
	void SetlogUserInfo(LogItemInfo & logInfo);//设置日志基础信息
	void ChickLuckStart(ACE_UINT32 tmp_drop_id);//	玩家抽奖GM检测
	void AddGmMsg(GMInfo & info);

	void set_basic_info(int nCoin, int nMoney, int nExp, int nPowerPoint, int nHonour, int nFriendship,int dynamicEnergy);

	void set_eat_everyday_count(int count);//每天已吃饭团的次数
	int get_eat_everyday_count();

	void send_month_card();
	void send_friendship_mail(ACE_UINT32 unCharacterID, ACE_INT32 unAssistType, const char *strAssistName,ACE_UINT32 unMultEnergy);

	int level_up_item(int idx, ACE_UINT32 unPlayerID, int dwLevelAdd);//物品或装备升级 client 入口
	int level_up_item(ACE_UINT32 unType, ItemInfo *itemInfo, int dwLevelAdd);//物品或装备升级

	int use_props(int idx, RewardInfo& roReward);
	void add_item(ACE_UINT32 unItemID, ACE_UINT32 unCount,LogItemInfo & logInfo);

	void update_energy_2_db();
	void update_snap_info_2_db();
	void update_friendship_2_db();
	
	PlayerMgr m_playerMgr;
	FriendMgr m_friendMgr;
	//EquipmentMgr m_equipmentMgr;
	EnergyMgr m_energyMgr;
	//EnergyMgr m_pvpEnergyMgr;
	MapInfoMgr m_mapInfoMgr;
	PackageMgr m_packageMgr;//装备列表
	SkillMgr m_skillMgr;
	//WeaponSkillMgr m_weaponSkillMgr;
	//TeamMgr m_teamMgr;
	LootMgr m_lootMgr;
	FunctionMgr m_functionMgr;
	VipMgr m_vipMgr;
	UnlockMgr m_unlockMgr;
	BenifitMgr m_benefitMgr;
	GiftMgr m_giftMgr;
	CooldownMgr m_cooldownMgr;
	HandbookMgr m_handbookMgr;
	MaterialMgr m_materialMgr;
	PropsMgr m_propsMgr;
	PVPMgr m_pvpMgr;//竞技场
	MoneyTreeMgr m_moneyTreeMgr;
	SecretMgr m_secretMgr;  //added by jinpan for secret shop mgr
	WingMgr m_wingMgr;
	GiftBagCodeMgr m_giftBagCodeMgr;
	TaskMgr m_taskMgr;
	DailyTaskMgr m_dailyTaskMgr;
	LimitedActivityMgr m_LAMgr;//限时活动任务
	CBadgeMgr m_badgeMgr;//徽章系统
	GoldenTouchMgr m_goldenTouchMgr;
	Bitfield m_bitField;
	ProductList m_Productlist;

	char m_strName[MAX_CHARACTER_LENGTH + 1];

	ACE_UINT32 m_unCoin;
	ACE_UINT32 m_unMoney;
	ACE_UINT32 m_unExp;
	ACE_UINT32 m_unMaxLevel;  //added by jinpan  max Level of player in gameconfig_player_lv
	ACE_UINT32 m_unPowerPoint;
	ACE_UINT32 m_unHonour;
	ACE_UINT32 m_unLevel;
	ACE_UINT32 m_unDynamicEnergy;//动感能量

	ACE_UINT32 m_unChangedDateTime;

	ACE_UINT32 m_unLastLoginTime;//最后登录时间
	ACE_UINT32 m_unLoginBonusCount;

	ACE_UINT32 m_unLoginCount;

	ACE_UINT32 m_unTutorialFlag; //新手引导标记位，第1位助战引导
	ACE_UINT32 m_dwFlag;//第(0,3]位表示每天吃饭团的次数，第(3,4]位表示是否已领取每天的普通签到奖励，第(4,5]位表示是否已领取每天的VIP签到奖励,第(5,10]位表示当月已领取多少天，第(10,14]位表示神秘商店付费刷新次数
	ACE_UINT32 m_unUseLuckCount;
	ACE_UINT32 m_dwBuyMonthCardTime;

	ACE_UINT32 m_unFriendshipPoint;
	ACE_UINT32 m_unCurrentTime;
	ACE_UINT32 m_unChannel;//渠道
	ACE_UINT32 m_unMultEnergy;//多倍体力，用于校验
	ACE_UINT32 m_unCurrentBuyEnergy;//当前买体力次数
	OrderInfo m_rechargeOrder;//充值返回结果
	ACE_UINT32 m_rechargeOrderSN;

	ACE_UINT32 m_unUserID;
	static ACE_UINT32 m_unUserInfoServerID;

	Secret m_secretCfg;
	float m_fSecretOff;

	ACE_UINT32 m_monthTime;//月卡过期时间
	ACE_UINT32 m_lastSendMonthTime;//以发送到的时间

	void send_response_end(ACE_UINT32 unTargetID, ACE_UINT32 unSN);

	bool CheckDateChanged();

	void SendLogInfoItemMSG(const std::vector<LogItemInfo> & ver);//日志系统发送一条日志信息

	bool is_cost_enough(CostInfoList &costList, int times);//判断cost是否足够
	void do_cost(CostInfoList &costList, int times,LogItemInfo & logInfo);
private:
	
	//int level_up_player(PlayerInfo *info);

	void Update_Bitfield();
	void add_old_gift();

	void send_basic_info(ObjAdr &targetAdr, ACE_UINT32 unSN);//向客户端发送基础信息 初始化信息
	void cal_level_up_exp();
	void update_all();

	int get_luck(ACE_UINT32 unCount, ACE_UINT32 unBehaviorID, RewardInfo &reward);
	
	void update_use_luck_count();//向数据库更新用户抽奖次数

	int delete_item(ACE_UINT32 unDeleteType, ACE_UINT32 unCount, ACE_UINT32 *list);

	int equipment_step_up(ACE_UINT32 unIdx, ACE_UINT32 unPlayerID, int idxList[], int nCount);

	int weapon_fragment_step_up(ACE_UINT32 unIdx, ACE_UINT32 unPlayerID, int idxList[], int nCount);

	bool is_same_equipment(int idxList[], int nCount, ItemInfo *itemInfo);

	void deal_admin_op(MsgAdminOPRequest *request, ObjAdr &targetAdr);
	void Deal_recharge_op_result();


	void do_map_before_cost(CostInfoList &costList, int times,LogItemInfo & logInfo);
	ACE_UINT32 do_map_after_cost(CostInfoList &costList,ACE_UINT32 unMapID, int times,LogItemInfo & logInfo);

	int do_compose(ACE_UINT32 unItemID = 0,ACE_UINT32 unTimes = 1);

	void do_gift_bag_code_reward(RewardInfo &oRewardInfo, ACE_UINT32 unRewardID, int &iOffset);

	void update_login_count();

	int deal_get_article(unsigned articleId,unsigned count);

	void moveToMateria();//在服务器重启后，用户第一次读入信息时将图鉴表的物品加入物品表中

	ACE_INT32 max_exp_get();  //判断达到最大等级时候能够获得的exp，返回的是能够获得的最大经验值

	int secret_refresh(SecretItemInfo dest[],ACE_UINT32 type);

	void set_secret_amount(MaterialInfo &dest);

	void set_sign_up_reward_sign(bool bHasSignUp);
	bool has_sign_up_reward();

	void set_vip_sign_up_reward_sign(bool bHasSignUp);
	bool has_vip_sign_up_reward();

	void set_sign_up_day_this_month(int dwDayinMonth);
	int get_sign_up_day_this_month();

	void set_secret_paid_refresh_times(ACE_UINT32 unTimes);
	int get_secret_paid_refresh_times();

	void set_secret_paid_refresh_price();

	SecretCfg get_secret_cfg(ACE_UINT32 type);
	ACE_UINT32 get_secret_id(ACE_UINT32 tmp_id);
	int secret_buy(ACE_UINT32 unId,ACE_UINT32 unIdx);
	int secret_expand(ACE_UINT32 unExpandCount);  //扩充神秘商店的容量
	//int get_gold_everyday();

	int golden_touch(ACE_UINT32 &unGoldenTouchProfit, ACE_UINT32 &unCriticalCount);
	void add_reward(RewardInfo &oRewardInfo, ACE_UINT32 unID, ACE_UINT32 unCount);

	void SendBuyEnergyToDB();

	bool m_bLoadingDone;

	ACE_UINT32 m_unExpLevelUp;

	//ACE_UINT32 m_unUserID;
	//char m_send_buffer[1024];
	

	static ACE_UINT32 m_unPhpProxyID;

	bool m_bValidHome;

	ACE_UINT32 m_unLastActivityTimer;

	std::vector<MailInfo> m_oMailListBuffer;	//缓存邮件列表，分多次发送给客户端
	ACE_UINT32 m_dwGetMailListIndex;	//从m_oMailListBuffer第几个元素开始发给客户端
	void UpdateProductList(ACE_UINT32 unProductID);

	int SharedActivites(RewardInfo & reward);//分享得奖励

	int Badge_step_up(MsgDoUpBadgeGaintRequest * msg,bool isTuJian);//徽章升级
};