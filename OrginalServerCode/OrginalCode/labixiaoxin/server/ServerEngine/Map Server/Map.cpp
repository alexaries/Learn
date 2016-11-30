#include "Map.h"
#include "..\GameBase\Protocol\ControlProtocol.h"
#include "..\GameBase\Protocol\DBProtocol.h"
#include "..\GameBase\res\ItemResMgr.h"
#include "..\GameBase\res\GameConfig.h"
#include "..\GameBase\res\MapRes.h"
#include "..\GameBase\res\EquipmentRes.h"
#include "..\GameBase\res\ImproveRes.h"
#include "..\GameBase\res\DropRes.h"
#include "module\MapEvent.h"
#include "module\LuckMgr.h"
#include "module\GlobalMapEventMgr.h"
#include "module\ActivityMgr.h"
#include "..\Base\Service\ServiceConfigMgr.h"
#include "..\GameBase\res\MallRes.h"
#include "..\GameBase\Logger\LoggerClient.h"
#include "..\GameBase\res\FormulaRes.h"

#include "..\Base\Service\BaseService.h"
#include "module\BroadcastMgr.h"
#include <math.h>
#include "ace/Date_Time.h"

#if _MEMORY_DEBUG
#include "../GameBase/Common/debugnew.h"
#define new new(__FILE__, __LINE__)
#endif
#include "../GameBase/res/DropCurrencyRes.h"
#include "../GameBase/res/DropMaterialRes.h"
#include "../Base/Communication/ServiceEndPoint.h"
#include "../GameBase/res/RechargeRes.h"
#include "../GameBase/res/PropsRes.h"

#define GET_MAIL_COUNT_PER_MESSAGE 10

ACE_UINT32 Map::m_unUserInfoServerID = 0;
ACE_UINT32 Map::m_unPhpProxyID = 0;

//#define ENERGY_RESTORE_INTERVAL (5 * 60 * 1000 * 1000L)

//#define ENERGY_RESTORE_INTERVAL (5 * 60)

//ACE_UINT32 g_exp_list[] = {0, 50, 110, 182, 268, 372,8 496, 646, 825, 1040};
//int gLoginBonusRewardList[] = {1,2,3,4,5,5,6};

Map::Map(ACE_UINT32 unID, BaseAdapter *parent)
:SubAdapterObj(unID, parent)
{
	m_lastSendMonthTime = 0;
	m_bDoTimeout = true;
	//memset(m_BGList, 0, sizeof(ACE_UINT32) * BG_PART_MAX);
	m_bLoadingDone = false;
	m_unLiveCounter = 0;

	//m_unExtraEnergyLocked = 0;
	m_unUserID = 0;
	m_unExpLevelUp = 0;

	m_unMaxLevel = GAME_CONFIG_INSTANCE::instance()->m_unMaxLv;
	m_unLastActivityTimer = 0;

	if(m_unUserInfoServerID == 0)
	{
		m_unUserInfoServerID = SERVICE_CONFIG_MGR_INSTANCE::instance()->get_service_instance_id("Test User Info Server");
	}

	m_unLoginBonusCount = 0;
	m_unFriendshipPoint = 0;

	m_bValidHome = true;

	m_playerMgr.set_map(this);
	m_friendMgr.set_map(this);
	//m_equipmentMgr.set_map(this);
	m_energyMgr.set_map(this);
	//m_pvpEnergyMgr.set_map(this);
	m_packageMgr.set_map(this);
	m_mapInfoMgr.set_map(this);
	m_skillMgr.set_map(this);
	m_friendMgr.set_map(this);
	//m_weaponSkillMgr.set_map(this);
	//m_teamMgr.set_map(this);
	m_unlockMgr.set_map(this);
	m_lootMgr.set_map(this);
	m_functionMgr.set_map(this);
	m_vipMgr.set_map(this);
	m_benefitMgr.set_map(this);
	m_giftMgr.set_map(this);
	m_cooldownMgr.set_map(this);
	m_handbookMgr.set_map(this);
	m_materialMgr.set_map(this);
	m_propsMgr.set_map(this);
	m_pvpMgr.set_map(this);
	m_moneyTreeMgr.set_map(this);
	m_secretMgr.set_map(this);
	m_wingMgr.set_map(this);
	m_giftBagCodeMgr.set_map(this);
	m_taskMgr.set_map(this);
	m_dailyTaskMgr.set_map(this);
	m_LAMgr.set_map(this);//活动任务
	m_badgeMgr.set_map(this);//徽章系统
	m_goldenTouchMgr.set_map(this);


	m_energyMgr.m_unMaxEnergy = GAME_CONFIG_INSTANCE::instance()->m_unMaxEnergy;
	m_energyMgr.m_unRestoreInterval = GAME_CONFIG_INSTANCE::instance()->m_unEnergyRestoreInterval;
	m_energyMgr.m_unRestoreEnergyGold = GAME_CONFIG_INSTANCE::instance()->m_unRestoreEnergyGold;

	//m_pvpEnergyMgr.m_unMaxEnergy = GAME_CONFIG_INSTANCE::instance()->m_unMaxPVPEnergy;
	//m_pvpEnergyMgr.m_unRestoreInterval = GAME_CONFIG_INSTANCE::instance()->m_unPVPEnergyRestoreInterval;
	//m_pvpEnergyMgr.m_unRestoreEnergyGold = GAME_CONFIG_INSTANCE::instance()->m_unPVPRestoreEnergyGold;

	m_secretCfg = GAME_CONFIG_INSTANCE::instance()->m_secretCfg;

	m_fSecretOff = GAME_CONFIG_INSTANCE::instance()->m_fSecretOff;

	m_dwGetMailListIndex = 0;
	m_unMultEnergy = 1;//默认1倍体力
	m_unCurrentBuyEnergy = 0;
	m_rechargeOrderSN = 0;
	m_unDynamicEnergy = 0;
}

Map::~Map()
{

}

int Map::start_create()
{
	//MsgDBHomeInfoRequest info;
	//info.unCharacterID = m_unID;

	//m_AchievementMgr.init();
	int ret =  SubAdapterObj::start_create();

	if(ret == -1)
	{
		return -1;
	}

	//this->add_event_listener(&m_teamMgr, GAME_EVENT_PROPERTY_UPDATE);
	this->add_event_listener(&m_playerMgr, GAME_EVENT_PVE_TEAM_UPDATE);
	//this->add_event_listener(&m_pvpMgr, GAME_EVENT_PLAYER_INFO_UPDATE);
	//this->add_event_listener(&m_lootMgr, GAME_EVENT_LOOT_IGNORE_UPDATE);
	this->add_event_listener(&m_skillMgr, GAME_EVENT_LEVEL_UP);
	this->add_event_listener(&m_unlockMgr, GAME_EVENT_LEVEL_UP);
	this->add_event_listener(&m_energyMgr, GAME_EVENT_LEVEL_UP);
	//this->add_event_listener(&m_unlockMgr, GAME_EVENT_VIP_LEVEL_UP);
	this->add_event_listener(&m_benefitMgr, GAME_EVENT_RESET_BENIFIT);
	this->add_event_listener(&m_energyMgr, GAME_EVENT_MAX_LIMIT_CHANGE);
	this->add_event_listener(&m_friendMgr, GAME_EVENT_MAX_LIMIT_CHANGE);
	this->add_event_listener(&m_packageMgr, GAME_EVENT_MAX_LIMIT_CHANGE);
	this->add_event_listener(&m_mapInfoMgr, GAME_EVENT_MAX_LIMIT_CHANGE);
	//this->add_event_listener(&m_mapInfoMgr, GAME_EVENT_BATTLE_DATA_UPDATE);

	ret = this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_LOAD_CHARACTER_INFO, NULL, 0);

	if(ret == -1)
	{
		//printf("fdfsdfsdfsdf\n");
	}

	return ret;
}

void Map::obj_timeout()
{
	//add by xxj 2015/05/23  18:39:57 
	// 	if(m_unLiveCounter >= MAX_MAP_SESSION_LIVE)
	// 	{
	// 			this->set_destroy(true);
	// 			this->setObjStatus(SERVICE_OBJ_DESTROY);
	// 			m_bValidHome = false;
	// 			//m_bDoDestroyMyself = true;
	// 	}
	// 	else
	// 	{
	// 		m_unLiveCounter ++;
	// 	}
	//-----------------------------------------end
	//SERVICE_OBJ_STATUS_NONE = -1,
	//SERVICE_OBJ_CREATE,
	//SERVICE_OBJ_RUN,
	//SERVICE_OBJ_DESTROY
	//printf("map timeout status = %d[%d] [%d]\n", this->m_unObjStatus, m_unID,m_unLiveCounter);
}

void Map::add_old_gift()
{
	static int vip_list[] = {0x04000001, 0x04000002, 0x04000003, 0x04000004, 0x04000005,0x04000006 };
	static int level_list[] = {2, 0x04010001, 4, 0x04010002, 6, 0x04010003, 8, 0x04010004, 12, 0x04010005, 20, 0x04010006, 30, 0x04010007, 40, 0x04010008, 50, 0x04010009, 60, 0x04010010, 70, 0x04010011, 80, 0x04010012, 90, 0x04010013, 100, 0x04010014};

	int i;

	//guo vip
	for(i = 0;i < 6;i ++)
	{
		if(m_vipMgr.m_unVipLevel < (i + 1))
		{
			break;
		}

		m_giftMgr.add_gift(vip_list[i]);
	}

	for(i = 0;i < 14;i ++)
	{
		if(m_unLevel < level_list[2 * i])
		{
			break;
		}

		m_giftMgr.add_gift(level_list[2 * i + 1]);
	}
}

