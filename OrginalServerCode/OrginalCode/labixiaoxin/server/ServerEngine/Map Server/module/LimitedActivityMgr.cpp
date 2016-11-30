#include "LimitedActivityMgr.h"
#include "..\..\GameBase\res\GameConfig.h"
#include "..\Map.h"

void LimitedActivityMgr::init( MsgDBLimitedActivityList * list )
{
	m_mapCurrentLA.clear();//���������
	ACE_UINT32 now = GameUtils::get_utc();//��ȡ��ǰʱ��
	for (int i=0;i<list->m_unCount;i++)
	{
		if (list->m_LAList[i].unCloseTime > now)//��ʱ���δ����
			m_mapCurrentLA[list->m_LAList[i].unLAID] = list->m_LAList[i];
	}
	Refresh();//�������е�

	m_isHaveNew = false;
}

void LimitedActivityMgr::Refresh()
{
	RefreshOld();//ɾ���ϵ�
	RefreshNew();//����µ�
}

void LimitedActivityMgr::RefreshNew()
{
	ACE_UINT32 now = GameUtils::get_utc();//��ȡ��ǰʱ��
	const std::map<ACE_UINT32, LimitedActivityConfig> &refLAConfig = GAME_CONFIG_INSTANCE::instance()->m_olimitedActivityConfig;
	std::map<ACE_UINT32, LimitedActivityConfig>::const_iterator c_it = refLAConfig.begin();//��ǰ������б�
	while(c_it != refLAConfig.end())
	{
		if(c_it->second.unStartTime <= now && c_it->second.unCloseTime > now)//����û���� �ҵ���
		{
			if (m_mapCurrentLA[c_it->first].unLAID == 0)
			{
				//m_mapCurrentLA[c_it->first].unCharacterID = m_map->m_unID;
				m_mapCurrentLA[c_it->first].unLAID = c_it->second.unID;
				m_mapCurrentLA[c_it->first].unCloseTime = c_it->second.unCloseTime;
				m_mapCurrentLA[c_it->first].unLinkType = c_it->second.unRelatedType;
				m_mapCurrentLA[c_it->first].unProgress = 0;
				m_mapCurrentLA[c_it->first].unRelatedCount = 0;
			}
		}
		c_it++;
	}
	ACE_ASSERT(m_mapCurrentLA.size()<= MAX_LIMITED_ACTIVITY_COUNT);
}

void LimitedActivityMgr::RefreshOld()
{
	ACE_INT32 now = GameUtils::get_utc();//��ȡ��ǰʱ��
	std::map<ACE_UINT32, LimitedActivityInfo>::iterator it = m_mapCurrentLA.begin();//��ǰ������б�
	while(it != m_mapCurrentLA.end())
	{
		if (it->second.unCloseTime < now)
			it = m_mapCurrentLA.erase(it);
		else
			it++;
	}
}

void LimitedActivityMgr::update_2_db()
{
	RefreshOld();//ɾ���ϵ����� �����ݻ�δ���عʲ���Ҫ�洢
	MsgDBLimitedActivityList db_request;
	int count =0;
	std::map<ACE_UINT32, LimitedActivityInfo>::const_iterator c_it = m_mapCurrentLA.begin();//��ǰ������б�
	while(c_it != m_mapCurrentLA.end() && count < MAX_LIMITED_ACTIVITY_COUNT )
	{
		db_request.m_LAList[count] = c_it->second;
		c_it++;
		count++;
	}

	db_request.m_unCount = count;
	m_map->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_LA_PROGRESS_RESPONSE, (char *)&db_request, sizeof(MsgDBLimitedActivityList));
}

void LimitedActivityMgr::update_2_db( const LimitedActivityInfo & laconfig )
{
	LimitedActivityInfo db_request = laconfig;
	db_request.unDealTime = GameUtils::get_utc();
	m_map->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_LA_PROGRESS_RESPONSE1, (char *)&db_request, sizeof(LimitedActivityInfo));
}

