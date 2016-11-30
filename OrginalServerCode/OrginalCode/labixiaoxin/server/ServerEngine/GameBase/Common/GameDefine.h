#pragma once

#include "ace\ace.h"
#include "ace\Hash_Map_Manager.h"
#include "ace\Null_Mutex.h"
#include <map>
#define ZeroVar(p) ZeroMemory( &p, sizeof( p ) )//������������̳й�ϵ

namespace Json 
{
	class Value;
}

#define MAX_ACCOUNT_LENGTH 32
#define MAX_EMAIL_LENGTH 32
#define MAX_MAC_LENGTH 40

#define MAX_ITEM_NAME 50

#define MAX_PLAYER_PER_ACCOUNT 1

#define MAX_CHARACTER_LENGTH 32

#define MAX_GIFT_BAG_CODE_LENGTH 24

#define MAX_GIFT_BAG_CODE_COUNT 1024

#define MAX_GLOBLE_GIFT_BAG_CODE_COUNT (1024 * 1024)

#define MAX_ACCOUNT_BRIEF_INFO_LEN 100

#define MAX_PLAYER_COUNT 64
#define MAX_PLAYER_SELECTED_COUNT 5

#define MAX_EQUIPMENT_COUNT 3

#define MAX_MATERIAL_COUNT 1024		//changed by jinpan for handbook count
#define MAX_HANDBOOK_COUNT 512

#define MAX_INIT_BENEFIT_COUNT 4

#define MAX_SECRET_LIST 64    //added by jinpan for secret cfg

#define MAX_PACKAGE_ITEMS_PER_SEGMENT 32
#define MAX_PACKAGE_SEGMENT_COUNT 7
#define MAX_PACKAGE_LIMIT 200
#define MAX_ITEMS_COUNT (MAX_PACKAGE_LIMIT + 32)

#define MAX_WING_LIST MAX_ITEMS_COUNT

#define CLIENT_SESSION_LEN 10

#define ERROR_CODE_VERSION_NOT_MATCH			-1001
#define ERROR_CODE_NOT_ENOUGH_COIN				-1201
#define ERROR_CODE_NOT_ENOUGH_POWER_POINT		-1301
#define ERROR_CODE_NO_EQUIPMENT					-2901
#define ERROR_CODE_NEED_HIGH_LEVEL				-1601
#define ERROR_CODE_NO_PLAYER					-1201
#define ERROR_CODE_NO_SKILL						-3201
#define ERROR_CODE_NOT_ENOUGH_MONEY				-1501
#define ERROR_CODE_ENERGY_FULL					-1401
#define ERROR_CODE_PACKAGE_FULL					-1901
#define ERROR_CODE_PACKAGE_MAX					-1801
#define ERROR_CODE_VERIFY_BATTLE_FAIL			-2701
#define ERROR_CODE_INVALID_NAME					-3501
#define ERROR_CODE_NOT_ENOUGH_EQUIPMENT			-1701
#define ERROR_CODE_FRIEND_FULL					-2101
#define ERROR_CODE_FRIEND_MAX					-2001
#define ERROR_CODE_FRIEND_NOT_EXIST				-3301
#define ERROR_CODE_NOT_ENOUGH_ENERGY			-2201
#define ERROR_CODE_NEED_FRIEND					-2301
#define ERROR_CODE_PACKAGE_FULL					-1901

/*
#define GET_RES_TYPE(t) ((t >> 24) & 0xff)
#define SET_RES_TYPE(t,i) (((t << 24) & 0xff000000) | (i & 0x00ffffff))
#define GET_RES_CHILD_ID(t) (t & 0x00ffffff)
#define GET_RES_QUALITY(t) ((t >> 16) & 0xff)
#define SET_RES_QUALITY(t, i) (((t << 16) & 0x00ff0000) | (i & 0xff00ffff))
#define GET_RES_BASIC_ID(t) (t & 0xff00ffff)
*/

//#define GET_ITEM_TYPE(t) (t / 100000000)
//#define GET_ITEM_SUB_TYPE(t) ((t / 1000000) % 100)
#define GET_ITEM_ID_INFO(t,start,len) ((t >> (start << 2)) & ((1 << (len << 2)) - 1))
#define GET_ITEM_TYPE(t) (GET_ITEM_ID_INFO(t,6, 2) & 0xff)
#define GET_ITEM_MASK_ID(t,mask) ((t >> (mask << 2)) << (mask << 2))

enum
{
	ITEM_RES_TYPE_EQUIPMENT = 12,////�豸��װ��
	ITEM_RES_TYPE_DROP = 19,//����(�齱)
	ITEM_RES_TYPE_MONSTER = 11,//����
	ITEM_RES_TYPE_LUCK = 13,//�齱
	ITEM_RES_TYPE_MALL = 17,//�̳ǣ��̵꣩
	ITEM_RES_TYPE_MAP = 15,//��ͼ
	ITEM_RES_TYPE_PLAYER = 10,//����
	ITEM_RES_TYPE_SKILL = 14,//����
	ITEM_RES_TYPE_IMPROVE = 16,//ǿ��
	ITEM_RES_TYPE_STAGE = 18,//����
	ITEM_RES_TYPE_FUNCTION = 8,//���ܽ��������һ���¹��ܣ�
	ITEM_RES_TYPE_VIP = 5,//vip
	ITEM_RES_TYPE_BENIFIT = 7,//����
	ITEM_RES_TYPE_UNLOCK = 6,//����
	ITEM_RES_TYPE_GIFT = 4,//���
	ITEM_RES_TYPE_CURRENCY = 20,//����
	ITEM_RES_TYPE_ACTIVITY = 21,//�
	ITEM_RES_TYPE_BEHAVIOR = 22,//������Ϊ
	ITEM_RES_TYPE_MATERIAL = 23,//����
	ITEM_RES_TYPE_FORMULA = 24,//�ϳɱ�
	ITEM_RES_TYPE_BASIC_PROPERTY = 25,//����Ȩ(��Ȩ)
	ITEM_RES_TYPE_GROWING = 32,//�ɳ�����
	ITEM_RES_TYPE_EXTRA_SKILL = 33,//���⼼��
	ITEM_RES_TYPE_PVP_REWARD = 34,//pvp����
	ITEM_RES_TYPE_MONEY_TREE = 35,//��-�����б�
	ITEM_RES_TYPE_MAP_GROUP = 36,//��ͼ����
	ITEM_RES_TYPE_DROP_MATERIAL = 38,//xxj 2015/04/20  19:48:54 ������Ʒ���ͣ�֧�ָ�����  0x26
	ITEM_RES_TYPE_CHANNEL = 39,//���� 0x27
	ITEM_RES_TYPE_DROP_CURRENCY = 40,//xxj 2015/04/17  14:20:43 ����������ͣ�֧�ָ�����  0x28
	ITEM_RES_TYPE_PIECE_ROLE_COST= 41,//��Ƭת��ɫ 0x29
	ITEM_RES_TYPE_RECHARGE= 42,//��ֵ 0x2A
	ITEM_RES_TYPE_BADGE = 43,//���� 0x2B
	ITEM_RES_TYPE_PROPS = 0x1B,	//����27
	ITEM_RES_TYPE_MAX = 64
};

#define ITEM_RES_TYPE_HANDBOOK_OFFSET 0x80

enum
{
	CONFIG_TYPE_EQUIPMENT = 0,
	CONFIG_TYPE_DROP,
	CONFIG_TYPE_MONSTER,
	CONFIG_TYPE_LUCK,
	CONFIG_TYPE_MALL,
	CONFIG_TYPE_MAP,
	CONFIG_TYPE_PLAYER,
	CONFIG_TYPE_SKILL,
	CONFIG_TYPE_IMPROVE,
	CONFIG_TYPE_STAGE,
	CONFIG_TYPE_LOOT_REPLACE,
	CONFIG_TYPE_FUNCTION,
	CONFIG_TYPE_VIP,
	CONFIG_TYPE_UNLOCK,
	CONFIG_TYPE_BENIFIT,
	CONFIG_TYPE_GIFT,
	CONFIG_TYPE_ACTIVITY,
	CONFIG_TYPE_BEHAVIOR,
	CONFIG_TYPE_MATERIAL,
	CONFIG_TYPE_FORMULA,
	CONFIG_TYPE_BASIC_PROPERTY,
	CONFIG_TYPE_GROWING,
	CONFIG_TYPE_EXTRA_SKILL,
	CONFIG_TYPE_PVP_REWARD,
	CONFIG_TYPE_MONEY_TREE,
	CONFIG_TYPE_MAP_GROUP,
	CONFIG_TYPE_GAME_CONFIG,
	CONFIG_TYPE_SECRET_CONFIG,
	CONFIG_TYPE_PVP100,
	CONFIG_TYPE_TEAM_LEVEL,
	CONFIG_TYPE_GRADE,
	CONFIG_TYPE_SIGN_UP,
	CONFIG_TYPE_GIFT_BAG_CODE,
	CONFIG_TYPE_DOUBLE_DROP,
	CONFIG_TYPE_TASK,
	CONFIG_TYPE_ACTIVE_MISSION,//�����
	CONFIG_TYPE_ACTIVE_PRIZE,//�����Ʒ
	CONFIG_TYPE_EQUIP_STEP,
	CONFIG_TYPE_ROBOT_PLAYER,
	CONFIG_TYPE_ROBOT_TEAM,
	CONFIG_TYPE_DROP_MATERIAL,//������Ʒ��֧�ָ�����
	CONFIG_TYPE_CHANNEL,//����
	CONFIG_TYPE_DROP_CURRENCY,//�������
	CONFIG_TYPE_LIMITED_ACTIVITY,//��ʱ�ʱ���
	CONFIG_TYPE_LIMITED_MISSION_ACTIVITY,//��ʱ������
	CONFIG_TYPE_GOLDEN_TOUCH_PROFIT,
	CONFIG_TYPE_GOLDEN_TOUCH_COST,
	CONFIG_TYPE_BUY_ENERGY_COST,
	CONFIG_TYPE_PIECE_ROLE_COST,//��Ƭת��ɫ
	CONFIG_TYPE_RECHARGE,		//��ֵ
	CONFIG_TYPE_BADGE,			//����
	CONFIG_TYPE_BADGELEVEL,		//������������
	CONFIG_TYPE_PROPS,
	CONFIG_TYPE_HONORLEVEL,		//pvpÿ������
	CONFIG_TYPE_HONORRANK,		//pvp��������
	CONFIG_TYPE_HONORCOST,		//pvp��������
};