int Map::deal_msg(MessageInfo &msgInfo)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ParentAdapter->m_mutex);

	m_unLiveCounter = 0;
	if(msgInfo.p_head->unType == SERVICE_MESSAGE_TYPE_USER)
	{
		//printf("get msg type = 0x%08x,character_id = %d\n", msgInfo.m_head->unType, m_unID);

		if(!m_bValidHome)
		{
			MsgInvalidHomeResponse response;
			MsgBaseRequest *tmp = (MsgBaseRequest *)msgInfo.body;
			response.nResult = 0;
			response.unSN = tmp->unSN;
			this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_INVALID_HOME, NULL, 0, msgInfo.s_head->srcAdr);

			if(msgInfo.s_head->srcAdr.unAdapterID == ADAPTER_GATEWAY_MGR)
			{
				send_response_end(msgInfo.s_head->srcAdr.unObjID, tmp->unSN);
			}

			return 0;
		}

		m_unCurrentTime = GameUtils::get_utc();

		/*
		{
		int ttt = m_unCurrentTime % (24 * 60 * 60);

		int j = ttt / 3600;
		int k = ttt % 3600;
		int l = k / 60;
		k = k % 60;

		printf("current time = %02d:%02d:%02d\n", j, l, k);
		}
		*/

		update_all();

		switch(msgInfo.m_head->unType)
		{
		case MSG_CHARACTER_UPDATE_DATA_REQUEST:
			{

				/*
				MsgDBUpdateDatabase db_request;
				db_request.unVersion = 1;
				this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_DATABASE, (char *)&db_request, sizeof(MsgDBUpdateDatabase));
				*/

				this->m_mapInfoMgr.clear_map();


				//ACE_UINT32 tmp[1024];
				//int c;
				//int i;

				add_old_gift();

				/*
				c = ITEM_RES_MGR_INSTANCE::instance()->get_item_id_list_by_type(ITEM_RES_TYPE_EXTRA_SKILL, 0, 0, 0, tmp, 0);
				for(i = 0;i < c;i ++)
				{
				m_skillMgr.add_player_skill(tmp[i]);
				}
				*/

				/*
				m_materialMgr.m_unCount = 0;
				c = ITEM_RES_MGR_INSTANCE::instance()->get_item_id_list_by_type(ITEM_RES_TYPE_MATERIAL, 0, 0, 0, tmp, 0);

				for(i = 0;i < c;i ++)
				{
				//m_skillMgr.add_player_skill(tmp[i]);
				m_materialMgr.add(tmp[i], 100);
				}
				*/
				/*
				c = ITEM_RES_MGR_INSTANCE::instance()->get_item_id_list_by_type(ITEM_RES_TYPE_PLAYER, 0, 0, 0, tmp, 0);

				for(i = 0;i < c;i ++)
				{
				if((i % 2) != 0)
				{
				continue;
				}
				this->add_item(BaseRes::handbook_ID(tmp[i]), 1);
				}

				c = ITEM_RES_MGR_INSTANCE::instance()->get_item_id_list_by_type(ITEM_RES_TYPE_EQUIPMENT, 0, 0, 0, tmp, 0);

				for(i = 0;i < c;i ++)
				{
				if((i % 2) != 0)
				{
				continue;
				}
				this->add_item(BaseRes::handbook_ID(tmp[i]), 1);
				}
				*/

				this->set_destroy(true);
			}
			break;
		case MSG_SELECT_CHARACTER_REQUEST:
			{
				CheckDateChanged();
				MsgSelectCharacterRequest *body = (MsgSelectCharacterRequest *)msgInfo.body;
				send_basic_info(msgInfo.s_head->srcAdr, body->unSN);
				//send_items_list(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				//send_player_list(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				//send_equipment_list(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				m_packageMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_playerMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				//m_equipmentMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_functionMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_mapInfoMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_giftMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_skillMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				//m_weaponSkillMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_cooldownMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_handbookMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_materialMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_propsMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				//m_pvpMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				m_moneyTreeMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);

				if(m_secretMgr.m_refreshTimes == 0){
					secret_refresh(m_secretMgr.m_list,0);//第一加载时候金币给他刷新下
				}
				m_secretMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);  //added by jinpan  加载的时候不给他发送，当他打开的时候再发送
				//				m_wingMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);	//wangwei wing
 				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}

			break;
		case MSG_GET_LOGIN_REWARD_REQUEST:
			{
				/*
				MsgGetLoginRewardRequest *body = (MsgGetLoginRewardRequest *)msgInfo.body;

				MsgGetLoginRewardResponse response;
				response.nResult = 0;
				response.unLoginCount = 0;
				response.unSN = body->unSN;

				int t_money = 0;

				int t = GameUtils::get_day_different(m_unCurrentTime, this->m_unLastLoginTime);

				if(t > 0)
				{
				if(t == 1)
				{
				this->m_unLoginBonusCount ++;
				}
				else
				{
				this->m_unLoginBonusCount = 1;
				}

				int idx = (int)(this->m_unLoginBonusCount - 1);
				if(idx >= MAX_LOGIN_BONUS_COUNT)
				{
				idx = MAX_LOGIN_BONUS_COUNT - 1;
				}
				t_money = GAME_CONFIG_INSTANCE::instance()->m_loginBonusList[idx];

				response.unLoginCount = m_unLoginBonusCount;
				}
				this->m_unLoginCount ++;

				m_unLastLoginTime = m_unCurrentTime;

				//update 2 db
				if(t_money > 0)
				{
				//t_money = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_GOLD, (float)t_money);
				int coin = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_COIN, 0);
				int power_point = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_POWER_POINT, 0);

				this->set_basic_info(coin, t_money, 0, power_point, 0);
				}

				MsgDBUpdateLoginInfoRequest db_request;
				db_request.unLoginTime = m_unLastLoginTime;
				db_request.unLoginCount = m_unLoginCount;
				db_request.unLoginBonusCount = m_unLoginBonusCount;

				this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_LOGIN_INFO, (char *)&db_request, sizeof(MsgDBUpdateLoginInfoRequest));


				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_LOGIN_REWARD_RESPONSE,(char *)&response, sizeof(MsgGetLoginRewardResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				*/

				//int nGold = 200;

				//nGold = (int)this->m_benefitMgr.get_benefit(BENEFIT_TYPE_FIRST_RECHARGE, (float)nGold);

				//printf("nGold = %d\n", nGold);
			}
			break;
		case MSG_RESET_MAP_REQUEST://重置管卡
			{
				MsgResetMapRequest *body = (MsgResetMapRequest*)msgInfo.body;
				MsgResetMapResponse response;
				response.unMapID = body->unMapID;
				response.nResult = m_mapInfoMgr.reset_map(response.unMapID);
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_RESET_MAP_RESPONSE,(char *)&response, sizeof(MsgResetMapResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}
			break;
		case MSG_GET_SIGN_UP_REWARD_REQUEST:
			{
				MsgGetSignUpRewardRequest *body = (MsgGetSignUpRewardRequest*)msgInfo.body;
				MsgGetSignUpRewardResponse response;
				response.nResult = ERROR_MSG_SING_UP;
				bool dirty = false;
				if(has_sign_up_reward() == false || has_vip_sign_up_reward() == false)
				{
					ACE_Date_Time oToday(ACE_OS::gettimeofday());
					int dwKey = get_sign_up_day_this_month();
					if(has_sign_up_reward() == false)
						++dwKey;
					dwKey += oToday.month() * 100 + oToday.year() *10000;
					std::map<int, SignUpGift>::iterator it = GAME_CONFIG_INSTANCE::instance()->m_oSignUpGift.find(dwKey);	
					if(it != GAME_CONFIG_INSTANCE::instance()->m_oSignUpGift.end())
					{
						RewardInfo oRewardInfo;
						memset(&oRewardInfo, 0, sizeof(RewardInfo));
						int i = 0;
						if(has_sign_up_reward() == false)
						{
							//if(it->second.m_dwRewardID == RES_CURRENCY_COIN)
							//	oRewardInfo.unCoin += it->second.m_dwRewardCount;
							//else if(it->second.m_dwRewardID == RES_CURRENCY_MONEY)
							//	oRewardInfo.unMoney += it->second.m_dwRewardCount;
							//else if(it->second.m_dwRewardID != 0)
							//{
							//	oRewardInfo.list[i].unCount = it->second.m_dwRewardCount;
							//	oRewardInfo.list[i].item.unResID = it->second.m_dwRewardID;
							//	i++;
							//}
							add_reward(oRewardInfo, it->second.m_dwRewardID, it->second.m_dwRewardCount);
							set_sign_up_reward_sign(true);
							set_sign_up_day_this_month(get_sign_up_day_this_month() + 1);
							dirty = true;
							response.nResult = 0;
						}
						if(has_vip_sign_up_reward() == false && m_vipMgr.m_unVipLevel >= it->second.m_dwVipCheck)
						{
							//if(it->second.m_dwVipRewardID == RES_CURRENCY_COIN)
							//	oRewardInfo.unCoin += it->second.m_dwVipRewardCount;
							//else if(it->second.m_dwVipRewardID == RES_CURRENCY_MONEY)
							//	oRewardInfo.unMoney += it->second.m_dwVipRewardCount;
							//else if(it->second.m_dwVipRewardID != 0)
							//{
							//	oRewardInfo.list[i].unCount = it->second.m_dwVipRewardCount;
							//	oRewardInfo.list[i].item.unResID = it->second.m_dwVipRewardID;
							//	i++;
							//}
							add_reward(oRewardInfo, it->second.m_dwVipRewardID, it->second.m_dwVipRewardCount);
							set_vip_sign_up_reward_sign(true);
							dirty = true;
							response.nResult = 0;
						}
						if(response.nResult == 0)
						{
							//-----------log 日志
							//获取pvp奖励
							LogItemInfo logInfo;
							logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_sign_up;//动作原因
							logInfo.SetOp_Way(_LOG_OP_WAY_sign_up);//途径(附加信息)
							get_reward(oRewardInfo,logInfo);
						}
					}
					else
						response.nResult = ERROR_MSG_RES;
				}
				if(dirty == true)
				{
					MsgDBUpdateCharacterFlagRequest db_request;
					db_request.unFlag = m_dwFlag;
					this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_CHARACTER_FLAG, (char *)&db_request, sizeof(db_request));
				}
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_SIGN_UP_REWARD_RESPONSE,(char *)&response, sizeof(response), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}
			break;
		case MSG_CONVERT_GIFT_BAG_CODE_REQUEST:
			{
				/*MsgConvertGiftBagCodeRequest *body = (MsgConvertGiftBagCodeRequest*)msgInfo.body;
				MsgConvertGiftBagCodeResponse response;
				response.nResult = 0; //-1 无此码， -2 被用过 -3 过期码 -4 用过同类型, -5 其它错误
				std::map<std::string, GiftBagCodeInfo> &refMapGiftBagCodeInfo = GAME_CONFIG_INSTANCE::instance()->m_oGiftBagCodeInfo;
				std::map<std::string, GiftBagCodeInfo>::iterator itInfo = refMapGiftBagCodeInfo.find(body->m_strGiftBagCode);
				std::map<ACE_UINT32, GiftBagCodeReward> &refMapGiftBagCodeReward = GAME_CONFIG_INSTANCE::instance()->m_oGiftBagCodeReward;
				std::map<ACE_UINT32, GiftBagCodeReward>::iterator itReward;
				RewardInfo oRewardInfo;
				memset(&oRewardInfo, 0, sizeof(RewardInfo));
				int iRewardInfoOffset = 0;
				bool bFlushToDB = false;

				if (itInfo != refMapGiftBagCodeInfo.end())
				{
				switch (itInfo->second.unCodeType)
				{
				case GIFT_BAG_CODE_ONCE:
				{
				if (itInfo->second.byUsed != 0)
				{
				response.nResult = -2;
				break;
				}
				}//break; 此处故意注释掉break，因为要走下面逻辑
				case GIFT_BAG_CODE_EVERYONE:
				{
				itReward = refMapGiftBagCodeReward.find(itInfo->second.unGiftBagType);
				if (itReward != refMapGiftBagCodeReward.end())
				{
				if (itReward->second.tExpiredTime > time(NULL))
				{
				if (m_giftBagCodeMgr.m_setClaimCode.count(itReward->first) == 0)
				{
				do_gift_bag_code_reward(oRewardInfo, itReward->second.unGiftBagID, iRewardInfoOffset);
				bFlushToDB = true;
				}
				else
				{
				response.nResult = -4;
				}
				}
				else
				{
				response.nResult = -3;
				}
				}
				else
				{
				response.nResult = -5;
				}
				}break;
				case GIFT_BAG_CODE_GM:
				{
				itReward = refMapGiftBagCodeReward.find(itInfo->second.unGiftBagType);
				if (itReward != refMapGiftBagCodeReward.end())
				{
				int iOffset = 0;
				do_gift_bag_code_reward(oRewardInfo, itReward->second.unGiftBagID, iRewardInfoOffset);
				get_reward(oRewardInfo);
				}
				}break;
				default:
				break;
				}
				}
				else
				{
				response.nResult = -1;
				}
				if (bFlushToDB)
				{
				m_giftBagCodeMgr.update_2_db(itInfo->second.unGiftBagType);
				}
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CONVERT_GIFT_BAG_CODE_RESPONSE,(char *)&response, sizeof(response), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);*/
				forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}break;
		case MSG_CONVERT_GIFT_BAG_CODE_USER_INFO_REQUEST://礼包码请求
			{
				MsgConvertGiftBagCodeUserInfoRequest *body = (MsgConvertGiftBagCodeUserInfoRequest*)msgInfo.body;
				MsgConvertGiftBagCodeResponse response;
				response.unSN = body->unSN;
				response.nResult = body->m_nResult; //ERROR_MSG_RES 无此码， ERROR_MSG_GOT 被用过 -3 过期码 -4 用过同类型, -5 其它错误
				std::map<ACE_UINT32, GiftBagCodeReward> &refMapGiftBagCodeReward = GAME_CONFIG_INSTANCE::instance()->m_oGiftBagCodeReward;
				std::map<ACE_UINT32, GiftBagCodeReward>::iterator itReward;
				RewardInfo &oRewardInfo = response.reward;
				memset(&oRewardInfo, 0, sizeof(RewardInfo));
				int iRewardInfoOffset = 0;
				bool bFlushToDB = false;

				if (body->m_nResult == 0)
				{
					itReward = refMapGiftBagCodeReward.find(body->m_dwGiftBagType);
					if (itReward != refMapGiftBagCodeReward.end())
					{
						if (itReward->second.tExpiredTime > GameUtils::get_utc())
						{
							if (m_giftBagCodeMgr.m_setClaimCode.count(itReward->first) == 0)
							{
								do_gift_bag_code_reward(oRewardInfo, itReward->second.unGiftBagID, iRewardInfoOffset);
								//-----------log 日志
								//获取pvp奖励
								LogItemInfo logInfo;
								logInfo.m_goodsID = body->m_dwGiftBagType;//id
								logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_convert_gift;//动作原因
								logInfo.SetOp_Way(_LOG_OP_WAY_convert_gift);//途径(附加信息)
								get_reward(oRewardInfo,logInfo);
								if (body->m_dwCodeType != GIFT_BAG_CODE_GM)
								{
									m_giftBagCodeMgr.update_2_db(body->m_dwGiftBagType);
									m_giftBagCodeMgr.m_setClaimCode.insert(itReward->first);
									if (body->m_dwCodeType == GIFT_BAG_CODE_ONCE)
									{
										MsgUpdateUserInfoGiftBagCodeRequest request;
										request.m_dwUsed = 1;
										ACE_OS::memcpy(request.m_strCode, body->m_strCode, MAX_GIFT_BAG_CODE_LENGTH);
										this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_UPDATE_USER_INFO_GIFT_BAG_CODE, (char *)&request, sizeof(MsgUpdateUserInfoGiftBagCodeRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
									}
								}
							}
							else
							{
								response.nResult = ERROR_MSG_GOT;
							}
						}
						else
						{
							response.nResult = ERROR_MSG_TIME_OUT;
						}
					}
					else
					{
						response.nResult = ERROR_MSG_RES;
					}
				}

				/*if (itInfo != refMapGiftBagCodeInfo.end())
				{
				switch (itInfo->second.unCodeType)
				{
				case GIFT_BAG_CODE_ONCE:
				{
				if (itInfo->second.byUsed != 0)
				{
				response.nResult = -2;
				break;
				}
				}//break; 此处故意注释掉break，因为要走下面逻辑
				case GIFT_BAG_CODE_EVERYONE:
				{

				}break;
				case GIFT_BAG_CODE_GM:
				{
				itReward = refMapGiftBagCodeReward.find(itInfo->second.unGiftBagType);
				if (itReward != refMapGiftBagCodeReward.end())
				{
				int iOffset = 0;
				do_gift_bag_code_reward(oRewardInfo, itReward->second.unGiftBagID, iRewardInfoOffset);
				get_reward(oRewardInfo);
				}
				}break;
				default:
				break;
				}
				}
				else
				{
				response.nResult = -1;
				}*/
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CONVERT_GIFT_BAG_CODE_RESPONSE,(char *)&response, sizeof(response), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_DOUBLE_DROP_LIST_REQUEST:
			{
				MsgDoubleDropListRequest *body = (MsgDoubleDropListRequest *)msgInfo.body; //其实没必要
				MsgDoubleDropListResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = 0;

				GameConfig *pConfig = GAME_CONFIG_INSTANCE::instance();
				if (pConfig != NULL)
				{
					if (m_mapInfoMgr.m_unDoubleVersion != pConfig->m_unRefreshDoubleDropVersion)
					{
						m_mapInfoMgr.m_unDoubleVersion = pConfig->m_unRefreshDoubleDropVersion;
						m_mapInfoMgr.m_setUsedDouble.clear();
					}
					ACE_UINT32 tUTC = GameUtils::get_utc();
					response.iNextFreshTime = tUTC + pConfig->m_unRefreshDoubleDropSecs - GameUtils::get_week_time(tUTC) % pConfig->m_unRefreshDoubleDropSecs;
					int i=0;
					std::map<UINT32, ACE_Byte>::iterator itMap = pConfig->m_vecDoubleDropMap[pConfig->m_unRefreshDoubleDropVersion].begin();
					if (m_unLevel >= 10)
					{
						for (;i<MAX_DOUBLE_DROP_COUNT && itMap!=pConfig->m_vecDoubleDropMap[pConfig->m_unRefreshDoubleDropVersion].end();++itMap)
						{
							if (m_mapInfoMgr.m_setUsedDouble.count(itMap->first) == 0)
							{
								response.doubleDropList[i].unRawMapID = itMap->first;
								response.doubleDropList[i].byType = itMap->second;
								++i;
							}
						}
					}
					response.iCount = i;
				}
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DOUBLE_DROP_LIST_RESPONSE,(char *)&response, sizeof(MsgDoubleDropListResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}break;
		case MSG_TASK_OPERATE_REQUEST:
			{
				MsgTaskOperateRequest *body = (MsgTaskOperateRequest *)msgInfo.body;
				switch (body->unOperateType)
				{
				case TASK_OPT_LIST:
					{
						MsgTaskList response;
						response.nResult = 0;
						response.unSN = body->unSN;
						m_taskMgr.BuildTaskListPkg(response);
						this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_TASK_LIST,(char *)&response, sizeof(MsgTaskList), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
					}break;
				case TASK_OPT_ADD: //添加
					{
						MsgTaskList response;
						response.nResult = 0;
						response.unSN = body->unSN;
						m_taskMgr.AddTask(body->iTaskID);

						m_taskMgr.BuildTaskListPkg(response);
						this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_TASK_LIST,(char *)&response, sizeof(MsgTaskList), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
					}break;
				case TASK_OPT_GETREWARD: //领奖
					{
						MsgTaskReward oRewardResponse;
						oRewardResponse.nResult = 0;
						oRewardResponse.unSN = body->unSN;
						bool canGet = m_taskMgr.GetReward(body->iTaskID, oRewardResponse.reward);
						if (!canGet)
							oRewardResponse.nResult = ERROR_MSG_GOT;

						sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_TASK_REWARD, (char *)&oRewardResponse, sizeof(MsgTaskReward), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);	
						send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);


						if (canGet)
						{
							//更新前置任务列表
							TaskConfig taskNow;
							ZeroVar(taskNow);
							taskNow.unID = body->iTaskID;
							m_taskMgr.RefreshTaskEx(Event_Base_Type_SpecificDuplication, taskNow);
						}

						/*m_taskMgr.BuildTaskListPkg(response);*/
					}break;
				case TASK_OPT_DAILY_LIST:
					{
						MsgTaskList response;
						response.nResult = 0;
						response.unSN = body->unSN;
						m_dailyTaskMgr.BuildTaskListPkg(response);

						this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_TASK_LIST,(char *)&response, sizeof(MsgTaskList), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);

						/*MsgObtainedActivenessReward obtained;
						obtained.nResult = 0;
						obtained.unObtainedCount = 0;
						std::set<ACE_UINT32>::iterator itSet = m_dailyTaskMgr.m_setObtainedActivenessReward.begin();
						for (;obtained.unObtainedCount<MAX_ACTIVENESS_COUNT && itSet != m_dailyTaskMgr.m_setObtainedActivenessReward.end();++itSet)
						{
						obtained.obtainedList[obtained.unObtainedCount++] = *itSet;
						}
						sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_OBTAINED_ACTIVENESS_REWARD, (char *)&obtained, sizeof(MsgObtainedActivenessReward), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);	*/
					}break;
				case TASK_OPT_DAILY_ADD: 
					{
						MsgTaskList response;
						response.nResult = 0;
						response.unSN = body->unSN;
						m_dailyTaskMgr.AddTask(body->iTaskID);
						m_dailyTaskMgr.BuildTaskListPkg(response);
						this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_TASK_LIST,(char *)&response, sizeof(MsgTaskList), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
					}break;
				case TASK_OPT_DAILY_GETREWARD:
					{
						MsgTaskReward oRewardResponse;
						oRewardResponse.nResult = 0;
						oRewardResponse.unSN = body->unSN;
						oRewardResponse.nResult = m_dailyTaskMgr.GetReward(body->iTaskID, oRewardResponse.reward);
						
						sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_TASK_REWARD, (char *)&oRewardResponse, sizeof(MsgTaskReward), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);	
						
						/*			m_dailyTaskMgr.BuildTaskListPkg(response);*/
					}break;
				case TASK_OPT_DAILY_OBTAINEDREWARD:
					{
						CheckDateChanged();
						MsgObtainedActivenessReward obtained;
						obtained.nResult = 0;
						obtained.unSN = body->unSN;
						obtained.unObtainedCount = 0;
						std::set<ACE_UINT32>::iterator itSet = m_dailyTaskMgr.m_setObtainedActivenessReward.begin();
						for (;obtained.unObtainedCount<MAX_ACTIVENESS_COUNT && itSet != m_dailyTaskMgr.m_setObtainedActivenessReward.end();++itSet)
						{
							obtained.obtainedList[obtained.unObtainedCount++] = *itSet;
						}
						sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_OBTAINED_ACTIVENESS_REWARD, (char *)&obtained, sizeof(MsgObtainedActivenessReward), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);	
					}break;
				case TASK_OPT_DAILY_ACTIVENESS:
					{
						CheckDateChanged();
						MsgActivenessChanged package;
						package.nResult = 0;
						package.unSN = body->unSN;
						package.unActivenessChanged = m_dailyTaskMgr.m_unActivenessPoints;
						sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ACTIVENESS_CHANGED,(char *)&package, sizeof(MsgActivenessChanged), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
					}break;
				case TASK_OPT_DAILY_SINGLE_REWARD:
					{
						CheckDateChanged();
						MsgDailySingleTaskReward package;
						ZeroVar(package);
						package.nResult = -1;
						package.unSN = body->unSN;
						if (m_dailyTaskMgr.GetSingleTaskReward(body->iTaskID, package.reward, package.unActivenessChanged))
						{
							package.nResult = 0;
						}
						sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DAILY_SINGLE_TASK_REWARD, (char *)&package, sizeof(MsgDailySingleTaskReward), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);			
					}break;
				default:
					break;

				}
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}break;
		case MSG_GOLDEN_TOUCH_REQUEST:
			{
				MsgGoldenTouchRequest *body = (MsgGoldenTouchRequest *)msgInfo.body;
				MsgGoldenTouchResponse response;
				response.unSN = body->unSN;
				response.nResult = golden_touch(response.m_unBellCount, response.m_unCriticalTimes);
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GOLDEN_TOUCH_RESPONSE,(char *)&response, sizeof(MsgGoldenTouchResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}break;
		case MSG_GET_REWARD_FROM_MAIL_REQUEST://邮件提取
			{
				MsgGetRewardFromMailRequest *body = (MsgGetRewardFromMailRequest*)msgInfo.body;
				MsgGetRewardFromMailResponse response;
				response.m_dwCharacterID = body->m_dwCharacterID;
				response.m_dwMailID = body->m_dwMailID;
				response.m_dwReadMailRequestSN = body->m_dwReadMailRequestSN;
				RewardInfo oRewardInfo;
				memset(&oRewardInfo, 0, sizeof(RewardInfo));
				for (int i=0;i<MAIL_ATTACHMENT_LEN;++i)
				{
					if (body->m_aMailAttachment[i].m_dwItemID == 0)
					{
						break;
					}
					add_reward(oRewardInfo, body->m_aMailAttachment[i].m_dwItemID, body->m_aMailAttachment[i].m_dwItemCount);
				}

				//-----------log日志
				//邮件
				LogItemInfo logInfo;
				logInfo.m_goodsID = response.m_dwMailID;//id
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_mail_get;//动作原因
				logInfo.SetOp_Way(body->m_strSenderName);//途径(附加信息)
				get_reward(oRewardInfo,logInfo);
				response.nResult = 0;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_REWARD_FROM_MAIL_RESPONSE, (char *)&response, 
					sizeof(response), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_GET_ALL_MAIL_REWARD_TO_MAP_SERVER_REQUEST:
			{
				MsgGetAllMailRewardToMapServerRequest *body = (MsgGetAllMailRewardToMapServerRequest*)msgInfo.body;
				MsgGetAllMailRewardToMapServerResponse response;
				response.m_dwCharacterID = body->m_dwCharacterID;
				response.m_dwGetAllMailRewardRequestSN = body->m_dwGetAllMailRewardRequestSN;
				RewardInfo oRewardInfo;
				memset(&oRewardInfo, 0, sizeof(RewardInfo));
				for (int i=0;i<body->m_dwCount;++i)
				{
					// if (body->m_aMailAttachment[i].m_dwItemID == 0)
					// {
					// 	break;
					// }
					add_reward(oRewardInfo, body->m_aMailAttachment[i].m_dwItemID, body->m_aMailAttachment[i].m_dwItemCount);
				}

				//-----------log日志
				//一键领取所有附件
				LogItemInfo logInfo;
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_mail_all;//动作原因
				logInfo.SetOp_Way(_LOG_OP_WAY_mail_all);//途径(附加信息)
				get_reward(oRewardInfo,logInfo);

				response.nResult = 0;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_ALL_MAIL_REWARD_TO_MAP_SERVER_RESPONSE, (char *)&response, 
					sizeof(response), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_UPDATE_PLAYER_LIST_REQUEST:
			{
				MsgUpdatePlayerListRequest *body = (MsgUpdatePlayerListRequest *)msgInfo.body;

				MsgUpdatePlayerListResponse response;
				response.unSN = body->unSN;
				response.nResult = -1;

				if(body->unType == TYPE_PVE)
				{
					response.nResult = m_playerMgr.update_player(body->list, body->playerSkillList);
				}
				else
				{
					response.nResult = 0;//m_pvpMgr.update_pvp_basic_info(body->list, body->playerSkillList);
				}
				//response.nResult = m_playerMgr.update_player(body->unCount, body->list);
				LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]update team info,type=%d", m_unID, body->unType);

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_UPDATE_PLAYER_LIST_RESPONSE,(char *)&response, sizeof(MsgUpdatePlayerListResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}

			break;
		case MSG_SELECT_EQUIPMENT_REQUEST:
			{
				MsgSelectEquipmentRequest *body = (MsgSelectEquipmentRequest *)msgInfo.body;
				MsgSelectEquipmentResponse response;
				response.unSN = body->unSN;
				response.nResult = -1;
				//PackageCell *cell = NULL;

				if(!m_functionMgr.is_locked(FUNCTION_EXCHANGE_EQUIPMENT))
				{ 
					/*
					cell = m_packageMgr.get_cell(body->idx);

					if(cell == NULL)
					{

					}
					else
					{
					cell = m_equipmentMgr.replace(cell);
					if(cell != NULL)
					{
					m_packageMgr.replace(body->idx, cell);
					response.nResult = 0;
					}
					}
					*/

					if(body->idx < 0)
						response.nResult = m_playerMgr.exchange_equipment(body->unPlayerID, (ACE_UINT32)-body->idx, body->equipment_type);
					else
						response.nResult = m_playerMgr.exchange_equipment(body->unPlayerID, body->idx);
				}

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_SELECT_EQUIPMENT_RESPONSE,(char *)&response, sizeof(MsgSelectEquipmentResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}

			break;
		case MSG_ENTER_MAP_REQUEST:
			{
				MsgEnterMapRequest *body = (MsgEnterMapRequest *)msgInfo.body;

				MsgEnterMapQueryCharacterRequest request;
				request.unSN = body->unSN;
				request.unMapID = body->unMapID;
				request.unCharacterID = body->unFriendID;
				request.specialMap = body->specialMap;
				m_unMultEnergy = body->unMultEnergy;
				if (m_unMultEnergy <= 0)
					m_unMultEnergy = 1;

				//多倍体力检查
				ACE_INT32 num = 0;
				FormulaInfo formula = GAME_CONFIG_INSTANCE::instance()->m_vecFormulaInfo[3];//xxj 2015/04/22  17:11:35 公式
				if (m_unLevel >= formula.paramA)
				{
					num = 2 + (m_unLevel-formula.paramA)/formula.paramB;
					if (num > formula.paramC)
						num = formula.paramC;
				}
				else//只有一倍
					num = 1;

				if (m_unMultEnergy <= num)
				{
					request.unMultEnergy = m_unMultEnergy;//xxj 2015/04/21  11:34:44 多倍体力
				}
				else
				{
					request.unMultEnergy = 1;//检查失败
					m_unMultEnergy =1;
				}

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ENTER_MAP_QUERY_CHARACTER_REQUEST, (char *)&request, sizeof(MsgEnterMapQueryCharacterRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);

				/*

				MsgEnterMapResponse response;
				response.unSN = body->unSN;

				response.nResult = -1;

				//GameUtils::reset_reward(response.info);



				if(!this->m_packageMgr.is_full())
				{
				if(body->unFriendID == 0 || m_friendMgr.use_friend(body->unFriendID) == 0)
				{
				GLOBAL_MAP_EVENT_MGR_INSTANCE::instance()->update(m_unCurrentTime);
				response.nResult = m_mapInfoMgr.enter_map(body->unMapID, GLOBAL_MAP_EVENT_MGR_INSTANCE::instance()->get_event_by_map_id(body->unMapID));
				//response.info = m_mapInfoMgr.m_info;
				}
				}

				response.info = m_mapInfoMgr.m_info;

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ENTER_MAP_RESPONSE,(char *)&response, sizeof(MsgEnterMapResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				*/
			}

			break;

		case MSG_ASSIST_FRIENDSHIP_REQUEST:
			{
				MsgAssistFriendshipRequest *body = (MsgAssistFriendshipRequest *)msgInfo.body;

				MsgAssistFriendshipResponse *response = (MsgAssistFriendshipResponse *)public_send_buff;
				response->unSN = body->unSN;
				response->nResult = 0;
				response->unCount = 0;
				for (int i=0;i<MAX_RANDOM_CHARACTER && body->characterIDList[i] != 0 ;++i)
				{
					response->list[i].unCharacterID = body->characterIDList[i];
					response->list[i].unAddFriendship = m_friendMgr.add_friendship(body->characterIDList[i]);
					++response->unCount;
				}

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ASSIST_FRIENDSHIP_RESPONSE,(char *)response, sizeof(MsgAssistFriendshipResponse)+response->unCount*sizeof(AssistFriendship), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
			}

			break;
		case MSG_ENTER_MAP_QUERY_CHARACTER_RESPONSE:
			{
				MsgEnterMapQueryCharacterResponse *body = (MsgEnterMapQueryCharacterResponse *)msgInfo.body;

				MsgEnterMapResponse response;
				response.nResult = body->nResult;
				response.unSN = body->unSN;

				if (response.nResult == 0 )
				{
					if (this->m_packageMgr.is_full())
					{
						response.nResult = ERROR_MSG_PKG_FULL;
					}
					else if (m_cooldownMgr.can_do(m_unCurrentTime, body->unMapID) != 0)
					{
						response.nResult = ERROR_MSG_LOCKED;
					}
				}

				if(response.nResult == 0)
				{
					MapRes *map_res = (MapRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(body->unMapID);

					m_friendMgr.update_friend_info(body->info.unItemID, body->info);

					if(map_res != NULL && is_cost_enough(map_res->m_costList, 1))
					{
						bool isMultEnergyEnough = true;//默认足够
						if(body->unMultEnergy > 1)//一倍体力is_cost_enough已经判断完成
						{
							for(int i = 0;i < map_res->m_costList.unCount;i ++)//判断多倍体力是否足够扣除
							{
								if (map_res->m_costList.list[i].unID == RES_CURRENCY_ENERGY)//体力
								{
									if(!m_energyMgr.is_enough( map_res->m_costList.list[i].cost * body->unMultEnergy))//不够扣
									{
										isMultEnergyEnough = false;
										response.nResult = ERROR_MSG_COST;
										break;
									}
								}
							}
						}

						if (isMultEnergyEnough)//足够扣除
						{
							int nRet = m_friendMgr.can_use_friend(body->info.unItemID);
							//nRet == 1 means friend in cd
							if(nRet != 1)
							{
								//m_energyMgr.decrease(1);

								GLOBAL_MAP_EVENT_MGR_INSTANCE::instance()->update(m_unCurrentTime);
								response.nResult = m_mapInfoMgr.enter_map(body->unMapID,body->specialMap, GLOBAL_MAP_EVENT_MGR_INSTANCE::instance()->get_event_by_map_id(body->unMapID), &body->info);

								if(response.nResult == 0)
								{
									//-----------log日志
									//多倍体力
									LogItemInfo logInfo;
									logInfo.m_goodsID = body->unMapID;//id
									logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_map_cost;//动作原因
									logInfo.SetOp_Way(_LOG_OP_WAY_map_cost);//途径(附加信息)
									do_map_before_cost(map_res->m_costList, 1,logInfo);//进入之前先扣除1点
									m_cooldownMgr.do_once(m_unCurrentTime, body->unMapID);
								}
							}
						}
					}
				}

				response.info = m_mapInfoMgr.m_info;
				response.reward = m_mapInfoMgr.m_reward;

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ENTER_MAP_RESPONSE,(char *)&response, sizeof(MsgEnterMapResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_BATTLE_REBORN_REQUEST:
			{
				MsgBattleRebornRequest *body = (MsgBattleRebornRequest *)msgInfo.body;

				MsgBattleRebornResponse response;

				response.nResult = -1;
				response.unSN = body->unSN;

				response.nResult = this->m_mapInfoMgr.battle_reborn(m_unCurrentTime);

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_BATTLE_REBORN_RESPONSE,(char *)&response, sizeof(MsgBattleRebornResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
			/*
			case MSG_BATTLE_COMMAND_REQUEST:
			{
			MsgBattleCommandRequest *body = (MsgBattleCommandRequest *)msgInfo.body;

			MsgBattleCommandResponse response;

			ACE_OS::memset(&response, 0, sizeof(MsgBattleCommandResponse));

			response.nResult = 0;
			response.unSN = 0;

			m_mapInfoMgr.add_battle_command(body->battleInfo, body->unBattleInfoSize, response);

			this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_BATTLE_COMMAND_RESPONSE,(char *)&response, sizeof(MsgBattleCommandResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
			send_response_end(m_unUserID, body->unSN);
			}
			break;
			*/
		case MSG_CLEAR_MAP_REQUEST:
			{
				MsgClearMapRequest *body = (MsgClearMapRequest *)msgInfo.body;

				//m_mapInfoMgr.add_battle_command(body->battleInfo, body->unBattleInfoSize);

				MsgClearMapResponse response;
				response.nResult = 0;
				response.unSN = body->unSN;

				if(body->unType == TYPE_PVE)
				{
					m_mapInfoMgr.clear_map();
				}
				else
				{
					//m_pvpMgr.surrender(m_unCurrentTime);
				}

				LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]clear map, type = %d", m_unID, body->unType);

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CLEAR_MAP_RESPONSE,(char *)&response, sizeof(MsgClearMapResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);

				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_MAP_REWARD_REQUEST:
			{
				MsgGetMapRewardRequest *body = (MsgGetMapRewardRequest *)msgInfo.body;

				MsgGetMapRewardResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				GameUtils::reset_reward(response.reward);
				response.nResult = -1;

				if(!this->m_packageMgr.is_full())
				{
					ACE_UINT32 tmp_MaxExp = -1; //从满级少一级升到满级所能增加的最多经验,-1表示不限制
					tmp_MaxExp = max_exp_get();
					response.nResult = m_mapInfoMgr.get_reward(body->unMapID, m_unCurrentTime, body->unBellCount, body->unLaunchCount,tmp_MaxExp);
				}

				if(response.nResult == 0)
				{
					m_friendMgr.use_friend(m_friendMgr.m_unCurrentAssistFriendID);
					m_friendMgr.record_friendship();
					//get reward
					//没有友情点了
					//m_mapInfoMgr.m_reward.unFriendship = m_friendMgr.m_unCurrentAssistFriendship;//add_friendship();

					//获取n倍奖励
					if( m_unMultEnergy > 1 )//校验成功
					{
						m_mapInfoMgr.m_reward.unCoin *= m_unMultEnergy;
						m_mapInfoMgr.m_rewardTemp.unCoin *= m_unMultEnergy;
						// 						m_mapInfoMgr.m_reward.unMoney *= m_unMultEnergy;
						// 						m_mapInfoMgr.m_reward.unExp *= m_unMultEnergy;
						// 						m_mapInfoMgr.m_reward.unPowerPoint *= m_unMultEnergy;
						// 						m_mapInfoMgr.m_reward.unHonour *= m_unMultEnergy;
						//m_mapInfoMgr.m_reward.unFriendship *= m_unMultEnergy;
						// 						m_mapInfoMgr.m_reward.unEnergy *= m_unMultEnergy;
						// 						m_mapInfoMgr.m_reward.unPVPEnergy *= m_unMultEnergy;
						// 						多倍体力个数已经在开始得到了
						// 						for(int i = 0;i < m_mapInfoMgr.m_reward.unCount;i ++)
						// 						{
						// 							m_mapInfoMgr.m_reward.list[i].unCount *= m_unMultEnergy;
						// 						}
					}

					//vip闯关战斗胜利贝壳奖励数（百分比）
					m_mapInfoMgr.m_reward.unCoin += m_mapInfoMgr.m_rewardTemp.unCoin * m_vipMgr.GetVipGold_profit() - m_mapInfoMgr.m_rewardTemp.unCoin;
					//-----------------------------------------------------------------
					//徽章系统
					m_mapInfoMgr.m_reward.unCoin += m_mapInfoMgr.m_rewardTemp.unCoin * m_badgeMgr.GetCoinGaint();
					m_mapInfoMgr.m_reward.unPowerPoint += m_mapInfoMgr.m_rewardTemp.unPowerPoint * m_badgeMgr.GetPowerPointGaint();
					//----------------------end


					//-----------log日志
					//结算奖励
					LogItemInfo logInfo;
					logInfo.m_goodsID = body->unMapID;//id
					logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_map_get;//动作原因
					logInfo.SetOp_Way(_LOG_OP_WAY_map_get);//途径(附加信息)
					get_reward(m_mapInfoMgr.m_reward,logInfo);

					MapRes *map_res = (MapRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(body->unMapID);
					ACE_UINT32 count = 0;
					if(map_res != NULL)
					{
						//-----------log日志
						//多倍体力
						//ZeroVar(logInfo);
						logInfo.m_goodsID = body->unMapID;//id
						logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_map_cost;//动作原因
						logInfo.SetOp_Way(_LOG_OP_WAY_map_cost);//途径(附加信息)
						//SetlogUserInfo(logInfo);
						count = do_map_after_cost(map_res->m_costList,body->unMapID, 1,logInfo);//添加n倍体力
						//------------------------------------
					}

					response.unMultEnergy = ((count<<4) | (0xf & m_unMultEnergy));
					response.reward = m_mapInfoMgr.m_reward;
					response.unReward3Star = m_mapInfoMgr.m_unReward3Star;

					m_mapInfoMgr.clear_map();
//没有友情点了
// 					if (m_friendMgr.m_bSendAssistEmail && m_friendMgr.m_unCurrentAssistFriendID != 0)
// 						send_friendship_mail(m_friendMgr.m_unCurrentAssistFriendID, m_friendMgr.m_unAssistType, m_friendMgr.m_strAssistName,m_unMultEnergy);
					m_friendMgr.m_unCurrentAssistFriendID = 0;
					m_friendMgr.m_unCurrentAssistFriendship = 0;
					m_friendMgr.m_unAssistType = 1;
					m_friendMgr.m_bSendAssistEmail = false;
				}

				m_unMultEnergy = 1;//还原消耗1倍体力

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_MAP_REWARD_RESPONSE,(char *)&response, sizeof(MsgGetMapRewardResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);

				//CLogBase *pLog = BaseService::GetLogInstance();
				//sprintf(pLog->m_szLogBuffer, "[%s][%d]:enter_map reward:coin=%d,money=%d,exp=%d,pp=%d,honour=%d,recharge=%d,enery=%d,pvp=%d,friendship=%d,count=%d,list[0].count=%d,list[0].resid=%x",__FILE__, __LINE__, 
				//	response.reward.unCoin,response.reward.unMoney,response.reward.unExp,response.reward.unPowerPoint,response.reward.unHonour,response.reward.unRecharge,
				//	response.reward.unEnergy,response.reward.unPVPEnergy,response.reward.unFriendship,response.reward.unCount,
				//	response.reward.list[0].unCount,response.reward.list[0].item.unResID);
				//pLog->RecordLog(Log_Type_Info, pLog->m_szLogBuffer);
				//send_basic_info(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				//send_items_list(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				//send_player_list(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				//send_equipment_list(msgInfo.s_head->srcAdr.unObjID, body->unSN);

				//printf("time cost%d\n",utc);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}

			break;
		case MSG_GET_FRIEND_DETAIL_REQUEST:
			{
				MsgGetFriendDetailRequest *body = (MsgGetFriendDetailRequest *)msgInfo.body;

				m_friendMgr.request_detail(body->unType, body->unSN,body->unPage);
			}
			break;
		case MSG_GET_CHARACTER_DETAIL_RESPONSE:
			{
				MsgGetCharacterDetailResponse *body = (MsgGetCharacterDetailResponse *)msgInfo.body;
				m_friendMgr.get_list_detail(body->unType, body->unCount, body->list);
			}
			break;
		case MSG_INVITE_FRIEND_REQUEST:
			{
				MsgInviteFriendRequest *body = (MsgInviteFriendRequest *)msgInfo.body;

				if(msgInfo.s_head->srcAdr.unAdapterID == ADAPTER_MAP_MGR)
				{
					m_friendMgr.get_invite_request(msgInfo.s_head->srcAdr.unObjID);
				}
				else
				{
					MsgBaseResponse response;
					response.unSN = body->unSN;
					response.nResult = m_friendMgr.invite_friend_request(body->unFriendID);
					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_INVITE_FRIEND_RESPONSE, (char *)&response, sizeof(MsgBaseResponse));
					send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				}
			}
			break;
		case MSG_APPLY_INVITATION_REQUEST:
			{
				MsgApplyInvitationRequest *body = (MsgApplyInvitationRequest *)msgInfo.body;

				if(msgInfo.s_head->srcAdr.unAdapterID == ADAPTER_MAP_MGR)
				{
					m_friendMgr.get_invite_answer(body->unSN, msgInfo.s_head->srcAdr.unObjID);
				}
				else
				{
					if(m_friendMgr.apply_invite_request(body->unFriendID, body->unSN) != 0)
					{
						send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
					}
				}
			}
			break;
		case MSG_APPLY_INVITATION_RESPONSE:
			{
				MsgApplyInvitationResponse *body = (MsgApplyInvitationResponse *)msgInfo.body;

				m_friendMgr.apply_invite_response(body->nResult, body->unFriendID, body->unSN);
			}
			break;
		case MSG_DELETE_FRIEND_REQUEST:
			{
				MsgDeleteFriendRequest *body = (MsgDeleteFriendRequest *)msgInfo.body;

				if(msgInfo.s_head->srcAdr.unAdapterID == ADAPTER_MAP_MGR)
				{
					m_friendMgr.delete_friend(body->unFriendID);
				}
				else
				{
					m_friendMgr.delete_friend_or_invitation(body->unType, body->unFriendID, body->unSN);
				}
			}
			break;
		case MSG_READ_MAIL_REQUEST:
			{
				forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_READ_MAIL_RESPONSE:
			{
				MsgReadMailResponse *body = (MsgReadMailResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_DELETE_READ_MAIL_REQUEST:
			{
				forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_DELETE_READ_MAIL_RESPONSE:
			{
				MsgDeleteReadMailResponse *body = (MsgDeleteReadMailResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_ALL_MAIL_REWARD_REQUEST:
			{
				forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_GET_ALL_MAIL_REWARD_RESPONSE:
			{
				MsgGetAllMailRewardResponse *body = (MsgGetAllMailRewardResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_DELETE_ALL_READ_MAIL_REQUEST:
			{
				forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_DELETE_ALL_READ_MAIL_RESPONSE:
			{
				MsgDeleteAllReadMailResponse *body = (MsgDeleteAllReadMailResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_MAIL_LIST_REQUEST:
			{
				if(m_oMailListBuffer.empty() == false)
				{
					if(m_dwGetMailListIndex >= m_oMailListBuffer.size())
					{
						m_oMailListBuffer.clear();
						//m_dwGetMailListIndex = 0;
					}
					MsgGetMailListRequest *body = (MsgGetMailListRequest *)msgInfo.body;
					MsgGetMailListResponse *response= (MsgGetMailListResponse *)public_send_buff;
					response->unSN = body->unSN;
					ACE_UINT32 dwLeftMailCount = (m_oMailListBuffer.size() > m_dwGetMailListIndex ? m_oMailListBuffer.size() - m_dwGetMailListIndex : 0);
					if(dwLeftMailCount > GET_MAIL_COUNT_PER_MESSAGE)
						response->unCount = GET_MAIL_COUNT_PER_MESSAGE;
					else
						response->unCount = dwLeftMailCount;
					response->StartIndex = m_dwGetMailListIndex;
					if(response->unCount != 0)
						memcpy(response->list, &m_oMailListBuffer[m_dwGetMailListIndex], response->unCount * sizeof(MailInfo));
					m_dwGetMailListIndex += response->unCount;
					int size = sizeof(MsgGetMailListResponse) + response->unCount * sizeof(MailInfo);
					sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_MAIL_LIST_RESPONSE,(char *)response, size, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
					send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
				}
				else
				{
					forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
				}
			}
			break;
		case MSG_KEEP_ALIVE_REQUEST:
			{
				MsgKeepAliveRequest *body = (MsgKeepAliveRequest *)msgInfo.body;
				MsgKeepAliveResponse response;
				ZeroVar(response);

				response.unSN = body->unSN;
				response.nResult = 0;
				response.unTime = GameUtils::get_utc();
				if (m_friendMgr.m_isHaveNew)
				{
					response.unKey |= 0x08;
				}
				sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_KEEP_ALIVE_RESPONSE,(char *)&response, sizeof(MsgKeepAliveResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}
			break;
		case MSG_GET_MAIL_LIST_RESPONSE:
			{
				MsgGetMailListResponse *body = (MsgGetMailListResponse *)msgInfo.body;
				m_oMailListBuffer.clear();
				m_dwGetMailListIndex = 0;
				if(body->unCount > 0)
				{
					m_oMailListBuffer.resize(body->unCount);
					memcpy(&m_oMailListBuffer[0], body->list, body->unCount * sizeof(MailInfo));
				}
				ZeroVar(public_send_buff);
				MsgGetMailListResponse *response= (MsgGetMailListResponse *)public_send_buff;
				response->unSN = body->unSN;
				ACE_UINT32 dwLeftMailCount = (m_oMailListBuffer.size() > m_dwGetMailListIndex ? m_oMailListBuffer.size() - m_dwGetMailListIndex : 0);
				if(dwLeftMailCount > GET_MAIL_COUNT_PER_MESSAGE)
					response->unCount = GET_MAIL_COUNT_PER_MESSAGE;
				else
					response->unCount = dwLeftMailCount;
				response->StartIndex = m_dwGetMailListIndex;
				if(response->unCount != 0)
					memcpy(response->list, &m_oMailListBuffer[m_dwGetMailListIndex], response->unCount * sizeof(MailInfo));
				m_dwGetMailListIndex += response->unCount;
				int size = sizeof(MsgGetMailListResponse) + response->unCount * sizeof(MailInfo);
				sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_MAIL_LIST_RESPONSE,(char *)response, size, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_RANDOM_CHARACTER_REQUEST:
			{
				MsgRandomCharacterRequest *body = (MsgRandomCharacterRequest *)msgInfo.body;
				MsgGetRandomCharacterRequest request;
				ZeroVar(request);
				request.unSN = body->unSN;
				request.unMapID = body->unMapID;

				int star = 0;
				switch(body->unMapID)
				{
					case 0x0F010101:
					case 0x0F010201:
					case 0x0F010301:
					case 0x0F010401:
					case 0x0F010501:
					case 0x0F010601:
					case 0x0F010701:
						{
							star = m_mapInfoMgr.get_map_star(body->unMapID);
						}break;
					default:
						{
							star = 0;
						}break;
				}
				
				//memcpy(&request, msgInfo.body, sizeof(MsgBaseRequest)); //由于原来的协议只有MsgBaseRequest, 现在转发前需要添加内容
				if (star == 0)
				{
					request.unGuidanceFlag = 0x01;
				}
				else
				{
					request.unGuidanceFlag = m_unTutorialFlag;
					request.unMapID = 0;
				}

				request.unCount = m_friendMgr.m_unFriendCount;
				memcpy(request.oFriendIDList, m_friendMgr.m_friendIDList, MAX_FRIEND_COUNT * sizeof(ACE_UINT32));
				sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_RANDOM_CHARACTER_REQUEST, (char *)&request, sizeof(MsgGetRandomCharacterRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
				//this->forward_msg(request, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_GET_RANDOM_CHARACTER_RESPONSE:
			{
				MsgGetRandomCharacterResponse *body = (MsgGetRandomCharacterResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_LUCK_REQUEST://抽奖
			{
				MsgGetLuckRequest *body = (MsgGetLuckRequest *)msgInfo.body;

				MsgGetLuckResponse response;

				ACE_OS::memset(&response, 0, sizeof(MsgGetLuckResponse));

				response.nResult = -1;
				//response.unItemID = 0;
				response.unSN = body->unSN;
				//BehaviorRes *behaviorRes;

				//behaviorRes = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(body->unBehaviorID);
				if (m_functionMgr.is_locked(FUNCTION_GET_LUCK))
				{
					response.nResult = ERROR_MSG_LOCKED;
				}
				else if (this->m_packageMgr.is_full())
				{
					response.nResult = ERROR_MSG_PKG_FULL;
				}
				else{
					/*
					int t = (int)GAME_CONFIG_INSTANCE::instance()->m_unGetLuckCost;
					if(this->m_unMoney >= t)
					{
					LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]get luck", m_unID);

					this->set_basic_info(0, -t, 0, 0, 0);
					response.nResult = 0;
					response.unItemID = this->get_luck(m_unLevel, 1);

					//response.unItemID = LUCK_MGR_INSTANCE::instance()->get_luck(m_unLevel, this, 1);
					//add_item(response.unItemID);

					this->update_use_luck_count();
					}
					*/

					ACE_UINT32 unCount = 1;
					if(body->unIsTen != 0)
					{
						unCount = 10;
					}

					response.nResult = get_luck(unCount, body->unBehaviorID, response.reward);//抽奖

					if(response.nResult == 0)
					{
						LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]get luck, luck ID = 0x%08x, count = %d", m_unID, body->unBehaviorID, unCount);

						this->update_use_luck_count();
						m_taskMgr.RefreshTask(Event_Base_Type_Lottery, 0,unCount);
						m_dailyTaskMgr.RefreshTask(Event_Base_Type_Lottery, 0,unCount);
					}
				}

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_LUCK_RESPONSE,(char *)&response, sizeof(MsgGetLuckResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);

			}
			break;
			/*
			case MSG_GET_MORE_LUCK_REQUEST:
			{
			MsgGetMoreLuckRequest *body = (MsgGetMoreLuckRequest *)msgInfo.body;

			MsgGetMoreLuckResponse response;
			response.nResult = -1;
			response.unSN = body->unSN;
			ACE_OS::memset(response.unItemList, 0, 10 * sizeof(ACE_UINT32));

			if(!m_functionMgr.is_locked(FUNCTION_GET_LUCK) && !this->m_packageMgr.is_full())
			{
			int t = (int)GAME_CONFIG_INSTANCE::instance()->m_unGetMoreLuckCost;
			if(this->m_unMoney >= t)
			{
			LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]get more luck", m_unID);
			this->set_basic_info(0, -t, 0, 0, 0);
			response.nResult = 0;
			for(int i = 0;i < 10;i ++)
			{
			response.unItemList[i] = this->get_luck(m_unLevel, 2);

			}

			this->update_use_luck_count();
			}
			}

			//int type = GET_ITEM_TYPE(response.unItemID);

			this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_MORE_LUCK_RESPONSE,(char *)&response, sizeof(MsgGetMoreLuckResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
			send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);

			}
			break;
			*/
		case MSG_ITEM_LEVEL_UP_REQUEST://物品升级
			{
				MsgItemLevelUpRequest *body = (MsgItemLevelUpRequest *)msgInfo.body;

				MsgItemLevelUpResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = level_up_item(body->unIdx, body->unPlayerID, body->unLevel);
				
				response.unCoin = m_unCoin;
				response.unPowerPoint = m_unPowerPoint;
				m_playerMgr.m_teamProperty.build_property();
				response.unPower = m_playerMgr.m_teamInfo.unPoint;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ITEM_LEVEL_UP_RESPONSE,(char *)&response, sizeof(MsgItemLevelUpResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);

			}
			break;
		case MSG_EQUIPMENT_STEP_UP_REQUEST://升阶
			{
				MsgEquipmentStepUpRequest *body = (MsgEquipmentStepUpRequest *)msgInfo.body;

				MsgEquipmentStepUpResponse response;
				response.unSN = body->unSN;

				if(body->unGrade < 1)
					body->unGrade = 1;

				response.nResult = equipment_step_up(body->unIdx, body->unPlayerID, (int *)body->unMaterialIdxList,body->unGrade);
				m_playerMgr.m_teamProperty.build_property();
				response.unPower = m_playerMgr.m_teamInfo.unPoint;

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_EQUIPMENT_STEP_UP_RESPONSE, (char *)&response, sizeof(MsgEquipmentStepUpResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_SKILL_LEVEL_UP_REQUEST://技能升级
			{
				MsgSkillLevelUpRequest *body = (MsgSkillLevelUpRequest *)msgInfo.body;

				MsgSkillLevelUpResponse response;
				response.unSN = body->unSN;
				response.nResult = ERROR_MSG_LOCKED;
				if(!m_functionMgr.is_locked(FUNCTION_IMPROVE_SKILL))
				{
					//if(body->skillType == 1)
					//{
					response.nResult = m_skillMgr.level_up(body->unSkillID, body->unLevel,response.unDynamicEnergy);

					LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]skill level up,skillID=0x%08x", m_unID, body->unSkillID);
					//}
					//else
					//{
					//	response.nResult = m_playerMgr.levelup_extra_skill(body->unSkillID);
					//	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]extra skill level up,skillID=0x%08x", m_unID, body->unSkillID);
					//}
				}

				m_playerMgr.m_teamProperty.build_property();
				response.unPower = m_playerMgr.m_teamInfo.unPoint;

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_SKILL_LEVEL_UP_RESPONSE,(char *)&response, sizeof(MsgSkillLevelUpResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);

				send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
			}
			break;
		case MSG_FIND_CHARACTER_REQUEST:
			{
				//MsgFindCharacterRequest *body = (MsgFindCharacterRequest *)msgInfo.body;

				this->forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_FIND_CHARACTER_RESPONSE:
			{
				MsgFindCharacterResponse *body = (MsgFindCharacterResponse *)msgInfo.body;

				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);

				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_DELETE_ITEM_REQUEST:
			{
				MsgDeleteItemRequest *body = (MsgDeleteItemRequest *)msgInfo.body;

				MsgDeleteItemResponse response;
				response.unSN = body->unSN;

				response.nResult = delete_item(body->unDeleteType, body->unCount, body->unIdxList);

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DELETE_ITEM_RESPONSE, (char *)&response, sizeof(MsgDeleteItemResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_CHANGE_NAME_REQUEST:
			{
				MsgChangeNameRequest *body = (MsgChangeNameRequest *)msgInfo.body;

				int t = 0;//(int)GAME_CONFIG_INSTANCE::instance()->m_unRenameCost; //现在改名不扣钱

				if(body->name[0] != '\0' && m_unMoney >= t)
				{
					MsgDBUpdateNameRequest db_request;
					db_request.unSN = body->unSN;
					db_request.unSig = m_unCurrentSig;
					ACE_OS::memcpy(db_request.name, body->name, MAX_CHARACTER_LENGTH);
					db_request.name[MAX_CHARACTER_LENGTH] = '\0';
					this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_UPDATE_NAME, (char *)&db_request, sizeof(MsgDBUpdateNameRequest));
				}
				else
				{
					MsgChangeNameResponse response;
					response.unSN = body->unSN;
					response.nResult = -1;
					memset(response.name,0,MAX_CHARACTER_LENGTH + 1);
					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_CHANGE_NAME_RESPONSE, (char *)&response, sizeof(MsgChangeNameResponse));
					send_response_end(m_unUserID, body->unSN);
				}
			}
			break;
		case MSG_GET_GLOBAL_MAP_EVENT_REQUEST:
			{
				MsgGetGlobalMapEventRequest *body = (MsgGetGlobalMapEventRequest *)msgInfo.body;

				GLOBAL_MAP_EVENT_MGR_INSTANCE::instance()->send_info(m_unCurrentTime, this, body->unSN);

				send_response_end(m_unUserID, body->unSN);
			}
			break;
			/*
			case MSG_SELECT_PLAYER_SKILL_REQUEST:
			{
			MsgSelectPlayerSkillRequest *body = (MsgSelectPlayerSkillRequest *)msgInfo.body;
			MsgSelectPlayerSkillResponse response;

			response.unSN = body->unSN;
			response.nResult = m_playerMgr.exchange_player_skill(body->unIndex, body->unSkillID);

			this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_SELECT_PLAYER_SKILL_RESPONSE, (char *)&response, sizeof(MsgSelectPlayerSkillResponse));
			send_response_end(m_unUserID, body->unSN);
			}
			break;
			*/
		case MSG_GOLD_BUY_REQUEST:
			{
				MsgGoldBuyRequest *body = (MsgGoldBuyRequest *)msgInfo.body;

				MsgGoldBuyResponse response;
				response.nResult = -1;
				response.unSN = body->unSN;

				/*
				if(body->unType == GOLD_BUY_TYPE_ENERGY)
				{

				if(this->m_cooldownMgr.can_do(m_unCurrentTime, BEHAVIOUR_BUY_ENERGY) == 0)
				{
				//nGold = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_GOLD, 0);
				response.nResult = m_energyMgr.set_full();

				if(response.nResult == 0)
				{
				LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]buy energy", m_unID);
				//m_limitMgr.do_once(BEHAVIOUR_BUY_ENERGY);
				m_cooldownMgr.do_once(m_unCurrentTime, BEHAVIOUR_BUY_ENERGY);
				}
				}


				}
				else if(body->unType == GOLD_BUY_TYPE_PVP_ENERGY)
				{

				if(this->m_cooldownMgr.can_do(m_unCurrentTime, BEHAVIOUR_BUY_ENERGY) == 0)
				{
				//nGold = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_GOLD, 0);
				response.nResult = m_pvpEnergyMgr.set_full();

				if(response.nResult == 0)
				{
				LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]buy pvp energy", m_unID);
				//m_limitMgr.do_once(BEHAVIOUR_BUY_ENERGY);
				m_cooldownMgr.do_once(m_unCurrentTime, BEHAVIOUR_BUY_ENERGY);
				}
				}
				}
				*/
				if(body->unType == GOLD_BUY_TYPE_PACKAGE)
				{
					response.nResult = m_packageMgr.expand_max_package();
					if(response.nResult == 0)
					{
						LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]buy package max", m_unID);
					}
				}
				else if(body->unType == GOLD_BUY_TYPE_FRIEND)
				{
					response.nResult = m_friendMgr.expand_max_friend();
					if(response.nResult == 0)
					{
						LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]buy friend max", m_unID);
					}
				}

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_GOLD_BUY_RESPONSE, (char *)&response, sizeof(MsgGoldBuyResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_USE_ITEM_REQUEST:
			{
				MsgUseItemRequest *body = (MsgUseItemRequest *)msgInfo.body;

				MsgUseItemResponse response;
				ZeroVar(response);
				response.nResult = ERROR_MSG_PARAMETER;
				response.unSN = body->unSN;

				switch(body->unItemType)
				{
				case USE_ITEM_TYPE_GIFT://使用道具，领礼包
					response.nResult = m_giftMgr.use_gift(body->unIndex, response.reward);
					break;
				case USE_ITEM_TYPE_PROPS:
					response.nResult = use_props(body->unIndex, response.reward);
					break;
				}

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_USE_ITEM_RESPONSE, (char *)&response, sizeof(MsgUseItemResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
// 		case MSG_PURCHASE_GOLD_REQUEST:
// 			{
// 				MsgPurchaseRequest *body = (MsgPurchaseRequest *)msgInfo.body;
// 
// 				MallRes *res = (MallRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(body->unPurchaseID);
// 
// 				MsgMsgPurchaseResponse response;
// 				response.nResult = -1;
// 
// 				if(res != NULL)
// 				{
// 					response.nResult = 0;
// 
// 					int nGold = res->m_unGold;
// 
// 					RewardInfo reward;
// 					ACE_OS::memset(&reward, 0, sizeof(RewardInfo));
// 
// 					reward.unMoney = nGold;
// 
// 					if(m_vipMgr.m_unRechargeCount == 0)
// 					{
// 						//nGold = (int)this->m_benefitMgr.get_benefit(BENEFIT_TYPE_FIRST_RECHARGE, (float)nGold);
// 						reward.unMoney += nGold;
// 
// 						this->add_reward_from_config_reward(reward, GAME_CONFIG_INSTANCE::instance()->m_firstRechargeReward, 1, 1);
// 						//nGold = 2 * nGold;
// 					}
// 
// 					//LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]recharge count = %d", m_unID, nGold);
// 					//-----------log日志
// 					//充值
// 					LogItemInfo logInfo;
// 					logInfo.m_goodsID = body->unPurchaseID;//id
// 					logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_purchase_gold;//动作原因
// 					logInfo.SetOp_Way(_LOG_OP_WAY_purchase_gold);//途径(附加信息)
// 					//类型【1001】充值类，0，数量
// 					m_LAMgr.DoLATypeLinkRecharge(LA_MISSION_TYPE_1001,nGold);
// 					this->get_reward(reward,logInfo);
// 					//this->set_basic_info(0, nGold, 0, 0, 0);
// 					//this->m_vipMgr.recharge(reward.unMoney, true);
// 					//this->m_vipMgr.recharge(nGold, true);
// 					this->m_vipMgr.recharge( res->m_unRMB, true); //adjust to RMB
// 				}
// 
// 				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_PURCHASE_GOLD_RESPONSE, (char *)&response, sizeof(MsgMsgPurchaseResponse), msgInfo.s_head->srcAdr);
// 			}
// 			break;
		case MSG_UPDATE_TUTORIAL_FLAG_REQUEST:
			{
				MsgUpdateTutorialFlagRequest *body = (MsgUpdateTutorialFlagRequest *)msgInfo.body;

				m_unTutorialFlag = body->unTutorialFlag;

				MsgUpdateTutorialFlagResponse response;
				response.nResult = 0;
				response.unSN = body->unSN;

				MsgDBUpdateTutorialFlagRequest db_request;
				db_request.unTutorialFlag = m_unTutorialFlag;

				this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_UPDATE_TUTORIAL_FLAG, (char *)&db_request, sizeof(MsgDBUpdateTutorialFlagRequest));

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_UPDATE_TUTORIAL_FLAG_RESPONSE, (char *)&response, sizeof(MsgUpdateTutorialFlagResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_DO_BEHAVIOR_REQUEST:
			{
				MsgDoBehaviorRequest *body = (MsgDoBehaviorRequest *)msgInfo.body;

				MsgDoBehaviorResponse response;

				response.nResult = -1;

				if(GET_ITEM_TYPE(body->unID) == ITEM_RES_TYPE_BEHAVIOR)
				{
					BehaviorRes *res = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(body->unID);

					if(res != NULL)
					{
						if(res->m_nVipLevelRequired != 0 && res->m_nVipLevelRequired != m_vipMgr.m_unVipLevel)
						{
							response.nResult = ERROR_MSG_COST;
						}
						else
						{
							if(is_cost_enough(res->m_cost, 1))
							{
								int dwEatCount = get_eat_everyday_count();
								if(/*body->unID == BEHAVIOR_EAT_RSG_REWARD || */m_cooldownMgr.can_do(m_unCurrentTime, body->unID) == 0)
								{
									m_cooldownMgr.do_once(m_unCurrentTime, body->unID);
									//-----------log日志
									//操作
									LogItemInfo logInfo;
									logInfo.m_goodsID = body->unID;
									logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_do_behavior;//动作原因
									logInfo.SetOp_Way(_LOG_OP_WAY_do_behavior);//途径(附加信息)
									do_cost(res->m_cost, 1,logInfo);
									response.nResult = 0;

									if(body->unID == BEHAVIOR_MONEY_TREE_WATERING)
									{
										//GameUtils::reset_reward(response.reward);
										//m_moneyTreeMgr.watering();
									}
									else if(body->unID == BEHAVIOR_MONEY_TREE_REWARD)
									{
										m_moneyTreeMgr.get_reward(response.reward);
										m_moneyTreeMgr.watering();
									}
									else if(body->unID == BEHAVIOR_EAT_RSG_REWARD)
									{
										/*response.nResult = -1;
										int dwCurTime = GameUtils::get_utc();
										int dw00_00_00 = dwCurTime - (dwCurTime % (24 * 3600));
										int dw12_00_00 = dw00_00_00 + 12 * 3600;
										int dw14_00_00 = dw00_00_00 + 14 * 3600;
										int dw18_00_00 = dw00_00_00 + 18 * 3600;
										int dw20_00_00 = dw00_00_00 + 20 * 3600;
										int dwEatCount = get_eat_everyday_count();
										switch(dwEatCount)
										{
										case 0:
										if(dwCurTime < dw12_00_00 || dwCurTime >= dw14_00_00)
										break;
										case 1:
										if(dwCurTime < dw18_00_00 || dwCurTime >= dw20_00_00)
										break;*/
										//GameUtils::reset_reward(response.reward);
										get_reward_from_config_reward(response.reward, res->m_reward, 1, 1);
										set_eat_everyday_count(++dwEatCount);
										response.nResult = 0;
										//	break;
										//default:
										//	break;
										//}

										//GameUtils::reset_reward(response.reward);
										//get_reward_from_config_reward(response.reward, res->m_reward, 1, 1);
										//response.reward.unEnergy = m_energyMgr.need_value_2_full();
										//ACE_UINT32 tmpEnergy = m_energyMgr.m_unEnergy;
										/*
										if(m_energyMgr.set_full() == 0)
										{
										response.reward.unEnergy = m_energyMgr.m_unEnergy - tmpEnergy;
										}
										*/
									}
									else if(body->unID == BEHAVIOR_EVERYDAY_REWARD)
									{
										int bonus_idx = abs((int)m_unLoginBonusCount) - 1;
										//if(bonus_idx >= MAX_LOGIN_BONUS_COUNT)
										//{
										//	bonus_idx = MAX_LOGIN_BONUS_COUNT - 1;
										//}
										//int t_money = GAME_CONFIG_INSTANCE::instance()->m_loginBonusList[bonus_idx];
										////this->set_basic_info(0, t_money, 0, 0, 0);
										//GameUtils::reset_reward(response.reward);
										//response.reward.unMoney = t_money;
										if(bonus_idx >= MAX_LOGIN_BONUS_COUNT || (int)m_unLoginBonusCount < 0)
										{
											response.nResult = -1;
										}
										else
										{
											BehaviorRes *resBounus = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(GAME_CONFIG_INSTANCE::instance()->m_loginBonusList[bonus_idx]);
											if(NULL == resBounus)
												response.nResult = ERROR_MSG_RES;
											else
											{
												GameUtils::reset_reward(response.reward);
												get_reward_from_config_reward(response.reward, resBounus->m_reward, 1, 1);
												m_unLoginBonusCount = (ACE_UINT32)(-(int)m_unLoginBonusCount);

												MsgDBUpdateLoginInfoRequest db_request;
												db_request.unLoginTime = m_unLastLoginTime;
												db_request.unLoginCount = m_unLoginCount;
												db_request.unLoginBonusCount = m_unLoginBonusCount;
												this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_LOGIN_INFO, (char *)&db_request, sizeof(MsgDBUpdateLoginInfoRequest));
											}
										}
									}
									else
									{
										get_reward_from_config_reward(response.reward, res->m_reward, 1, 1);
									}
									LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]do behaviour,behaviour ID = 0x%08x", m_unID, body->unID);
									if(response.nResult == 0)
									{
										//-----------log日志
										//操作
										LogItemInfo logInfo;
										logInfo.m_goodsID = body->unID;
										logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_do_behavior;//动作原因
										logInfo.SetOp_Way(_LOG_OP_WAY_do_behavior);//途径(附加信息)
										get_reward(response.reward,logInfo);
									}
								}
							}
							else
								response.nResult = ERROR_MSG_COST;
						}
					}
					else
						response.nResult = ERROR_MSG_RES;
				}

				if(response.nResult != 0)
				{
					GameUtils::reset_reward(response.reward);
				}

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_BEHAVIOR_RESPONSE, (char *)&response, sizeof(MsgDoBehaviorResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_DO_BUYENERGY_REQUEST:
			{
				MsgDoBuyEnergyRequest *body = (MsgDoBuyEnergyRequest *)msgInfo.body;
				MsgDoBuyEnergyResponse response;
				ZeroVar(response);
				response.nResult = -1;
				response.unCurrentCount = m_unCurrentBuyEnergy;

				BuyEnergyCost buyEnergyCost= GAME_CONFIG_INSTANCE::instance()->m_oBuyEnergyCost.GetOne(m_unCurrentBuyEnergy+1);

				CostInfoList cost;
				ZeroVar(cost);
				cost.unCount = 1;
				cost.list[0].unID = RES_CURRENCY_MONEY;//消耗金卡值
				cost.list[0].cost = buyEnergyCost.cost;
				if (!m_energyMgr.CanBuy())
					response.nResult = ERROR_MSG_MAX;
				else if (!is_cost_enough(cost, 1))
					response.nResult = ERROR_MSG_COST;
				else if (!m_vipMgr.isEnergyCountIn(m_unCurrentBuyEnergy))
					response.nResult = ERROR_MSG_TIMES;
				else//足够扣 并且没有超过购买量
				{
					response.nResult = 0;
					response.unMoneyCost = buyEnergyCost.cost;
					response.unEnergy =buyEnergyCost.Energy_Recover;

					++m_unCurrentBuyEnergy;
					SendBuyEnergyToDB();
					response.unCurrentCount = m_unCurrentBuyEnergy;
					//-----------log日志
					//操作
					LogItemInfo logInfo;
					logInfo.m_goodsID = RES_CURRENCY_MONEY;
					logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_buy_energy;//动作原因
					logInfo.SetOp_Way(_LOG_OP_WAY_buy_energy);//途径(附加信息)
					do_cost(cost, 1,logInfo);

					LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]do buy energy", m_unID);
					//if(response.nResult == 0)
					{
						//-----------log日志
						//操作
						LogItemInfo logInfo;
						logInfo.m_goodsID = RES_CURRENCY_ENERGY;
						logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_buy_energy;//动作原因
						logInfo.SetOp_Way(_LOG_OP_WAY_buy_energy);//途径(附加信息)
						RewardInfo reward;
						ZeroVar(reward);
						reward.unEnergy = response.unEnergy;
						get_reward(reward,logInfo);
					}
				}

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_BUYENERGY_RESPONSE, (char *)&response, sizeof(MsgDoBuyEnergyResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_COMPOSE_REQUEST://合成
			{
				MsgComposeRequest *body = (MsgComposeRequest *)msgInfo.body;

				MsgComposeResponse response;
				response.unSN = body->unSN;
				response.nResult = do_compose(body->unItemID,body->unTimes);

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_COMPOSE_RESPONSE, (char *)&response, sizeof(MsgComposeResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_BROADCAST_MESSAGE_REQUEST:
			{
				MsgGetBroadcastMessageRequest *body = (MsgGetBroadcastMessageRequest *)msgInfo.body;

				BROADCAST_MGR_INSTANCE::instance()->send_info(this, msgInfo.s_head->srcAdr, body->unSN);
			}
			break;
		case MSG_PVP_RANK_INFO_REQUEST:
			{
				MsgPVPRankInfoRequest *body = (MsgPVPRankInfoRequest *)msgInfo.body;
				//m_pvpMgr.send_info(msgInfo.s_head->srcAdr, body->unSN);
				//m_pvpMgr.get_pvp_info(body->unSN);
			}
			break;
		case MSG_COMMIT_PVP_RESULT_REQUEST:
			{
				//MsgCommitPVPResultRequest *body = (MsgCommitPVPResultRequest *)msgInfo.body;
				//m_pvpMgr.commit_result(msgInfo.s_head->srcAdr, body->unSN, body->win, body->commandData, body->unSize, m_unCurrentTime);
			}
			break;
		case MSG_GET_PVP_RANK_REWARD_REQUEST:
			{
				//MsgGetPVPRankRewardRequest *body = (MsgGetPVPRankRewardRequest *)msgInfo.body;
				//m_pvpMgr.get_pvp_reward(msgInfo.s_head->srcAdr, body->unSN);
			}
			break;
		case MSG_GET_PVP_LEADERBOARD_REQUEST:
			{
				MsgGetPVPLeaderboardRequest *body = (MsgGetPVPLeaderboardRequest *)msgInfo.body;
				if(body->unType == PVP_LEADERBOARD_FRIEND)
				{
					MsgGetPVPLeaderboardRequest *request = (MsgGetPVPLeaderboardRequest *)public_send_buff;
					request->unSN = body->unSN;
					request->unType = body->unType;
					request->unCount = m_friendMgr.m_unFriendCount;
					int i;

					for(i = 0;i < m_friendMgr.m_unFriendCount;i ++)
					{
						request->list[i] = m_friendMgr.m_friendIDList[i];
					}

					int size = sizeof(MsgGetPVPLeaderboardRequest) + sizeof(ACE_UINT32) * (request->unCount-1);
					sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_PVP_LEADERBOARD_REQUEST, (char *)request, size, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
				}
				else
				{
					this->forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
				}
			}
			break;
		case MSG_GET_PVP_LEADERBOARD_RESPONSE:
			{
				MsgGetPVPLeaderboardResponse *body = (MsgGetPVPLeaderboardResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				//if (body->unPage <= 1)
					send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_START_PVP_REQUEST:
			{
				MsgStartPVPRequest *body = (MsgStartPVPRequest *)msgInfo.body;

				//m_pvpMgr.start_pvp(msgInfo.s_head->srcAdr, body->unSN, m_unCurrentTime, body->unContinue, body->unFriendID);
			}
			break;
		case MSG_START_PVP_RESPONSE:
			{
				//MsgStartPVPResponse *body = (MsgStartPVPResponse *)msgInfo.body;
				//m_pvpMgr.start_pvp(msgInfo.s_head->srcAdr, body->unSN, m_unCurrentTime);
				//m_pvpMgr.get_opponent_response(body->nResult, body->unSN, body->unStatus, body->pvpType, &body->opponent, m_unCurrentTime);
			}
			break;
		case MSG_UPDATE_PLAYER_RANK_INFO_RESPONSE:
			{
				//MsgUpdatePlayerRankInfoResponse *body = (MsgUpdatePlayerRankInfoResponse *)msgInfo.body;
				//m_pvpMgr.start_pvp(msgInfo.s_head->srcAdr, body->unSN, m_unCurrentTime);
				//m_pvpMgr.get_pvp_rank_response(&body->rankInfo, body->unTotalCount);
			}
			break;
			/*
			case MSG_UPDATE_PVP_TEAM_REQUEST:
			{
			MsgUpdatePVPTeamRequest *body = (MsgUpdatePVPTeamRequest *)msgInfo.body;

			m_pvpMgr.update_pvp_basic_info(body->playerList, body->skillList, body->unSN);
			}
			break;
			*/

		case MSG_GET_PVP_TEAM_INFO_REQUEST:
			{
				this->forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			break;
		case MSG_GET_PVP_TEAM_INFO_RESPONSE:
			{
				MsgGetPVPTeamInfoResponse *body = (MsgGetPVPTeamInfoResponse *)msgInfo.body;
				this->forward_msg(msgInfo, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_GET_MAP_GROUP_REWARD_REQUEST:
			{
				MsgGetMapGroupRewardRequest *body = (MsgGetMapGroupRewardRequest *)msgInfo.body;
				MsgGetMapGroupRewardResponse response;
				response.unSN = body->unSN;
				response.nResult = m_mapInfoMgr.get_map_gift(body->unMapGroupID, body->unIdx, response.reward);

				if(response.nResult == 0)
				{
					LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]get map group reward,Group ID = 0x%08x, index = %d", m_unID, body->unMapGroupID, body->unIdx);
				}

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_GET_MAP_GROUP_REWARD_RESPONSE, (char *)&response, sizeof(MsgGetMapGroupRewardResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_BIND_SUCCESS_REQUEST:
			{
				MsgBindSuccessRequest *body = (MsgBindSuccessRequest *)msgInfo.body;
				MsgBindSuccessResponse response;
				response.nResult = 0;

				LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]bind bonus", m_unID);

				//this->set_basic_info(0, 10, 0, 0, 0);

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_BIND_SUCCESS_RESPONSE, (char *)&response, sizeof(MsgBindSuccessResponse), msgInfo.s_head->srcAdr);
			}
			break;

		case MSG_WEAPON_FRAGMENT_STEP_UP_REQUEST:
			{
				MsgWeaponFragementStepUpRequest *body = (MsgWeaponFragementStepUpRequest *)msgInfo.body;

				MsgWeaponFragementStepUpResponse response;
				response.unSN = body->unSN;

				response.nResult = equipment_step_up(body->unIdx, body->unPlayerID, (int *)body->unMaterialIdxList, 1);

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_WEAPON_FRAGMENT_STEP_UP_RESPONSE, (char *)&response, sizeof(MsgWeaponFragementStepUpResponse));
				send_response_end(m_unUserID, body->unSN);
			}
			break;
		case MSG_ADMIN_OP_REQUEST:
			{
				deal_admin_op((MsgAdminOPRequest *)msgInfo.body, msgInfo.s_head->srcAdr);
			}
			break;
		case MSG_SECRET_LIST_REQUEST:
			{
				MsgSecretListRequest *body = (MsgSecretListRequest *)msgInfo.body;
				MsgSecretListResponse response;
				response.nResult = 0;	

				response.unSN = body->unSN;
				response.nResult = secret_refresh(m_secretMgr.m_list,body->unType);				
				m_secretMgr.send_info(msgInfo.s_head->srcAdr,body->unSN,response.nResult);//发送返回信息
				send_response_end(m_unUserID, body->unSN);

			}
			break;

		case MSG_SECRET_EXPAND_REQUEST:
			{
				MsgSecreteExpandRequest *body = (MsgSecreteExpandRequest *)msgInfo.body;
				MsgSecretListResponse response;
				response.nResult = 0;	

				response.unSN = body->unSN;
				response.nResult = secret_expand(body->unCellCount);				
				m_secretMgr.send_info(msgInfo.s_head->srcAdr,body->unSN,response.nResult);//发送返回信息,-1貌似没用了
				send_response_end(m_unUserID, body->unSN);

			}
			break;
		case MSG_GET_ARTICLE_REQUEST:  //added for test jinpan
			{

				MsgGetArticleRequest *body = (MsgGetArticleRequest *)msgInfo.body;				
				MsgGetArticleResponse response;



				response.nResult = deal_get_article(body->unArticleId,body->unCount);

				//	this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_COMPOSE_RESPONSE, (char *)&response, sizeof(MsgGetArticleResponse));
				//	send_response_end(m_unUserID,body->unSN);
			}
			break;
		case MSG_SECRET_BUY_REQUEST:
			{
				MsgSecretBuyRequest *body = (MsgSecretBuyRequest *)msgInfo.body;

				MsgSecretListResponse response;
				response.unSN = body->unSN;
				response.nResult = secret_buy(body->unItemId,body->unIdx);

				m_secretMgr.send_info(msgInfo.s_head->srcAdr,body->unSN,response.nResult);
				send_response_end(m_unUserID, body->unSN);

			}
			break;

		case MSG_ADD_NEW_MAP_REQUEST:
			{
				MsgAddNewMapRequest *body = (MsgAddNewMapRequest *)msgInfo.body;
				MsgAddNewMapResponse response;
				response.unSN = body->unSN;
				response.nResult = m_mapInfoMgr.unlock_map(body->unMapID);
				response.unMapID = body->unMapID;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_ADD_NEW_MAP_RESPONSE, (char *)&response, sizeof(MsgAddNewMapResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_GM_REQUEST://GM推送请求
			{
				MsgAnnouncementRequest *body = (MsgAnnouncementRequest*)msgInfo.body;
				MsgGMRequest request;
				request.unSN = body->unSN;
				request.unTypeID = body->unTypeID;
				request.unToID = m_unID;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DB_GM_REQUEST, (char *)&request, sizeof(MsgGMRequest), this->m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
				//this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_RESET_MAP_RESPONSE,(char *)&response, sizeof(MsgAnnouncementRsponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
			}break;
		case MSG_DB_GM_RESPONSE://GM推送返回
			{
				MsgGMRsponse * response = (MsgGMRsponse *)msgInfo.body;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GM_RESPONSE,(char *)response, sizeof(MsgGMRsponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
			}break;
		case MSG_GET_LA_LIST_REQUEST://获取限时活动请求
			{
				MsgBaseRequest * body = (MsgBaseRequest *)msgInfo.body;
				MsgGMLimitedActivityListResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = m_LAMgr.GetLACurrentList(response);//当前活动列表
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_LA_LIST_RESPONST,(char *)&response, sizeof(MsgGMLimitedActivityListResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_GET_LA_REWARD_REQUEST://限时活动领取请求
			{
				MsgGMLARewardRequest * body = (MsgGMLARewardRequest *)msgInfo.body;
				MsgGMLARewardResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = m_LAMgr.GetReward(body->unLAMID,response.stReward);
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_LA_REWARD_RESPONST,(char *)&response, sizeof(MsgGMLimitedActivityListResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_DESTROY_MSG_ACK://异常断开
			{
				//
				MsgDestroyRequest * body = (MsgDestroyRequest *)msgInfo.body;
				LogItemInfo logInfo;
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_quit;//动作原因
				logInfo.SetOp_Way(_LOG_OP_WAY_quit);//途径(附加信息)
				SetlogUserInfo(logInfo);//写入基本信息
				std::vector<LogItemInfo> logInfoVer;
				logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_DEFALT;
				logInfoVer.push_back(logInfo);
				SendLogInfoItemMSG(logInfoVer);//开始日志请求

				this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DESTROY_MSG_ACK, (char *)body, sizeof(MsgDestroyRequest));
				this->sendMsg(SERVICE_MESSAGE_TYPE_DB_REQUEST, MSG_DESTROY_MSG_ACK, (char *)&body, sizeof(MsgDestroyRequest),
					OBJ_ID_NONE, ADAPTER_DB_USER_MGR, m_unID);
				if (body->unErrorCode == 1)//封停消息
				{
					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DESTROY_MSG_ACK, (char *)body, sizeof(MsgDestroyRequest));
				}

				this->set_destroy(true);
				//this->setObjStatus(SERVICE_OBJ_DESTROY);
				m_unLiveCounter = MAX_MAP_SESSION_LIVE-1;
				printf("map destroy obj：[%d]\n",m_unID);
				m_bValidHome = false;

			}break;
		case MSG_DO_ORDER_REQUEST://订单号请求
			{
				MsgDoOrderRequest *info = (MsgDoOrderRequest *)msgInfo.body;
				const RechargeRes * res = (const RechargeRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(info->product_id);
				int nResult = m_LAMgr.ChickLARecharge(res);
				if (nResult >= 0)
				{
					MsgDBOrderRequest db_request;
					db_request.unSig = m_unCurrentSig;
					db_request.unSN = info->unSN;

					db_request.product_id = info->product_id;
					db_request.product_count = info->product_count;
					this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_ORDER_REQUEST, (char *)&db_request, sizeof(db_request));
				}
				else
				{
					MsgDoOrderResponse response;
					ZeroVar(response);
					response.nResult = nResult;

					response.unSN = info->unSN;
					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_ORDER_RESPONSE, (char *)&response, sizeof(MsgDoOrderResponse));
					send_response_end(m_unUserID, info->unSN);
				}


			}break;
		case MSG_DB_ORDER_RESPONSE://订单号请求返回结果
			{
				MsgDBOrderResponse *info = (MsgDBOrderResponse *)msgInfo.body;
				MsgDoOrderResponse response;
				ZeroVar(response);
				memcpy(response.orderID,info->orderID,RECHARGE_ORDERID_LEN);
				response.nResult = info->nResult;
				response.unSN = info->unSN;
				ACE_ASSERT(info->m_unID == m_unID);
				//response.m_unID = info->m_unID;

				ACE_UINT32 unOldSig = m_unCurrentSig;
				m_unCurrentSig = info->unSig;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_ORDER_RESPONSE, (char *)&response, sizeof(MsgDoOrderResponse));
				send_response_end(m_unUserID, info->unSN);
				m_unCurrentSig = unOldSig;
				
			}break;
		case MSG_DO_RECHARGE_REQUEST://充值客户端请求
			{
				MsgDoRechargeRequest * body = (MsgDoRechargeRequest *)msgInfo.body;
				m_rechargeOrderSN = body->unSN;
				if (ACE_OS::strcmp(body->orderID, m_rechargeOrder.orderID) == 0)//结果消息已经来了
				{
					Deal_recharge_op_result();
				}
				else /*if(ACE_OS::strlen(m_rechargeOrder.orderID) > 0)*///orderid不匹配
				{
					MsgDoRechargeResponse response;
					ZeroVar(response);
					response.nResult = ERROR_MSG_RECHARGE_NULL;
					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_RECHARGE_RESPONSE, (char *)&response, sizeof(MsgDoRechargeResponse));
					send_response_end(m_unUserID, response.unSN);

					m_rechargeOrderSN = 0;
					ZeroVar(m_rechargeOrder);
				}

			}break;
		case MSG_ADMIN_OP_RECHARGE_REQUEST://充值gm返回
			{
				m_rechargeOrder = *((OrderInfo *)msgInfo.body);
				m_unMoney += m_rechargeOrder.unMoney;//更新钱
				m_bitField = m_rechargeOrder.unbitfield;//更新标记

				UpdateProductList(m_rechargeOrder.unProductId);

				//充值
				LogItemInfo logInfo;
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_purchase_gold;//动作原因
				logInfo.SetOp_Way(_LOG_OP_WAY_purchase_gold);//途径(附加信息)
// 				RewardInfo reward;
// 				ZeroVar(reward);
// 				get_reward_from_config_reward(reward, m_rechargeOrder.reward, 1, 1);//此处已经去掉了金币的内容，现在加入其它东西
// 				get_reward(reward,logInfo);

				logInfo.m_goodsID = m_rechargeOrder.unMoney;//id
				SetlogUserInfo(logInfo);//写入基本信息
				std::vector<LogItemInfo> logInfoVer;
				logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_money_add;
				logInfo.m_op_before = m_unMoney - m_rechargeOrder.unMoney;
				logInfo.m_op_after = m_unMoney;
				logInfoVer.push_back(logInfo);
				SendLogInfoItemMSG(logInfoVer);//开始日志请求
				//类型【1001】充值类，0，数量
				m_LAMgr.DoLATypeLinkRecharge(LA_MISSION_TYPE_1001,m_rechargeOrder.unMoney);
				m_LAMgr.DoLATypeLinkRechargeGift(m_rechargeOrder.unProductId);

				//Deal_recharge_op_result();
			}break;
		case MSG_DO_PIECE_ROLE_REQUEST://碎片转角色
			{
				MsgDoPieceToRoleRequest * body = (MsgDoPieceToRoleRequest *)msgInfo.body;
				MsgDoPieceToRoleResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.unPieceRoleID = body->unItemID;
				response.nResult = m_playerMgr.PieceToPlayer(body->unItemID,response.unPlayerID);

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_PIECE_ROLE_RESPONSE, (char *)&response, sizeof(MsgDoPieceToRoleResponse));
				send_response_end(m_unUserID, response.unSN);
			}break;
		case MSG_DO_UPDATE_LIVEL_REQUEST://更新vip等级
			{
				m_vipMgr.UpdateVipRes(*((VipInfo *)msgInfo.body));
			}break;
		case MSG_DO_GET_RECHARGE_INFO_REQUEST://获取充值信息
			{
				MsgBaseRequest * body = (MsgBaseRequest *)msgInfo.body;
				MsgDoGetRechargeInfoResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.m_bitField = m_bitField.GetValue();
				response.m_Productlist = m_Productlist;
				response.info = m_vipMgr.m_vipInfo;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_GET_RECHARGE_INFO_RESPONSE, (char *)&response, sizeof(MsgDoGetRechargeInfoResponse));
			}break;
		case MSG_DO_SHARED_ACTIVITES_REQUEST://分享活动
			{
				MsgBaseRequest * body = (MsgBaseRequest *)msgInfo.body;
				MsgSharedActivitesResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult=SharedActivites(response.reward);//此处应该添加分享功能
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_SHARED_ACTIVITES_RESPONSE, (char *)&response, sizeof(MsgSharedActivitesResponse));
				send_response_end(m_unUserID, response.unSN);
			}break;
		case MSG_DO_VIP_GIFT_REQUEST:
			{
				MsgVipGiftRequest * body = (MsgVipGiftRequest *)msgInfo.body;
				MsgVipGiftResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.unGiftID = body->ungiftId;
				response.nResult = m_vipMgr.GetVipGift(body->ungiftId,body->unvipLevel,response.reward);
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_VIP_GIFT_RESPONSE, (char *)&response, sizeof(MsgVipGiftResponse));
				send_response_end(m_unUserID, response.unSN);
			}break;
		case MSG_DO_DB_SERVER_TIME_HARD:
			{
				printf("db server is hard !!!sleep some times\n");
				MsgDoDBTimeHardRequest * body = (MsgDoDBTimeHardRequest *)msgInfo.body;

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_DB_SERVER_TIME_HARD, (char *)body, sizeof(MsgDoDBTimeHardRequest));
				//Sleep(ITME_HARD_SLEEP);
			}break;
		case MSG_DO_Get_BadgeGaint_REQUEST:
			{
				MsgBaseRequest * body = (MsgBaseRequest *)msgInfo.body;
				m_badgeMgr.send_Gaint_info(body->unSN);

			}break;
		case MSG_DO_Up_BadgeGaint_REQUEST:
			{
				MsgDoUpBadgeGaintRequest * body = (MsgDoUpBadgeGaintRequest *)msgInfo.body;
				MsgBaseResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = Badge_step_up(body,false);
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_Up_BadgeGaint_RESPONSE, (char *)&response, sizeof(MsgBaseResponse));
				send_response_end(m_unUserID, response.unSN);
			}break;
		case MSG_DO_GET_Badge_REQUEST:
			{
				MsgGETBadgeRequest *body = (MsgGETBadgeRequest *)msgInfo.body;
				MsgGETBadgeResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = m_badgeMgr.GetGaintPage(response,body->unPage,body->isClear);
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_GET_Badge_RESPONSE, (char *)&response, sizeof(MsgGETBadgeResponse));
				send_response_end(m_unUserID, response.unSN);

			}break;
		case MSG_GET_CHARACTER_INFO_REQUEST://获取用户信息
			{
				this->forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}break;
		case MSG_GET_CHARACTER_INFO_RESPONSE:
			{
				MsgGetCharacterInfoResponse *body = (MsgGetCharacterInfoResponse *)msgInfo.body;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_GET_CHARACTER_INFO_RESPONSE, (char *)body, sizeof(MsgGetCharacterInfoResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_GET_PVP_INFO_REQUEST://获取信息
			{
				MsgBaseRequest *body = (MsgBaseRequest *)msgInfo.body; 
				m_pvpMgr.OnDo();
				MsgDBUpdatePVPInfoResquestEx request;
				request.unSN = body->unSN;
				request.info.m_characterID = m_unID;
				request.info.m_honour = m_pvpMgr.m_pvpInfo.nhonour;

				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_PVP_INFO_REQUEST, 
					(char *)&request, sizeof(MsgDBUpdatePVPInfoResquestEx), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}break;
		case MSG_GET_PVP_INFO_RESPONSE:
			{
				MsgGetPVPInfoResponse * body = (MsgGetPVPInfoResponse *)msgInfo.body;
				m_pvpMgr.update();
				m_pvpMgr.update_pvp_Info(body->info);//更新剩余部分的
				if (body->info.nhonour < 0)
				{
					body->info.nhonour = 0;
					body->info.nhonourRank = -1;
				}

				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_GET_PVP_INFO_RESPONSE, (char *)body, sizeof(MsgGetPVPInfoResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_PVP_ENTER_FIGHT_REQUEST:
			{
				MsgPVPEnterFightRequestEx *body = (MsgPVPEnterFightRequestEx *)msgInfo.body;
				m_pvpMgr.OnDo();

				int nResult = 0;

				if (!body->isReFreash )//不刷新
				{
					if (m_pvpMgr.m_array.uncount == 0)//还没取到
						nResult = 1;
				}
				else
				{
					nResult = 1;
					if(!m_pvpMgr.DoFreashen())//扣次数
						nResult = ERROR_MSG_MAX;
				}

				if (nResult <= 0)
				{
					MsgPVPEnterFightResponse response;
					ZeroVar(response);
					response.nResult = nResult;
					response.unSN = body->unSN;
					response.info = m_pvpMgr.m_pvpInfo;
					response._array = m_pvpMgr.m_array;

					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_ENTER_FIGHT_RESPONSE, (char *)&response, sizeof(MsgPVPEnterFightResponse));
					send_response_end(m_unUserID, body->unSN);
				}
				else//需要刷新
				{
					MsgPVPEnterFightRequest request;
					request.unSN = body->unSN;
					request.m_characterID = m_unID;
					request.isReFreash = body->isReFreash;
					m_pvpMgr.GetQuitList(request.quitList);
					this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_ENTER_FIGHT_REQUEST, 
						(char *)&request, sizeof(MsgPVPEnterFightRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
				}
			}break;
		case MSG_PVP_ENTER_FIGHT_RESPONSE:
			{
				m_pvpMgr.update();
				MsgPVPEnterFightResponse * body = (MsgPVPEnterFightResponse *)msgInfo.body;
				if (body->nResult >= 0)//进入战斗
					m_pvpMgr.EnterPVPFight(body->_array);

				if (body->isReFreash != 2)//客户端请求
				{
					body->info = m_pvpMgr.m_pvpInfo;
					this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_ENTER_FIGHT_RESPONSE, (char *)body, sizeof(MsgPVPEnterFightResponse));
					send_response_end(m_unUserID, body->unSN);
				}
			}break;
		case MSG_PVP_GET_ORDER_REQUEST:
			{
				this->forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}break;
		case MSG_PVP_GET_ORDER_RESPONSE:
			{
				MsgPVPGetOrderResponse * response = (MsgPVPGetOrderResponse *)msgInfo.body;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_GET_ORDER_RESPONSE, (char *)response, sizeof(MsgPVPGetOrderResponse));
				send_response_end(m_unUserID, response->unSN);
			}break;
		case MSG_PVP_BUY_TIMES_REQUEST:
			{
				MsgBaseResponse *body = (MsgBaseResponse *)msgInfo.body;
				MsgPVPBuyTimesResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = m_pvpMgr.BuyTimes();
				response.unMoney = m_unMoney;
				response.info = m_pvpMgr.m_pvpInfo;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_BUY_TIMES_RESPONSE, (char *)&response, sizeof(MsgPVPBuyTimesResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_PVP_ENTER_REQUEST:
			{
				MsgBaseResponse *body = (MsgBaseResponse *)msgInfo.body;
				MsgPVPEnterResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = m_pvpMgr.EnterMap();
				response.info = m_pvpMgr.m_pvpInfo;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_ENTER_RESPONSE, (char *)&response, sizeof(MsgPVPEnterResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_PVP_ACCOUNTS_REQUEST:
			{
				MsgPVPAccountRequest * body = (MsgPVPAccountRequest *)msgInfo.body;
				MsgPVPAccountResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = m_pvpMgr.AccountsMap(body->info,response.score);
				response.info = m_pvpMgr.m_pvpInfo;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_ACCOUNTS_RESPONSE, (char *)&response, sizeof(MsgPVPEnterResponse));
				send_response_end(m_unUserID, body->unSN);
				//-----更新闯关最高伤害排行榜
				//MSG_FIGHT_ROOLE_REQUEST
				//------------------------------end
			}break;
		case MSG_PVP_GET_FightRecordInfo_REQUEST:
			{
				MsgBaseRequest *body = (MsgBaseRequest *)msgInfo.body;
				MsgPVPFightRecordInfoRequest request;
				request.unSN = body->unSN;
				request.m_characterID = m_unID;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_GET_FightRecordInfo_REQUEST, 
					(char *)&request, sizeof(MsgPVPFightRecordInfoRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}break;
		case MSG_PVP_GET_FightRecordInfo_RESPONSE:
			{
				MsgPVPFightRecordInfoResponse * response = (MsgPVPFightRecordInfoResponse *)msgInfo.body;
				if (response->nResult == 0)//更新战绩
				{
					m_pvpMgr.m_pvpInfo.unWinLost = response->pvpinfo.unWinLost;
					response->pvpinfo = m_pvpMgr.m_pvpInfo;
				}
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_PVP_GET_FightRecordInfo_RESPONSE, (char *)response, sizeof(MsgPVPFightRecordInfoResponse));
				send_response_end(m_unUserID, response->unSN);
			}break;
		case MSG_DO_Up_BadgeGaint_REQUESTEX:
			{
				MsgDoUpBadgeGaintRequest * body = (MsgDoUpBadgeGaintRequest *)msgInfo.body;
				MsgBaseResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				response.nResult = Badge_step_up(body,true);
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_Up_BadgeGaint_RESPONSEEX, (char *)&response, sizeof(MsgBaseResponse));
				send_response_end(m_unUserID, response.unSN);
			}break;
		case MSG_Update_badge_info_REQUESTE:
			{
				MsgUpdateBadgeInfoRequest * body = (MsgUpdateBadgeInfoRequest *)msgInfo.body;
				body->unCharacterID = m_unID;
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_Update_badge_info_REQUESTE, 
					(char *)body, sizeof(MsgUpdateBadgeInfoRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);

				//MsgBaseResponse response;
				//ZeroVar(response);
				//response.unSN = body->unSN;
				//this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_Update_badge_info_RESPONSE, (char *)&response, sizeof(MsgBaseResponse));
				//send_response_end(m_unUserID, response.unSN);
			}break;
		case MSG_Get_badge_info_REQUESTE:
			{
				this->forward_msg(msgInfo, m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}break;
		case MSG_Get_badge_info_RESPONSE:
			{
				MsgGetBadgeInfoResponse * response = (MsgGetBadgeInfoResponse *)msgInfo.body;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_Get_badge_info_RESPONSE, (char *)response, sizeof(MsgGetBadgeInfoResponse));
				send_response_end(m_unUserID, response->unSN);
			}break;
		case MSG_Get_active_inofo_REQUEST:
			{
				MsgBaseRequest * body = (MsgBaseRequest *)msgInfo.body;
				MsgGetActiveInfoResponse response;
				ZeroVar(response);
				response.unSN = body->unSN;
				if (m_LAMgr.m_isHaveNew)
				{
					response.key |= 0x01;
				}
				if (m_dailyTaskMgr.m_isHaveNew)
				{
					response.key |= 0x02;
				}
				if (m_taskMgr.m_isHaveNew)
				{
					response.key |= 0x04;
				}		
				
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_Get_active_inofo_RESBONSE, (char *)&response, sizeof(MsgGetActiveInfoResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		case MSG_Get_POWER_REQUEST:
			{
				MsgGetPowerRequest * body = (MsgGetPowerRequest *)msgInfo.body;
				m_playerMgr.m_teamProperty.build_property(body->power);
				m_playerMgr.highest_team_info_2_db(1);

				MsgCharacterInfoUpdateRequest request;
				request.unCharacterID = m_unID;
				request.unType = 0;	//pve
				request.teamInfo = m_playerMgr.m_teamInfo;
				sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CHARACTER_INFO_UPDATE_REQUEST, (char *)&request, sizeof(MsgCharacterInfoUpdateRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);

				LogItemInfo logInfo;
				SetlogUserInfo(logInfo);
				//log日志文件
				logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_POWER;
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_POWER;
				logInfo.m_op_after = m_playerMgr.m_teamInfo.unPoint;
				logInfo.SetOp_Way(_LOG_OP_WAY_POWER);
				std::vector<LogItemInfo> logver;
				logver.push_back(logInfo);
				SendLogInfoItemMSG(logver);//log 日志
				//-------------------------------------------

				MsgGetPowerResponse response;
				response.unSN = body->unSN;
				ZeroVar(response);
				response.power = m_playerMgr.m_teamInfo.unPoint;
				this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_Get_POWER_RESBONSE, (char *)&response, sizeof(MsgGetPowerResponse));
				send_response_end(m_unUserID, body->unSN);
			}break;
		}
	}

	return 0;
}


int Map::get_luck(ACE_UINT32 unCount, ACE_UINT32 unBehaviorID, RewardInfo &reward)
{	
	//------------------------test
// 	MsgDBAdminRechargeRequest rechargeRequest;
// 	std::string strBuffer = "20150604160049066000000111";
// 	int iSize = strBuffer.size() > RECHARGE_ORDERID_LEN ? RECHARGE_ORDERID_LEN : strBuffer.size();
// 	memcpy(rechargeRequest.orderID, strBuffer.c_str(), iSize);
// 
// 	rechargeRequest.unCharacterID = m_unID;
// 	rechargeRequest.unRenminbi = 123;
// 	rechargeRequest.unProductId = 33;
// 	rechargeRequest.unProductCount = 22;
// 	rechargeRequest.unPayStats = 1;
// 	//rechargeRequest.unChannel = 456;
// 
// 	sendMsg(SERVICE_MESSAGE_TYPE_DB_REQUEST, MSG_DB_ADMINCOMMAND_RECHARGE_INFO_REQUEST, (char *)&rechargeRequest, 
// 		sizeof(MsgDBAdminRechargeRequest), OBJ_ID_NONE, ADAPTER_DB_USER_MGR, m_unID);

// 	MsgDBOrderRequest db_request;
// 	db_request.unSN = 11;
// 	db_request.product_count = 22;
// 	db_request.product_id = 33;
// 	this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_ORDER_REQUEST, (char *)&db_request, sizeof(db_request));
	// 	MsgGMRequest request;
	// 	request.unTypeID = ADMIN_OP_Announcement;
	// 	request.unToID = m_unID;
	// 	this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DB_GM_REQUEST, (char *)&request, sizeof(MsgGMRequest), this->m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
	// 	return 0;
	//---------------------------end

	BehaviorRes *res = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unBehaviorID);

	GameUtils::reset_reward(reward);
	if(res == NULL)
	{
		return ERROR_MSG_RES;
	}

	int flag = 0;
	if(res->has_cooldown())
	{
		flag = m_cooldownMgr.can_do(m_unCurrentTime, unBehaviorID);
	}
	else
	{
		flag = -1;
	}

	if(unCount == 1)
	{
		if(flag == 0 || this->is_cost_enough(res->m_cost, 1))
		{
			if((unBehaviorID == 0x16000007 && (m_unUseLuckCount & 0x0000ffff) == 0) ||
				(unBehaviorID == 0x16000008 && (m_unUseLuckCount & 0xffff0000) == 0))//首次抽奖
			{
				BehaviorRes *res1 = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unBehaviorID + 3);
				if(NULL != res1)
					this->get_reward_from_config_reward(reward, res1->m_reward, 1, 1,true);
			}
			else
				this->get_reward_from_config_reward(reward, res->m_reward, 1, 1,true);

			m_LAMgr.DoLATypeLuck(LA_MISSION_TYPE_1004,res->m_cost);//单抽，一次
			if(flag == -1)//要消费抽奖
			{
				//-----------log日志
				//操作
				LogItemInfo logInfo;
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_get_luck;//动作原因
				logInfo.SetOp_Way(_LOG_OP_WAY_get_luck);//途径(附加信息)
				this->do_cost(res->m_cost, 1,logInfo);
			}
			else
			{
				m_cooldownMgr.do_once(m_unCurrentTime, unBehaviorID);
			}

			if(unBehaviorID == 0x16000007)
				m_unUseLuckCount += 0x1;
			else
				m_unUseLuckCount += 0x10000;
		}
		else
			return ERROR_MSG_COST;

	}
	else
	{
		/*
		if(flag != -1)
		{
		return -1;
		}
		*/

		int discount = 10;
		switch(unBehaviorID)
		{
			case 0x16000007://金卡
				{
					discount = 9;
				}break;
			case 0x16000008://金卡
				{
					discount = 10;
				}break;
		}

		if(!is_cost_enough(res->m_cost, discount))
		{
			return ERROR_MSG_COST;
		}

		int i;
		for(i = 0;i < 10;i ++)
		{
			if(i == 0 && unBehaviorID == 0x16000007)
			{
				BehaviorRes *res1 = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(0x16000009);
				if(NULL != res1)
					this->add_reward_from_config_reward(reward, res1->m_reward, 1, 2,true);
			}
			else
			{
				this->add_reward_from_config_reward(reward, res->m_reward, 1, 2,true); //十连抽，临时方案，不行再改回去
			}
		}

		m_LAMgr.DoLATypeLuck(LA_MISSION_TYPE_1003,res->m_cost);//十连抽，一次
		//-----------log日志
		//抽奖
		LogItemInfo logInfo;
		logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_get_luck;//动作原因
		logInfo.SetOp_Way(_LOG_OP_WAY_get_luck);//途径(附加信息)
		do_cost(res->m_cost, discount,logInfo);

		if(unBehaviorID == 0x16000007)
			m_unUseLuckCount += 0xA;
		else
			m_unUseLuckCount += 0xA0000;
	}

	//-----------log日志
	//抽奖
	LogItemInfo logInfo;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_get_luck;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_get_luck);//途径(附加信息)
	this->get_reward(reward,logInfo);

	return 0;


	/*
	ConfigReward *configReward = NULL;

	ACE_UINT32 unRet = 0;
	DropRes *res;
	if(m_unUseLuckCount < 2)
	{
	configReward = &GAME_CONFIG_INSTANCE::instance()->m_luckFixRewardList[m_unUseLuckCount];
	}
	else
	{
	configReward = LUCK_MGR_INSTANCE::instance()->get_config_reward(m_unLevel);
	//unRet = LUCK_MGR_INSTANCE::instance()->get_luck(m_unLevel, this, nCount);
	}

	RewardInfo reward;

	this->get_reward_from_config_reward(reward, *configReward, 1, nCount);

	this->get_reward(reward);
	if(reward.unCount > 0)
	{
	unRet = reward.list[0].item.unResID;
	}

	m_unUseLuckCount ++;
	*/
}

void Map::update_use_luck_count()
{
	MsgDBUpdateUseLuckCountRequest db_request;
	db_request.unUseLuckCount = m_unUseLuckCount;

	this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_UPDATE_USE_LUCK_COUNT, (char *)&db_request, sizeof(MsgDBUpdateUseLuckCountRequest));
}

int Map::deal_db_msg(ACE_UINT32 unType, void *msg)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ParentAdapter->m_mutex);

	bool bDestroy = false;
	bool bDone = false;
	switch(unType)
	{
	case MSG_DB_CHARACTER_BASIC:
		{
			MsgDBCharacterBasicResponse *info = (MsgDBCharacterBasicResponse *)msg;

			m_unUserID = info->unUserID;
			ACE_OS::memcpy(m_strName, info->name, MAX_CHARACTER_LENGTH + 1);
			m_unCoin = info->unCoin;
			m_unMoney = info->unMoney;			
			m_unPowerPoint = info->unPowerPoint;
			m_unHonour = info->unHonour;
			m_unLevel = info->unLevel;
			m_unExp = info->unExp;
			m_unLastLoginTime = info->unLastLoginTime;
			m_unLoginBonusCount = info->unLoginBonusCount;
			m_unLoginCount = info->unLoginCount;
			m_packageMgr.m_unPackageMax = info->unPackageMax;
			m_unTutorialFlag = info->unTutorialFlag;
			m_dwFlag = info->unFlag;
			m_goldenTouchMgr.init(info->unGoldenTouchUsedTimes);
			m_unChannel = info->m_unChannel;//渠道
			m_unCurrentBuyEnergy = info->m_unCurrentBuyEnergy;//买体力
			set_secret_paid_refresh_price();
			m_dwBuyMonthCardTime = info->unBuyMonthCardTime;

			m_unChangedDateTime = info->unChangedDate;
			m_unFriendshipPoint = info->unFriendship;
			m_unDynamicEnergy = info->unDynamicEnergy;
			m_playerMgr.m_teamInfo.unPoint =info->m_power;

			m_oMailListBuffer.clear();
			m_dwGetMailListIndex = 0;
			m_unUseLuckCount = info->unUseLuckCount;
			m_vipMgr.init(info->unRechargeCount, info->unRechargeAmount,info->m_vipInfo,info->m_vipGift);
			m_energyMgr.init(info->unEnergy, info->unEnergyRestoreTime);
			//m_pvpEnergyMgr.init(info->unPVPEnergy, info->unPVPEnergyRestoreTime);
			m_bitField = info->m_bitField;
			m_Productlist = info->m_Productlist;
			m_monthTime = info->m_monthTime;
			m_lastSendMonthTime= m_unLastLoginTime;

			//--------------------登录
			LogItemInfo _info;
			SetlogUserInfo(_info);
			_info.m_op_action = enum_LOG_ACTION::LOG_ACTION_Login;
			_info.SetOp_Way(_LOG_OP_WAY_login);
			_info.m_unType = LOG_TYPE_DEFALT;
			_info.m_op_time = GameUtils::get_utc();
			std::vector<LogItemInfo> ver;
			ver.push_back(_info);
			SendLogInfoItemMSG(ver);
			//------------------------------end
		}
		break;
	case MSG_DB_ITEMS_LIST:
		{
			MsgDBItemsListResponse *info = (MsgDBItemsListResponse *)msg;

			m_packageMgr.Init_list(info->unCount, info->list);
		}
		break;
	case MSG_DB_PLAYER_LIST:
		{
			MsgDBPlayerListResponse *info = (MsgDBPlayerListResponse *)msg;

			m_playerMgr.init(info->playerList, info->selectedList, &info->staff, info->playerSkillList, info->unHighestPoint);
		}
		break;
		/*
		case MSG_DB_EQUIPMENT_LIST:
		{
		MsgDBEquipmentListResponse *info = (MsgDBEquipmentListResponse *)msg;

		m_equipmentMgr.init(info->equipmentList);
		}
		break;
		*/
	case MSG_DB_MAP_LIST:
		{
			MsgDBMapListResponse *info = (MsgDBMapListResponse *)msg;
			m_mapInfoMgr.init(info->instanceInfo, info->reward, info->flagList, info->unLastRewardTime, info->unFreeRebornCount, info->unLastRebornTime, info->giftList);
		}
		break;
	case MSG_DB_SKILL_LIST:
		{
			MsgDBSkillListResponse *info = (MsgDBSkillListResponse *)msg;
			m_skillMgr.init(info->skillList, info->playerSkillList, info->bookRes);
			//			m_skillMgr.init(info->skillList, info->playerSkillList);

		}
		break;
		/*
		case MSG_DB_WEAPON_SKILL_LIST:
		{
		MsgDBWeaponSkillListResponse *info = (MsgDBWeaponSkillListResponse *)msg;
		m_weaponSkillMgr.init(info->list, info->improvePropertyList);
		}
		break;
		*/
	case MSG_DB_FUNCTION_LIST:
		{
			MsgDBFunctionListResponse *info = (MsgDBFunctionListResponse *)msg;
			m_functionMgr.init(info->list);
		}
		break;
	case MSG_DB_GIFT_LIST:
		{
			MsgDBGiftListResponse *info = (MsgDBGiftListResponse *)msg;
			m_giftMgr.init(info->list);
		}
		break;
	case MSG_DB_PLAYER_GIFT_BAG_CODE:
		{
			MsgDBPlayerGiftBagCodeResponse *info = (MsgDBPlayerGiftBagCodeResponse *)msg;
			m_giftBagCodeMgr.init(info->m_giftBagCodeSet, info->m_dwGiftBagCodeNum);
		}break;
	case MSG_DB_DOUBLE_DROP_INFO:
		{
			MsgDBDoubleDropInfoResponse *info = (MsgDBDoubleDropInfoResponse *)msg;
			m_mapInfoMgr.initDoubleDrop(*info);
		}break;
	case MSG_DB_TASK_LIST:
		{
			MsgDBTaskList *info = (MsgDBTaskList *)msg;
			//m_taskMgr.init(info);
			m_dailyTaskMgr.Init(info);
		}break;
	case MSG_DB_OBTAINED_ACTIVENESS_REWARD_LIST:
		{
			MsgDBObtainedActivenessRewardList *info = (MsgDBObtainedActivenessRewardList *)msg;
			m_dailyTaskMgr.InitObtainedReward(info);
		}break;
	case MSG_DB_UPDATE_TASK_PROGRESS_RESPONSE:
		{
			MsgDBTaskList *info = (MsgDBTaskList *)msg;
			m_taskMgr.init(info);
		}break;
	case MSG_DB_UPDATE_LA_PROGRESS_RESPONSE:
		{
			MsgDBLimitedActivityList *info = (MsgDBLimitedActivityList *)msg;
			m_LAMgr.init(info);
			//连续登录
			m_LAMgr.DoLATypeLogin();
			//-----------------------end;
		}
		break;
	case MSG_DB_UPDATE_BADGE_PROGRESS_RESPONSE:
		{
			MsgDBBadgeList *body = (MsgDBBadgeList *)msg;
			m_badgeMgr.init(body);
		}break;
		/*case MSG_DB_GLOBLE_GIFT_BAG_CODE:
		{
		MsgDBGlobleGiftBagCodeResponse *info = (MsgDBGlobleGiftBagCodeResponse *)msg;
		std::map<std::string, GiftBagCodeInfo> &oGiftBagCodeInfo = GAME_CONFIG_INSTANCE::instance()->m_oGiftBagCodeReward;
		std::string strCode = "";
		GiftBagCodeInfo oCodeInfo;
		for (int i=0;i<info->m_unCount&&i<MAX_GLOBLE_GIFT_BAG_CODE_COUNT;++i)
		{
		oCodeInfo.byUsed = info->m_oCodeItemList[i].m_byUsed;
		oCodeInfo.unCodeType = info->m_oCodeItemList[i].m_unCodeType;
		oCodeInfo.unGiftBagType = info->m_oCodeItemList[i].m_unGiftBagType;
		strCode.assign(info->m_oCodeItemList[i].m_strGiftBagCode);
		oGiftBagCodeInfo.insert(make_pair(strCode, oCodeInfo));
		}
		}break;*/
	case MSG_DB_COUNT_LIMIT_LIST:
		{
			MsgDBCooldownListResponse *info = (MsgDBCooldownListResponse *)msg;

			m_cooldownMgr.init(info->list);
		}
		break;
	case MSG_DB_HANDBOOK_LIST:
		{
			MsgDBHandbookListResponse *info = (MsgDBHandbookListResponse *)msg;

			m_handbookMgr.init(info->unCount, info->list);
		}
		break;
	case MSG_DB_MATERIAL_LIST:
		{
			MsgDBMaterialListResponse *info = (MsgDBMaterialListResponse *)msg;

			m_materialMgr.init(info->unCount, info->list);

			moveToMateria(); //added by jinpan 将materialMgr里面没有的图鉴从handbookmgr里面copy过来
		}
		break;
	case MSG_DB_PROPS_LIST:
		{
			MsgDBPropsListResponse *info = (MsgDBPropsListResponse *)msg;

			m_propsMgr.init(info->unCount, info->list);
		}
		break;
	case MSG_DB_PVP_INFO:
		{
			MsgDBPVPInfoResponse *info = (MsgDBPVPInfoResponse *)msg;

			m_pvpMgr.init(info->info);
			//m_materialMgr.init(info->unCount, info->list);
		}
		break;
	case MSG_UPDATE_NAME:
		{
			MsgDBUpdateNameResponse *info = (MsgDBUpdateNameResponse *)msg;
			MsgChangeNameResponse response;
			response.unSN = info->unSN;
			response.nResult = info->nResult;

			int t = 0;//(int)GAME_CONFIG_INSTANCE::instance()->m_unRenameCost;

			if(info->nResult == 0)
			{
				this->set_basic_info(0, -t, 0, 0, 0,0,0);

				ACE_OS::memcpy(this->m_strName, info->name, MAX_CHARACTER_LENGTH);
				this->m_strName[MAX_CHARACTER_LENGTH] = '\0';

				MsgCharacterNameUpdateRequest update_request;
				update_request.unCharacterID = m_unID;
				ACE_OS::memcpy(update_request.name, m_strName, MAX_CHARACTER_LENGTH);
				update_request.name[MAX_CHARACTER_LENGTH] = '\0';
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CHARACTER_NAME_UPDATE_REQUEST, (char *)&update_request, sizeof(MsgCharacterNameUpdateRequest), 
					this->m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}

			ACE_UINT32 unOldSig = m_unCurrentSig;
			m_unCurrentSig = info->unSig;
			std::memcpy(response.name,m_strName,MAX_CHARACTER_LENGTH + 1);
			this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_CHANGE_NAME_RESPONSE, (char *)&response, sizeof(MsgChangeNameResponse));
			send_response_end(m_unUserID, info->unSN);

			m_unCurrentSig = unOldSig;
		}
		break;
	case MSG_DB_FRIEND_LIST:
		{
			MsgDBFriendListResponse *info = (MsgDBFriendListResponse *)msg;

			m_friendMgr.init(info->unMaxFriendCount, info->inviteList, info->friendList,info->m_bs, info->assistList,info->deleteFriendList);

			cal_level_up_exp();

			dispatch_map_event(GAME_EVENT_PVE_TEAM_UPDATE);
			//dispatch_map_event(GAME_EVENT_LOOT_IGNORE_UPDATE);
			//dispatch_map_event(GAME_EVENT_BATTLE_DATA_UPDATE);

			m_bLoadingDone = true;
		}
		break;
	case MSG_DB_MONEY_TREE_INFO:
		{
			MsgDBMoneyTreeInfoResponse *info = (MsgDBMoneyTreeInfoResponse *)msg;

			m_moneyTreeMgr.init(info->unLevel, info->unExp);
		}
		break;

	case MSG_DB_SECRET_LIST:  //added by jinpan for secret
		{
			MsgDBSecretListResponse *info = (MsgDBSecretListResponse *)msg;

			m_secretMgr.init(info->unCellCount,info->unRefreshTimes,info->unLastRefreshTime, info->list);
		}
		break;
	case MSG_DB_WING_LIST:
		{
			MsgDBWingListResponse *info = (MsgDBWingListResponse *)msg;
			m_wingMgr.init(info->unCount, info->list);
		}
		break;
	}

	if(bDestroy)
	{
		//this->setObjStatus(SERVICE_OBJ_DESTROY);//change by xxj 2015/05/23  18: 04:26 
		m_bValidHome = false;
	}

	return 0;
}

//数据改变
bool Map::CheckDateChanged()
{
	ACE_UINT32 unCurrentUTC = GameUtils::get_utc();
	int dwDayOffset = GameUtils::get_day_different(unCurrentUTC, m_unChangedDateTime);
	if ( dwDayOffset > 0)
	{
		m_mapInfoMgr.Reset_map_day_different_count();
		m_unChangedDateTime = unCurrentUTC;
		MsgDBUpdateChangedDate db_request;
		db_request.m_unUpdateChangedDate = m_unChangedDateTime;
		add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_CHANGED_DATE, (char *)&db_request, sizeof(MsgDBUpdateChangedDate));

		m_friendMgr.AssistFriendReset();
		m_dailyTaskMgr.Reset();
		m_goldenTouchMgr.Reset();
		/* &&m_dwBuyMonthCardTime + 30 * 24 * 3600 > GameUtils::get_utc()*/
		ACE_Date_Time tvTime(ACE_OS::gettimeofday());
		if(dwDayOffset >= tvTime.day())
		{
			set_sign_up_day_this_month(0);
		}
		set_sign_up_reward_sign(false);
		set_vip_sign_up_reward_sign(false);
		set_eat_everyday_count(0);

		//重置
		m_unCurrentBuyEnergy = 0;
		SendBuyEnergyToDB();
		send_month_card();//关闭月卡功能
		m_friendMgr.DoDayChange();
		m_pvpMgr.DoDayChange();
		return true;
	}
	else
	{
		return false;
	}
}

int Map::send_msg_2_client(ACE_UINT32 unMsgType, ACE_UINT32 unBodyType, char *strBody, ACE_UINT32 unBodyLen)
{
	return this->sendMsg(unMsgType, unBodyType,strBody, unBodyLen, OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
}

void Map::dispatch_map_event(char *strEventName, ACE_UINT32 unResID, ACE_UINT32 unCount)
{
	MapEvent e(strEventName, unResID, unCount);

	this->dispatcher_event(&e);
}

void Map::send_response_end(ACE_UINT32 unTargetID, ACE_UINT32 unSN)
{
	MsgResponseEnd body;

	body.nResult = 0;
	body.unSN = unSN;

	this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_RESPONSE_END,(char *)&body, sizeof(MsgResponseEnd), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, unTargetID);
}

void Map::send_basic_info(ObjAdr &targetAdr, ACE_UINT32 unSN)
{
	MsgCharacterInfoResponse response;

	//ACE_UINT32 t = GameUtils::get_utc();
	response.nResult = 0;
	response.unSN = unSN;

	response.unCharacterID = m_unID;
	ACE_OS::memcpy(response.name, m_strName, sizeof(char) * (MAX_CHARACTER_LENGTH + 1));
	response.unLevel = m_unLevel;
	response.unExp = m_unExp;
	response.unCoin = m_unCoin;
	response.unMoney = m_unMoney;
	response.unPowerPoint = m_unPowerPoint;
	response.unHonour = m_unHonour;
	response.unFriendship = m_unFriendshipPoint;
	response.unDynamicEnergy = m_unDynamicEnergy;//动感能量
	response.unLastLoginTime = m_unLastLoginTime;
	response.unLoginCount = m_unLoginCount;
	response.unLoginBonusCount = m_unLoginBonusCount;
	response.unCreateTime = 0;
	response.unEnergy = m_energyMgr.m_unEnergy;
	response.unEnergyRestoreTime = m_unCurrentTime - m_energyMgr.m_unRestoreTime;
	//response.unPVPEnergy = m_pvpEnergyMgr.m_unEnergy;
	//response.unPVPEnergyRestoreTime = m_unCurrentTime - m_pvpEnergyMgr.m_unRestoreTime;
	response.unRechargeCount = m_vipMgr.m_unRechargeCount;
	response.unRechargeAmount = m_vipMgr.m_unRechargeAmount;
	response.unPackageMax = m_packageMgr.m_unPackageMax;
	response.unTutorialFlag = m_unTutorialFlag;
	response.unFlag = m_dwFlag;
	response.unGoldenTouchUsedCount = m_goldenTouchMgr.m_unUsedTouchCount;
	response.unCurrentBuyCount = m_unCurrentBuyEnergy;
	response.m_vipInfo = m_vipMgr.m_vipInfo;
	response.m_vipGift = m_vipMgr.m_vipGift;//已获得的vip礼包列表
	response.m_bitField = m_bitField.GetValue();
	response.m_Productlist = m_Productlist;
	m_playerMgr.m_teamProperty.build_property();
	response.m_power = m_playerMgr.m_teamInfo.unPoint;//战斗力

	this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CHARACTER_INFO_RESPONSE, (char *)&response, sizeof(MsgCharacterInfoResponse), targetAdr);


	//--------------------登录
	LogItemInfo _info;
	SetlogUserInfo(_info);
	_info.m_op_action = enum_LOG_ACTION::LOG_ACTION_TEST;
	_info.SetOp_Way(_LOG_OP_WAY_TEST);
	_info.m_unType = LOG_TYPE_DEFALT;
	_info.m_op_time = GameUtils::get_utc();
	std::vector<LogItemInfo> ver;
	ver.push_back(_info);
	SendLogInfoItemMSG(ver);
	//------------------------------end
}

void Map::get_reward_from_config_reward(RewardInfo &dest,const ConfigReward &configReward, int item_times, int nCount,bool isLuck)
{
	GameUtils::reset_reward(dest);

	add_reward_from_config_reward(dest, configReward, item_times, nCount,isLuck);
}

void Map::add_reward_from_config_reward(RewardInfo &dest,const ConfigReward &configReward, int item_times, int nCount,bool isLuck)
{
	dest.unCoin += configReward.unCoin;
	dest.unMoney += configReward.unMoney;
	dest.unPowerPoint += configReward.unPowerPoint;
	dest.unHonour += configReward.unHonour;
	dest.unRecharge += configReward.unRecharge;
	dest.unExp += configReward.unExp;
	dest.unEnergy += configReward.unEnergy;
	dest.unPVPEnergy += configReward.unPVPEnergy;
	dest.unFriendship += configReward.unFriendship;
	dest.unDynamicEnergy += configReward.unDynamicEnergy;

	int c = dest.unCount;

	ACE_UINT32 tmp_id;
	ACE_UINT32 tmp_type;

	int i, j;
	ACE_UINT32 tmp_drop_id = 0;

	for(i = 0;i < configReward.unCount && c<MAX_REWARD_ITEM;i ++)
	{
		tmp_id = configReward.list[i].unResID;

		tmp_type = GET_ITEM_TYPE(tmp_id);

		if(tmp_type == ITEM_RES_TYPE_DROP)
		{
			for(j = 0;j < configReward.list[i].unCount;j ++)
			{
				DropRes *dropRes = (DropRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_id);
				if(dropRes == NULL)
				{
					tmp_drop_id = 0;
				}
				else
				{
					tmp_drop_id = m_lootMgr.loot(dropRes->m_dropGroup, nCount);
					if (isLuck)//gm 工具
					{
						ChickLuckStart(tmp_drop_id);
					}
				}

				if(tmp_drop_id == 0)
				{
					continue;
				}

				dest.list[c].unCount = 1;
				dest.list[c].item.unResID = tmp_drop_id;
				dest.list[c].item.info.equipment.unLevel = 1;
				dest.list[c].item.info.equipment.unStep = 1;

				//tmp_drop_id = 0x26000001;
				//=============================================================================
				//xxj 2015/04/17  15:24:14 
				int _type = GET_ITEM_TYPE(tmp_drop_id);
				switch(_type)
				{
				case ITEM_RES_TYPE_DROP_CURRENCY://0x28 掉落货币
					{
						CDropCurrencyRes *res = (CDropCurrencyRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);//获取掉落配置资源
						if(res)//+所有货币
						{
							dest.unCoin += res->m_reward.unCoin;
							dest.unMoney += res->m_reward.unMoney;
							dest.unPowerPoint += res->m_reward.unPowerPoint;
							dest.unHonour += res->m_reward.unHonour;
							dest.unRecharge += res->m_reward.unRecharge;
							dest.unExp += res->m_reward.unExp;
							dest.unEnergy += res->m_reward.unEnergy;
							dest.unPVPEnergy += res->m_reward.unPVPEnergy;
							dest.unFriendship += res->m_reward.unFriendship;
						}
						else
						{
							//记录错误日志信息
							CLogBase *pLog = BaseService::GetLogInstance();
							sprintf(pLog->m_szLogBuffer, "[%s][%d]:DropCurrency RES failed",__FILE__, __LINE__);
							pLog->RecordLog(Log_Type_Error, pLog->m_szLogBuffer);
						}
					}
					break;
				case ITEM_RES_TYPE_DROP_MATERIAL://材料（装备碎片）
					{
						CDropMaterialRes *res = (CDropMaterialRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);//获取掉落配置资源
						if(res)//+所有材料
						{
							ACE_ASSERT((int)(res->m_unCount) > 0);
							dest.list[c].unCount = res->m_list[0].unCount;//只取第一个
							dest.list[c].item.unResID = res->m_list[0].unResID;//id
							dest.list[c].item.info.equipment.unLevel = 1;
							dest.list[c].item.info.equipment.unStep = 1;
						}
						else
						{
							//记录错误日志信息
							CLogBase *pLog = BaseService::GetLogInstance();
							sprintf(pLog->m_szLogBuffer, "[%s][%d]:DropCurrency RES failed",__FILE__, __LINE__);
							pLog->RecordLog(Log_Type_Error, pLog->m_szLogBuffer);
						}
					}
					break;
				default:
					break;
				}
				//------------------------------------------------------------------------
				c++;
			}
		}
		else
		{
			dest.list[c].unCount = configReward.list[i].unCount;
			dest.list[c].item.unResID = tmp_id;
			dest.list[c].item.info.equipment.unLevel = 1;
			dest.list[c].item.info.equipment.unStep = 1;
			c ++;
		}
	}

	dest.unCount = c;

	for(int i = 0; i != dest.unCount; i++)
	{
		dest.list[i].unCount *= item_times;
	}
}

void Map::get_reward(const RewardInfo &reward,LogItemInfo & logInfo)
{
	//-----------
	//添加日志类型
	std::vector<LogItemInfo> logInfoVer;
	SetlogUserInfo(logInfo);//写入基本信息
	SetRewardLogItem(reward,logInfo,logInfoVer);
	//----------------------------

	//--------------------------------end

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]get reward", m_unID);
	/*if (reward.unFriendship != 0)
	{
	m_unFriendshipPoint += reward.unFriendship;
	update_friendship_2_db();
	}*/
	set_basic_info(reward.unCoin, reward.unMoney, reward.unExp, reward.unPowerPoint, reward.unHonour, reward.unFriendship,reward.unDynamicEnergy);

	//guo m_vipMgr.recharge(reward.unRecharge, false);

	this->m_energyMgr.increase(reward.unEnergy);
	//this->m_pvpEnergyMgr.increase(reward.unPVPEnergy);


	int i;

	//增加物品
	logInfo.m_op_before = 0;
	logInfo.m_op_after = 0;
	logInfo.m_unType = LOG_TYPE_FAIL;
	LogItemInfo logInfoTemp = logInfo;
	if (reward.unCount>0)
		add_item(reward.list[0].item.unResID, reward.list[0].unCount,logInfo);


	for(i = 1;i < reward.unCount;i ++)
	{
		add_item(reward.list[i].item.unResID, reward.list[i].unCount,logInfoTemp);

		//log日志
		if (logInfo.m_goodsID != logInfoTemp.m_goodsID)
		{
			logInfoVer.push_back(logInfo);//先加入老的
			logInfo = logInfoTemp;//更新新的
			logInfo.m_unType = LOG_TYPE_FAIL;
		}
		else
		{
			logInfo.m_op_after += logInfoTemp.m_op_after;
			logInfo.m_unType = logInfoTemp.m_unType;
		}
		//--------------------------------------
	}

	if (logInfo.m_unType != LOG_TYPE_FAIL)
	{
		logInfoVer.push_back(logInfo);
	}

	SendLogInfoItemMSG(logInfoVer);//开始日志请求
}

void Map::set_basic_info(int nCoin, int nMoney, int nExp, int nPowerPoint, int nHonour, int nFriendship,int dynamicEnergy)
{

	//限时活动消费记录
	m_LAMgr.Cost_basic_info(nCoin, nMoney, nExp, nPowerPoint, nHonour, nFriendship);
	//--------------------

	int t = (int)m_unCoin;
	t = t + nCoin;
	if(t <= 0)
	{
		t = m_unCoin;
	}
	m_unCoin = (ACE_UINT32)t;

	t = (int)m_unMoney;
	t = t + nMoney;
	if(t < 0)
	{
		t = m_unMoney;
	}
	m_unMoney = (ACE_UINT32)t;

	t = (int)m_unPowerPoint;
	t = t + nPowerPoint;
	if(t < 0)
	{
		t = m_unPowerPoint;
	}
	m_unPowerPoint = (ACE_UINT32)t;

	t = (int)m_unHonour;
	t = t + nHonour;
	if(t < 0)
	{
		t = m_unHonour;
	}
	m_unHonour = (ACE_UINT32)t;

	t = (int)m_unFriendshipPoint;
	t = t + nFriendship;
	if(t < 0)
	{
		t = m_unFriendshipPoint;
	}
	m_unFriendshipPoint = (ACE_UINT32)t;

	t = (int)m_unDynamicEnergy;
	t = t + dynamicEnergy;
	if(t < 0)
	{
		t = m_unDynamicEnergy;
	}
	m_unDynamicEnergy = (ACE_UINT32)t;

	if(nExp > 0)
	{
		if (m_unLevel >=  GAME_CONFIG_INSTANCE::instance()->m_maxLevel)//当登记到最大之后就不加经验值了
			nExp = 0;
	}

	if(nExp > 0 && m_unLevel < m_unMaxLevel)
	{
		m_unExp += nExp;

		ACE_UINT32 old_level = m_unLevel;

		while(m_unExp >= m_unExpLevelUp)
		{
			m_unLevel ++;
			if(m_unLevel >= m_unMaxLevel){
				m_unExp = m_unLevel;
			}else{
				m_unExp -= m_unExpLevelUp;
			}

			cal_level_up_exp();

			dispatch_map_event(GAME_EVENT_LEVEL_UP);
		}

		if(old_level != m_unLevel)
		{
			//-----------log日志
			//邮件
			LogItemInfo logInfo;
			std::vector<LogItemInfo> logInfoVer;
			SetlogUserInfo(logInfo);//写入基本信息
			logInfo.m_goodsID = m_unUserID;//id
			logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_user_level_up;//动作原因
			logInfo.SetOp_Way(_LOG_OP_WAY_user_level_up);//途径(附加信息)
			logInfo.m_op_before = old_level;
			logInfo.m_op_after = m_unLevel;
			logInfoVer.push_back(logInfo);

			logInfo.m_goodsID = m_unUserID;//id
			logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_user_exp_up;//动作原因
			logInfo.SetOp_Way(_LOG_OP_WAY_user_exp_up);//途径(附加信息)
			logInfo.m_op_before = m_unExp - nExp;
			logInfo.m_op_after = m_unExp;
			logInfoVer.push_back(logInfo);

			SendLogInfoItemMSG(logInfoVer);//开始日志请求
			//-----------------------------------------
			MsgCharacterLevelUpdateRequest request;
			request.unCharacterID = m_unID;
			request.unLevel = m_unLevel;

			this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_CHARACTER_LEVEL_UPDATE_REQUEST, (char *)&request, sizeof(MsgCharacterLevelUpdateRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);

			//this->m_energyMgr.set_full();
			//			int safei =0;
			FormulaInfo formula = GAME_CONFIG_INSTANCE::instance()->m_vecFormulaInfo[2];//xxj 2015/04/22  17:11:35 公式
			int n = 0;
			for (int live = old_level+1;live<=m_unLevel;live++)
			{
				//m_energyMgr.increase(2 + (int)pow(live, 0.7));
				m_energyMgr.increase((int)formula.paramC + (int)(formula.paramA*pow(live, formula.paramB)));
				//safei++;
				// 				if (safei >= 10)
				// 					break;
			}
			//dispatch_map_event(GAME_EVENT_PROPERTY_UPDATE);
			//m_taskMgr.RefreshTask(Event_Base_Type_LevelUp, 0);
			//m_dailyTaskMgr.RefreshTask(Event_Base_Type_LevelUp, 0);

			//m_taskMgr.RefreshTask(Event_Base_Type_ReachSpecificLevel, m_unLevel);
			//m_dailyTaskMgr.RefreshTask(Event_Base_Type_ReachSpecificLevel, m_unLevel);
		}
	}

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]set basic info[coin=%d,money=%d,exp=%d,powerpoint=%d,honour=%d,level=%d][%d,%d,%d,%d,%d]\n", 
		m_unID, m_unCoin, m_unMoney, m_unExp, m_unPowerPoint, m_unHonour, m_unLevel, nCoin, nMoney, nExp, nPowerPoint,nHonour);

	MsgDBUpdateBasicInfoRequest db_request;
	db_request.unCoin = m_unCoin;
	db_request.unMoney = m_unMoney;
	db_request.unPowerPoint = m_unPowerPoint;
	db_request.unExp = m_unExp;
	db_request.unLevel = m_unLevel;
	db_request.unHonour = m_unHonour;
	db_request.unFriendship = m_unFriendshipPoint;
	db_request.unDynamicEnergy = m_unDynamicEnergy;

	this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_BASIC_INFO, (char *)&db_request, sizeof(MsgDBUpdateBasicInfoRequest));

}

void Map::cal_level_up_exp()
{
	//m_unExpLevelUp = 3 * m_unLevel * 3 * m_unLevel + 30;

	//m_unExpLevelUp = m_unLevel * m_unLevel * m_unLevel + 40;

	//int a = 10;
	//int b = 600;
	//int idx;
	//static int exp_param[][2] = 
	//{
	//	{10, 600},
	//	{12, 720},
	//	{14, 840},
	//	{17, 1020},
	//	{20, 1220},
	//	{24, 1440},
	//	{40, 2400},
	//};

	//if(m_unLevel <= 9)
	//{
	//	idx = 0;
	//}
	//else if(m_unLevel <= 14)
	//{
	//	idx = 1;
	//}
	//else if(m_unLevel <= 19)
	//{
	//	idx = 2;
	//}
	//else if(m_unLevel <= 24)
	//{
	//	idx = 3;
	//} 
	//else if(m_unLevel <= 29)
	//{
	//	idx = 4;
	//}
	//else if(m_unLevel <= 59)
	//{
	//	idx = 5;
	//}
	//else
	//{
	//	idx = 6;
	//}

	//m_unExpLevelUp = ((m_unLevel * 3 * m_unLevel * 3 * exp_param[idx][0] + exp_param[idx][1]) / 10);
	//int idx;
	//static int exp_param[] = 
	//{
	//	48,
	//	54,
	//	63,
	//	74,
	//	86,
	//	99,
	//	113,
	//	128,
	//	145,
	//	162,
	//	180,
	//	199,
	//	219,
	//	240,
	//	288,
	//	342,
	//	401,
	//	465,
	//	535,
	//	610,
	//	692,
	//	779,
	//	873,
	//	973,
	//	1080,
	//	1193,
	//	1313,
	//	1440,
	//	1740,
	//	2067,
	//	2423,
	//	2808,
	//	3225,
	//	3675,
	//	4159,
	//	4679,
	//	5236,
	//	5833,
	//	6470,
	//	7149,
	//	7871,
	//	8640,
	//	9723,
	//	10885,
	//	12128,
	//	13458,
	//	14877,
	//	16389,
	//	18000,
	//	20097,
	//	22348,
	//	24757,
	//	27335,
	//	30089,
	//	33027,
	//	36158,
	//	39491,
	//	43035,
	//	46800,
	//	1000000
	//};

	//idx = m_unLevel - 1;
	//m_unExpLevelUp = exp_param[idx];
	TeamLevel &t = GAME_CONFIG_INSTANCE::instance()->m_teamLevel[m_unLevel];
	m_unExpLevelUp = t.levelup_exp;
	/*

	float t = 0.0f;
	t =  (m_unLevel * 3 * m_unLevel * 3 * exp_param[idx][0] + exp_param[idx][1]) * 0.6f / 10.0f;

	m_unExpLevelUp = (ACE_UINT32)t;

	m_unExpLevelUp = (ACE_UINT32)(m_unExpLevelUp * 10);
	*/
	//主将每级升级需要经验值=floor(((lv*3)^2+60)*0.6,10)
}

void Map::update_all()
{
	//ACE_UINT32 t = GameUtils::get_utc();
	m_energyMgr.update(m_unCurrentTime);
	//m_pvpEnergyMgr.update(m_unCurrentTime);
	//m_pvpMgr.update(m_unCurrentTime);

	ACTIVITY_MGR_INSTANCE::instance()->update(m_unCurrentTime);

	if(m_unLastActivityTimer != ACTIVITY_MGR_INSTANCE::instance()->m_unTimer)
	{
		//update benefit
		dispatch_map_event(GAME_EVENT_RESET_BENIFIT);
	}

	update_login_count();
}


int Map::level_up_item(int idx, ACE_UINT32 unPlayerID, int dwLevelAdd)//client 入口
{
	int ret = -1;
	if(unPlayerID == 0)//如果是升级背包里的装备，idx为实际的，playerID为0//
	{
		if(m_functionMgr.is_locked(FUNCTION_IMPROVE_EQUIPMENT))
		{
		}
		else
		{
			PackageCell *cell = m_packageMgr.get_cell(idx);
			if(cell != NULL)
			{
				//level up item
				ret = this->level_up_item(LEVEL_UP_TYPE_EQUIPMENT, &cell->item.item, dwLevelAdd);
				if(ret == 0)
				{
					m_packageMgr.update_2_db(idx);
				}
			}
		}
	}
	else
	{
		if(idx == 0)//升级将领
		{
			if(m_functionMgr.is_locked(FUNCTION_IMPROVE_PLAYER))
			{
				ret = ERROR_MSG_LOCKED;
			}
			else
			{
				/*
				PlayerInfo *info = m_playerMgr.get_player_by_id(unPlayerID);
				if(info != NULL)
				{
				//level up player
				ret = this->level_up_player(info);
				if(ret == 0)
				{
				m_playerMgr.update_2_db();
				}
				}
				*/
				ret = m_playerMgr.level_up_player(unPlayerID, dwLevelAdd);
			}
		}
		else//身上装备
		{
			if(m_functionMgr.is_locked(FUNCTION_IMPROVE_EQUIPMENT))
			{
				ret = ERROR_MSG_LOCKED;
			}
			else
			{
				ret = m_playerMgr.level_up_equipment(unPlayerID, idx, dwLevelAdd);
			}
		}
	}

	if(ret == 0)
	{
		LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]level up item, idx = %d, player ID = 0x%08x", m_unID, idx, unPlayerID);
	}

	return ret;
}

int Map::level_up_item(ACE_UINT32 unType, ItemInfo *itemInfo, int dwLevelAdd)
{
	ACE_UINT32 tmpID = itemInfo->unResID;
	int t = GET_ITEM_TYPE(tmpID);

	if(t != ITEM_RES_TYPE_EQUIPMENT)
	{
		return ERROR_MSG_RES;
	}
	//-----------log 日志
	//装备升级
	LogItemInfo logInfo;
	logInfo.m_goodsID = tmpID;//装备id
	SetlogUserInfo(logInfo);
	std::vector<LogItemInfo> logver;
	//-------------------------------------------

	if(unType == LEVEL_UP_TYPE_EQUIPMENT)
	{
		if(itemInfo->info.equipment.unLevel + dwLevelAdd > (100 + MAX_EQUIPMENT_LEVEL_PER_STEP * (itemInfo->info.equipment.unStep - 1)))
		{
			return ERROR_MSG_MAX;
		}

		TeamLevel &oTeamLevel = GAME_CONFIG_INSTANCE::instance()->m_teamLevel[m_unLevel];
		ACE_UINT32 unBonusLv = itemInfo->info.equipment.unStep > GAME_CONFIG_INSTANCE::instance()->m_oEquipStepConfig.size() ? 0 : GAME_CONFIG_INSTANCE::instance()->m_oEquipStepConfig[itemInfo->info.equipment.unStep-1].unMaxLevelBonus;
		if(itemInfo->info.equipment.unLevel + dwLevelAdd > oTeamLevel.equip_levelmax + unBonusLv)
		{
			return ERROR_MSG_MAX;
		}

		EquipmentRes *res = (EquipmentRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(itemInfo->unResID);
		if(res == NULL)
			return ERROR_MSG_RES;
		//int price = (cell->item.item.info.equipment.unLevel * 2) * (cell->item.item.info.equipment.unLevel * 2) * 6;
		//int price = lv * lv * lv * 4 + 100;
		//int pt = lv * 10;

		int pt = 0;
		ACE_UINT32 unImproveID = GENERATE_IMPROVE_ID(res->m_unQuality, itemInfo->unResID);

		ImproveRes *improveRes = (ImproveRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unImproveID);
		if(NULL != improveRes)
		{
			FormulaInfo formula = GAME_CONFIG_INSTANCE::instance()->m_vecFormulaInfo[1];//xxj 2015/04/22  17:11:35 公式
			for(int lv = itemInfo->info.equipment.unLevel; lv != itemInfo->info.equipment.unLevel + dwLevelAdd; lv++)
			{
				//pt += (int)((int)(10.0f * pow((float)lv, 1.0f + (float)lv / 112.0f)) * improveRes->m_dImproveRate);
				//pt += ((int)((int)(A * pow((float)lv, 1.0f + (float)lv / B))+ C) * improveRes->m_dImproveRate);
				pt += (int)((int)(formula.paramA * pow((float)lv, 1.0f + (float)lv /(float)formula.paramB  )+ (float)formula.paramC) * improveRes->m_dImproveRate);
				//pt += (int)((int)(formula.paramA * pow((float)lv,(float)lv /formula.paramB + formula.paramC )) * improveRes->m_dImproveRate);
			}
		}
		//int price = lv * lv * lv * 10 + 100;

		//---
		//编号【6】装备强化折扣，折扣率
		ACE_UINT32 count = 100;
		if (m_LAMgr.DoLATypeLeveUP(LA_TYPE_6,count))
		{
			pt = pt*count*0.01;//立即折扣
		}
		//----------------------------------end

		if(/*m_unCoin < price || */this->m_unPowerPoint < pt)
		{
			return ERROR_MSG_COST;
		}

		//log日志文件
		logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_level_up;//动作原因
		logInfo.SetOp_Way(_LOG_OP_WAY_level_up);//途径(附加信息)

		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_PowerPoint_lower;
		logInfo.m_op_before = m_unPowerPoint;
		logInfo.m_op_after = m_unPowerPoint - pt;
		logver.push_back(logInfo);
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_equipment_up;
		logInfo.m_op_before = itemInfo->info.equipment.unLevel;
		logInfo.m_op_after = itemInfo->info.equipment.unLevel + dwLevelAdd;
		logver.push_back(logInfo);
		//-------------
		this->set_basic_info(0/*-price*/, 0, 0, -pt, 0,0,0);
		itemInfo->info.equipment.unLevel += dwLevelAdd;
		// 		//---gm 后台
		// 		GMInfo info;
		// 		ZeroVar(info);
		// 		info.unType = ADMIN_OP_GM_STEP_EQUIPMENT;
		// 		info.unGoodID = tmpID;
		// 		info.unLevel = itemInfo->info.equipment.unLevel;
		// 		AddGmMsg(info);
		// 		//----------------------------------------------

		m_taskMgr.RefreshTask(Event_Base_Type_EquipmentUpgrade, 0);
		m_dailyTaskMgr.RefreshTask(Event_Base_Type_EquipmentUpgrade, 0);

		m_taskMgr.RefreshTask(Event_Base_Type_EquipmentSpecificLevel, itemInfo->info.equipment.unLevel);
		m_dailyTaskMgr.RefreshTask(Event_Base_Type_EquipmentSpecificLevel, itemInfo->info.equipment.unLevel);

		LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]level up equipment,ID=0x%08x,level=%d", m_unID, itemInfo->unResID, itemInfo->info.equipment.unLevel);

		//set weapon skill
		//m_weaponSkillMgr.update_weapon_skill(tmpID, cell->item.item.info.equipment.unLevel);

		BROADCAST_MGR_INSTANCE::instance()->add_message(m_strName, BROADCAST_MESSAGE_TYPE_LEVEL_UP, itemInfo->unResID, itemInfo->info.equipment.unLevel);
	}
	else if(unType == LEVEL_UP_TYPE_EQUIPMENT_STEP)//进阶
	{
		if(itemInfo->info.equipment.unStep >= GAME_CONFIG_INSTANCE::instance()->m_oEquipStepConfig.size())
		{
			return ERROR_MSG_MAX;
		}

		//log日志文件
		//升阶
		logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_step;//动作原因
		logInfo.SetOp_Way(_LOG_OP_WAY_step);//途径(附加信息)

		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_step;
		logInfo.m_op_before = itemInfo->info.equipment.unStep;
		logInfo.m_op_after = itemInfo->info.equipment.unStep +1;
		logver.push_back(logInfo);
		//-------------

		itemInfo->info.equipment.unStep ++;

		//---gm 后台
		GMInfo info;
		ZeroVar(info);
		info.unType = ADMIN_OP_GM_STEP_EQUIPMENT;
		info.unGoodID = tmpID;
		info.unLevel = itemInfo->info.equipment.unStep;
		AddGmMsg(info);
		//----------------------------------------------

		m_taskMgr.RefreshTask(Event_Base_Type_EquipmentSpecificStar, itemInfo->info.equipment.unStep);
		m_dailyTaskMgr.RefreshTask(Event_Base_Type_EquipmentSpecificStar, itemInfo->info.equipment.unStep);

		LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]step up equipment,ID=0x%08x,step=%d", m_unID, itemInfo->unResID, itemInfo->info.equipment.unStep);

		BROADCAST_MGR_INSTANCE::instance()->add_message(m_strName, BROADCAST_MESSAGE_TYPE_STEP_UP, itemInfo->unResID, itemInfo->info.equipment.unStep);
	}
	else
	{
		return -1;
	}

	SendLogInfoItemMSG(logver);//发送日志
	return 0;
}

/*
int Map::level_up_player(PlayerInfo *info)
{

ACE_UINT32 tmpID = info->unItemID;
int t = GET_ITEM_TYPE(tmpID);

if(t != ITEM_RES_TYPE_PLAYER)
{
return -1;
}

if(info->unLevel >= m_unLevel)
{
return -1;
}

//int price = (info->unLevel * 3) * (info->unLevel * 3) + 100;
int lv = info->unLevel;
lv = lv - 1;

int price = lv * lv * lv * 10 + 50;

if(m_unCoin < price)
{
return -1;
}

this->set_basic_info(-price, 0, 0, 0);

info->unLevel ++;

LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]level up player,ID=0x%08x,step=%d", m_unID, info->unItemID, info->unLevel);

return 0;

}
*/

int Map::delete_item(ACE_UINT32 unDeleteType, ACE_UINT32 unCount, ACE_UINT32 *list)
{
	/*
	PackageCell cell;

	if(this->m_packageMgr.delete_item(unIdx, cell) != 0)
	{
	return -1;
	}

	EquipmentRes *res = (EquipmentRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(cell.item.item.unResID);

	if(res == NULL)
	{
	return 0;
	}
	*/

	int i;

	EquipmentRes *res;

	PackageCell *pCell;

	int coin = 0;
	int power_point = 0;

	int lv;

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]delete item start,delete type=%d", m_unID, unDeleteType);

	//-----------log 日志
	//分解
	LogItemInfo logInfo;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_melt;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_melt);//途径(附加信息)
	SetlogUserInfo(logInfo);
	std::vector<LogItemInfo> logver;

	for(i = 0;i < unCount;i ++)
	{
		pCell = this->m_packageMgr.get_cell(list[i]);

		if(pCell == NULL)
		{
			return ERROR_MSG_DELETE_ITEM;
		}

		LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]delete item info,ID=0x%08x,level=%d,step=%d,package idx=%d", 
			m_unID, pCell->item.item.unResID, pCell->item.item.info.equipment.unLevel, pCell->item.item.info.equipment.unStep, pCell->unIdx);

		lv = pCell->item.item.info.equipment.unLevel - 1;

		res = (EquipmentRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(pCell->item.item.unResID);

		if(res == NULL)
		{
			continue;
		}

		if(unDeleteType == DELETE_ITEM_TYPE_SOLD)
		{
			coin += (res->m_unPrice + lv * lv * lv * 2 + 10);
		}
		else if(unDeleteType == DELETE_ITEM_TYPE_MELT)
		{
			ACE_UINT32 unImproveID = GENERATE_IMPROVE_ID(res->m_unQuality, res->m_unResID);

			ImproveRes *improveRes = (ImproveRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unImproveID);

			if(improveRes != NULL)
			{
				//power_point += (improveRes->m_unPowerPoint + lv * lv * 2 + 1);
				FormulaInfo formula = GAME_CONFIG_INSTANCE::instance()->m_vecFormulaInfo[1];//xxj 2015/04/22  17:11:35 公式
				int basePPT = 0;
				for (int i = 1; i < pCell->item.item.info.equipment.unLevel; i++)
				{ 
					//basePPT += (int)(10.0f * pow((float)i, 1.0f + ((float)i) / 112.0f));
					basePPT += (int)(formula.paramA * pow((float)i,1.0f + ((float)i) / (float)formula.paramB) + (float)formula.paramC);
				}
				power_point += basePPT * improveRes->m_dPowerRate + improveRes->m_unPowerPoint;
			}
		}

		//----日志
		logInfo.m_goodsID = pCell->item.item.unResID;
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_equipment_lower;
		logInfo.m_op_before = 0;
		logInfo.m_op_after = 1;
		logver.push_back(logInfo);
	}

	logInfo.m_goodsID = 0;
	logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_coin_add;
	logInfo.m_op_before = m_unCoin;
	logInfo.m_op_after = m_unCoin + coin;
	logver.push_back(logInfo);

	logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_PowerPoint_add;
	logInfo.m_op_before = m_unPowerPoint;
	logInfo.m_op_after = m_unPowerPoint + power_point;
	logver.push_back(logInfo);
	//-------------------------------------------

	this->m_packageMgr.delete_item_list((int *)list, unCount);

	coin = m_benefitMgr.get_benefit(BENEFIT_TYPE_SOLD, (float)coin);
	power_point = m_benefitMgr.get_benefit(BENEFIT_TYPE_MELT, (float)power_point);

	printf("delete item reward coin = %d, power point = %d\n", coin, power_point);

	this->set_basic_info(coin, 0, 0, power_point, 0,0,0);

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]delete item end", m_unID);

	SendLogInfoItemMSG(logver);//log日志
	return 0;
}

int Map::equipment_step_up(ACE_UINT32 unIdx, ACE_UINT32 unPlayerID, int idxList[], int nCount)
{
	int ret = -1;
	int i;

	if(m_functionMgr.is_locked(FUNCTION_STEPUP_EQUIPMENT))
	{
		return ERROR_MSG_LOCKED;
	}

	ItemInfo *itemInfo = NULL;

	if(unPlayerID == 0)
	{
		for(i = 0;i < nCount;i ++)
		{
			if(unIdx == idxList[i])
			{
				return ERROR_MSG_PARAMETER;
			}
		}

		PackageCell *cell = m_packageMgr.get_cell(unIdx);

		if(cell == NULL)
		{
			return ERROR_MSG_OBJ_NULL;
		}

		itemInfo = &cell->item.item;
	}
	else
	{
		if(unIdx == 0)
		{
			ret = m_playerMgr.step_up_player(unPlayerID);

			return ret;
		}

		itemInfo = m_playerMgr.get_equipment(unPlayerID, unIdx);
	}

	if(itemInfo == NULL)
	{
		return ERROR_MSG_OBJ_NULL;
	}

	if(!this->is_same_equipment(idxList, nCount, itemInfo))
	{
		return ERROR_MSG_SAME;
	}

	ret = this->level_up_item(LEVEL_UP_TYPE_EQUIPMENT_STEP, itemInfo, 1);

	if(ret == 0)
	{

		if(unPlayerID == 0)
		{
			m_packageMgr.update_2_db(unIdx);
			m_taskMgr.RefreshTask(Event_Base_Type_EquipmentSpecificStar, m_packageMgr.get_cell(unIdx)->item.item.info.equipment.unStep);
			m_dailyTaskMgr.RefreshTask(Event_Base_Type_EquipmentSpecificStar, m_packageMgr.get_cell(unIdx)->item.item.info.equipment.unStep);
		}
		else
		{
			m_playerMgr.update_2_db();
		}

		m_packageMgr.delete_item_list(idxList, nCount);
	}

	return ret;
}

bool Map::is_same_equipment(int idxList[], int nCount, ItemInfo *itemInfo)
{
	int i, j;
	for(i = 0;i < nCount;i ++)
	{
		for(j = i + 1;j < nCount;j ++)
		{
			if(idxList[i] == idxList[j])
			{
				return false;
			}
		}
	}

	for(i = 0;i < nCount;i ++)
	{
		PackageCell *cell = m_packageMgr.get_cell(idxList[i]);
		if(cell == NULL)
		{
			return false;
		}

		if(itemInfo->unResID != cell->item.item.unResID/*
													   || itemInfo->info.equipment.unStep != cell->item.item.info.equipment.unStep*/)
		{
			return false;
		}
	}

	return true;
}

//add by guo
int Map::weapon_fragment_step_up(ACE_UINT32 unIdx, ACE_UINT32 unPlayerID, int idxList[], int nCount)
{
	int ret = -1;
	int i;

	if(m_functionMgr.is_locked(FUNCTION_STEPUP_EQUIPMENT))
	{
		return -1;
	}

	ItemInfo *itemInfo = NULL;

	if(unPlayerID == 0)
	{
		for(i = 0;i < nCount;i ++)
		{
			if(unIdx == idxList[i])
			{
				return -1;
			}
		}

		PackageCell *cell = m_packageMgr.get_cell(unIdx);

		if(cell == NULL)
		{
			return -1;
		}

		itemInfo = &cell->item.item;
	}
	else
	{
		if(unIdx == 0)
		{
			ret = m_playerMgr.step_up_player(unPlayerID);

			return ret;
		}

		itemInfo = m_playerMgr.get_equipment(unPlayerID, unIdx);
	}

	if(itemInfo == NULL)
	{
		return -1;
	}

	if(!this->is_same_equipment(idxList, nCount, itemInfo))
	{
		return -1;
	}

	ret = this->level_up_item(LEVEL_UP_TYPE_EQUIPMENT_STEP, itemInfo, 1);

	if(ret == 0)
	{
		if(unPlayerID == 0)
		{
			m_packageMgr.update_2_db(unIdx);
		}
		else
		{
			m_playerMgr.update_2_db();
		}

		m_packageMgr.delete_item_list(idxList, nCount);
	}

	return ret;
}


void Map::add_item(ACE_UINT32 unItemID, ACE_UINT32 unCount,LogItemInfo & logInfo)
{
	logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_FAIL;
	logInfo.m_op_before = 0;
	logInfo.m_op_after = 0;
	int type = GET_ITEM_TYPE(unItemID);

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]get item,ID = 0x%08x", m_unID, unItemID);

	if(type >= ITEM_RES_TYPE_HANDBOOK_OFFSET)
	{
		//-----log日志
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_handBook_add;
		logInfo.m_goodsID = unItemID;
		logInfo.m_op_after = unCount;
		//---------------------------------
		m_handbookMgr.add(unItemID);			//图鉴数据保留
		m_materialMgr.add(unItemID, unCount);   //将图鉴作为物品加入 jinpan 如果id改变，该处也该改变
		return;
	}

	switch(type)
	{
	case ITEM_RES_TYPE_PLAYER://真人或者碎片
		{
			m_playerMgr.add_player(unItemID, unCount,logInfo);//添加一个真人
		}
		break;
	case ITEM_RES_TYPE_EQUIPMENT://设备，装备
		{
			ItemInfo item_info;
			ZeroVar(item_info);
			item_info.unResID = unItemID;
			item_info.info.equipment.unLevel = 1;
			item_info.info.equipment.unStep = 1;
			for (int i=0;i<unCount;++i)
			{
				m_packageMgr.add_item(item_info);    //装备发送一次数据只能给他一个啊
			}
			//-----log日志
			//设备
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_equipment_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unCount;
			//---------------------------------
		}
		break;
	case ITEM_RES_TYPE_FUNCTION://功能解锁
		{
			m_functionMgr.unlock(unItemID);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_function_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unCount;
			//---------------------------------
		}
		break;
	case ITEM_RES_TYPE_GIFT://礼包
		{
			m_giftMgr.add_gift(unItemID);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_gift_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unCount;
			//---------------------------------	
		}
		break;
	case ITEM_RES_TYPE_EXTRA_SKILL://特殊技能
		{
			m_skillMgr.add_player_skill(unItemID);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_extra_skill_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unCount;
			//---------------------------------	
		}
		break;
		/*
		case ITEM_RES_TYPE_HANDBOOK:
		{
		m_handbookMgr.add(unItemID);
		}
		break;
		*/
	case ITEM_RES_TYPE_MATERIAL://添加物品
		{
			m_materialMgr.add(unItemID, unCount);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_material_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unCount;
			//---------------------------------	
		}
		break;
	case ITEM_RES_TYPE_PROPS:
		{
			m_propsMgr.add(unItemID, unCount);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_props_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unCount;
			//---------------------------------	
		}
		break;
	default:
		break;
	}

}

void Map::deal_admin_op(MsgAdminOPRequest *request, ObjAdr &targetAdr)
{
	switch(request->unOpType)
	{
	case ADMIN_OP_TYPE_GET_BASIC_INFO:
		{
			this->send_basic_info(targetAdr, 0);
		}
		break;
	case ADMIN_OP_TYPE_GET_PLAYER_LIST:
		this->m_playerMgr.send_info(targetAdr, 0);
		break;
		/*
		case ADMIN_OP_TYPE_GET_EQUIPMENT_LIST:
		this->m_equipmentMgr.send_info(targetAdr, 0);
		break;
		*/
	case ADMIN_OP_TYPE_GET_PACKAGE:
		this->m_packageMgr.send_info_i(targetAdr, 0, request->paramList[0]);
		break;
	case ADMIN_OP_TYPE_GET_LOCK_FUNCTION_LIST:
		this->m_functionMgr.send_info(targetAdr, 0);
		break;
	case ADMIN_OP_TYPE_GET_SKILL_LIST:
		this->m_skillMgr.send_info(targetAdr, 0);
		break;
	case ADMIN_OP_TYPE_SET_LEVEL:
		break;
	case ADMIN_OP_TYPE_SET_COIN:
		break;
	case ADMIN_OP_TYPE_SET_MONEY:
		break;
	case ADMIN_OP_TYPE_SET_POWER_POINT:
		break;
	case ADMIN_OP_TYPE_ADD_EQUIPMENT:
		break;
	case ADMIN_OP_TYPE_DELETE_EQUIPMENT:
		break;
	case ADMIN_OP_TYPE_EDIT_EQUIPMENT:
		break;
	case ADMIN_OP_TYPE_ADD_PLAYER:
		break;
	case ADMIN_OP_TYPE_DELETE_PLAYER:
		break;
	case ADMIN_OP_TYPE_EDIT_PLAYER:
		break;
	case ADMIN_OP_TYPE_SET_SKILL_LEVEL:
		break;
	case ADMIN_OP_TYPE_UNLOCK:
		break;
	case ADMIN_OP_TYPE_ADD_COIN:
		//set_basic_info(request->paramList[0], 0, 0, 0, 0);
		break;
	case ADMIN_OP_TYPE_ADD_MONEY:
		//set_basic_info(0, request->paramList[0], 0, 0, 0);
		break;
	}
}

bool Map::is_cost_enough(CostInfoList &costList, int times)
{
	int i;

	int type;

	for(i = 0;i < costList.unCount;i ++)
	{
		type = GET_ITEM_TYPE(costList.list[i].unID);

		if(type >= ITEM_RES_TYPE_HANDBOOK_OFFSET)					//判断装备图鉴够不够	jinpan  以后要整合到 ITEM_RES_TYPE_MATERIAL
			if(!m_materialMgr.is_enough(costList.list[i], times))
			{
				return false;		//有一个欠缺就直接跳出
			}

			if(type == ITEM_RES_TYPE_CURRENCY)
			{
				switch(costList.list[i].unID)
				{
				case RES_CURRENCY_COIN:
					if(m_unCoin < (costList.list[i].cost * times))
					{
						return false;
					}
					break;
				case RES_CURRENCY_MONEY:
					if(m_unMoney < (costList.list[i].cost * times))
					{
						return false;
					}
					break;
				case RES_CURRENCY_ENERGY:
					if(!m_energyMgr.is_enough(costList.list[i].cost * times))
					{
						return false;
					}
					break;
				case RES_CURRENCY_POWERPOINT:
					if(m_unPowerPoint < (costList.list[i].cost * times))
					{
						return false;
					}
					break;
				case RES_CURRENCY_HONOUR:
					if(m_unHonour < (costList.list[i].cost * times))
					{
						return false;
					}
					break;
				case RES_CURRENCY_PVP_ENERGY:
// 					if(!m_pvpEnergyMgr.is_enough(costList.list[i].cost * times))
// 					{
// 						return false;
// 					}
					break;
				case RES_CURRENCY_FRIENDSHIP:
					{
						if (m_unFriendshipPoint < costList.list[i].cost * times)
						{
							return false;
						}
					}break;
				case RES_CURRENCY_EXP:
					break;
				case RES_CURRENCY_RECHARGE:
					break;
				}
			}
			else
			{
				switch(type)
				{
				case ITEM_RES_TYPE_MATERIAL:

					if(!m_materialMgr.is_enough(costList.list[i], times))
					{
						return false;
					}
					break;
				case ITEM_RES_TYPE_PROPS:
					if(!m_propsMgr.is_enough(costList.list[i], times))
					{
						return false;
					}
					break;

				}
			}
	}

	return true;
}

void Map::do_cost(CostInfoList &costList, int times,LogItemInfo & logInfo)
{
	int i;

	bool flag = false;
	bool material_flag = false;
	bool props_flag = false;

	int nCoin = 0;
	int nMoney = 0;
	int nPowerPoint = 0;
	int nHonour = 0;
	int nFriendship = 0;
	int nDynamicEnergy = 0;
	int type;

	//log日志相关
	SetlogUserInfo(logInfo);
	std::vector<LogItemInfo> logVer;//日志相关

	for(i = 0;i < costList.unCount;i ++)
	{
		logInfo.m_goodsID = costList.list[i].unID;

		type = GET_ITEM_TYPE(costList.list[i].unID);
		if(type >= ITEM_RES_TYPE_HANDBOOK_OFFSET){
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_handBook_lower;
			logInfo.m_op_before = 0;
			logInfo.m_op_after = -(costList.list[i].cost * times);
			logVer.push_back(logInfo);
			//---------------------------------

			m_materialMgr.do_cost(costList.list[i], times, false,1);
			material_flag = true;

			int idx;
			idx = GameUtils::find_element(m_materialMgr.m_unCount, m_materialMgr.m_list, sizeof(MaterialInfo), costList.list[i].unID);

			if(m_materialMgr.m_list[idx].unCount <= 0){
				m_handbookMgr.del(costList.list[i].unID);//当物品消耗光了，删除掉图鉴的图标显示
			}
			continue;				//并不需要跳出循环，对图鉴做特殊处理 jinpan
		}

		if(type == ITEM_RES_TYPE_CURRENCY)
		{
			switch(costList.list[i].unID)
			{
			case RES_CURRENCY_COIN:
				if(costList.list[i].cost > 0)
				{
					nCoin = costList.list[i].cost * times;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_coin_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unCoin;
					logInfo.m_op_after = m_unCoin - nCoin;
					logVer.push_back(logInfo);
					//---------------------------------
					flag = true;
				}
				break;
			case RES_CURRENCY_MONEY:
				if(costList.list[i].cost > 0)
				{
					nMoney = costList.list[i].cost * times;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_money_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unMoney;
					logInfo.m_op_after = m_unMoney - nMoney;
					logVer.push_back(logInfo);
					//---------------------------------
					flag = true;
				}
				break;
			case RES_CURRENCY_ENERGY:
				{
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_energy_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_energyMgr.m_unEnergy;
					logInfo.m_op_after = m_energyMgr.m_unEnergy - costList.list[i].cost * times;
					logVer.push_back(logInfo);
					//---------------------------------
					m_energyMgr.decrease(costList.list[i].cost * times);
				}
				break;
			case RES_CURRENCY_POWERPOINT:
				if(costList.list[i].cost > 0)
				{
					nPowerPoint = costList.list[i].cost * times;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_PowerPoint_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unPowerPoint;
					logInfo.m_op_after = m_unPowerPoint -nPowerPoint;
					logVer.push_back(logInfo);
					//---------------------------------
					flag = true;
				}
				break;
			case RES_CURRENCY_HONOUR:
				if(costList.list[i].cost > 0)
				{
					nHonour = costList.list[i].cost * times;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_honour_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unHonour;
					logInfo.m_op_after = m_unHonour -nHonour;
					logVer.push_back(logInfo);
					//---------------------------------
					flag = true;
				}
				break;
			case RES_CURRENCY_PVP_ENERGY:
				{
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_pvp_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					//logInfo.m_op_before = m_pvpEnergyMgr.m_unEnergy;
					//logInfo.m_op_after = m_pvpEnergyMgr.m_unEnergy -costList.list[i].cost * times;
					logVer.push_back(logInfo);
					//---------------------------------
					//m_pvpEnergyMgr.decrease(costList.list[i].cost * times);
				}
				break;
			case RES_CURRENCY_FRIENDSHIP:
				{
					if (costList.list[i].cost > 0)
					{
						nFriendship = costList.list[i].cost * times;
						//-----log日志
						logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_Friendship_lower;
						//logInfo.m_goodsID = costList.list[i].unID;
						logInfo.m_op_before = m_unFriendshipPoint;
						logInfo.m_op_after = m_unFriendshipPoint -nFriendship;
						logVer.push_back(logInfo);
						//---------------------------------
						flag = true;
					}
				}break;
			case RES_CURRENCY_EXP:
				break;
			case RES_CURRENCY_RECHARGE:
				break;
			case RES_CURRENCY_DYNAMIC_ENERGY:
				{
					if(costList.list[i].cost > 0)
					{
						nDynamicEnergy = costList.list[i].cost * times;
						//-----log日志
						logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_Dynamic_Energy_lower;
						//logInfo.m_goodsID = costList.list[i].unID;
						logInfo.m_op_before = m_unDynamicEnergy;
						logInfo.m_op_after = m_unDynamicEnergy - nDynamicEnergy;
						logVer.push_back(logInfo);
						//---------------------------------
						flag = true;
					}
				}break;
			}
		}
		else
		{
			switch(type)
			{
			case ITEM_RES_TYPE_MATERIAL:
				{
					m_materialMgr.do_cost(costList.list[i], times, false,1);
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_material_lower;
					logInfo.m_op_before = 0;
					logInfo.m_op_after = -(costList.list[i].cost * times);
					logVer.push_back(logInfo);
					//---------------------------------

				}
				material_flag = true;
				break;
			case ITEM_RES_TYPE_PROPS:
				{
					m_propsMgr.do_cost(costList.list[i], times, false,1);
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_props_lower;
					logInfo.m_op_before = 0;
					logInfo.m_op_after = -(costList.list[i].cost * times);
					logVer.push_back(logInfo);
					//---------------------------------

				}
				props_flag = true;
				break;

			}
		}
	}

	if(flag)
	{
		set_basic_info(-nCoin, -nMoney, 0, -nPowerPoint, -nHonour, -nFriendship,-nDynamicEnergy);
	}
	if(material_flag)
	{
		m_materialMgr.update_2_db();
	}
	if(props_flag)
	{
		m_propsMgr.update_2_db();
	}

	SendLogInfoItemMSG(logVer);//log日志
}

int Map::do_compose(ACE_UINT32 unItemID ,ACE_UINT32 unTimes )
{
	if(unTimes <= 0){   //当为0的时候所有的过程都作了一遍但是没有实际作用，可能会被人攻击 jinpan
		return ERROR_MSG_PARAMETER;
	}
	BaseRes *res = ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unItemID);

	if(res == NULL || res->m_unFormulaID == 0)
	{
		return ERROR_MSG_RES;
	}

	FormulaRes *formulaRes = (FormulaRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(res->m_unFormulaID);

	if(formulaRes == NULL)
	{
		return ERROR_MSG_RES;
	}

	if(formulaRes->m_unRecipeRequired == 1)	//修改以后将判断加入is_cost_enough，同时保持之前存储图鉴表不变吧
	{
		ACE_UINT32 tmpID = BaseRes::handbook_ID(unItemID);

		if(!m_handbookMgr.has_handbook(tmpID))
		{
			return ERROR_MSG_HANDBOOK;
		}	
	}


	if(!is_cost_enough(formulaRes->m_costList, unTimes))
	{
		return ERROR_MSG_COST;
	}

	//-----------log日志
	//操作
	std::vector<LogItemInfo> logver;
	LogItemInfo logInfo;
	logInfo.m_goodsID = unItemID;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_do_compose;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_do_compose);//途径(附加信息)
	SetlogUserInfo(logInfo);//设置基础信息

	int type = GET_ITEM_TYPE(unItemID);

	switch(type)
	{

	case ITEM_RES_TYPE_MATERIAL:   //added by jinpan for material compose
		{
			m_materialMgr.add(unItemID, unTimes);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_material_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unTimes;
			logver.push_back(logInfo);
			//---------------------------------	
		}
		break;
	case ITEM_RES_TYPE_PLAYER:
		//m_playerMgr.add_player(unItemID);
		{
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_DEFALT;
			logInfo.m_goodsID = 0;
			logInfo.m_op_after = 0;
			if(m_playerMgr.add_player(unItemID,unTimes,logInfo))
			{
				return -1;
			}
			logver.push_back(logInfo);
		}
		break;
	case ITEM_RES_TYPE_EQUIPMENT:
		if(m_packageMgr.is_full())
		{
			return ERROR_MSG_PKG_FULL;
		}
		else
		{
			ItemInfo itemInfo;
			ZeroVar(itemInfo);
			itemInfo.unResID = unItemID;
			itemInfo.info.equipment.unLevel = 1;
			itemInfo.info.equipment.unStep = 1;

			for (int i=0;i<unTimes;i++)
			{
				if(m_packageMgr.add_item(itemInfo) == -1)
				{
					return ERROR_MSG_PKG_FULL;
				}
			}

			//-----log日志
			//设备
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_equipment_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unTimes;
			logver.push_back(logInfo);
			//---------------------------------
			//---gm 后台
			GMInfo gminfo;
			ZeroVar(gminfo);
			gminfo.unType = ADMIN_OP_GM_COMPOSE;
			gminfo.unGoodID = unItemID;
			gminfo.unLevel = 0;
			AddGmMsg(gminfo);
			//----------------------------------------------
		}

		break;
	case ITEM_RES_TYPE_PROPS:   //added by jinpan for material compose
		{
			m_propsMgr.add(unItemID, unTimes);
			//-----log日志
			logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_props_add;
			logInfo.m_goodsID = unItemID;
			logInfo.m_op_after = unTimes;
			logver.push_back(logInfo);
			//---------------------------------	
		}
		break;
	default:
		return -1;
	}

	SendLogInfoItemMSG(logver);//log日志
	logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_DEFALT;
	logInfo.m_goodsID = 0;
	logInfo.m_op_after = 0;
	do_cost(formulaRes->m_costList, unTimes,logInfo);

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]compose item ID = 0x%08x", m_unID, unItemID);

	return 0;
}

void Map::update_energy_2_db()
{
	MsgDBUpdateEnergyRequest db_request;
	db_request.unEnergy = m_energyMgr.m_unEnergy;
	db_request.unRestoreTime = m_energyMgr.m_unRestoreTime;
	//db_request.unPVPEnergy = m_pvpEnergyMgr.m_unEnergy;
	//db_request.unPVPRestoreTime = m_pvpEnergyMgr.m_unRestoreTime;
	//db_request.unEnergyBuyCount = m_energyMgr.m_unBuyCount;
	//db_request.unEnergyBuyTime = m_energyMgr.m_unBuyTime;
	add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_ENERGY, (char *)&db_request, sizeof(MsgDBUpdateEnergyRequest));
}

void Map::update_snap_info_2_db()
{
	MsgDBUpdateCharacterSnapInfoRequest db_request;
	db_request.teamInfo = m_playerMgr.m_teamInfo;
	//db_request.pvpTeamInfo = m_pvpMgr.m_teamInfo;
	add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_CHARACTER_SNAP_INFO, (char *)&db_request, sizeof(MsgDBUpdateCharacterSnapInfoRequest));
}

void Map::update_friendship_2_db()
{
	MsgDBUpdateFriendshipRequest db_request;
	db_request.m_unFriendship = m_unFriendshipPoint;
	add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_FRIENDSHIP, (char *)&db_request, sizeof(MsgDBUpdateFriendshipRequest));
}


void Map::moveToMateria()
{
	ACE_INT32 idx,i;
	ACE_UINT32	unItemId;	
	ACE_INT32 flag = 0;
	for(i = 0;i < m_handbookMgr.m_unCount; ++i){
		unItemId = m_handbookMgr.m_list[i].unItemID;
		idx = GameUtils::find_element(m_materialMgr.m_unCount,m_materialMgr.m_list,sizeof(MaterialInfo),unItemId);
		//if(idx == -1){
		if( idx == -1 || m_materialMgr.m_list[idx].unCount ==0 ){  //之前用掉物品后只是将他的数量为0并没有做删除其物品 id jinpan
			m_materialMgr.add(unItemId,1);		//只需要加入一个
			flag = 1;
		}
	}
	if(flag == 1){
		m_materialMgr.update_2_db();
	}

}

int Map::deal_get_article(unsigned articleId,unsigned count)  //added by jinpan
{
// 
// 	//	m_secretMgr.m_unCellCount = 10;
// 	//	secret_expand(7);
// 	//	secret_refresh(m_secretMgr.m_list,0);
// 	//	secret_buy(386007297);
// 
// 
// 
// 	m_vipMgr.recharge(18089,true);
// 	//	ItemInfo itemInfo;
// 	//	itemInfo.unResID = articleId;
// 	//	itemInfo.info.equipment.unLevel = 30;
// 	//	itemInfo.info.equipment.unStep = 3;
// 	//	m_packageMgr.add_item(itemInfo, 1);
// 	//	//void Map::set_basic_info(int nCoin, int nMoney, int nExp, int nPowerPoint, int nHonour)
// 	set_basic_info(5000000,50000000,30000000,30,30);//获取金币经验等
// 	//ACE_UINT32 tmpID = BaseRes::handbook_ID(2317373441);
// 	//	ACE_UINT32 tmpID = BaseRes::handbook_ID(articleId);//直接获得图纸，获取偏移量，图鉴id
// 	LogItemInfo loginfo;
// 	add_item(2349879311,50,loginfo);
// 	add_item(articleId,count,loginfo);
// 	////	add_item(2349862927,20);//二郎神
// 	//	add_item(articleId,count);
// 	//	add_item(202395661,70);
// 	//	add_item(tmpID,120);
// 	//	if(m_packageMgr.add_item(itemInfo, count) == -1){
// 	//		return -1;
// 	//	}else 
	return 0;

}
void Map::update_login_count()
{
	bool dirty = false;
	int t = GameUtils::get_day_different(m_unCurrentTime, this->m_unLastLoginTime);

	if(t > 0)
	{
		if((int)m_unLoginBonusCount <= 0)
		{
			this->m_unLoginBonusCount = (ACE_UINT32)(1 - (int)m_unLoginBonusCount);
		}
		//else
		//{
		//	this->m_unLoginBonusCount = 1;
		//}


		/*
		int idx = (int)(this->m_unLoginBonusCount - 1);
		if(idx >= MAX_LOGIN_BONUS_COUNT)
		{
		idx = MAX_LOGIN_BONUS_COUNT - 1;
		}
		t_money = GAME_CONFIG_INSTANCE::instance()->m_loginBonusList[idx];

		response.unLoginCount = m_unLoginBonusCount;
		*/
		m_unLastLoginTime = m_unCurrentTime;
		this->m_unLoginCount ++;
		dirty = true;
	}
	else
	{
		if(m_unCurrentTime > m_unLastLoginTime)
		{
			if((m_unCurrentTime - m_unLastLoginTime) >= 3600)
			{
				m_unLastLoginTime = m_unCurrentTime;
				this->m_unLoginCount ++;
				dirty = true;
			}
		}
	}

	if(dirty)
	{
		MsgDBUpdateLoginInfoRequest db_request;
		db_request.unLoginTime = m_unLastLoginTime;
		db_request.unLoginCount = m_unLoginCount;
		db_request.unLoginBonusCount = m_unLoginBonusCount;

		this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_LOGIN_INFO, (char *)&db_request, sizeof(MsgDBUpdateLoginInfoRequest));
	}




	//this->m_unLoginCount ++;

	//update 2 db
	/*
	if(t_money > 0)
	{
	//t_money = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_GOLD, (float)t_money);
	int coin = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_COIN, 0);
	int power_point = this->m_benefitMgr.get_benefit(BENEFIT_TYPE_LOGIN_POWER_POINT, 0);

	this->set_basic_info(coin, t_money, 0, power_point, 0);
	}

	MsgDBUpdateLoginInfoRequest db_request;
	db_request.unLoginTime = m_unLastLoginTime;
	db_request.unLoginCount = m_unLoginCount;
	db_request.unLoginBonusCount = m_unLoginBonusCount;

	this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_LOGIN_INFO, (char *)&db_request, sizeof(MsgDBUpdateLoginInfoRequest));


	this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_GET_LOGIN_REWARD_RESPONSE,(char *)&response, sizeof(MsgGetLoginRewardResponse), OBJ_ID_NONE, ADAPTER_GATEWAY_MGR, m_unUserID);
	send_response_end(msgInfo.s_head->srcAdr.unObjID, body->unSN);
	*/
}

ACE_INT32 Map::max_exp_get()
{
	ACE_UINT32 tmp_MaxExp = -1;
	if ((m_unLevel == (m_unMaxLevel - 1)) && (m_unExpLevelUp - m_unExp) >= 0 ){
		tmp_MaxExp = m_unExpLevelUp - m_unExp;
	}else if(m_unLevel >= m_unMaxLevel){
		tmp_MaxExp = 0;
	}
	else {
		tmp_MaxExp = -1;
	}

	return tmp_MaxExp;


}

int Map::secret_refresh(SecretItemInfo dest[],ACE_UINT32 type)//刷新的方式 0是普通刷新，其他高级刷新,1元宝,2道具 
{//还需要添加刷新一次次数+1，减去刷新消费，更新刷新时间
	SecretCfg tmpCfg;
	tmpCfg = get_secret_cfg(type);
	CostInfoList cost;
	ACE_UINT32 curTime = GameUtils::get_utc();//当前时间戳

	if((type == 0) && ((curTime - m_secretMgr.m_unLastRefreshTime) < m_secretCfg.unRefreshSec)  && (m_secretMgr.m_refreshTimes > 0)){  //当前时间和上次刷新的时间之差小雨设置的时间，自动刷新时候
		return -1;	
	}

	int d = GameUtils::get_day_different(curTime, m_secretMgr.m_unLastRefreshTime);

	if(d > 0)
	{
		set_secret_paid_refresh_times(0);
		MsgDBUpdateCharacterFlagRequest db_request;
		db_request.unFlag = m_dwFlag;
		this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_CHARACTER_FLAG, (char *)&db_request, sizeof(db_request));
	}

	switch (type)
	{
	case SECRET_REFRESH_3:
		{
			cost.unCount = 1;//低级元宝刷新每次刷新一次，而且只有一种物品
			cost.list[0].cost = m_secretCfg.moenyRefresh.unCount;
			cost.list[0].unID = m_secretCfg.moenyRefresh.unId;
		}break;
	case SECRET_REFRESH_2:
		{
			cost.unCount = 1;//每次刷新一次，而且只有一种物品材料低级刷新
			cost.list[0].cost = m_secretCfg.itemRefresh.unCount;
			cost.list[0].unID = m_secretCfg.itemRefresh.unId;
		}break;
	case SECRET_REFRESH_1://高级元宝刷新, 实际上现在在用的也只有这一种
		{
			cost.unCount = 1;
			//ACE_UINT32 unCurrentCost = m_secretCfg.higher_moenyRefresh.unCount + m_secretCfg.unHigher_moneyRefresh_increase * get_secret_paid_refresh_times();
			cost.list[0].cost = m_secretMgr.m_unPaidRefreshPrice;//m_secretCfg.unHigher_moneyRefresh_increaseUpperBound > unCurrentCost ? unCurrentCost : m_secretCfg.unHigher_moneyRefresh_increaseUpperBound;
			cost.list[0].unID = m_secretCfg.higher_moenyRefresh.unId;
		}break;
	case SECRET_REFRESH_4://高级材料刷新
		{
			cost.unCount = 1;
			cost.list[0].cost = m_secretCfg.higher_itemRefresh.unCount;
			cost.list[0].unID = m_secretCfg.higher_itemRefresh.unId ;
		}break;
	default:
		break;
	}

	if(type != 0 && !is_cost_enough(cost, 1))//排除自动刷新是否够的检测
	{
		return -1;//条件不够 
	}

	int c,k ;
	int cellCount = m_secretMgr.m_unCellCount;

	if (tmpCfg.unCertainCount > m_secretMgr.m_unCellCount){
		return -1;  //必出的比格子都多，不可能出现的情况,除非配置错误 
	}


	for(c = 0; c < tmpCfg.unCertainCount; c++){
		dest[c].unItemID = get_secret_id(tmpCfg.certainList[c]);
		dest[c].bBuy = 0;
		set_secret_amount(dest[c]);
		cellCount--;
	}
	for(k = 0; k < cellCount; k++,c++){
		dest[c].unItemID = get_secret_id(tmpCfg.unOtherItems[0]);//其他种类只需要设置一个总表，读取第一个
		dest[c].bBuy = 0;
		set_secret_amount(dest[c]);
	}



	if(type != 0){  //0表示自动刷新
		//-----------log日志
		//刷新神秘商店
		LogItemInfo logInfo;
		logInfo.m_goodsID = type;
		logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_secret_refresh;//动作原因
		logInfo.SetOp_Way(_LOG_OP_WAY_secret_refresh);//途径(附加信息)
		do_cost(cost,1,logInfo);
		m_secretMgr.m_unLastRefreshTime = curTime;  //手动刷新倒计时时间按现在时间来算
		if (get_secret_paid_refresh_times() < SECRET_PAID_REFRESH_TIMES_COUNT_UPPER_BOUND)
		{
			set_secret_paid_refresh_times(get_secret_paid_refresh_times()+1);
			MsgDBUpdateCharacterFlagRequest db_request;
			db_request.unFlag = m_dwFlag;
			this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_CHARACTER_FLAG, (char *)&db_request, sizeof(db_request));
		}

	}else{  //时间到了自动刷新要注意
		if(m_secretMgr.m_refreshTimes == 0){  //第一次刷新
			m_secretMgr.m_unLastRefreshTime = curTime;
		}else{
			m_secretMgr.m_unLastRefreshTime = (curTime - (curTime - m_secretMgr.m_unLastRefreshTime)% (m_secretCfg.unRefreshSec));  //上次自动刷新的时间
		}
	}
	m_secretMgr.m_refreshTimes++;//每次成功刷新次数加一
	m_secretMgr.update_2_db(); //每次刷新都要更新数据库

	return 0;


}

void Map::set_secret_amount(MaterialInfo &dest)//设置各种晶石个数
{
	int c = 0;
	dest.unCount = 1;//一般物品个数都是1
	for(c = 0; c < MAX_SECRET_LIST; c++){
		if(dest.unItemID == m_secretCfg.specialAmount[c].unId){
			dest.unCount = m_secretCfg.specialAmount[c].unCount;
			break;
		}		
	}

}
SecretCfg Map::get_secret_cfg(ACE_UINT32 type)
{
	SecretCfg tmpCfg;
	int c;
	if(type == 3 || type == 4){  //3,4表示是高级刷新
		for(c = 0;c < m_secretCfg.unCount;c++){
			if(m_secretCfg.secretCfg[c].unCell == m_secretMgr.m_unCellCount){
				tmpCfg = m_secretCfg.secretCfg[c];
			}
		}
	}else{//0,1,2表示低级刷新
		for(c = 0;c < m_secretCfg.unCount;c++){
			if(m_secretCfg.secretCfg[c].unCell == 0){
				tmpCfg = m_secretCfg.secretCfg[c];
			}
		}
	}

	return tmpCfg;
}
ACE_UINT32 Map::get_secret_id(ACE_UINT32 tmp_id)
{
	ACE_UINT32 tmp_drop_id;
	DropRes *dropRes = (DropRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_id);
	if(dropRes == NULL)
	{
		tmp_drop_id = 0;
	}
	else
	{
		tmp_drop_id = m_lootMgr.loot(dropRes->m_dropGroup, 2);//2什么意思...
	}

	return tmp_drop_id;

}

int Map::secret_expand(ACE_UINT32 unExpandCount)
{
	if(unExpandCount > m_secretCfg.unMaxCell ){
		return -1;//格子已经开满了
	}

	int k;//money需要的元宝个数
	int money = 0;
	int oldCellCount;
	oldCellCount = m_secretMgr.m_unCellCount;
	int c = m_secretMgr.m_unCellCount + 1 ;//从从下一个格子开始收费
	CostInfoList cost;
	cost.unCount = 1;//只算费一次
	SecretCfg tmpCfg;
	tmpCfg = get_secret_cfg(0); //扩充按普通刷新给他个东西
	for(c;c <= unExpandCount;c++){
		for(k = 0;k < MAX_SECRET_LIST;k++){
			if(c == m_secretCfg.cellPrice[k].unId){
				money+= m_secretCfg.cellPrice[k].unCount;
				break;
			}
		}
	}
	cost.list[0].cost = money;
	cost.list[0].unID = 335675393;  //这个是元宝的id...可能需要优化?

	if(!is_cost_enough(cost,1)){
		return -1;
	}
	m_secretMgr.m_unCellCount += (unExpandCount - m_secretMgr.m_unCellCount);

	//-----------log日志
	//扩充神秘商店
	LogItemInfo logInfo;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_secret_expand;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_secret_expand);//途径(附加信息)
	do_cost(cost,1,logInfo);
	for(k = oldCellCount;k < m_secretMgr.m_unCellCount;k++){
		m_secretMgr.m_list[k].unItemID = get_secret_id(tmpCfg.unOtherItems[0]);//其他种类只需要设置一个总表，读取第一个
		m_secretMgr.m_list[k].bBuy = 0;
		set_secret_amount(m_secretMgr.m_list[k]);
	}
	m_secretMgr.update_2_db();
	return 0;
}

int Map::secret_buy(ACE_UINT32 unId,ACE_UINT32 unIdx)  //神秘商店购买
{
	int type = GET_ITEM_TYPE(unId);
	BaseRes *res ;
	int target = 0; //当前列
	int c;
	for(c = 0;c < m_secretMgr.m_unCellCount ;c++ ){
		if ((unId == m_secretMgr.m_list[c].unItemID) && (m_secretMgr.m_list[c].bBuy == 0) && c == unIdx){	
			//target = c;
			break;//找到该物品
		}
	}
	if(c >= m_secretMgr.m_unCellCount || m_secretMgr.m_list[c].bBuy != 0){//没找到或者已经购买过了
		return -1;//列表里面没有该id
	}
	target = c;
	CostInfoList cost;

	if(type >= ITEM_RES_TYPE_HANDBOOK_OFFSET) //图鉴通过formulaId来获得配置
	{
		ACE_UINT32 tmpID = BaseRes::handbook_ID(unId);

		res = ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmpID);
		if(res == NULL)
		{
			return -1;
		}
		FormulaRes *formulaRes = (FormulaRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(res->m_unFormulaID);

		if(formulaRes == NULL)
		{
			return -1;
		}

		if(formulaRes->m_unRecipeRequired == 1){
			cost = formulaRes->m_shop; 

		}else{//不需要图纸
			return -1;
		}

		// 	}else if(type == ITEM_RES_TYPE_CURRENCY){
		// 		if(unId == RES_CURRENCY_POWERPOINT){
		// 			cost.unCount = 1;
		// 			cost.list->cost = 5000;  //晶石的价格和消耗品写死，是14010001金币购买，消耗5000个
		// 			cost.list->unID = 335609857;
		// 		}


	}else{
		res = ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unId);
		if(res == NULL)
		{
			return -1;
		}
		cost = res->m_shop;

	}

	for(c = 0; c < cost.unCount; c++){//打折销售
		cost.list[c].cost =  int(cost.list[c].cost * m_fSecretOff);
	}

	//神秘商店打折
	m_LAMgr.DoLATypeSecret(0,cost);
	//-----------------------------

	//修改以后将判断加入is_cost_enough，同时保持之前存储图鉴表不变吧
	if(!is_cost_enough(cost,m_secretMgr.m_list[target].unCount)){
		return -1;//钱不够
	}
	LogItemInfo logInfo;
	logInfo.m_goodsID = unId;//物品id
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_secret_buy;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_secret_buy);//途径(附加信息)
	do_cost(cost,m_secretMgr.m_list[target].unCount,logInfo);
	m_secretMgr.m_list[target].bBuy = 1;//表示已经被购买

	//----------log日志
	//神秘商店购买
	// 	ZeroVar(logInfo);
	// 	SetlogUserInfo(logInfo);//log日志相关
	logInfo.m_goodsID = unId;//物品id
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_secret_buy;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_secret_buy);//途径(附加信息)
	add_item(unId,m_secretMgr.m_list[target].unCount,logInfo);
	std::vector<LogItemInfo> ver;
	ver.push_back(logInfo);
	SendLogInfoItemMSG(ver);

	m_secretMgr.update_2_db();//更新数据 库
	m_taskMgr.RefreshTask(Event_Base_Type_SecretBuy, 0);
	m_dailyTaskMgr.RefreshTask(Event_Base_Type_SecretBuy, 0);
	return 0;
}

int Map::golden_touch(ACE_UINT32 &unGoldenTouchProfit, ACE_UINT32 &unCriticalCount)
{
	CheckDateChanged();
	if (m_goldenTouchMgr.m_unUsedTouchCount >= m_goldenTouchMgr.m_buy_gold_limit)
	{
		return ERROR_MSG_TIMES;
	}
	std::map<ACE_UINT32, GoldenTouchProfit> &refGoldenTouchProfit = GAME_CONFIG_INSTANCE::instance()->m_oGoldenTouchProfit;
	std::map<ACE_UINT32, GoldenTouchProfit>::iterator itMap1 = refGoldenTouchProfit.find(m_unLevel);
	if (itMap1 == refGoldenTouchProfit.end())
	{
		return -1;
	}
	std::map<ACE_UINT32, GoldenTouchCost> &refGoldenTouchCost = GAME_CONFIG_INSTANCE::instance()->m_oGoldenTouchCost;
	std::map<ACE_UINT32, GoldenTouchCost>::iterator itMap2 = refGoldenTouchCost.find(m_goldenTouchMgr.m_unUsedTouchCount+1);
	if (itMap2 == refGoldenTouchCost.end())
	{
		return -1;
	}
	CostInfoList cost;
	cost.unCount = 1;
	cost.list[0].cost = itMap2->second.unTouchCost;
	cost.list[0].unID = RES_CURRENCY_MONEY;  
	if(!is_cost_enough(cost,1))
	{
		return -1;
	}
	LogItemInfo logInfo;
	logInfo.m_goodsID = RES_CURRENCY_MONEY;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_golden_touch;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_golden_touch);//途径(附加信息)
	do_cost(cost,1,logInfo); 
	unGoldenTouchProfit = itMap1->second.unBaseProfit+itMap1->second.unCountProfit * (m_goldenTouchMgr.m_unUsedTouchCount++);
	ACE_UINT32 unRand = rand() % 100;
	//double golden_profit = 1.0;//点金手暴击收益百分比
	unCriticalCount = 1;
	if (unRand < itMap2->second.unCriticalOdds)
	{
		unCriticalCount = m_vipMgr.GetVipGolden_profit();
	}

	unGoldenTouchProfit = unGoldenTouchProfit * unCriticalCount;

	set_basic_info(unGoldenTouchProfit,0, 0, 0, 0,0,0);
	m_goldenTouchMgr.update_2_db();
	return 0;
}

void Map::do_map_before_cost( CostInfoList &costList, int times,LogItemInfo & logInfo )
{
	//log日志
	SetlogUserInfo(logInfo);
	std::vector<LogItemInfo> logVer;
	//----------------------------
	int i;
	int type;
	for(i = 0;i < costList.unCount;i ++)
	{
		type = GET_ITEM_TYPE(costList.list[i].unID);
		if(type == ITEM_RES_TYPE_CURRENCY)
		{
			switch(costList.list[i].unID)
			{
			case RES_CURRENCY_ENERGY:
				{
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_energy_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_energyMgr.m_unEnergy;
					logInfo.m_op_after = m_energyMgr.m_unEnergy - 1 * times;
					logVer.push_back(logInfo);
					//---------------------------------
					m_energyMgr.decrease(1 * times);//先剪掉一个体力值
				}
				break;
			case RES_CURRENCY_PVP_ENERGY:
				{
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_pvp_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					//logInfo.m_op_before = m_pvpEnergyMgr.m_unEnergy;
					//logInfo.m_op_after = m_pvpEnergyMgr.m_unEnergy -costList.list[i].cost * times;
					logVer.push_back(logInfo);
					//---------------------------------
					//m_pvpEnergyMgr.decrease(costList.list[i].cost * times);
				}
				break;
			}
		}
	}

	SendLogInfoItemMSG(logVer);//log日志
}

ACE_UINT32 Map::do_map_after_cost( CostInfoList &costList,ACE_UINT32 unMapID, int times,LogItemInfo & logInfo )
{
	int i;

	bool flag = false;
	bool material_flag = false;
	bool props_flag = false;

	int nCoin = 0;
	int nMoney = 0;
	int nPowerPoint = 0;
	int nHonour = 0;
	int nFriendship = 0;
	int nDynamicEnergy = 0;
	int type;

	//编号【3】关卡消耗减半，关卡1ID，关卡2ID，…
	ACE_UINT32 count = 1;
	if (m_LAMgr.DoLATypeMapCount(unMapID,LA_TYPE_3,count))
	{
		count = 2;
	}
	else
		count =1;
	//-----------------------------------end

	std::vector<LogItemInfo> logVer;
	
	for(i = 0;i < costList.unCount;i ++)
	{
		type = GET_ITEM_TYPE(costList.list[i].unID);
		if(type >= ITEM_RES_TYPE_HANDBOOK_OFFSET){
			m_materialMgr.do_cost(costList.list[i], times, false,1);
			material_flag = true;

			int idx;
			idx = GameUtils::find_element(m_materialMgr.m_unCount, m_materialMgr.m_list, sizeof(MaterialInfo), costList.list[i].unID);

			if(m_materialMgr.m_list[idx].unCount <= 0){
				m_handbookMgr.del(costList.list[i].unID);//当物品消耗光了，删除掉图鉴的图标显示
			}
			continue;				//并不需要跳出循环，对图鉴做特殊处理 jinpan
		}

		if(type == ITEM_RES_TYPE_CURRENCY)
		{
			switch(costList.list[i].unID)
			{
			case RES_CURRENCY_COIN:
				if(costList.list[i].cost > 0)
				{
					nCoin = costList.list[i].cost * times;
					flag = true;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_coin_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unCoin;
					logInfo.m_op_after = m_unCoin - nCoin;
					logVer.push_back(logInfo);
					//---------------------------------
				}
				break;
			case RES_CURRENCY_MONEY:
				if(costList.list[i].cost > 0)
				{
					nMoney = (costList.list[i].cost * times)/count;
					flag = true;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_money_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unMoney;
					logInfo.m_op_after = m_unMoney - nMoney;
					logVer.push_back(logInfo);
					//---------------------------------
				}
				break;
			case RES_CURRENCY_ENERGY:
				{
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_energy_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_energyMgr.m_unEnergy;
					logInfo.m_op_after = m_energyMgr.m_unEnergy - (costList.list[i].cost - 1) * times;
					logVer.push_back(logInfo);
					//---------------------------------
					ACE_UINT32 cost = (costList.list[i].cost - 1) * times + costList.list[i].cost * (m_unMultEnergy-1);
					cost =(cost +1)/count -1; //编号【3】关卡消耗减半，关卡1ID，关卡2ID，…
					m_energyMgr.decrease(cost);
				}
				break;
			case RES_CURRENCY_POWERPOINT:
				if(costList.list[i].cost > 0)
				{
					nPowerPoint = (costList.list[i].cost * times)/count;
					flag = true;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_PowerPoint_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unPowerPoint;
					logInfo.m_op_after = m_unPowerPoint - nPowerPoint;
					logVer.push_back(logInfo);
					//---------------------------------
				}
				break;
			case RES_CURRENCY_HONOUR:
				if(costList.list[i].cost > 0)
				{
					nHonour = (costList.list[i].cost * times)/count;
					flag = true;
					//-----log日志
					logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_honour_lower;
					//logInfo.m_goodsID = costList.list[i].unID;
					logInfo.m_op_before = m_unHonour;
					logInfo.m_op_after = m_unHonour - nHonour;
					logVer.push_back(logInfo);
					//---------------------------------
				}
				break;
			case RES_CURRENCY_FRIENDSHIP:
				{
					if (costList.list[i].cost > 0)
					{
						nFriendship = (costList.list[i].cost * times)/count;
						flag = true;
						//-----log日志
						logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_Friendship_lower;
						//logInfo.m_goodsID = costList.list[i].unID;
						logInfo.m_op_before = m_unFriendshipPoint;
						logInfo.m_op_after = m_unFriendshipPoint - nFriendship;
						logVer.push_back(logInfo);
						//---------------------------------
					}
				}break;
			case RES_CURRENCY_PVP_ENERGY:
				// 				{
				// 					m_pvpEnergyMgr.decrease((costList.list[i].cost-1) * times);
				// 				}
				break;
			case RES_CURRENCY_DYNAMIC_ENERGY:
				{
					if(costList.list[i].cost > 0)
					{
						nDynamicEnergy = costList.list[i].cost * times;
						//-----log日志
						logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_Dynamic_Energy_lower;
						//logInfo.m_goodsID = costList.list[i].unID;
						logInfo.m_op_before = m_unDynamicEnergy;
						logInfo.m_op_after = m_unDynamicEnergy - nDynamicEnergy;
						logVer.push_back(logInfo);
						//---------------------------------
						flag = true;
					}
				}break;
			case RES_CURRENCY_EXP:
				break;
			case RES_CURRENCY_RECHARGE:
				break;
			}
		}
		else
		{
			switch(type)
			{
			case ITEM_RES_TYPE_MATERIAL:			
				m_materialMgr.do_cost(costList.list[i], times, false,1);
				material_flag = true;
				break;			
			case ITEM_RES_TYPE_PROPS:
				m_propsMgr.do_cost(costList.list[i], times, false,1);
				props_flag = true;
				break;
			}
		}
	}


	if(flag)
	{
		set_basic_info(-(nCoin), -(nMoney), 0, -(nPowerPoint), -(nHonour), -(nFriendship),-nDynamicEnergy);
	}
	if(material_flag)
	{
		m_materialMgr.update_2_db();
	}
	if(props_flag)
	{
		m_propsMgr.update_2_db();
	}

	SendLogInfoItemMSG(logVer);//log日志
	return count;
}

void Map::set_eat_everyday_count( int count )
{
	if(count == get_eat_everyday_count())
		return;
	m_dwFlag &= ~(0x07 << 1);
	m_dwFlag |= (count << 1);

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]eat [count=%d]", m_unID, count);

	MsgDBUpdateCharacterFlagRequest db_request;
	db_request.unFlag = m_dwFlag;

	this->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_CHARACTER_FLAG, (char *)&db_request, sizeof(db_request));
}

