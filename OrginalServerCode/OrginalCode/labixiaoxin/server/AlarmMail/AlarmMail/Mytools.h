#pragma once
#include <string>
#include <vector>

class CMytools
{
public:
	CMytools(void);
	~CMytools(void);
public:
	bool ReadText();
	bool ParseJson(const std::string & str);

	std::string FROMaddress;//������
	std::string FROPassword;//����
	std::string Subject;//�ʼ�����
	std::string Message;//�ʼ�����
	std::string server;//�����ʼ�������
	std::vector<std::string> m_verTOaddress;//������
};