enum
{
	SERVICE_STATUS_POWER_OFF,
	SERVICE_STATUS_AVAILABLE,
	SERVICE_STATUS_CLOSING,
	SERVICE_STATUS_UNAVAILABLE,
};

#define MAX_SERVICE_INFO_MSG_LEN 2048

/*
enum
{
	PROPERTY_ID_NONE = -1,
	PROPERTY_ID_HP = 0,
	PROPERTY_ID_DAMAGE,
	PROPERTY_ID_INCREASE_COIN,
	PROPERTY_ID_ALL_HP,
	PROPERTY_ID_ALL_DAMAGE,
	PROPERTY_ID_CRITICAL,
	PROPERTY_ID_ARMOR,
	PROPERTY_ID_ALL_CRITICAL,
	PROPERTY_ID_ALL_ARMOR,
	PROPERTY_ID_MAX
};
*/

//(0����,1Ѫ��,2����,3����,4���)

//char *GAME_EVENT_PROPERTY_UPDATE = "property update";
extern char *GAME_EVENT_PVE_TEAM_UPDATE;
extern char *GAME_EVENT_PLAYER_INFO_UPDATE;
extern char *GAME_EVENT_MAP_DONE;
//extern char *GAME_EVENT_LOOT_IGNORE_UPDATE;
extern char *GAME_EVENT_LEVEL_UP;
extern char *GAME_EVENT_VIP_LEVEL_UP;
extern char *GAME_EVENT_RESET_BENIFIT;
extern char *GAME_EVENT_MAX_LIMIT_CHANGE;
//extern char *GAME_EVENT_BATTLE_DATA_UPDATE;

enum
{
	PLAYER_PROPERTY_NONE = -1,
	PLAYER_PROPERTY_ATTACK = 0,
	PLAYER_PROPERTY_HP,
	PLAYER_PROPERTY_DEFENSE,
	PLAYER_PROPERTY_CRITICAL,
	PLAYER_PROPERTY_COIN,
	PLAYER_PROPERTY_MAX
};

/*
enum
{
	BATTLE_PROPERTY_ID_HP = 0,
	BATTLE_PROPERTY_ID_DAMAGE,
	BATTLE_PROPERTY_ID_CRITICAL,
	BATTLE_PROPERTY_ID_ARMOR,
	BATTLE_PROPERTY_ID_MAX
};
*/

#define RES_CURRENCY_COIN			0x14010001//����
#define RES_CURRENCY_MONEY			0x14020001
#define RES_CURRENCY_ENERGY			0x14030001//����
#define RES_CURRENCY_POWERPOINT		0x14040001//�Ͻ� ��ʯ
#define RES_CURRENCY_EXP			0x14050001
#define RES_CURRENCY_RECHARGE		0x14060001
#define RES_CURRENCY_HONOUR			0x14070001
#define RES_CURRENCY_PVP_ENERGY		0x14080001
#define RES_CURRENCY_FRIENDSHIP     0x14090001
#define RES_CURRENCY_DYNAMIC_ENERGY 0x140A0001//��������


#define REWARD_MAIL_TEMPLATE_ID 0x1A000001	//���mail
#define MONTH_CARD_MAIL_TEMPLATE_ID 0x1A000002//�¿�mail
#define FRIENDSHIP_MAIL_TEMPLATE_ID 0x1A000003//����ֵmail
#define FRIENDSHIP_MAIL_PLAYER_ID 0x1A000004//��ɫmail
#define FRIENDSHIP_MAIL_EQUIPMENT_ID 0x1A000005//װ��mail
#define FRIENDSHIP_MAIL_POST_ID 0x1A000006//�ʼ�����mail
#define FRIENDSHIP_MAIL_FIRST_RECHARGE 0x1A000007//�׳����mail
#define FRIENDSHIP_MAIL_LIMITED_ACTIVITE 0x1A000008 //��ֵ��������ʱ���
#define FRIENDSHIP_MAIL_LIMITED_ZHANDOULI 0x1A000009 //�·�ս�����а�
#define FRIENDSHIP_MAIL_PVP_DAY 0x1A00000A //PVP ÿ�ս���
#define FRIENDSHIP_MAIL_PVP_RANK 0x1A00000B //pvp �������а���
#define FIRST_RECHARGE_REWARD_ID 0x1600000C //�׳佱��id
#define FIRST_SHAREAD_ACTIVITE_ID 0x1600000D //����ý���


#define POST_MAIL_TITLE_STR "ϵͳ��Ϣ"

typedef struct tagPropertyComponent
{
	//float fValue;
	//float fScale;
	ACE_UINT32 unValue;
	ACE_UINT32 unScale;
}PropertyComponent;

//#define PLAYER_AMOUNT 6

typedef struct tagBaseItemInfo
{
	ACE_UINT32 unItemID;
}BaseItemInfo;

typedef struct tagMaterialInfo : public BaseItemInfo
{
	ACE_UINT32 unCount;
}MaterialInfo;

typedef struct tagPropsInfo : public BaseItemInfo
{
	ACE_UINT32 unCount;
}PropsInfo;

typedef struct tagCharacterInfo : public BaseItemInfo
{
	//ACE_UINT32 unCharacterID;
	ACE_UINT32 unLevel;
	char name[MAX_CHARACTER_LENGTH + 1];
}CharacterInfo;

typedef struct tagRankData
{
	ACE_UINT32 unRank;
	ACE_UINT32 unHonour;//����
	ACE_UINT32 unPoint;
}RankData;

//װ����Ϣ
typedef struct tagEquipmentItemInfo
{	
	ACE_UINT32 unLevel;
	ACE_UINT32 unStep;//���� Խ��Խţ��

	void ToJson(Json::Value &root);
}EquipmentItemInfo;

typedef union tagItemDetailInfo
{
	EquipmentItemInfo equipment;

	void ToJson(Json::Value &root);
}ItemDetailInfo;

typedef struct tagItemInfo
{
	ACE_UINT32 unResID;
	ItemDetailInfo info;

	void ToJson(Json::Value &root);
}ItemInfo;

typedef struct tagItemInfoExt
{
	ACE_UINT32 unCount;
	ItemInfo item;

	void ToJson(Json::Value &root);
}ItemInfoExt;

typedef struct tagPackageCell
{
	ACE_UINT32 unIdx;
	ACE_UINT32 unOldIndex;
	ItemInfoExt item;

	void ToJson(Json::Value &root);
}PackageCell;

/*
typedef struct tagOldPlayerInfo : public BaseItemInfo
{
	ACE_UINT32 unSelected;
	ACE_UINT32 unLevel;
}OldPlayerInfo;
*/

typedef struct tagPlayerInfo : public BaseItemInfo
{
	ACE_UINT32 unLevel;
	ACE_UINT32 unStep;//���� Խ��Խţ��
	ItemInfo weapon;//����
	ItemInfo armor;//��������
	ItemInfo belt;//����
	ItemInfo staff;//��ָ
	ACE_UINT32 unPlayerSkillID;
	ACE_UINT32 unSoulCount;//��Ƭ����

	void ToJson(Json::Value &root);
}PlayerInfo;

#define MAX_GLOBAL_DROP_COUNT 16
#define MAX_DROP_ENTITY_PER_LIST 128

typedef struct tagDropEntityInfo
{
	ACE_UINT32 unResID;
	//ACE_UINT32 unType;
	float fRate;
}DropEntityInfo;

typedef struct tagDropGroup
{
	//ACE_UINT32 unGlobalCount;
	//ACE_UINT32 globalDropList[MAX_GLOBAL_DROP_COUNT];
	ACE_UINT32 unGlobalDropID;
	ACE_UINT32 unGlobalCount;
	DropEntityInfo globalList[MAX_GLOBAL_DROP_COUNT];
	ACE_UINT32 unCount;
	DropEntityInfo list[MAX_DROP_ENTITY_PER_LIST];
}DropGroup;

//typedef struct tagDrop



//#define GET_MAP_UNLOCK(t,level) (((t >> ((level-1) * 3)) & 0x7) & 0x1)
//#define SET_MAP_UNLOCK(t,f,level) ((t & (~(0x7 << ((level-1) * 3)))) | ((f & 0x1) << ((level-1) * 3)))
//#define GET_MAP_STAR_LEVEL(t, level) ((((t >> ((level-1) * 3)) & 0x7) >> 1) & 0x3)
//#define SET_MAP_STAR_LEVEL(t,s,level) ((t & (~(0x7 << ((level-1) * 3)))) | (((s & 0x3) << ((level-1) * 3)) << 1))