int Map::get_eat_everyday_count()
{
	return (m_dwFlag >> 1) & 0x07;
}

void Map::set_sign_up_reward_sign( bool bHasSignUp )
{
	m_dwFlag &= ~(0x01 << 4);
	m_dwFlag |= (((ACE_UINT32)bHasSignUp) << 4);
}

bool Map::has_sign_up_reward()
{
	return (bool)((m_dwFlag >> 4) & 0x01);
}

void Map::set_vip_sign_up_reward_sign( bool bHasSignUp )
{
	m_dwFlag &= ~(0x01 << 5);
	m_dwFlag |= (((ACE_UINT32)bHasSignUp) << 5);
}

bool Map::has_vip_sign_up_reward()
{
	return (bool)((m_dwFlag >> 5) & 0x01);
}

void Map::set_sign_up_day_this_month( int dwDayinMonth )
{
	m_dwFlag &= ~(0x01f << 6);
	m_dwFlag |= (((ACE_UINT32)dwDayinMonth) << 6);
}

int Map::get_sign_up_day_this_month()
{
	return (int)((m_dwFlag >> 6) & 0x01f);
}

void Map::set_secret_paid_refresh_times(ACE_UINT32 unTimes)
{
	m_dwFlag &= ~(0x0F/**/ << 11);
	m_dwFlag |= unTimes << 11;
	set_secret_paid_refresh_price();
}