int LimitedActivityMgr::GetReward( ACE_UINT32 unLAMissionID, RewardInfo &oRewardInfo )
{
	ACE_INT32 unProgress = 0;
	LimitedActivityInfo * pLa = NULL;
	if (ChickCanGetReward(unLAMissionID,unProgress,pLa))//��ڷ�Χ��
	{
		//�鿴�Ƿ����
		for (int i = 0;i<pLa->unRelatedCount;i++)
		{
			if (pLa->unRelatedItems[i] == unLAMissionID)//�Ѿ��������
				return ERROR_MSG_GOT;
		}
		//-----------------------end
		const std::map<ACE_UINT32, LimitedActivityMissionConfig> &refLAConfig = GAME_CONFIG_INSTANCE::instance()->m_olimitedActivityMissionConfig;
		std::map<ACE_UINT32, LimitedActivityMissionConfig>::const_iterator c_it = refLAConfig.find(unLAMissionID);
		if (c_it != refLAConfig.end())
		{
			if(unProgress >= c_it->second.unMissionType[2])//�Ѿ��ﵽ���Ⱦ�����
			{
				if(pLa->unRelatedCount<MAX_RelatedT_ITEMS)
				{
					pLa->unRelatedItems[pLa->unRelatedCount] = unLAMissionID;
					pLa->unRelatedCount++;
					update_2_db(*pLa);//���µ����ݿ�
				}
				else
					return ERROR_MSG_MAX;

				m_map->get_reward_from_config_reward(oRewardInfo,c_it->second.stReward,1,1);
				//----------log��־
				//��ȡ��ʱ�
				LogItemInfo logInfo;
				logInfo.m_goodsID = unLAMissionID;//װ��id
				logInfo.m_op_action = enum_LOG_ACTION::LOG_ACTION_Limited_Activity;//����ԭ��
				logInfo.SetOp_Way(_LOG_OP_WAY_Limited_Activity);//;��(������Ϣ)
				m_map->get_reward(oRewardInfo,logInfo);//������ݵ����ݿ�
				return 0;
			}
		}
	}

	return ERROR_MSG_ACTIVE_GOT;
}


int LimitedActivityMgr::GetLACurrentList( MsgGMLimitedActivityListResponse & response )
{
	Refresh();
	response.m_unCount = 0;
	std::map<ACE_UINT32, LimitedActivityInfo>::const_iterator c_it = m_mapCurrentLA.begin();//��ǰ������б�
	while(c_it != m_mapCurrentLA.end())
	{
		response.m_LAList[response.m_unCount] = c_it->second;
		response.m_unCount++;
		if (response.m_unCount >= MAX_LIMITED_ACTIVITY_COUNT)
			break;
		c_it++;
	}

	return 0;
}

bool LimitedActivityMgr::ChickCanGetReward( ACE_UINT32 unLAMissionID,ACE_INT32 & unProgress,LimitedActivityInfo *& pLa )
{
	unProgress = 0;
	//���ж��ܲ�����ȡ
	Refresh();
	const std::map<ACE_UINT32, LimitedActivityConfig> &refLAConfig = GAME_CONFIG_INSTANCE::instance()->m_olimitedActivityConfig;
	std::map<ACE_UINT32, LimitedActivityConfig>::const_iterator c_it = refLAConfig.begin();
	while(c_it != refLAConfig.end())
	{
		for (int i=0;i<c_it->second.unRelatedCount;i++)
		{
			if (c_it->second.unRelatedItems[i] == unLAMissionID)//�ҵ���
			{
				std::map<ACE_UINT32, LimitedActivityInfo>::iterator _c_it = m_mapCurrentLA.find(c_it->first);//��ǰ������б�
				if (_c_it != m_mapCurrentLA.end())//��ʱ�䷶Χ�� û���ж��Ⱦ���������getreward���ж�
				{
					unProgress = _c_it->second.unProgress;
					pLa = &_c_it->second;
					return true;
				}
			}
		}
		c_it++;
	}
	//------------------------end

	return false;
}

LimitedActivityMissionConfig LimitedActivityMgr::FindLAmissionItem( ACE_UINT32 unLAMissionID )
{
	LimitedActivityMissionConfig lamConfig;
	ZeroVar(lamConfig);
	const std::map<ACE_UINT32, LimitedActivityMissionConfig> &refLAConfig = GAME_CONFIG_INSTANCE::instance()->m_olimitedActivityMissionConfig;
	std::map<ACE_UINT32, LimitedActivityMissionConfig>::const_iterator c_it = refLAConfig.find(unLAMissionID);
	if (c_it != refLAConfig.end())
		lamConfig = c_it->second;

	return lamConfig;
}