#define MAX_MAP_COUNT 256
#define MAX_MAP_DIFFRENCE 3
#define MAP_REWARD_COUNT 3

#define MAX_SKILL_COUNT 16

#define MAX_PLAYER_SKILL_COUNT 32

#define TANG_MONK_ID 0x0A300001

//#define MAX_WEAPON_SKILL_COUNT 256

#define MAX_REWARD_ITEM 50

typedef struct tagRewardInfo
{
	ACE_UINT32 unCoin;//����
	ACE_UINT32 unMoney;//��� ��
	ACE_UINT32 unExp;//����ֵ
	ACE_UINT32 unPowerPoint;//�Ͻ�
	ACE_UINT32 unHonour;//����ֵ
	ACE_UINT32 unRecharge;
	ACE_UINT32 unEnergy;//����
	ACE_UINT32 unPVPEnergy;//pvp����
	ACE_UINT32 unFriendship;//�����
	ACE_UINT32 unDynamicEnergy;//��������
	ACE_UINT32 unCount;//list ����
	ItemInfoExt list[MAX_REWARD_ITEM];
}RewardInfo;

typedef struct tagDoubleDropInfo
{
	ACE_UINT32 unRawMapID;
	ACE_Byte   byType;
}DoubleDropInfo;

#define MAX_CONFIG_REWARD_COUNT 24

typedef struct tagConfigRewardInfo
{
	ACE_UINT32 unResID;
	ACE_UINT32 unCount;
}ConfigRewardInfo;

typedef struct tagConfigReward
{
	ACE_UINT32 unCoin;
	ACE_UINT32 unMoney;
	ACE_UINT32 unExp;
	ACE_UINT32 unPowerPoint;
	ACE_UINT32 unHonour;
	ACE_UINT32 unRecharge;
	ACE_UINT32 unEnergy;
	ACE_UINT32 unPVPEnergy;
	ACE_UINT32 unFriendship;
	ACE_UINT32 unDynamicEnergy;//��������
	ACE_UINT32 unCount;
	ConfigRewardInfo list[MAX_CONFIG_REWARD_COUNT];
}ConfigReward;

typedef struct tagTaskConfig
{
	ACE_UINT32 unID;
	ACE_UINT32 unCompleteCount;//��������
	ACE_UINT32 unPremiseTaskCase;
	ACE_UINT32 unNextTaskID;
	ACE_UINT32 unLevelCase;
	ACE_UINT32 unActiveEventType;//��������
	ACE_UINT32 unAttachID;//�Ǽ�����

	//ACE_UINT32 unExpiredTime;

	ACE_UINT32 unOtherCase; //����
	ConfigReward stReward;
}TaskConfig;

typedef struct tagTaskInfo
{
	ACE_UINT32 unTaskID;
	ACE_UINT32 unCurrentProgress;
	ACE_Byte byStatus;
}TaskInfo;

typedef struct tagActivenessInfo
{
	ACE_UINT32 unCompleteCount;
	ACE_UINT32 unLevelCase;
	ACE_UINT32 unActivenessPoints;
	ACE_UINT32 unActiveEventType;
	ACE_UINT32 unAttachID;
	ConfigReward stReward;
}ActivenessInfo;

typedef struct tagGoldenTouchProfit
{
	ACE_UINT32 unBaseProfit;
	ACE_UINT32 unCountProfit;
}GoldenTouchProfit;

typedef struct tagGoldenTouchCost
{
	ACE_UINT32 unTouchCost;
	ACE_UINT32 unCriticalOdds;
}GoldenTouchCost;

//pvp�������� �ս���
typedef struct tagHonorLevel
{
	ACE_UINT32 unLevel;//��λ�ȼ�
	ACE_UINT32 unlevelupHonor;//��λ��������
	ACE_UINT32 undayReward;//"�ս���behavior"
	tagHonorLevel()
	{
		ZeroVar(*this);
	}
}HonorLevel;

//pvp�������� �ս���
typedef struct tagHonorRank
{
	ACE_UINT32 rankid;//��λ�ȼ�
	ACE_UINT32 unrank_limited;//��λ��
	ACE_UINT32 season_reward;//"��������behavior"
	tagHonorRank()
	{
		ZeroVar(*this);
	}
}HonorRank;

enum
{
	DROP_LOCAL_TYPE = 0,
	DROP_GLOBAL_TYPE
};

#define MAX_INVITE_COUNT 20
#define MAX_FRIEND_COUNT 64
#define MAX_FRIEND_DELETE_COUNT 51

#define MAX_ASSIST_COUNT 256
typedef struct tagAssistInfo
{
	ACE_UINT32 unAssistPlayerID;
	ACE_Byte unAssistType;
}AssistInfo;

typedef struct tagEquipStepConfig
{
	ACE_UINT32 unEquipStep;
	ACE_UINT32 unMaxLevelBonus;
	float fAttributeRate;
}EquipStepConfig;

/*
typedef struct tagCharacterInfoExt : public CharacterInfo
{
	PackageCell equipmentList[MAX_EQUIPMENT_COUNT];
	PropertyComponent propertyList[PROPERTY_ID_MAX];
	ACE_UINT32 unSkillID;
	ACE_UINT32 unVipLevel;
}CharacterInfoExt;
*/


#define MAX_BSINFO_NUM 16
typedef struct tagBadgeShuxing
{
	//0�̶�ֵ 1�ٷֱ�
	int gongji_1_0;
	int gongji_1_1;
	int shengming_2_0;
	int shengming_2_1;
	int baoji_3_0;
	int baoji_3_1;
	int fangyu_4_0;
	int fangyu_4_1;
	int beike_5_0;
	int beike_5_1;
	int hejin_6_0;
	int hejin_6_1;
	ACE_UINT32 roleID;

	tagBadgeShuxing()
	{
		ZeroVar(*this);
	}
}BadgeShuxing;

typedef struct tagBadgeShuxingS
{
	ACE_UINT32 unCount;
	BadgeShuxing bsInfo[MAX_BSINFO_NUM];

	tagBadgeShuxingS()
	{
		ZeroVar(*this);
	}
}BadgeShuxingS;


typedef struct tagAssistFriendship
{
	ACE_UINT32 unCharacterID;
	ACE_UINT32 unAddFriendship;
}AssistFriendship;

typedef struct tagPlayerTeamInfo
{
	ItemInfo staff;
	ACE_UINT32 unCount;
	ACE_UINT32 unPoint;
	PlayerInfo playerList[MAX_PLAYER_SELECTED_COUNT];
	ACE_UINT32 skillList[MAX_PLAYER_SELECTED_COUNT];
	ACE_UINT32 playerSkillList[MAX_PLAYER_SELECTED_COUNT];
	ACE_UINT32 unStarCount;
	void ToJson(Json::Value &root);
}PlayerTeamInfo;

typedef struct tagCharacterInfoExt : public CharacterInfo
{
	tagCharacterInfoExt()
	{
		ZeroVar(*this);
	}
	//PackageCell equipmentList[MAX_EQUIPMENT_COUNT];
	//PropertyComponent propertyList[PROPERTY_ID_MAX];
	//PlayerTeamInfo teamInfo;
	PlayerTeamInfo pveTeam;
	PlayerTeamInfo pvpTeam;
	ACE_UINT32 unVipLevel;
	BadgeShuxingS badgeShuxing;
	void ToJson(Json::Value &root);
}CharacterInfoExt;

typedef struct tagFriendInfo : public CharacterInfoExt
{
	ACE_UINT32 unCooldown;
	ACE_UINT32 unStartTime;
	ACE_UINT32 unLastInteractionTime;
	ACE_UINT32 unInteractionFlag;
	//ACE_UINT32 unAddFriendshipFlag;
}FriendInfo;


typedef struct tagDBFriendInfo 
{
	ACE_UINT32 unCharacterID;
	ACE_UINT32 unCooldown;
	ACE_UINT32 unStartTime;
	ACE_UINT32 unLastInteractionTime;
	ACE_UINT32 unInteractionFlag;
	//ACE_UINT32 unAddFriendshipFlag;
}DBFriendInfo;

typedef struct tagPVPTeamInfo : public CharacterInfo
{
	ACE_UINT32 unScore;
	ACE_UINT32 unWin;
	ACE_UINT32 unAll;
	ACE_UINT32 unRankID;
	PlayerTeamInfo teamInfo;
}PVPTeamInfo;

#define MAIL_SENDER_NAME_LEN 32//���������ֳ���
#define MAIL_TITLE_LEN 24//���ⳤ��
#define MAIL_BODY_LEN 255//���ݳ���
#define MAIL_ATTACHMENT_LEN 5//��������
#define MAIL_FLAG_HAVE_READ 0x1//�Ѷ�
#define MAIL_FLAG_PUBLIC_MAIL 0x2//�����ʼ�
#define MAIL_FLAG_HAVE_DELETED 0x4//��ɾ��
#define MAIL_TIME_OUT (7 * 24 * 3600) //�ʼ���ʱʱ��

typedef struct tagMailAttachment
{
	tagMailAttachment() : m_dwItemID(0), m_dwItemCount(0)
	{
	}
	ACE_UINT32 m_dwItemID;
	ACE_UINT32 m_dwItemCount;
}MailAttachment;