void Map::set_secret_paid_refresh_price()
{
	m_secretMgr.m_unPaidRefreshPrice = m_secretCfg.higher_moenyRefresh.unCount + m_secretCfg.unHigher_moneyRefresh_increase * get_secret_paid_refresh_times();
	m_secretMgr.m_unPaidRefreshPrice = m_secretCfg.unHigher_moneyRefresh_increaseUpperBound > m_secretMgr.m_unPaidRefreshPrice ? m_secretMgr.m_unPaidRefreshPrice : m_secretCfg.unHigher_moneyRefresh_increaseUpperBound;
}


int Map::get_secret_paid_refresh_times()
{
	return (m_dwFlag >> 11) & 0x0F/**/;
}

void Map::send_month_card()
{
	if (m_bitField.monthCard & 0x1)
	{
		if (m_monthTime > m_lastSendMonthTime)//最后登录时间
		{
			ACE_UINT32 nowTime = GameUtils::get_utc();
			if (nowTime > m_monthTime)
				nowTime = m_monthTime;

			ACE_UINT32 offsetday = GameUtils::cut_current_day_0_0(nowTime) - GameUtils::cut_current_day_0_0(m_lastSendMonthTime);
			
			offsetday = offsetday/(24*60*60);
			if (offsetday > 30)
				offsetday = 30;
			m_lastSendMonthTime = GameUtils::cut_current_day_0_0(m_lastSendMonthTime);
			MsgSendMonthCardRequest db_request;
			for (int i=0;i<offsetday;i++)
			{
				db_request.m_dwCharacterID = m_unID;
				db_request.m_unTime = m_lastSendMonthTime + (i+1)*(24*60*60);
				this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_SEND_MONTH_CARD, (char *)&db_request, 
					sizeof(db_request), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
			}
			m_lastSendMonthTime = GameUtils::get_utc();
		}
		else
		{
			m_bitField.monthCard = 0;
			Update_Bitfield();
		}
	}
}

