﻿// ******************************************************************************************************/
// 这个是工具自动生成的表代码，这个里面的任何更改在下次生成之后会变得无效，请注意！<UTF-8 With BOM>
// 文件名：     SPetCFG.cpp
// Copyright(c) 2012-2100 上海恺英网络科技有限公司，All rights reserved.
// ******************************************************************************************************/


#include "PetCFG.h"
#include "tinyxml.h"

CPetLoader::~CPetLoader()
{
	ClearData();
}

void CPetLoader::ClearData()
{
	for(size_t i=0; i<_vecData.size();++i)
	{
		SPetCFG *p = _vecData[i];
		delete p;
	}
	_vecData.clear();
}

bool CPetLoader::LoadData(const std::string& strPath)
{
	ClearData();
	TiXmlDocument doc;
	if(!doc.LoadFile(strPath.c_str(), TIXML_ENCODING_UTF8))
	{
		doc.Clear();
		return false;
	}
	TiXmlElement* pElemRoot = doc.RootElement();
	if(pElemRoot == NULL)
	{
		doc.Clear();
		return false;
	}
	TiXmlElement* pDataElem = pElemRoot->FirstChildElement("Row");
	while(pDataElem != NULL)
	{
		SPetCFG* pData = new SPetCFG;
		const char* pstr_ID = pDataElem->Attribute("ID");
		if(pstr_ID != NULL)
			pData->_ID = (UINT16)atoi(pstr_ID);
		else
			pData->_ID = 0;
		const char* pstr_Name = pDataElem->Attribute("Name");
		if(pstr_Name != NULL)
			pData->_Name = pstr_Name;
		else
			pData->_Name = "";
		const char* pstr_Unlock = pDataElem->Attribute("Unlock");
		if(pstr_Unlock != NULL)
			pData->_Unlock = (UINT32)atoi(pstr_Unlock);
		else
			pData->_Unlock = 0;
		const char* pstr_FighterProp = pDataElem->Attribute("FighterProp");
		if(pstr_FighterProp != NULL)
			pData->_FighterProp = (UINT32)atoi(pstr_FighterProp);
		else
			pData->_FighterProp = 0;
		const char* pstr_Distance = pDataElem->Attribute("Distance");
		if(pstr_Distance != NULL)
			pData->_Distance = (UINT16)atoi(pstr_Distance);
		else
			pData->_Distance = 0;
		_vecData.push_back(pData);
		pDataElem = pDataElem->NextSiblingElement();
	}
	doc.Clear();
	return true;
}