typedef struct tagMailInfo
{
	tagMailInfo() : m_dwID(0), m_dwIDinTable(0), m_dwCharacterID(0), m_dwFlag(0), m_dwTimeoutDate(0), m_dwReadDate(0)
	{
		memset(m_strSenderName, 0, sizeof(char) *(MAIL_SENDER_NAME_LEN + 1));
		memset(m_strTitle, 0, sizeof(char) * (MAIL_TITLE_LEN + 1));
		memset(m_strBody, 0, sizeof(char) * (MAIL_BODY_LEN + 1));
		memset(m_aMailAttachment, 0, sizeof(MailAttachment) * MAIL_ATTACHMENT_LEN);
	}
	ACE_UINT32 m_dwID;
	ACE_UINT32 m_dwIDinTable;//�ض��ʼ�������
	ACE_UINT32 m_dwCharacterID; //��˭����0��ʾ�㲥
	ACE_UINT32 m_dwFlag;
	ACE_UINT32 m_dwTimeoutDate;//����ʱ��
	ACE_UINT32 m_dwReadDate;	//�Ķ�ʱ��
	MailAttachment m_aMailAttachment[MAIL_ATTACHMENT_LEN];//����
	char m_strSenderName[MAIL_SENDER_NAME_LEN + 1];//������
	char m_strTitle[MAIL_TITLE_LEN + 1];//����
	char m_strBody[MAIL_BODY_LEN + 1];//����
	void SetSenderName(const wchar_t * str)
	{
		ZeroVar(m_strSenderName);
		char rstrUtf8[MAIL_SENDER_NAME_LEN + 1];
		ZeroVar(rstrUtf8);
		int dwLen = -1;
		WideCharToMultiByte(CP_UTF8, 0, str, dwLen, &rstrUtf8[0], MAIL_SENDER_NAME_LEN, NULL, NULL);
		memcpy(m_strSenderName,rstrUtf8,MAIL_SENDER_NAME_LEN + 1);
	}
}MailInfo;

typedef struct tagPVPBasicInfo
{
	ACE_UINT32 playerList[MAX_PLAYER_SELECTED_COUNT];
	ACE_UINT32 playerSkillList[MAX_PLAYER_SELECTED_COUNT];
	//ACE_UINT32 honour;
}PVPBasicInfo;

typedef struct tagPVPRankInfo
{
	ACE_UINT32 unSeasonID;
	ACE_UINT32 unScore;
	ACE_UINT32 unAll;
	ACE_UINT32 unWin;
	int nOrder;
	ACE_UINT32 unRank;
	ACE_UINT32 unRewardSeasonID;
	ACE_UINT32 unRewardID;
	//ACE_UINT32 unRewardID;
}PVPRankInfo;

typedef struct tagDBPVPRankInfo
{
	ACE_UINT32 unRewardSeasonID;
	ACE_UINT32 unRewardID;
}DBPVPRankInfo;

/*
typedef struct tagTeamPointSnapInfo
{
	ACE_UINT32 unPoint;
	PlayerTeamInfo teamInfo;
}TeamPointSnapInfo;

typedef struct tagLeaderboardCell
{
	ACE_UINT32 *pScore;
	int unOrder;
}LeaderboardCell;
*/

enum
{
	PLAYER_POOL_TYPE_LEVEL = 0,
	PLAYER_POOL_TYPE_TEAM_RANK,
	PLAYER_POOL_TYPE_PVP_RANK,
	PLAYER_POOL_TYPE_MAX
};

typedef struct tagPlayerPoolPosition
{
	int nPoolID;
	int nIndex;
}PlayerPoolPosition;

typedef struct tagPublicPlayerInfo
{
	tagPublicPlayerInfo()
	{
		ZeroVar(*this);
	}
	CharacterInfoExt characterInfo;

	//PVPBasicInfo pvpBasicInfo;
	PVPRankInfo pvpRankInfo;
	//PlayerTeamInfo pvpTeamInfo;
	PlayerTeamInfo highestTeamInfo;

	PlayerPoolPosition poolPosition[PLAYER_POOL_TYPE_MAX];

	ACE_UINT32 unPVPOrderUpdateTime;

}PublicPlayerInfo;

/*
typedef struct tagTeamRankCell
{
	ACE_UINT32 unCharacterID;
	ACE_UINT32 unPoint;
	ACE_UINT32 unUpdateTime;
	PlayerTeamInfo teamInfo;
}TeamRankCell;

typedef struct tagPVPSeasonRankCell
{
	ACE_UINT32 unCharacterID;
	ACE_UINT32 unScore;
	ACE_UINT32 unAll;
	ACE_UINT32 unWin;
	ACE_UINT32 unUpdateTime;
}PVPSeasonRankCell;
*/

#define MAX_PVP_COMMAND_DATA_SIZE 4096

typedef struct tagPVPCommand
{
	ACE_UINT32 unRound;
	ACE_UINT32 unTurn;
	ACE_UINT32 unSkillIndex;
}PVPCommand;

typedef struct tagPVPOpponentInfo
{
	ACE_UINT32 unSeasonID;
	ACE_UINT32 unStartTime;
	ACE_UINT32 pvpType;
	ACE_UINT32 unSeed;
	CharacterInfoExt opponent;
}PVPOpponentInfo;

enum
{
	PVP_TYPE_SEASON = 0,
	PVP_TYPE_FRIEND
};

enum
{
	PVP_LEADERBOARD_TEAM_POINT = 0,//ս���� ���а�
	PVP_LEADERBOARD_SCORE,//���� ���а�
	PVP_LEADERBOARD_FRIEND,//���� ���а�
	PVP_LEADERBOARD_STAR_COUNT//�Ǽ� ���а�
};

enum
{
	PVP_STATUS_NORMAL = 0,
	PVP_STATUS_SEASON_END
};

#define PVP_WIN_SCORE 5
#define PVP_LOSE_SCORE 2
#define PVP_WIN_HONOUR 4
#define PVP_LOSE_HONOUR 2
#define PVP_HONOUR_FRIEND 1

#define MAX_RANDOM_CHARACTER 20
#define MAX_RANDOM_FRIEND 10

#define MAX_DOUBLE_DROP_COUNT 20

#define MAX_TOTAL_TASK_COUNT 64
#define MAX_CURRENT_TASK_COUNT 16
#define MAX_ACTIVENESS_COUNT 10
#define MAX_LIMITED_ACTIVITY_COUNT 32//��ʱ������

/*
#define MAX_ITEM_LIST_PER_PLAYER 8
#define MAX_MONSTER_PER_SECTION 32
#define MAX_ITEM_PER_SECTION 32


typedef struct tagBattleEntity
{
	int hp;
}BattleEntity;

typedef struct tagBattlePlayerInfo : public tagBattleEntity
{
	ACE_UINT32 itemList[MAX_ITEM_LIST_PER_PLAYER];
}BattlePlayerInfo;

typedef struct tagBattleMonsterInfo : public tagBattleEntity
{
}BattleMonsterInfo;

typedef struct tagBattleItemInfo
{
	int isDestroyed;
}BattleItemInfo;

typedef struct tagBattleEntityInfo
{
	BattlePlayerInfo playerList[MAX_PLAYER_SELECTED_COUNT + 2];
	BattleMonsterInfo monsterList[MAX_MONSTER_PER_SECTION];
	BattleItemInfo itemList[MAX_ITEM_PER_SECTION];
}BattleEntityInfo;

typedef struct tagMapBattleInfo
{
	int isValid;
	int nCurrentSection;
	int nLauchCount;
	int nBellCount;
	int nRandSeed;
	//int unPlayerCount;
	//ACE_UINT32 playerList[MAX_PLAYER_SELECTED_COUNT];
	BattleEntityInfo entityInfo;
}MapBattleInfo;
*/

typedef struct tagMapInstanceInfo
{
	ACE_UINT32 unMapID;
	ACE_UINT32 unSpecialBossID;
	ACE_UINT32 unItemCount;
	int nGlobalMapEventType;
	CharacterInfoExt friendInfo;
	//MapBattleInfo battleInfo;
	void ToJson(Json::Value &root);
}MapInstanceInfo;

/*
typedef struct tagBattleSnapInfo
{
	ACE_UINT32 unStep;
	ACE_UINT32 rebornFlag;
	BattleEntityInfo entityInfo;
}BattleSnapInfo;
*/

typedef struct tagMapFlagInfo
{
	ACE_UINT32 unMapID;
	ACE_UINT32 unFlag;
	ACE_UINT32 unFlag2;	//4λ��ʶһ���Ѷȵ����ô���
}MapFlagInfo;


typedef struct tagWeaponSkillInfo : public BaseItemInfo
{
	ACE_UINT32 unLevel;
}WeaponSkillInfo;

/*
enum
{
	PACKAGE_TYPE_PACKAGE = 0,
	PACKAGE_TYPE_PLAYER,
	PACKAGE_TYPE_EQUIPMENT,
};
*/

#define MAX_EQUIPMENT_LEVEL 30
#define MAX_SKILL_LEVEL 60

/*
typedef struct tagTeamMember
{
	ACE_UINT32 unItemID;
	PropertyComponent propertyList[PLAYER_PROPERTY_MAX];
	ACE_UINT32 unSkillID;
}TeamMember;
*/

/*
typedef struct tagPropertyInfo
{
	ACE_UINT32 unPropertyID;
	ACE_UINT32 unType;
	float fValue;
}PropertyInfo;

#define MAX_PROPERTY_COUNT 8

typedef struct tagPropertyList
{
	ACE_UINT32 unCount;
	PropertyInfo list[MAX_PROPERTY_COUNT];
}PropertyList;
*/