LimitedActivityConfig LimitedActivityMgr::FindLAItem( ACE_UINT32 unLAID )
{
	LimitedActivityConfig laConfig;
	ZeroVar(laConfig);
	const std::map<ACE_UINT32, LimitedActivityConfig> &refLAConfig = GAME_CONFIG_INSTANCE::instance()->m_olimitedActivityConfig;
	std::map<ACE_UINT32, LimitedActivityConfig>::const_iterator c_it = refLAConfig.find(unLAID);
	if (c_it != refLAConfig.end())
		laConfig = c_it->second;

	return laConfig;
}

bool LimitedActivityMgr::ChickTimeIsInActivity( LimitedActivityConfig & laconfig )
{
	//ʱ���ж�
	ACE_INT32 now = GameUtils::get_utc();
	if(laconfig.unStartTime <= now && laconfig.unEndTime > now)//��ʱ�����
	{
		int week = GameUtils::get_week(now);
		if((laconfig.unWeekCircle & (1 << week)) != 0)//��ѭ��ͨ��
		{
			ACE_INT32 day = GameUtils::Get_current_day_0_0();//��ȡ������賿���
			ACE_INT32 unDayStart = day + laconfig.unDayTimeStart;//һ��Ŀ�ʼʱ��
			ACE_INT32 unDayEnd = day + laconfig.unDayTimeEnd;//һ��Ľ���ʱ��
			if(unDayStart<=now && unDayEnd>=now)
				return true;
		}
	}

	return false;
}

bool LimitedActivityMgr::DoLATypeLinkcost( ACE_UINT32 unLAMissionType,CostInfoList & costList/*,int times*/)
{
	//LA_TYPE_LINK= 0,
	//��š�0�����������࣬����1ID������2ID
	return DoActionFindByType(LA_TYPE_LINK,unLAMissionType,costList,/*times,*/&LimitedActivityMgr::CallFun_Cost);
}

bool LimitedActivityMgr::DoLATypeLuck( ACE_UINT32 unLAMissionType,const CostInfoList & costList,ACE_UINT32 unMissionType/*,int times*/ )
{
	//LA_TYPE_LINK= 0,
	//��š�0�����������࣬����1ID������2ID
	unMissionType = 1;
	for(int i = 0;i < costList.unCount;i ++)
	{
		switch(costList.list[i].unID)
		{
		case RES_CURRENCY_MONEY:
			{
				unMissionType =2;
			}break;
		case RES_CURRENCY_FRIENDSHIP:
			{
				unMissionType = 1;
			}break;
		}
	}
	return DoActionFindByType(LA_TYPE_LINK,unLAMissionType,unMissionType,/*times,*/&LimitedActivityMgr::CallFun_Luck);
}

bool LimitedActivityMgr::DoLATypeSecret( ACE_UINT32 unLAMissionType,CostInfoList & costList/*,int times*/ )
{
	//��š�1�������̵��ۿ������࣬�������ͣ��ۿ���
	return DoActionFindByType(LA_TYPE_1,unLAMissionType,costList,/*times,*/&LimitedActivityMgr::CallFun_Secret);
}

bool LimitedActivityMgr::DoLATypeMapCount( ACE_UINT32 unMapID,ACE_UINT32 type,ACE_UINT32 & nMult )
{
	//��š�2���ؿ�����˫�����ؿ�1ID���ؿ�2ID����
	return DoActionFindByType(type,unMapID,nMult,/*times,*/&LimitedActivityMgr::CallFun_MapCount);
}

bool LimitedActivityMgr::DoLATypeLeveUP( ACE_UINT32 unLAtype,ACE_UINT32 & nMult )
{
	return DoActionFindByType(unLAtype,0,nMult,/*times,*/&LimitedActivityMgr::CallFun_LeveUp);
}

bool LimitedActivityMgr::DoLATypeLinkRecharge( ACE_UINT32 unLAMissionType,ACE_UINT32 nGold/*,int times*/ )
{
	return DoActionFindByType(LA_TYPE_LINK,0,nGold,/*times,*/&LimitedActivityMgr::CallFun_Recharge);
}

