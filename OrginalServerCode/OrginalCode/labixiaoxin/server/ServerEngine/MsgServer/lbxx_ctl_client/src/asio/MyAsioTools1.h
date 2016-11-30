#ifndef __MYASIOTOOLS_H_
#define __MYASIOTOOLS_H_
#include <string>
#include <vector>

#define MAX_RESERVE_SIZE 1024*8  //�����ճߴ�
#define MAX_HEAD_LEN 5
class CMyAsioTools
{
public:
	CMyAsioTools();
public:
	std::string Serializing(const std::string & str);//���л�
	std::vector<std::string> UnSerializing(const char * data, int size);//�����л�
private:
	char _Data[2*MAX_RESERVE_SIZE];
	int _len;
};
#endif