#define MAX_BENEFIT_COUNT 8

typedef struct tagBenefitInfo
{
	ACE_UINT32 unBenefitID;
	ACE_UINT32 unType;
	ACE_UINT32 unValue;
	//float fValue;
}BenefitInfo;

typedef struct tagBenefitList
{
	ACE_UINT32 unCount;
	BenefitInfo list[MAX_BENEFIT_COUNT];
}BenefitList;

#define MAX_MAIN_CHARACTER_ID 5

typedef struct tagCareerInfo
{
	ACE_UINT32 unCharacterID;
	ACE_UINT32 unEquipmentTypeID;
	ACE_UINT32 unSkillTypeID;
	ACE_UINT32 unRequiredPlayerID;
}CareerInfo;

enum
{
	EQUIPMENT_TYPE_WEAPON = 0x01,
	EQUIPMENT_TYPE_ARMOR = 0x02,
	EQUIPMENT_TYPE_STAFF = 0x03,
	EQUIPMENT_TYPE_WING = 0x04,
	EQUIPMENT_TYPE_BELT = 0x05,

	EQUIPMENT_TYPE_MAX = EQUIPMENT_TYPE_BELT,
};

enum
{
	LEVEL_UP_TYPE_EQUIPMENT = 0,//װ������
	LEVEL_UP_TYPE_EQUIPMENT_STEP,//����
	LEVEL_UP_TYPE_PLAYER//��� �������
};

#define MAX_EQUIPMENT_STEP 3
#define MAX_EQUIPMENT_LEVEL_PER_STEP 5

enum
{
	DELETE_ITEM_TYPE_SOLD = 0,//����ȥ
	DELETE_ITEM_TYPE_MELT//�ֽ� �ڻ�
};

enum
{
	GLOBAL_MAP_EVENT_NONE = -1,
	GLOBAL_MAP_EVENT_COIN_DOUBLE = 0,
	GLOBAL_MAP_EVENT_DROP_DOUBLE,
	GLOBAL_MAP_EVENT_MAX
};

//typedef struct 
#define GLOBAL_MAP_EVENT_COOLDOWN (2 * 60 * 60)

typedef struct tagPackageExpandInfo
{
	ACE_UINT32 unMin;
	ACE_UINT32 unMax;
	ACE_UINT32 unCountPerTime;
	ACE_UINT32 unGold;
}PackageExpandInfo;

enum
{
	GOLD_BUY_TYPE_ENERGY = 0,
	GOLD_BUY_TYPE_PACKAGE,
	GOLD_BUY_TYPE_FRIEND,
	GOLD_BUY_TYPE_PVP_ENERGY
};

enum
{
	STAGE_ITEM_TYPE_NONE = 0,
	STAGE_ITEM_TYPE_HP,
	STAGE_ITEM_TYPE_BELL,
	STAGE_ITEM_TYPE_BUFF
};

#define MAX_VIP_BENIFIT_COUNT 32

#define MAX_FUNCTION_COUNT 32

#define MAX_VIP_LEVEL 32

#define MAX_LOGIN_BONUS_COUNT 7

enum
{
	UNLOCK_CONDITION_TYPE_LEVEL_UP = 0,
	UNLOCK_CONDITION_TYPE_STAGE_DONE,
	UNLOCK_CONDITION_TYPE_VIP_UP,
	UNLOCK_CONDITION_TYPE_MAX
};


//1��½��Ԫ����2��Ѹ��3�������ޣ�4�������ޣ�5�������ޣ�6��������Ǯ��7�������Ӿ�ʯ��8����ʱ��������9��ʤ�������ӣ�10�齱�ۿۣ�11��ֵ����
enum 
{
	BENEFIT_TYPE_LOGIN_GOLD = 1,
	BENEFIT_TYPE_REBORN,
	BENEFIT_TYPE_PACKAGE_LIMIT,
	BENEFIT_TYPE_FRIEND_LIMIT,
	BENEFIT_TYPE_ENERGY_LIMIT,
	BENEFIT_TYPE_SOLD,
	BENEFIT_TYPE_MELT,
	BENEFIT_TYPE_ENERGY_RESTORE,
	BENEFIT_TYPE_MAP_REWARD,
	BENEFIT_TYPE_LUCK_COST,
	BENEFIT_TYPE_FIRST_RECHARGE,
	BENEFIT_TYPE_LOGIN_COIN,
	BENEFIT_TYPE_LOGIN_POWER_POINT,
	BENEFIT_TYPE_MAX
};

#define FUNCTION_EXCHANGE_EQUIPMENT		800000002
#define FUNCTION_IMPROVE_PLAYER			800000006
#define FUNCTION_IMPROVE_EQUIPMENT		800000007
#define FUNCTION_IMPROVE_SKILL			800000008
#define FUNCTION_STEPUP_EQUIPMENT		800000009
#define FUNCTION_GET_LUCK				800000010
#define FUNCTION_PVP					800000019


typedef struct tagGiftInfo
{
	ACE_UINT32 unGiftID;
	ACE_UINT32 unTimeStamp;
}GiftInfo;

#define MAX_GIFT_COUNT 32

enum
{
	USE_ITEM_TYPE_GIFT = 0,
	USE_ITEM_TYPE_PROPS = 1
};

typedef struct tagBattleUnitInfo
{
	int oldHP;
	int newHP;
	int damage;
	int heal;
	ACE_UINT32 itemList[8];
}BattleUnitInfo;

#define MAX_COMMAND_BUFF_SIZE 64

typedef struct tagCooldownInstanceInfo : public BaseItemInfo
{
	ACE_UINT32 unCount;
	ACE_UINT32 unLastUpdateTime;
}CooldownInstanceInfo;

#define MAX_COOLDOWN_INFO 64

#define MAX_ADMIN_OP_PARAM_COUNT 8

#define MAX_ADMIN_OP_BUFFER_LENGTH 128

#define MAX_ADMIN_OP_BUFFER_ANNOUNCEMENT_LENGTH 256//���泤��

//��̨����
enum
{
	ADMIN_OP_TYPE_GET_BASIC_INFO = 0,
	ADMIN_OP_TYPE_GET_PLAYER_LIST,
	ADMIN_OP_TYPE_GET_EQUIPMENT_LIST,
	ADMIN_OP_TYPE_GET_PACKAGE,
	ADMIN_OP_TYPE_GET_LOCK_FUNCTION_LIST,
	ADMIN_OP_TYPE_GET_SKILL_LIST,
	ADMIN_OP_TYPE_SET_LEVEL,
	ADMIN_OP_TYPE_SET_COIN,
	ADMIN_OP_TYPE_ADD_COIN,//���ӱ���
	ADMIN_OP_TYPE_SET_MONEY,
	ADMIN_OP_TYPE_ADD_MONEY,//���ӽ��
	ADMIN_OP_TYPE_SET_POWER_POINT,
	ADMIN_OP_TYPE_ADD_EQUIPMENT,//����װ��
	ADMIN_OP_TYPE_DELETE_EQUIPMENT,
	ADMIN_OP_TYPE_EDIT_EQUIPMENT,
	ADMIN_OP_TYPE_ADD_PLAYER,//���ӽ�ɫ
	ADMIN_OP_TYPE_DELETE_PLAYER,
	ADMIN_OP_TYPE_EDIT_PLAYER,
	ADMIN_OP_TYPE_SET_SKILL_LEVEL,
	ADMIN_OP_TYPE_UNLOCK,
	ADMIN_OP_TYPE_DISABLE_ACCOUNT,//��ͣ
	ADMIN_OP_TYPE_ENABLE_ACCOUNT,//���
	ADMIN_OP_TYPE_ADD_GIFT_BAG_CODE,//�����
	ADMIN_OP_TYPE_STATISTICS,//ͳ��
	ADMIN_OP_TYPE_MAIL_POST,//�ʼ�����
	ADMIN_OP_TYPE_GM_POST,//GM����GM����
};


#define  MAX_GM_USER_COUNT 10//���10��
enum
{
	ADMIN_OP_GM_ALL = 0,//Ĭ��
	ADMIN_OP_GM_PUT = 2,//��Ӫ��Ϣ

	ADMIN_OP_GM_USER = 10,//��ҫ��Ϣ�ϵ�

	ADMIN_OP_GM_LUCK_EQUIPMENT = 11,//�齱 ���3�ǻ�����װ��
	ADMIN_OP_GM_LUCK_PLAYER = 12,//�齱 ��ý�ɫ
	ADMIN_OP_GM_STEP_EQUIPMENT = 13,//����װ��
	ADMIN_OP_GM_STEP_PLAYER = 14,//���׽�ɫ
	ADMIN_OP_GM_COMPOSE =15,//�ϳ�װ��
	ADMIN_OP_GM_PROPS = 16	//����
};

/*
#define GAME_COIN_ID	0x14010001
#define GAME_GOLD_ID	0x14020001
#define GAME_ENERGY_ID	0x14030001
#define GAME_POWER_POINT_ID	0x14040001
#define GAME_EXP_ID	0x14050001
#define GAME_RECHARGE_ID	0x14060001
#define GAME_HONOUR_ID	0x14070001
*/

#define MAX_COST_TYPE_COUNT 7

typedef struct tagCostInfo
{
	ACE_UINT32 unID;
	int cost;
}CostInfo;