void Map::send_friendship_mail(ACE_UINT32 unCharacterID, ACE_INT32 unAssistType, const char *strAssistName,ACE_UINT32 unMultEnergy)
{
	MsgAddFriendshipMailRequest db_request;
	db_request.m_dwCharacterID = unCharacterID;
	db_request.m_dwAddFriendship = unAssistType == 2 ? GAME_CONFIG_INSTANCE::instance()->m_unReturnFriendshipFriend: GAME_CONFIG_INSTANCE::instance()->m_unReturnFriendshipStranger;
	db_request.m_dwAddFriendship *= unMultEnergy;//多倍体力
	db_request.m_dwAssistFlag = unAssistType;
	memcpy(db_request.m_strName, strAssistName, MAX_CHARACTER_LENGTH+1);

	this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_ADD_FRIENDSHIP_MAIL_REQUEST, (char *)&db_request,
		sizeof(db_request), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
}

void Map::do_gift_bag_code_reward(RewardInfo &oRewardInfo, ACE_UINT32 unRewardID, int &iOffset)
{
	RewardRes *pReward = dynamic_cast<RewardRes *>(ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unRewardID));
	if (pReward != NULL)
	{
		oRewardInfo.unCoin += pReward->m_reward.unCoin;
		oRewardInfo.unCount += pReward->m_reward.unCount;
		oRewardInfo.unEnergy += pReward->m_reward.unEnergy;
		oRewardInfo.unExp += pReward->m_reward.unExp;
		oRewardInfo.unHonour += pReward->m_reward.unHonour;
		oRewardInfo.unMoney += pReward->m_reward.unMoney;
		oRewardInfo.unPowerPoint += pReward->m_reward.unPowerPoint;
		oRewardInfo.unPVPEnergy += pReward->m_reward.unPVPEnergy;
		oRewardInfo.unRecharge += pReward->m_reward.unRecharge;

		for(int i=0;pReward->m_reward.list[i].unCount != 0 && i+iOffset<MAX_CONFIG_REWARD_COUNT; ++i, ++iOffset)
		{
			oRewardInfo.list[iOffset].unCount = pReward->m_reward.list[i].unCount;
			oRewardInfo.list[iOffset].item.unResID = pReward->m_reward.list[i].unResID;
		}
	}
}

