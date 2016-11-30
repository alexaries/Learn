#include "Mytools.h"
#include <fstream> 
#include <iostream>
#include "..\json\MyJson.h"
#include "..\data\IGameBase.h"

CMytools::CMytools(void)
{
	m_serverPort = 0;
}

CMytools::~CMytools(void)
{
}

bool CMytools::ReadText()
{
	//����Ǹ��ܴ�Ķ��д洢���ı����ļ�������ô����  
	char buf[1024];                //��ʱ�����ȡ�������ļ�����  
	std::string str;  
	std::ifstream infile;  
	infile.open("config.json");  
	if(infile.is_open())          //�ļ��򿪳ɹ�,˵������д�������  
	{  
		while(infile.good() && !infile.eof())  
		{  
			memset(buf,0,1024);  
			infile.getline(buf,1024); 
			str.append(buf); 
			str.append("\n");
		}  
		infile.close();  

		return ParseJson(str);
	}  
	else
	{
		std::cout<<"����config.json�ļ�ʧ��"<<std::endl;
		std::cin.get();
		return false;
	}
}

bool CMytools::ParseJson( const std::string & str )
{
	//m_verTOaddress.clear();
	//std::vector<std::string>().swap(m_verTOaddress);
	CMyJson m_myjson;
	CMyJson::JsonRoot json;
	m_myjson.InitJson(str,json);
	m_serverIP = m_myjson.GetStrValue(json,"serverIP");
	m_serverPort = m_myjson.GetValue<int>(json,"serverPort");
	for (unsigned i=0;i<json["opKey"].size();i++)
	{
		m_opMap[json["opKey"][i].asString()] = json["opvalue"][i].asInt();
	}
	m_title = m_myjson.GetStrValue(json,"title");;
	m_exestart = m_myjson.GetStrValue(json,"exestart");;

	IGameBase::isLog = m_myjson.GetValue<bool>(json,"isLog");

	return true;
}