bool LimitedActivityMgr::DoLATypeLinkRechargeGift(ACE_UINT32 unProductId )
{
	const RechargeRes * res = (const RechargeRes *)ITEM_RES_MGR_INSTANCE::instance()->get_item_res(unProductId);
	if (res)
	{
		if (res->m_unSubType == RechargeRes_type_LIMITED_ACTIVITY)//����ʱ�
		{
			return DoActionFindByType(LA_TYPE_11,0,unProductId,/*times,*/&LimitedActivityMgr::callFun_RechargeGift);

			// 			Refresh();//�ȸ���
			// 			std::map<ACE_UINT32, LimitedActivityInfo>::iterator it = m_mapCurrentLA.begin();//��ǰ������б�
			// 			while(it != m_mapCurrentLA.end())
			// 			{
			// 				if (it->second.unLinkType == LA_TYPE_11)
			// 				{
			// 					LimitedActivityConfig laconfig = FindLAItem(it->second.unLAID);
			// 					for (int i=0;i<laconfig.unRelatedCount;i++)
			// 					{
			// 						if (laconfig.unRelatedItems[i] == unProductId)//�ҵ��� ������ȡ
			// 						{
			// 							ACE_UINT32 unCurrentUTC = GameUtils::get_utc();
			// 							int dwDayOffset = GameUtils::get_day_different(unCurrentUTC, it->second.unDealTime);
			// 							if ( dwDayOffset > 0)
			// 							{
			// 								it->second.unRelatedCount = 0;
			// 								ZeroVar(it->second.unRelatedItems);
			// 							}	
			// 						
			// 							//�жϽ����Ƿ��Ѿ���ȡ��
			// 							bool isGet =false;
			// 							for(int j=0;j<it->second.unRelatedCount;j++)
			// 							{
			// 								if (it->second.unRelatedItems[j] == unProductId)//�Ѿ���ȡ��
			// 								{
			// 									isGet = true;
			// 									break;
			// 								}
			// 							}
			// 
			// 							if (!isGet && it->second.unRelatedCount < MAX_RelatedT_ITEMS)//��û����ȡ��
			// 							{
			// 								it->second.unRelatedItems[it->second.unRelatedCount] = unProductId;
			// 								it->second.unRelatedCount ++;
			// 							}
			// 
			// 
			// 							it->second.unDealTime = unCurrentUTC;
			// 							update_2_db(it->second);
			// 							return true;
			// 						}
			// 					}
			// 				}
			// 				it++;
			// 			}
		}
	}

	return false;
}

bool LimitedActivityMgr::DoLATypeLogin()
{
	ACE_UINT32 unLAMissionType = LA_MISSION_TYPE_1005;
	ACE_UINT32 unTimes = 1;//����
	return DoActionFindByType(LA_TYPE_LINK,unLAMissionType,unTimes,/*times,*/&LimitedActivityMgr::callFun_Login);
}

bool LimitedActivityMgr::CallFun_Cost( ACE_INT32 unLAMissionType,LimitedActivityInfo & info,CostInfoList & costList /*,int times*/)
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			for (int i=0;i<laconfig.unRelatedCount;i++)
			{
				LimitedActivityMissionConfig lamConfig = FindLAmissionItem(laconfig.unRelatedItems[i]);
				if (lamConfig.unID > 0 && lamConfig.unMissionType[0] == unLAMissionType)//�ҵ��� ��Ծ��
				{
					for(int j = 0;j < costList.unCount;j ++)
					{
						if (costList.list[j].unID == lamConfig.unMissionType[1])
						{
							info.unProgress += costList.list[j].cost/**times*/;//��Ʒid
							update_2_db(info);//���µ����ݿ�
							isDown(info.unLAID);
							return true;
						}
					}
				}
			}
		}
		//---------------------
	}
	return false;
}

bool LimitedActivityMgr::CallFun_Luck( ACE_INT32 unLAMissionType,LimitedActivityInfo & info,ACE_UINT32 & unMissionType/*,int times*/ )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			for (int i=0;i<laconfig.unRelatedCount;i++)
			{
				LimitedActivityMissionConfig lamConfig = FindLAmissionItem(laconfig.unRelatedItems[i]);
				if (lamConfig.unID > 0 && lamConfig.unMissionType[0] == unLAMissionType)//�ҵ��� ��Ծ��
				{
					if (unMissionType == lamConfig.unMissionType[1])
					{
						info.unProgress += 1;//����
						update_2_db(info);//���µ����ݿ�
						isDown(info.unLAID);
						return true;
					}
				}
			}
		}
		//---------------------
	}
	return false;
}


