/********************************************************************
    ����ʱ�䣺    2015/04/17  14:36:17 
    �ļ�����      DropCurrencyRes.h
    ���ߣ�        
    
    ����:		1.���ڿ���ʱ�����˵���Դ��Ϣ��ȡ������
				2.֧�ֿ���ʱ�������ͼ����
                
    ˵��:		1. 
                2. 
*********************************************************************/
#ifndef __DROPCURRENCY_H_
#define __DROPCURRENCY_H_
#include "RewardRes.h"

class CDropCurrencyRes : public RewardRes
{
public:
	CDropCurrencyRes(ACE_UINT32 unType);
	virtual ~CDropCurrencyRes();

	virtual int parse_xml_node(xmlNodePtr node);
	virtual void process_res_id_by_name();

	//ACE_UINT32 m_unCountDown;
private:
	
};

#endif