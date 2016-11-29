﻿// ******************************************************************************************************/
// 这个是工具自动生成的表代码，这个里面的任何更改在下次生成之后会变得无效，请注意！<UTF-8 With BOM>
// 文件名：     SActCenterCFG.cpp
// Copyright(c) 2012-2100 上海恺英网络科技有限公司，All rights reserved.
// ******************************************************************************************************/


#include "ActCenterCFG.h"
#include "tinyxml.h"

CActCenterLoader::~CActCenterLoader()
{
	ClearData();
}

void CActCenterLoader::ClearData()
{
	for(size_t i=0; i<_vecData.size();++i)
	{
		SActCenterCFG *p = _vecData[i];
		delete p;
	}
	_vecData.clear();
}

bool CActCenterLoader::LoadData(const std::string& strPath)
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
		SActCenterCFG* pData = new SActCenterCFG;
		const char* pstr_ID = pDataElem->Attribute("ID");
		if(pstr_ID != NULL)
			pData->_ID = (UINT16)atoi(pstr_ID);
		else
			pData->_ID = 0;
		const char* pstr_Icon = pDataElem->Attribute("Icon");
		if(pstr_Icon != NULL)
			pData->_Icon = (UINT32)atoi(pstr_Icon);
		else
			pData->_Icon = 0;
		const char* pstr_moduleName = pDataElem->Attribute("moduleName");
		if(pstr_moduleName != NULL)
			pData->_moduleName = pstr_moduleName;
		else
			pData->_moduleName = "";
		const char* pstr_Type = pDataElem->Attribute("Type");
		if(pstr_Type != NULL)
			pData->_Type = (UINT8)atoi(pstr_Type);
		else
			pData->_Type = 0;
		const char* pstr_IconAppearID = pDataElem->Attribute("IconAppearID");
		if(pstr_IconAppearID != NULL)
			pData->_IconAppearID = (UINT16)atoi(pstr_IconAppearID);
		else
			pData->_IconAppearID = 0;
		_vecData.push_back(pData);
		pDataElem = pDataElem->NextSiblingElement();
	}
	doc.Clear();
	return true;
}