bool LimitedActivityMgr::CallFun_Secret( ACE_INT32 unLAMissionType,LimitedActivityInfo & info,CostInfoList & costList/*,int times*/ )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			ACE_ASSERT(laconfig.unRelatedCount == 2);//0�ۿ����� 1�ۿ���
			for(int i=0;i<costList.unCount;i++)
			{
				if (costList.list[i].unID == laconfig.unRelatedItems[0])//�ҵ����ۿ���Ʒ
				{
					//�����ۿ�
					costList.list[i].cost *= (double)laconfig.unRelatedItems[1]*0.01;
					return true;
				}
			}
		}
		//---------------------
	}
	return false;
}


bool LimitedActivityMgr::CallFun_MapCount( ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & nMult/*,int times*/ )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			for(int i=0;i<laconfig.unRelatedCount;i++)
			{
				if (unMapID == laconfig.unRelatedItems[i])//�ҵ����ۿ���Ʒ
				{
					//�����ۿ�
					nMult = 2;
					return true;
				}
			}
		}
		//---------------------
	}
	return false;
}

bool LimitedActivityMgr::CallFun_LeveUp( ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & nMult/*,int times*/ )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			nMult = laconfig.unRelatedItems[1];//�����ۿ�
			return true;
		}
		//---------------------
	}
	return false;
}

bool LimitedActivityMgr::CallFun_Recharge( ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & nGold/*,int times*/ )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			info.unProgress += nGold;//��������
			update_2_db(info);//���µ����ݿ�
			isDown(info.unLAID);
			return true;
		}
		//---------------------
	}
	return false;
}

void LimitedActivityMgr::Cost_basic_info( int nCoin, int nMoney, int nExp, int nPowerPoint, int nHonour, int nFriendship )
{
	CostInfoList costList;
	ZeroVar(costList);
	int c = 0;
	if (nCoin < 0)//���ı���
	{
		costList.list[c].unID = RES_CURRENCY_COIN;
		costList.list[c].cost = -nCoin;
		c++;
	}

	if (nMoney < 0)//������
	{
		costList.list[c].unID = RES_CURRENCY_MONEY;
		costList.list[c].cost = -nMoney;
		c++;
	}

	if (nExp < 0)//exp����
	{
		costList.list[c].unID = RES_CURRENCY_EXP;
		costList.list[c].cost = -nExp;
		c++;
	}

	if (nPowerPoint < 0)//�Ͻ�
	{
		costList.list[c].unID = RES_CURRENCY_POWERPOINT;
		costList.list[c].cost = -nPowerPoint;
		c++;
	}

	if (nHonour < 0)//����ֵ
	{
		costList.list[c].unID = RES_CURRENCY_HONOUR;
		costList.list[c].cost = -nHonour;
		c++;
	}

	if (nFriendship < 0)//�����
	{
		costList.list[c].unID = RES_CURRENCY_FRIENDSHIP;
		costList.list[c].cost = -nFriendship;
		c++;
	}

	costList.unCount = c;
	if (c>0)
	{
		DoLATypeLinkcost(LA_MISSION_TYPE_1002, costList);
	}
}

int LimitedActivityMgr::ChickLARecharge( const RechargeRes * res )
{
	if (!res)
		return ERROR_MSG_RES;

	if (res->m_unSubType == RechargeRes_type_LIMITED_ACTIVITY)//����ʱ�
	{
		Refresh();//�ȸ���
		std::map<ACE_UINT32, LimitedActivityInfo>::const_iterator c_it = m_mapCurrentLA.begin();//��ǰ������б�
		while(c_it != m_mapCurrentLA.end())
		{
			if (c_it->second.unLinkType == LA_TYPE_11)
			{
				LimitedActivityConfig laconfig = FindLAItem(c_it->second.unLAID);
				if (ChickTimeIsInActivity(laconfig))//ʱ����
				{
					for (int i=0;i<laconfig.unRelatedCount;i++)
					{
						if (laconfig.unRelatedItems[i] == res->m_unResID)//�ҵ��� ������ȡ
						{
							ACE_UINT32 unCurrentUTC = GameUtils::get_utc();
							int dwDayOffset = GameUtils::get_day_different(unCurrentUTC, c_it->second.unDealTime);
							if ( dwDayOffset > 0)
								return 0;
							else//�жϽ����Ƿ��Ѿ���ȡ��
							{
								bool isGet =false;
								for(int j=0;j<c_it->second.unRelatedCount;j++)
								{
									if (c_it->second.unRelatedItems[j] == res->m_unResID)//�Ѿ���ȡ��
									{
										isGet = true;
										break;
									}
								}

								if (!isGet)//��û����ȡ��
									return 0;
								else
									return ERROR_MSG_GOT;
							}

						}
					}
				}
			}
			c_it++;
		}

		return ERROR_MSG_TIME_OUT;
	}
	else
		return 0;

}