typedef struct tagCostInfoList
{
	ACE_UINT32 unCount;
	CostInfo list[MAX_COST_TYPE_COUNT];
}CostInfoList;

typedef struct tagCooldownInfo
{
	ACE_UINT32 unCD;
	ACE_UINT32 unPrecision;
	ACE_UINT32 unCountLimit;
	ACE_UINT32 unOffset;
}CooldownInfo;



#define MAX_RELATED_ID_PER_ACTIVITY 8

/*
typedef struct tagLimitInfo : public BaseItemInfo
{
	ACE_UINT32 unCountLimit;
	ACE_UINT32 unActivityLimit;
}LimitInfo;
*/


#define COMMON_MAP_STAGE_COUNT 5
#define COMMON_MAP_SUB_STAGE_COUNT 7


#define MAX_SECURITY_KEY_COUNT 80000
#define SECURITY_KEY_LENGTH 8

enum
{
	BROADCAST_MESSAGE_TYPE_GET_EQUIPMENT = 0,
	BROADCAST_MESSAGE_TYPE_STEP_UP,
	BROADCAST_MESSAGE_TYPE_LEVEL_UP,
	BROADCAST_MESSAGE_TYPE_VIP,
	BROADCAST_MESSAGE_TYPE_GET_PLAYER
};

typedef struct tagBroadcastMessage
{
	char name[MAX_CHARACTER_LENGTH + 1];
	ACE_UINT32 unType;
	ACE_UINT32 unItemID;
	ACE_UINT32 unParam;
// 	ACE_UINT32 unCoin;//����
// 	ACE_UINT32 unPowerPoint;//�Ͻ�
}BroadcastMessage;

#define MAX_BROADCAST_MESSAGE_COUNT 10


//#define BEHAVIOUR_BUY_ENERGY 0x16000001
#define BEHAVIOR_MONEY_TREE_WATERING 0x16100019
#define BEHAVIOR_MONEY_TREE_REWARD 0x16100020
#define BEHAVIOR_EAT_RSG_REWARD 0x16000022
#define BEHAVIOR_EVERYDAY_REWARD 0x16000023 //ÿ�յ�½���

#define SECRET_PAID_REFRESH_TIMES_COUNT_UPPER_BOUND 15    //��Ҫ�����ֵ����Ϊ�漰����λ��������ݴ洢�����

enum
{
	TYPE_PVE = 0,
	TYPE_PVP
};

enum SECRET_REFRESH_TYPE
{
	SECRET_REFRESH_FREE = 0,
	SECRET_REFRESH_1,
	SECRET_REFRESH_2,
	SECRET_REFRESH_3,
	SECRET_REFRESH_4
};

typedef struct tagSecretItem
{
	ACE_UINT32 unId;
	ACE_UINT32 unCount;
}SecretItem;   //����������������

typedef struct tagSecretCfg   //jinpan  secretshop
{
	ACE_UINT32 unCell;					//�������ӵĸ���
	ACE_UINT32 certainList[MAX_SECRET_LIST];//�س��б�
	ACE_UINT32 unCertainCount;				//�س���Ʒ�ĸ���
	
	ACE_UINT32 unOtherItems[MAX_SECRET_LIST];  //������Ʒ������ܱ�
	ACE_UINT32 unOtherCount;
}SecretCfg;


typedef struct tagSecret{
	SecretCfg secretCfg[MAX_SECRET_LIST];
	ACE_UINT32 unCount;
	ACE_UINT32 unRefreshSec;
	ACE_UINT32 unMaxCell;

	SecretItem cellPrice[MAX_SECRET_LIST];
	SecretItem specialAmount[MAX_SECRET_LIST];
	SecretItem moenyRefresh;//�ͼ�Ԫ��ˢ�� type 1
	SecretItem itemRefresh;//�ͼ�����ˢ�� type 2
	SecretItem higher_moenyRefresh;//�߼����ˢ�� type3
	SecretItem higher_itemRefresh;//�߼�����ˢ��	type 4

	ACE_UINT32  unHigher_moneyRefresh_increase;
	ACE_UINT32  unHigher_moneyRefresh_increaseUpperBound;
	
}Secret;

typedef struct tagSecretItemInfo : MaterialInfo{
	ACE_UINT32 bBuy;//�Ƿ��Ѿ����� true��ʾ�Ѿ������ˣ�false ��ʾδ������
}SecretItemInfo;
typedef struct tagsSecretInfo{
	SecretItemInfo m_list[MAX_SECRET_LIST];  //��Ʒ��id�͸���
	ACE_UINT32 m_unCellCount;//��ǰӵ�и�����
	ACE_UINT32 m_refreshTimes;//ˢ�´���
	ACE_UINT32 m_unLastRefreshTime;//�ϴ�ˢ��ʱ��

}SecretInfo;

enum GIFT_BAG_CODE_TYPE
{
	GIFT_BAG_CODE_ONCE = 0,
	GIFT_BAG_CODE_EVERYONE = 1,
	GIFT_BAG_CODE_GM = 2,

	GIFT_BAG_CODE_INVALID = 3,
};

typedef struct tagAdminGiftBagCode
{
	char strCode[MAX_GIFT_BAG_CODE_LENGTH+1];
	ACE_UINT32 unGiftBagType;  //������ͣ�ÿ���������ÿ��ֻ����һ��
	ACE_UINT32 unCodeType;     //�����ͣ�0:����ֻ�ܱ�һ���ˣ�1�������ܱ��������ã� 2�������ã������Ʋ��Ǳ�
	//ACE_Byte   byUsed;         //�Ƿ�������ʹ�ù���ֻ������0�����ø�����
}AdminGiftBagCode;

typedef struct tagGiftBagCodeInfo
{
	ACE_UINT32 unGiftBagType;  //������ͣ�ÿ���������ÿ��ֻ����һ��
	ACE_UINT32 unCodeType;     //�����ͣ�0:����ֻ�ܱ�һ���ˣ�1�������ܱ��������ã� 2�������ã������Ʋ��Ǳ�
	ACE_Byte   byUsed;         //�Ƿ�������ʹ�ù���ֻ������0�����ø�����
}GiftBagCodeInfo;

//xxj 2015/05/06 15:01:27 
typedef struct tagGMInfo
{
	ACE_UINT32 unFromID;  //����ԭ��ַ
	ACE_UINT32 unToID;  //����Ŀ���ַ
	ACE_UINT32 unType;			//����
	ACE_UINT32 unGoodID;		//��Ʒid
	ACE_UINT32 unLevel;			//�ȼ�
	//ACE_UINT32 unBufferLen;
	char fromName[MAX_CHARACTER_LENGTH + 1];  //����Դ��ַ����
	char strBuffer[MAIL_BODY_LEN + 1];//����
}GMInfo;

typedef struct tagGiftBagCodeReward
{
	time_t tExpiredTime;
	ACE_UINT32 unGiftBagID; 
	//ConfigReward oReward;//���һ�����ID��Ӧһ������������������û��Ҫ�洢��
}GiftBagCodeReward;

//----------------wangwei
typedef struct tagWingItemInfo : public BaseItemInfo{
	ACE_UINT32 unPlayerId;		//��ɫid��δװ��Ϊ0
	ACE_UINT32 unIdx;		//����index��װ����Ϊ0����ע���޸ı���indexʱ������Ҫ�涯
	ACE_UINT32 unExpires;		//ʣ��ʱ��
}WingItemInfo;
typedef struct tagWingInfo{
	WingItemInfo m_list[MAX_WING_LIST];
	ACE_UINT32 m_count;
}WingInfo;
//----------------------

//#define MAX_SECRET_CELL 10  //��������

typedef struct tagSkillBook{
	ACE_UINT32 unId;
	ACE_UINT32 unCount;
}SkillBook;



#define MAX_TEAM_RANK_100_COUNT 1000  //added by jinpan for 100pvp
#define MAX_PVP100_STAGE_COUNT 10   //���ֳ�20��
#define MAX_PVP100_GROUP_COUNT 100 //ÿ���ڵ��������
#define MAX_PVP100_CFG  24  //����ն����24��

typedef struct tagPvp100{
	ACE_UINT32 unStart;  //��ǰ���ο�ʼʱ��ս����
	ACE_UINT32 unEnd;	 //��ǰ���ν���ʱ��ս����
}Pvp100;

typedef struct tagPvp100Cfg{
	ACE_UINT32 unStage;		//pvp100�Ľ׶�
	ACE_UINT32 unCount;  //��ɱ��׶���Ҫնɱ������
	Pvp100 points;			//��ǰ�׶�ƥ��Ĺ�����
	CostInfoList reward;//����
}Pvp100Cfg;

typedef struct tagPvp100ListCfg{
	Pvp100Cfg pvp100Cfg[MAX_PVP100_CFG];
	ACE_UINT32 unCount;//һ�������˶�����
}Pvp100ListCfg;

typedef struct tagPvp100_node{	
	ACE_UINT32 unId;
	ACE_UINT32 unScore;
	tagPvp100_node *next;
}Pvp100_node;

typedef struct tagPvp100Group{
	Pvp100_node *head;//ͷָ��
	Pvp100_node *cur;//��ǰָ��ָ��
	ACE_UINT32 unCount;
}Pvp100Group;

