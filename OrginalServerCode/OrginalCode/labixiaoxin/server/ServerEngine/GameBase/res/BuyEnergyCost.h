#ifndef __BUYENERGYCOST_H_
#define __BUYENERGYCOST_H_
#include "BaseRes.h"
typedef struct tagBuyEnergyCost
{
	tagBuyEnergyCost()
	{
		buy_count = 0;
		cost = 0;
		Energy_Recover = 0;
	}
	ACE_UINT32 buy_count;//ʹ�ô���
	ACE_UINT32 cost;//������ �����������������������"
	ACE_UINT32 Energy_Recover;//ÿ�λָ���������
}BuyEnergyCost;

class BuyEnergyCostRes
{
public:
	BuyEnergyCostRes();
	virtual ~BuyEnergyCostRes();
	void AddOne(const BuyEnergyCost & cost);
	BuyEnergyCost GetOne(ACE_UINT32 unSetp);
private:
	std::map<ACE_UINT32 , BuyEnergyCost> m_mapBuyEnergyCost;
	BuyEnergyCost m_maxBuy;
};

#endif