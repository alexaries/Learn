#include "SecretMgr.h"
#include "..\Map.h"
#include "..\..\GameBase\res\GameConfig.h"

SecretMgr::SecretMgr()
{
	m_unCellCount = 0;//��ǰӵ�и�����
	m_refreshTimes = 0;
}

SecretMgr::~SecretMgr()
{

}



void SecretMgr::init(ACE_UINT32 unCellCount,ACE_UINT32 refreshTimes,ACE_UINT32 lastRefreshTime,SecretItemInfo *list)//���Ӵ�����ˢ�´������ϴ�ˢ��ʱ�䣬��Ʒ�б� 
{
//	ACE_OS::memcpy(m_list, list, sizeof(SecretItemInfo) * unCellCount);MAX_SECRET_LIST
		ACE_OS::memcpy(m_list, list, sizeof(SecretItemInfo) * MAX_SECRET_LIST);
	m_unCellCount =  unCellCount;
	m_refreshTimes = refreshTimes;
	m_unLastRefreshTime = lastRefreshTime;

}

void SecretMgr::send_info(ObjAdr &targetAdr, ACE_UINT32 unSN,int result) //��һ��ˢ�µ�ʱ���͹�ȥǰҪ��ˢ����,resutl���������̵�ˢ�µ�ʱ��Ҫ�úͿ�����ҲҪ�ã���һ�η���Ĭ��0
{    //���о���ʱ�䵽�˵��û��򿪵�ʱ���Զ�ˢ��һ��  ����Ҫ�Ӹ���ǰ̨�㿪ʱ��ʱ�䵽�˷��͸���Ϣ����ҲҪ���ص�

	MsgSecretListResponse *response = (MsgSecretListResponse *)public_send_buff;
	ACE_UINT32 curTime = GameUtils::get_utc();//��ǰʱ���
	Secret tmpCfg = GAME_CONFIG_INSTANCE::instance()->m_secretCfg;
	ACE_OS::memcpy(response->list, m_list, sizeof(SecretItemInfo) * m_unCellCount);	
	response->unRefreshTimes = m_refreshTimes;
	response->unPaidRefreshPrice = m_unPaidRefreshPrice;
	int tmpTime = m_unLastRefreshTime +tmpCfg.unRefreshSec - curTime;
	response->unRemainingTime =   tmpTime > 0 ? tmpTime:0;//ʱ�䵽�˷���0��û������ʣ��ʱ��
	response->unSN = unSN;
	response->unCell = m_unCellCount;	
	response->nResult = result;
	int size = sizeof(MsgSecretListResponse) + sizeof(SecretItemInfo) * m_unCellCount;
	m_map->sendMsg(SERVICE_MESSAGE_TYPE_USER, MSG_SECRET_LIST_RESPONSE, (char *)response, size, targetAdr);

}



void SecretMgr::update_2_db()
{
	MsgDBUpdateSecretListRequest *db_request = (MsgDBUpdateSecretListRequest *)public_send_buff;
	ACE_OS::memcpy(db_request->list, m_list, sizeof(SecretItemInfo) * m_unCellCount);
	db_request->unLastRefreshTime = m_unLastRefreshTime;
	db_request->unRefreshTimes = m_refreshTimes;
	db_request->unCellCount = m_unCellCount;
	
	int size = sizeof(MsgDBUpdateSecretListRequest) + m_unCellCount * sizeof(SecretItemInfo);
	m_map->add_db_request(ADAPTER_DB_HOME_MGR, MSG_DB_UPDATE_SECRET_LIST, (char *)db_request, size);
}