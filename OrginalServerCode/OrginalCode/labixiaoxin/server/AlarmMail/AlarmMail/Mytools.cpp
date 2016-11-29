#include "Mytools.h"
#include <fstream> 
#include <iostream>
#include "..\include\json\MyJson.h"
#include <Windows.h>

CMytools::CMytools(void)
{
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
	char chpath[MAX_PATH];  
	GetModuleFileNameA(NULL,(LPSTR)chpath,sizeof(chpath));  
	std::string strPath(chpath);
	int loc = strPath.rfind('\\');
	if (loc != std::string::npos)
	{
		strPath = strPath.substr(0,loc+1);
	}
	strPath += "mailConfig.json";

	infile.open(strPath.c_str());  
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
		std::cout<<"����mailConfig.json�ļ�ʧ��"<<std::endl;
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
	FROMaddress = m_myjson.GetStrValue(json,"FROMaddress");
	FROPassword = m_myjson.GetStrValue(json,"FROPassword");
	Subject = m_myjson.GetStrValue(json,"Subject");
	Message = m_myjson.GetStrValue(json,"Message");
	server = m_myjson.GetStrValue(json,"server");

	for (int i=0;i<json["TOaddress"].size();i++)
	{
		m_verTOaddress.push_back(json["TOaddress"][i].asCString());
	}


	if (m_verTOaddress.size()<1)
		return false;
	else
		return true;
}
