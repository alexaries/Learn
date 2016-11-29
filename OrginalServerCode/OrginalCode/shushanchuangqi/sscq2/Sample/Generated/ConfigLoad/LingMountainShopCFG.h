﻿// ******************************************************************************************************/
// 这个是工具自动生成的表代码，这个里面的任何更改在下次生成之后会变得无效，请注意！<UTF-8 With BOM>
// 文件名：     SLingMountainShopCFG.h
// Copyright(c) 2012-2100 上海恺英网络科技有限公司，All rights reserved.
// ******************************************************************************************************/


#ifndef _CLINGMOUNTAINSHOPLOADER_H
#define _CLINGMOUNTAINSHOPLOADER_H

#include <string>
#include <vector>
#include "Platform.h"

using namespace std;

//struct definition for the excel table
struct SLingMountainShopCFG
{
	/**
	商品编号
	**/
	UINT32	_Id;

	/**
	物品ID
	**/
	UINT16	_ItemId;

	/**
	商品名称
	**/
	string	_Name;

	/**
	所属灵山
	**/
	UINT8	_Type;

	/**
	货币类型
	**/
	UINT16	_MoneyType;

	/**
	货币价格
	**/
	UINT16	_MoneyPrice;

	/**
	库存
	**/
	UINT16	_StoreNum;
}; // SLingMountainShopCFG define end

//data loader for the excel table
class CLingMountainShopLoader
{
public:
	std::vector<SLingMountainShopCFG*> _vecData;

	~CLingMountainShopLoader();
	bool LoadData(const std::string& strPath);
	void ClearData();
};
#endif