typedef struct tagTeamLevel
{
public :
	tagTeamLevel() :
		level(0), levelup_exp(0), extraskill_slots(0),
		extraskill_learn(0), player_levelmax(0), equip_levelmax(0)
	{ }
	int level;
	int levelup_exp;
	int extraskill_slots;
	int extraskill_learn;
	int player_levelmax;
	int equip_levelmax;
}TeamLevel;
//  typedef struct tagPvp100List{
//  	Pvp100Group list[MAX_PVP100_STAGE_COUNT];
// }Pvp100List; //pvp100���չ��������е�1000�� 

typedef struct tagSignUpGift
{
	tagSignUpGift() : m_dwDate(0), m_dwVipCheck(0), m_dwRewardID(0),
		m_dwRewardCount(0), m_dwVipRewardID(0), m_dwVipRewardCount(0)
	{ }
	int m_dwDate;
	int m_dwVipCheck;
	int m_dwRewardID;
	int m_dwRewardCount;
	int m_dwVipRewardID;
	int m_dwVipRewardCount;
}SignUpGift;

typedef struct tagRoleStepInfo
{
	int step;
	int upgrade_cost;
	ACE_UINT32 role_lv_require;//��ɫ����Ҫ���ɫ�ȼ�
}RoleStepInfo;

//������Ϣ
typedef struct tagSkillInfo
{
	tagSkillInfo()
	{
		skillID = 0;
		skillLevel = 0;
		memset(m_strName,0,MAX_ITEM_NAME + 1);
	}
	ACE_UINT32 skillID;//����id
	ACE_UINT32 skillLevel;//���ܵȼ�
	char m_strName[MAX_ITEM_NAME + 1];//��������
	void ToJson(Json::Value &root);
}SkillInfo;

typedef struct tagAccountBasicInfo
{
	ACE_UINT32 m_unCharacterID;
	char m_email[MAX_EMAIL_LENGTH + 1];
	char m_mac[MAX_ACCOUNT_LENGTH + 1];
	char m_PlayerName[MAX_CHARACTER_LENGTH + 1];
	SkillInfo m_skillList[MAX_SKILL_COUNT];//�����б�
	ACE_UINT32 m_unLevel;
	ACE_UINT32 m_unExp;
	ACE_UINT32 m_unCoin;
	ACE_UINT32 m_unMoney;
	ACE_UINT32 m_unPowerPoint;
	ACE_UINT32 m_unHonour;
	ACE_UINT32 m_unLastLoginTime;
	ACE_UINT32 m_unLoginBonusCount;
	ACE_UINT32 m_unLoginCount;
	ACE_UINT32 m_unEnergy;
	ACE_UINT32 m_unEnergyRestoreTime;
// 	ACE_UINT32 m_unPVPEnergy;
// 	ACE_UINT32 m_unPVPEnergyRestoreTime;
	ACE_UINT32 m_unRechargeCount;
	ACE_UINT32 m_unRechargeAmount;
	ACE_UINT32 m_unPackageMax;
	ACE_UINT32 m_unTutorialFlag;//�����������
	ACE_UINT32 m_unUseLuckCount;
	ACE_UINT32 m_unCreateTime;//xxj 2015/04/23  11:29:41 ����ʱ��
	ACE_UINT32 m_unPoint;//ս��
	ACE_UINT32 m_unChannel;//����
	
	void ToJson(Json::Value &root);
}AccountBasicInfos;

typedef struct tagAccountBriefInfo
{
	ACE_UINT32 m_unCharacterID;
	char m_mac[MAX_ACCOUNT_LENGTH + 1];
	char m_email[MAX_EMAIL_LENGTH + 1];
	char m_PlayerName[MAX_CHARACTER_LENGTH + 1];

	void ToJson(Json::Value &root);
}AccountBriefInfo;

typedef struct tagAccountBriefInfoList
{
	ACE_UINT32 m_AccountCount;
	AccountBriefInfo m_oBriefInfos[MAX_ACCOUNT_BRIEF_INFO_LEN];

	void ToJson(Json::Value &root);
}AccountBriefInfoList;

typedef struct tagAccountItemInfo
{
	ACE_UINT32 m_unItemsCount;
	PackageCell m_itemsList[MAX_ITEMS_COUNT];

	void ToJson(Json::Value &root);
}AccountItemInfos;

typedef struct tagAccountPlayerInfo
{
	ItemInfo m_staff;
	ACE_UINT32 m_selectedPlayerList[MAX_PLAYER_SELECTED_COUNT];
	PlayerInfo m_playerList[MAX_PLAYER_COUNT];
	ACE_UINT32 m_selectedPlayerSkillList[MAX_PLAYER_SELECTED_COUNT];

	void ToJson(Json::Value &root);
}AccountPlayerInfos;

//ɾ�������б�
struct DeleteFriendList
{
	ACE_UINT32 m_time;//�������ʱ��
	ACE_UINT32 m_count;
	ACE_UINT32 m_delete_list[MAX_FRIEND_DELETE_COUNT];//ɾ�����ѵ�id
};

/*typedef struct tagGiftBagCodeDBItem
{
	char m_strGiftBagCode[MAX_GIFT_BAG_CODE_LENGTH];
	ACE_UINT32 m_unCodeType;
	ACE_UINT32 m_unGiftBagType;
	ACE_Byte   m_byUsed;
	//ACE_UINT32 m_unExpiredTime;
}GiftBagCodeDBItem;*/

typedef struct tagAdminStatistics
{
	char strDate[11];
	ACE_UINT32 unStatisticsType;  
}AdminStatistics;

enum E_Statistics_Type
{
	E_Statistics_Character_Amount = 0,//��ɫ��
	E_Statistics_Daily_Online = 1,//��������
	E_Statistics_Nextday_Remain = 2,//��������
	E_Statistics_3days_Remain = 3,//3������
	E_Statistics_7days_Remain = 4,//7������
	E_Statistics_15days_Remain = 5,//15������
	E_Statistics_30days_Remain = 6,//30������

	E_Statistics_ACU = 7,   //ÿ��ƽ���������� �Ȱ�10���ӳ���
	E_Statistics_PCU = 8,   //ÿ��ͬʱ���������ķ�ֵ
	E_Statistics_Tutorial= 9,//������������
	E_Statistics_Effect_Character= 10,//��Ч�û�
	E_Statistics_Effect_Register= 11,//ע���ʺ�
	E_Statistics_Effect_Login= 12,//��¼�ʺ�
	E_Statistics_lost_Login= 13,//�û���ʧ

	//Add before this line
	E_Statistics_ItemAmount,
};


#define Statistics_ACU_Interval    (600 * 1000)

#define Statistics_ACU_Num (144)

#define Statistics_Reload_Interval (60 * 1000)

//�����¼�����
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

enum E_TASK_STATUS
{
	TASK_STATUS_OPEN = 1,	//����״̬
	TASK_STATUS_FINISHED = 2,	//���״̬
	TASK_STATUS_REWARED = 3,	//����� ����ȡ

	TASK_STATUS_ERROR,
};

enum E_TASK_OPERATOR
{
	TASK_OPT_LIST = 1,
	TASK_OPT_ADD = 2,//���
	TASK_OPT_GETREWARD = 3,//�콱
	TASK_OPT_DAILY_LIST = 4,
	TASK_OPT_DAILY_ADD = 5,
	TASK_OPT_DAILY_GETREWARD = 6,
	TASK_OPT_DAILY_OBTAINEDREWARD = 7,
	TASK_OPT_DAILY_ACTIVENESS = 8,
	TASK_OPT_DAILY_SINGLE_REWARD = 9,
};

enum E_TASK_LIST_TYPE
{
	TASK_LIST_MAIN = 1,
	TASK_LIST_DAILY = 2,
};

struct ActiveEvent 
{
	ActiveEvent() {
		Reset();
	};

	void Reset()
	{
		m_byTaskStatus = TASK_STATUS_OPEN;
		m_unCurrentProgress = 0;
	}

	ACE_Byte m_byTaskStatus;

	ACE_UINT32 m_unTaskID;
	ACE_UINT32 m_unActiveEventType;
	//ACE_UINT32 m_unAttachObj;
	ACE_UINT32 m_unCurrentProgress;
};

class TaskComp
{
public:
	bool operator () (const ActiveEvent &obj_left, const ActiveEvent &obj_right) const
	{
		return obj_left.m_unTaskID < obj_right.m_unTaskID;
	}
};

typedef struct tagCommunicationRecord
{
	ACE_UINT32 unCmd;
	ACE_UINT32 unRecvTime;
	ACE_UINT32 unSendTime;
}CommunicationRecord;

//��ʱ����

//��ʱ�ʱ���
#define MAX_RelatedT_ITEMS 20
typedef struct tagLimitedActivityConfig
{
	ACE_UINT32 unID;
	ACE_UINT32 unStartTime;//��ʼʱ��
	ACE_UINT32 unEndTime;//����ʱ��
	ACE_UINT32 unWeekCircle;//һ������ѭ��
	ACE_UINT32 unDayTimeStart;//һ��Ŀ���ʱ�俪ʼ
	ACE_UINT32 unDayTimeEnd;//һ��Ŀ���ʱ�������
	ACE_UINT32 unRelatedType;//�������ͱ��
	ACE_UINT32 unRelatedCount;//�������ݳ���
	ACE_UINT32 unRelatedItems[MAX_RelatedT_ITEMS];//��������
	ACE_UINT32 unCloseTime;//�ر�ʱ��
}LimitedActivityConfig;


