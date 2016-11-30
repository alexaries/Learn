/********************************************************************
    ����ʱ�䣺    2015/04/17  14:36:17 
    �ļ�����      DropCurrencyRes.h
    ���ߣ�        
    
    ����:		1.���ڿ���ʱ�����˵���Դ��Ϣ��ȡ������
				2.֧�ֿ���ʱ�������ͼ����
                
    ˵��:		1. 
                2. 
*********************************************************************/
#ifndef __DROPMATERIALRES_H_
#define __DROPMATERIALRES_H_
#include "BaseRes.h"

class CDropMaterialRes : public BaseRes
{
public:
	CDropMaterialRes(ACE_UINT32 unType);
	virtual ~CDropMaterialRes();

	virtual int parse_xml_node(xmlNodePtr node);
	virtual void process_res_id_by_name();

	ACE_UINT32 m_unCount;
	ConfigRewardInfo m_list[MAX_CONFIG_REWARD_COUNT];

private:
	void parse_reward(char *str);
	int Parse_info(ConfigRewardInfo &dest, char *str);
	
};

#endif