void Map::add_reward(RewardInfo &oRewardInfo, ACE_UINT32 unID, ACE_UINT32 unCount)
{
	switch (unID)
	{
	case 0:
		break;
	case RES_CURRENCY_COIN:
		{
			oRewardInfo.unCoin += unCount;
		}break;
	case RES_CURRENCY_MONEY:
		{
			oRewardInfo.unMoney += unCount;
		}break;
	case RES_CURRENCY_EXP:
		{
			oRewardInfo.unExp += unCount;
		}break;
	case RES_CURRENCY_FRIENDSHIP:
		{
			oRewardInfo.unFriendship += unCount;
		}break;
	case RES_CURRENCY_ENERGY:
		{
			oRewardInfo.unEnergy += unCount;
		}break;
	case RES_CURRENCY_RECHARGE:
		{
			oRewardInfo.unRecharge += unCount;
		}break;
	case RES_CURRENCY_HONOUR:
		{
			oRewardInfo.unHonour += unCount;
		}break;
	case RES_CURRENCY_POWERPOINT:
		{
			oRewardInfo.unPowerPoint += unCount;//body->m_aMailAttachment[i].m_dwItemCount;
		}break;
	case RES_CURRENCY_DYNAMIC_ENERGY:
		{
			oRewardInfo.unDynamicEnergy += unCount;
		}break;
		//其他货币类型以后需要时再加
	default:
		{
			//++oRewardInfo.unCount; //应该可以吧，策划应该不会同一个物品配两份吧
			for (int i=oRewardInfo.unCount;i<MAX_REWARD_ITEM;++i)
			{
				if (oRewardInfo.list[i].item.unResID == 0)
				{
					oRewardInfo.list[i].item.unResID = unID;//body->m_aMailAttachment[i].m_dwItemID;
					oRewardInfo.list[i].unCount = unCount;//body->m_aMailAttachment[i].m_dwItemCount;
					++oRewardInfo.unCount;//直接用oRewardInfo.unCount做下标访问不好
					break;
				}
			}	
		}break;
	}
}

