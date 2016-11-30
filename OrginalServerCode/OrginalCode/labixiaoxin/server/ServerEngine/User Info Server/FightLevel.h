/********************************************************************
    ����ʱ�䣺    2015/09/08  15:02:25 
    �ļ�����      PlayerLevel.h
    ���ߣ�        xxj
    
    ����:		1.��������˺����а�
				2.
                
    ˵��:		1. 
                2. 
*********************************************************************/

#pragma once
#include "..\ace\Basic_Types.h"
#include "..\GameBase\Common\GameDefine.h"

class CFightLevel
{
public:
	CFightLevel();
	~CFightLevel();
public:
	void Update(const FightPlayerInfoBase & info);

	ACE_UINT32 m_unCount;
	FightPlayerInfoBase m_list[MAX_HONOUR_LEVEL_COUNT + 1];//��Ԥ��һ����������//
private:
	void InsertionSort(const FightPlayerInfoBase & info);
};