bool LimitedActivityMgr::callFun_RechargeGift( ACE_INT32 unMapID,LimitedActivityInfo & info,ACE_UINT32 & unProductId )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);
	if (ChickTimeIsInActivity(laconfig))//ʱ����
	{
		for (int i=0;i<laconfig.unRelatedCount;i++)
		{
			if (laconfig.unRelatedItems[i] == unProductId)//�ҵ��� ������ȡ
			{
				ACE_UINT32 unCurrentUTC = GameUtils::get_utc();
				int dwDayOffset = GameUtils::get_day_different(unCurrentUTC, info.unDealTime);
				if ( dwDayOffset > 0)
				{
					info.unRelatedCount = 0;
					ZeroVar(info.unRelatedItems);
				}	

				//�жϽ����Ƿ��Ѿ���ȡ��
				bool isGet =false;
				for(int j=0;j<info.unRelatedCount;j++)
				{
					if (info.unRelatedItems[j] == unProductId)//�Ѿ���ȡ��
					{
						isGet = true;
						break;
					}
				}

				if (!isGet && info.unRelatedCount < MAX_RelatedT_ITEMS)//��û����ȡ��
				{
					info.unRelatedItems[info.unRelatedCount] = unProductId;
					info.unRelatedCount ++;
				}


				info.unDealTime = unCurrentUTC;
				update_2_db(info);
				return true;
			}
		}
	}

	return false;
}

bool LimitedActivityMgr::callFun_Login( ACE_INT32 unLAMissionType,LimitedActivityInfo & info,ACE_UINT32 & unTimes )
{
	LimitedActivityConfig laconfig = FindLAItem(info.unLAID);//����limitedactivity.xml
	if (laconfig.unID > 0)
	{
		if (ChickTimeIsInActivity(laconfig))//ʱ����
		{
			for (int i=0;i<laconfig.unRelatedCount;i++)
			{
				LimitedActivityMissionConfig lamConfig = FindLAmissionItem(laconfig.unRelatedItems[i]);//����limitedactivitymission.xml
				if (lamConfig.unID > 0 && lamConfig.unMissionType[0] == unLAMissionType)//�ҵ��� ��Ծ��1005
				{
					ACE_UINT32 unNow = GameUtils::get_utc();
					ACE_UINT32 offsetday = GameUtils::cut_current_day_0_0(unNow) - GameUtils::cut_current_day_0_0(info.unDealTime);
					offsetday = offsetday/(24*60*60);

					if (offsetday != 0)//����ͬ���¼
					{
						if (offsetday > 1)
							info.unProgress = 1;
						else
							info.unProgress++;
						info.unDealTime = unNow;//�������ʱ��
						update_2_db(info);
						isDown(info.unLAID);
					}

					return true;
				}
			}
		}
		//---------------------
	}
	return false;
}

void LimitedActivityMgr::isDown( ACE_UINT32 unLAMissionID )
{
	ACE_INT32 unProgress = 0;
	LimitedActivityInfo * pLa = NULL;
	if (ChickCanGetReward(unLAMissionID,unProgress,pLa))//��ڷ�Χ��
	{
		//�鿴�Ƿ����
		for (int i = 0;i<pLa->unRelatedCount;i++)
		{
			if (pLa->unRelatedItems[i] == unLAMissionID)//�Ѿ��������
				return;
		}
		//-----------------------end

		const std::map<ACE_UINT32, LimitedActivityMissionConfig> &refLAConfig = GAME_CONFIG_INSTANCE::instance()->m_olimitedActivityMissionConfig;
		std::map<ACE_UINT32, LimitedActivityMissionConfig>::const_iterator c_it = refLAConfig.find(unLAMissionID);
		if (c_it != refLAConfig.end())
		{
			if(unProgress >= c_it->second.unMissionType[2])//�Ѿ��ﵽ���Ⱦ�����
			{
				if(pLa->unRelatedCount<MAX_RelatedT_ITEMS)
				{
					m_isHaveNew = true;
				}
				else
					return;
			}
		}
	}

	return;
}