void Map::SetRewardLogItem( const RewardInfo &reward,LogItemInfo & logInfo,std::vector<LogItemInfo> & ver )
{
	//(reward.unCoin, reward.unMoney, reward.unExp, reward.unPowerPoint, reward.unHonour, reward.unFriendship);
	if (reward.unMoney > 0)//金卡log
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_money_add;
		logInfo.m_op_before = m_unMoney;
		logInfo.m_op_after = m_unMoney + reward.unMoney;
		ver.push_back(logInfo);
	}
	if (reward.unCoin > 0)//贝壳log
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_coin_add;
		logInfo.m_op_before = m_unCoin;
		logInfo.m_op_after = m_unCoin + reward.unCoin;
		ver.push_back(logInfo);
	}
	if (reward.unExp > 0)//经验值log
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_exp_add;
		logInfo.m_op_before = m_unExp;
		logInfo.m_op_after = m_unExp + reward.unExp;
		ver.push_back(logInfo);
	}
	if (reward.unPowerPoint > 0)//合金log
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_PowerPoint_add;
		logInfo.m_op_before = m_unPowerPoint;
		logInfo.m_op_after = m_unPowerPoint + reward.unPowerPoint;
		ver.push_back(logInfo);
	}
	if (reward.unFriendship > 0)//友情点log
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_Friendship_add;
		logInfo.m_op_before = m_unFriendshipPoint;
		logInfo.m_op_after = m_unFriendshipPoint + reward.unFriendship;
		ver.push_back(logInfo);
	}
	if (reward.unHonour > 0)//经验值
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_honour_add;
		logInfo.m_op_before = m_unHonour;
		logInfo.m_op_after = m_unHonour + reward.unHonour;
		ver.push_back(logInfo);
	}
	if(reward.unEnergy > 0)//体力
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_energy_add;
		logInfo.m_op_before = m_energyMgr.m_unEnergy;
		logInfo.m_op_after = m_energyMgr.m_unEnergy + reward.unEnergy;
		ver.push_back(logInfo);
	}
	if(reward.unPVPEnergy > 0)//pvp体力
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_pvpenergy_add;
		//logInfo.m_op_before = m_pvpEnergyMgr.m_unEnergy;
		//logInfo.m_op_after = m_pvpEnergyMgr.m_unEnergy + reward.unPVPEnergy;
		ver.push_back(logInfo);
	}
	if (reward.unDynamicEnergy > 0)
	{
		logInfo.m_unType = enum_LOG_TYPE::LOG_TYPE_Dynamic_Energy_add;
		logInfo.m_op_before = m_unDynamicEnergy;
		logInfo.m_op_after = m_unDynamicEnergy + reward.unDynamicEnergy;
		ver.push_back(logInfo);
	}

	logInfo.m_op_before = 0;
	logInfo.m_op_after = 0;
}

