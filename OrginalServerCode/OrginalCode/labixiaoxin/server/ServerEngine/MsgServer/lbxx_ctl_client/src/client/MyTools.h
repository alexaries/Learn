#ifndef MYTOOLS_H_
#define MYTOOLS_H_
#include <boost\algorithm\string.hpp>
#include <vector>

//ȫ�ֺ��� ��ʼ�� ����
class MyTools
{
public:
  /*
  fname:��ǰ��ִ��·�����ļ�����dataĿ¼���ļ���
  prints��Ҫ��ӡ��ֵ��һ��Ԫ�أ�std::string������Ҫ��ӡ��һ������
  isClear:�Ƿ��������
  */ 
  static void PrintToFile(std::string fname,std::vector<std::string> & prints,bool isClear=true);//��ӡ���

  /*
  fname:��ǰ��ִ��·�����ļ�����dataĿ¼���ļ���
  reads�������洢��λ�ã�һ���ļ����ݴ浽һ��boost::string��
  */
  static bool ReadFromFile(std::string fname,std::vector<std::string> & reads);//��������
  
public://ini �����ļ����
	static std::string g_strServerIP;
	
	static std::string GetStringConfig(const char * tag);

	static void SetStringConfig(const char * tag,std::string str);
};

#endif