//��ʱ������
#define MAX_PreMission_COUNT 4
#define MAX_MissionPrize_COUNT 10
typedef struct tagMissionPrize
{
	ACE_UINT32 unID;
	ACE_UINT32 unCount;
}MissionPrize;

typedef struct tagLimitedActivityMissionConfig
{
	ACE_UINT32 unID;
	ACE_UINT32 unActiviID;//��Ӧ�Ļid
	ACE_UINT32 unLvLimit;//�����ȼ�
	ACE_UINT32 unPreMissionID[MAX_PreMission_COUNT];//ǰ������ID
	ACE_UINT32 unMissionType[3];//���ͱ��
	ConfigReward stReward;//����Ʒ
	//MissionPrize unPrizeItems[MAX_MissionPrize_COUNT];//����Ʒ
}LimitedActivityMissionConfig;

//��Ӧ���ݿ���Ϣ
#define ZeroVar(p) ZeroMemory( &p, sizeof( p ) )
typedef struct tagLimitedActivityInfo
{
	tagLimitedActivityInfo()
	{
		ZeroVar(*this);
	}
	//ACE_UINT32 unCharacterID;//��ɫid
	ACE_UINT32 unLAID;//��ʱ�id
	ACE_UINT32 unCloseTime;//�ر�ʱ��
	ACE_UINT32 unProgress;//��������(�Ⱦ�����)
	ACE_UINT32 unDealTime;//�������ʱ��
	ACE_UINT32 unLinkType;//��������
	ACE_UINT32 unRelatedCount;//����ȡ�б���
	ACE_UINT32 unRelatedItems[MAX_RelatedT_ITEMS];//����ȡ�б�
}LimitedActivityInfo;

/*
"���ͱ�ţ���������1����������2����
��š�0�����������࣬����1ID������2ID,��
��š�1�������̵��ۿ������࣬�������ͣ��ۿ���
��š�2���ؿ�����˫�����ؿ�1ID���ؿ�2ID����
��š�3���ؿ����ļ��룬�ؿ�1ID���ؿ�2ID����
��š�4���ؿ���Դ˫�����ؿ�1ID���ؿ�2ID����
��š�5���ؿ�����˫�����ؿ�1ID���ؿ�2ID����
��š�6��װ��ǿ���ۿۣ��ۿ���
��š�7����ɫ�����ۿۣ��ۿ���
��š�8���ȼ����л������1ID������2ID����
��š�9��ս�����л������1ID������2ID����
��š�10���������л������1ID������2ID����
��š�11����ʱ����������1ID������2ID����
��š�12�����߶һ��������1ID������2ID����"
*/
enum E_L_A_TYPE
{
	LA_TYPE_LINK= 0,
	LA_TYPE_1 = 1,
	LA_TYPE_2 = 2,
	LA_TYPE_3 = 3,
	LA_TYPE_4 = 4,
	LA_TYPE_5 = 5,
	LA_TYPE_6 = 6,
	LA_TYPE_7 = 7,
	LA_TYPE_9 = 9,
	LA_TYPE_11 = 11,
};

/*
���ʽ:����1����,����1����(16����),��ɴ���
���͡�1001����ֵ�࣬0������
���͡�1002�������࣬��ԴID������
���͡�1003��ʮ���飬�������1/��2��������
���͡�1004�����飬���͡������1/��2��������
���͡�1005��������½���ۼƵ�½���������֡���0
���͡�1006�����н����࣬�������ޣ���������
���͡�1007���һ��࣬0��0
*/
#define LA_MISSION_TYPE_1001 1001
#define LA_MISSION_TYPE_1002 1002
#define LA_MISSION_TYPE_1003 1003
#define LA_MISSION_TYPE_1004 1004
#define LA_MISSION_TYPE_1005 1005
#define LA_MISSION_TYPE_1006 1006


typedef struct tagBadgeInfo
{
	tagBadgeInfo(ACE_UINT32 id = 0)
	{
		ZeroVar(*this);
		unID = id;
		m_flag = 1;
	}

	ACE_UINT32 unID;//��Դid
	ACE_UINT32 m_gain;//�ӳ�
	ACE_UINT32 m_charge_value;//����ֵ
	ACE_UINT32 m_flag;
}BadgeInfo;

//add by xxj 2015/08/18  16:01:47 
//pvp ���
//pvp��Ϣ
#define DAY_TIME_SETTLE_START (47*30*60) //���� 23:30 ��ʼ
#define DAY_TIME_SETTLE_END (24*60*60-1) //���� 23:59:59 ����
#define DAY_TIME_SETTLE_STARTEX (21*60*60) //���㿪ʼ21:00 ÿ�ս���
#define DAY_TIME_SETTLE_ENDEX (21*60*60 + 30*60) //���� 21:15 ����

#define MAX_FightRecordInfo_LEN 10		//��¼�������

typedef struct tagPVPInfo
{
	ACE_INT32  nhonour;//����ֵ
	ACE_UINT32 unAttackTimes;//��ս����
	ACE_UINT32 unLastChangeTime;//������ʱ��
	ACE_UINT32 unWinLost;//ʤ������16λһ�� 0x0FFFFǰ16λʤ����0xFFFF0000��16λʧ��
	ACE_UINT32 unBuyCount;//�������
	ACE_UINT32 unFreshenCount;//ˢ�´���
	tagPVPInfo()
	{
		ZeroVar(*this);
	}
}PVPInfo;

typedef struct tagPVPInfoEx:public tagPVPInfo
{
	ACE_INT32  nhonourRank;//����ֵ����
	tagPVPInfoEx()
	{
		ZeroVar(*this);
	}
}PVPInfoEx;

typedef struct tagHLInfo
{
	tagHLInfo()
	{
		ZeroVar(*this);
		isRobt = false;
	}

	bool operator>(const tagHLInfo& rhs)const//��������ʱ����д�ĺ���
	{   
		return m_honour > rhs.m_honour; 
	}

	ACE_UINT32 m_characterID;
	ACE_INT32  m_honour;
	bool isRobt;
}HLInfo;

typedef struct tagHLInfoArray
{
	int uncount;
	HLInfo m_array[3];
	CharacterInfoExt playerinfo[3];
	tagHLInfoArray()
	{
		uncount = 0;
	}
}HLInfoArray;

#define MAX_HONOUR_LEVEL_COUNT 50

typedef struct tagHLUserInfo
{
	ACE_UINT32 m_userID;//�˻�id
	ACE_UINT32 m_roleHeadID;//�ӳ�id
	ACE_UINT32 m_unStep;//�ӳ�����
	ACE_INT32 m_honour;//����ֵ
	char name[MAX_CHARACTER_LENGTH + 1];//����
	tagHLUserInfo()
	{
		ZeroVar(*this);
	}
}HLUserInfo;

typedef struct tagFightScoreInfo//�÷ּ�¼
{
	ACE_UINT32 unCount;//��������
	ACE_UINT32 role_list[MAX_PLAYER_SELECTED_COUNT];//��ɫid
	ACE_UINT32 setup_list[MAX_PLAYER_SELECTED_COUNT];//��ɫ����
	ACE_INT32 score_list[MAX_PLAYER_SELECTED_COUNT];//�÷�����
	tagFightScoreInfo()
	{
		ZeroVar(*this);
	}
}FightScoreInfo;

typedef struct tagFightUserInfo
{
	ACE_UINT32 m_characterID;//�˻�id
	ACE_UINT32 m_level;//�ȼ�
	ACE_INT32 m_honour;//����ֵ
	char name[MAX_CHARACTER_LENGTH + 1];//����
	FightScoreInfo fightInfo;//�÷ּ�¼

	tagFightUserInfo()
	{
		ZeroVar(*this);
	}
}FightUserInfo;

typedef struct tagFightRecordInfo//��¼
{
	ACE_INT32 score;//�����ߵ÷�
	ACE_INT32 isWin;//�������Ƿ��ʤ
	FightUserInfo from;//������
	FightUserInfo to;//��������

	tagFightRecordInfo()
	{
		ZeroVar(*this);
	}
}FightRecordInfo;

typedef struct tagFightRecordInfoEx//��¼
{
	FightRecordInfo info;
	bool isRoobt;//�Ƿ������
}FightRecordInfoEx;

//ս���б�
typedef struct tagFightRecordInfoList
{
	ACE_UINT32 unCount;
	FightRecordInfo info[MAX_FightRecordInfo_LEN];
}FightRecordInfoList;

//����˺�ֵ
typedef struct tagFightRoleHurtInfo
{
	ACE_UINT32 unCharacterID;
	ACE_UINT32 unRoleID;
	ACE_UINT32 unHurt;//�˺�
}FightRoleHurtInfo;
//-----------------------------------------end

//------------------------------��������˺����а� ���
#define MAX_FIGHT_LEVEL_COUNT 50
typedef struct tagFightPlayerInfoBase
{
	ACE_UINT32 m_userID;//�˻�id
	ACE_UINT32 m_roleID;//����˺���ɫid
	ACE_UINT32 m_unStep;//��ɫ����
	ACE_INT32 m_hurt;//�˺�ֵ
	tagFightPlayerInfoBase()
	{
		ZeroVar(*this);
	}
}FightPlayerInfoBase;

typedef struct tagFightPlayerInfo
{
	ACE_UINT32 m_level;//�ȼ�
	FightPlayerInfoBase info;//�˺�������Ϣ
	char name[MAX_CHARACTER_LENGTH + 1];//����
	tagFightPlayerInfo()
	{
		ZeroVar(*this);
	}
}FightPlayerInfo;
//--------------------