void Map::SendLogInfoItemMSG( const std::vector<LogItemInfo> & ver )
{
	//发送日志信息
	MsgDBLogInfoRequest request;
	ZeroVar(request);
	int n=0;
	for (std::vector<LogItemInfo>::const_iterator c_it = ver.begin(); c_it!= ver.end();c_it++ )
	{
		request.logInfoItem[n] = *c_it;
		n++;
		if (n == MSG_DB_LOG_INFO_MAX_LEN)
		{
			request.unCount = n;
			this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DB_LOG_INFO_REQUEST, (char *)&request, 
				sizeof(MsgDBLogInfoRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
// 			this->sendMsg(SERVICE_MESSAGE_TYPE_DB_REQUEST, MSG_DB_LOG_INFO_REQUEST, (char *)&request, sizeof(MsgDBLogInfoRequest),
// 				OBJ_ID_NONE, ADAPTER_DB_USER_MGR, m_unID);
			ZeroVar(request);
			n=0;
		}
	}

	if (n > 0)
	{
		request.unCount = n;
		this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DB_LOG_INFO_REQUEST, (char *)&request, 
 			sizeof(MsgDBLogInfoRequest), m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
// 		this->sendMsg(SERVICE_MESSAGE_TYPE_DB_REQUEST, MSG_DB_LOG_INFO_REQUEST, (char *)&request, sizeof(MsgDBLogInfoRequest),
// 			OBJ_ID_NONE, ADAPTER_DB_USER_MGR, m_unID);
	}

}

void Map::SetlogUserInfo( LogItemInfo & logInfo )
{
	//-----------log日志
	//写入基础信息
	logInfo.m_unUserID = m_unUserID;
	logInfo.m_charactID = m_unID;
	ACE_OS::memcpy(logInfo.m_name, m_strName, MAX_CHARACTER_LENGTH + 1);
	logInfo.m_unLevel = m_unLevel;
	logInfo.m_unChannel = m_unChannel;//渠道
	logInfo.m_op_time = GameUtils::get_utc();//操作时间
	//------------------------------
}

void Map::ChickLuckStart( ACE_UINT32 tmp_drop_id )
{
	//tmp_drop_id = 0x26000001;
	//=============================================================================
	//xxj 2015/04/17  15:24:14 
	int _type = GET_ITEM_TYPE(tmp_drop_id);
	switch(_type)
	{
	case ITEM_RES_TYPE_DROP_CURRENCY://0x28 掉落货币
		break;
	case ITEM_RES_TYPE_DROP_MATERIAL://材料（装备碎片）
		{
			CDropMaterialRes *res = (CDropMaterialRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);//获取掉落配置资源
			if(res)//+所有材料
			{
				ACE_ASSERT((int)(res->m_unCount) > 0);
				_type = GET_ITEM_TYPE(res->m_list[0].unResID);
				if (_type == ITEM_RES_TYPE_EQUIPMENT)
				{
					EquipmentRes *res = (EquipmentRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);
					if(res)
					{
						if (res->m_unQuality >= 3)
						{
							GMInfo info;
							ZeroVar(info);
							info.unType = ADMIN_OP_GM_LUCK_EQUIPMENT;
							info.unGoodID = tmp_drop_id;
							info.unLevel = 0;
							AddGmMsg(info);
						}
					}
				}
				else if (_type == ITEM_RES_TYPE_PLAYER)
				{
					PlayerRes *res = (PlayerRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);
					if(res)
					{
						GMInfo info;
						ZeroVar(info);
						info.unType = ADMIN_OP_GM_LUCK_PLAYER;
						info.unGoodID = tmp_drop_id;
						info.unLevel = 0;
						AddGmMsg(info);
					}
				}
			}
		}
		break;
	case ITEM_RES_TYPE_EQUIPMENT://设备，装备
		{
			EquipmentRes *res = (EquipmentRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);
			if(res)
			{
				if (res->m_unQuality >= 3)
				{
					GMInfo info;
					ZeroVar(info);
					info.unType = ADMIN_OP_GM_LUCK_EQUIPMENT;
					info.unGoodID = tmp_drop_id;
					info.unLevel = 0;
					AddGmMsg(info);
				}
			}
		}
		break;
	case ITEM_RES_TYPE_PLAYER://角色
		{
			PlayerRes *res = (PlayerRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);
			if(res)
			{
				GMInfo info;
				ZeroVar(info);
				info.unType = ADMIN_OP_GM_LUCK_PLAYER;
				info.unGoodID = tmp_drop_id;
				info.unLevel = 0;
				AddGmMsg(info);
			}
		}break;
	case ITEM_RES_TYPE_PROPS:
		{
			PropsRes *res = (PropsRes*)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(tmp_drop_id);
			if(res)
			{
				GMInfo info;
				ZeroVar(info);
				info.unType = ADMIN_OP_GM_PROPS;
				info.unGoodID = tmp_drop_id;
				info.unLevel = 0;
				AddGmMsg(info);
			}
		}
		break;
	default:
		break;
	}
	//------------------------------------------------------------------------
}

void Map::AddGmMsg( GMInfo & info )
{
	info.unToID = 0;
	info.unFromID = m_unUserID;
	memcpy(info.fromName,m_strName,MAX_CHARACTER_LENGTH);
	MsgGMAddRequest request;
	request.gm = info;
	this->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_DB_GM_ADD_REQUEST, (char *)&request, sizeof(MsgGMAddRequest), this->m_unUserInfoServerID, ADAPTER_USER_INFO_MGR, OBJ_ID_NONE);
}

void Map::SendBuyEnergyToDB()
{
	MsgDBUpdateBuyEnergyRequest db_request;
	db_request.m_unCurrentCount = m_unCurrentBuyEnergy;
	add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_BUY_ENERGY_REQUEST, (char *)&db_request, sizeof(MsgDBUpdateBuyEnergyRequest));//更新数据库
}

void Map::Deal_recharge_op_result()
{
	if(m_rechargeOrderSN != 0)
	{
		MsgDoRechargeResponse response;
		ZeroVar(response);
		response.unSN = m_rechargeOrderSN;
		response.nResult = 0;
		memcpy(response.orderID,m_rechargeOrder.orderID,RECHARGE_ORDERID_LEN);
		response.unRenminbi = m_rechargeOrder.unRenminbi;
		response.unMoney = m_rechargeOrder.unMoney;
		response.unProductId = m_rechargeOrder.unProductId;
		response.unProductCount = m_rechargeOrder.unProductCount;
		response.unbitField = m_bitField.GetValue();
		response.unMoneyEx = m_rechargeOrder.unFlagEx;
		
		this->send_msg_2_client(SERVICE_MESSAGE_TYPE_USER, MSG_DO_RECHARGE_RESPONSE, (char *)&response, sizeof(MsgDoRechargeResponse));
		send_response_end(m_unUserID, response.unSN);

		m_rechargeOrderSN = 0;
		ZeroVar(m_rechargeOrder);
	}
}

void Map::UpdateProductList( ACE_UINT32 unProductID )
{
	if (m_Productlist.unCount < RECHARGE_MAX_PRODUCT)
	{
		for (int i=0;i<m_Productlist.unCount;i++)
		{
			if (m_Productlist.list[i] == unProductID)
				return;//已经有了 不必更新
		}

		//没有这个商品id 加入
		m_Productlist.list[m_Productlist.unCount] = unProductID;
		m_Productlist.unCount ++;
	}

}

void Map::Update_Bitfield()
{
	MsgDoBitFieldRequest request;
	request.bitfield = m_bitField.GetValue();
	request.unCharactID = m_unID;
	sendMsg(SERVICE_MESSAGE_TYPE_DB_REQUEST, MSG_Update_Bitfield, (char *)&request, sizeof(MsgDoBitFieldRequest),OBJ_ID_NONE,ADAPTER_DB_USER_MGR , m_unID);
}

int Map::SharedActivites(RewardInfo & reward)
{
	if (!(m_bitField.sharedActivites & 0x1))
	{
		BehaviorRes *res = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(FIRST_SHAREAD_ACTIVITE_ID);//开始首充奖励
		if (res)
		{
			//-----------log 日志
			//获取pvp奖励
			LogItemInfo logInfo;
			logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_shared;//动作原因
			logInfo.SetOp_Way(_LOG_OP_WAY_shared);//途径(附加信息)
			get_reward_from_config_reward(reward, res->m_reward, 1, 1);
			get_reward(reward,logInfo);

			m_bitField.sharedActivites = 1;
			Update_Bitfield();//更新

			return 0;
		}
		else
			return -1;
	}
	else
		return -1;
}

int Map::Badge_step_up( MsgDoUpBadgeGaintRequest * msg,bool isTuJian )//徽章升级
{
	int len = 0;
	ACE_INT32 unIdx[MsgEquipmentStepUp_unGrade_MAX];
	ZeroVar(unIdx);

	ItemInfo *itemInfo = NULL;
	CostInfoList cost;
	ZeroVar(cost);
	cost.unCount = 1;
	cost.list[0].unID =RES_CURRENCY_POWERPOINT;
	cost.list[0].cost = 0;
	ACE_UINT32 powerPoint = 0;

	if(isTuJian)
	{
		if (msg->unCount > 6)
			msg->unCount = 6;
	}
	
	m_badgeMgr.DoCharge_Add_Begin();
	for (int i=0;i<msg->unCount;i++)
	{
		powerPoint = 0;
		if (isTuJian)//装备图纸
		{
			ACE_UINT32 tuzhiID = msg->unIds[i] & 0x0FFFFFFF;
			for(int j=0;j<msg->unIndex[i];j++)
			{
				if (m_badgeMgr.DoCharge_Add(tuzhiID,powerPoint))//充能
				{
					cost.list[0].cost += powerPoint;
					if(!is_cost_enough(cost,1))
					{
						return ERROR_MSG_COST;
					}
				}
				else
				{
					return ERROR_MSG_RES;
				}
			}
		}
		else
		{
			unIdx[len] = msg->unIndex[i];
			len++;

			PackageCell *cell = m_packageMgr.get_cell(msg->unIndex[i]);
			if(cell == NULL)
			{
				return ERROR_MSG_OBJ_NULL;
			}

			itemInfo = &cell->item.item;
			if (m_badgeMgr.DoCharge_Add(itemInfo,powerPoint))//充能
			{
				cost.list[0].cost += powerPoint;
				if(!is_cost_enough(cost,1))
				{
					return ERROR_MSG_COST;
				}
			}
			else
			{
				return ERROR_MSG_RES;
			}
		}
	}

	if(isTuJian)
	{
		for (int i=0;i<msg->unCount;i++)
		{
			cost.list[cost.unCount].unID = msg->unIds[i];
			cost.list[cost.unCount].cost = msg->unIndex[i];
			cost.unCount++;
		}

		if(!is_cost_enough(cost,1))
		{
			return ERROR_MSG_COST;
		}
	}
	//-----------log日志
	//操作
	LogItemInfo logInfo;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_badge_up;//动作原因
	logInfo.SetOp_Way(_LOG_OP_WAY_badge_up);//途径(附加信息)
	do_cost(cost, 1,logInfo);
	m_badgeMgr.DoCharge_Add_After();

	m_packageMgr.delete_item_list(unIdx, len);

	return 0;
}

int Map::use_props( int id, RewardInfo& roReward )
{
	const PropsInfo* poPropsInfo = m_propsMgr.GetPropsInfoByID(id);
	if(poPropsInfo == NULL || poPropsInfo->unCount == 0)
		return ERROR_MSG_GIFT_NULL;

	if(m_packageMgr.is_full())
	{
		return ERROR_MSG_PKG_FULL;
	}

	PropsRes *res = (PropsRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(poPropsInfo->unItemID);

	if(res == NULL)
	{
		return ERROR_MSG_RES;
	}

	LOGGER_CLIENT_INSTANCE::instance()->log("[ID=0x%08x]use props,propsID=0x%08x", m_unID, poPropsInfo->unItemID);

	//use props
	switch(res->m_unSubType)
	{
	case 1:	// 被消耗类，参数(无)
		break;
	case 2:	// 回复体力类，参数(体力值)
		if(res->m_oParameters.size() != 1)
			return ERROR_MSG_RES;
		roReward.unEnergy += res->m_oParameters[0];
		break;
	case 3:	// buff类，参数(待定)
		break;
	case 4:	// 宝箱类，参数(behavior的ID)
		{
			if(res->m_oParameters.size() != 1)
				return ERROR_MSG_RES;
			BehaviorRes * pbehaviorRes = (BehaviorRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(res->m_oParameters[0]);
			if(!pbehaviorRes)
				return ERROR_MSG_RES;
			if(pbehaviorRes->m_nVipLevelRequired != 0 && pbehaviorRes->m_nVipLevelRequired != m_vipMgr.m_unVipLevel)
			{
				return ERROR_MSG_COST;
			}
			if(!is_cost_enough(pbehaviorRes->m_cost, 1))
				return ERROR_MSG_PROPS_COST;
			if(m_cooldownMgr.can_do(m_unCurrentTime, res->m_oParameters[0]) != 0)
				return ERROR_MSG_COST;
			m_cooldownMgr.do_once(m_unCurrentTime, res->m_oParameters[0]);
			LogItemInfo logInfo;
			do_cost(pbehaviorRes->m_cost, 1,logInfo);
			get_reward_from_config_reward(roReward, pbehaviorRes->m_reward, 1, 1);
		}
		break;
	}

	//----------log日志
	//使用道具
	LogItemInfo logInfo;
	logInfo.m_unType = LOG_TYPE_props;
	logInfo.m_goodsID = poPropsInfo->unItemID;
	logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_use_props;
	logInfo.SetOp_Way(_LOG_OP_WAY_use_props);

	CostInfo oCostInfo;
	ZeroVar(oCostInfo);
	oCostInfo.unID = poPropsInfo->unItemID;
	oCostInfo.cost = 1;//poPropsInfo->unCount;
	m_propsMgr.do_cost(oCostInfo, 1, true, 1);

	logInfo.m_op_after = 1;
	std::vector<LogItemInfo> ver;
	ver.push_back(logInfo);
	SendLogInfoItemMSG(ver);

	get_reward(roReward, logInfo);

	return 0;
}


