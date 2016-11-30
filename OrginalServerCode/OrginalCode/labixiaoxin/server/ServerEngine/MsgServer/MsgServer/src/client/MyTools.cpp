#include "stdAfx.h"
#include "MyTools.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>

#define   FOLDER_NAME  "TXT"

std::string MyTools::g_strServerIP = "" ;

std::string GetModulePath( std::string folder,std::string fname)
{
	//std::string path = boost::filesystem::initial_path<boost::filesystem::path>().string();
// 	HMODULE module = GetModuleHandle(0); 
// 	CHAR buf[MAX_PATH]; 
// 	GetModuleFileName(module, buf, sizeof buf); 
	CString strPath;
	::GetModuleFileName( NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	std::string path = strPath.GetBuffer();
	strPath.ReleaseBuffer();
	path.append("\\");
	path+=folder;
	if(!boost::filesystem::exists(path))
	{
		boost::filesystem::create_directory(path);
	}

	if (folder!="")
		path+="/";

	path+=fname;//��ʼ��path

	return path;
}

void MyTools::PrintToFile( std::string fname,std::vector<std::string> & prints,bool isClear)
{
	setlocale(LC_ALL, "chs");
	std::string path=GetModulePath(FOLDER_NAME,fname);
	//path = "\\\\QGFPVZ3ZLRZTTEH\\iPad\\TXT\\"+ fname;
	namespace newfs = boost::filesystem;
	newfs::ofstream ofs(path.c_str(),(isClear?(std::ios_base::out|std::ios::trunc):(std::ios_base::app|std::ios::ate)));
	if (ofs.is_open())//�������
	{
		std::vector<std::string>::const_iterator it = prints.begin();
		while(it != prints.end())
		{
			ofs<<*it<<std::endl;
			it++;
		}
		ofs.close();
	}
	else//û�д�
		assert(false);
}

bool MyTools::ReadFromFile( std::string fname,std::vector<std::string> & reads )
{
	reads.clear();//�����Ԫ��

	setlocale(LC_ALL, "chs");
	std::string path=fname;//GetModulePath(FOLDER_NAME,fname);
	namespace newfs = boost::filesystem;
	newfs::ifstream ifs(path.c_str(),std::ios::in);
	std::string str;
	if (ifs.is_open())//�������
	{
		while(!ifs.eof())
		{
			std::getline(ifs,str);
			//if (str.length() >0)//����ȡ����ȡ
			reads.push_back(str);
		}
		ifs.close();

		return true;
	}
	else//û�д�
		return false;

}

std::string MyTools::GetStringConfig( const char * tag )
{
	char ConfigFile[256] = {0};
	WIN32_FIND_DATA FindFileData;
	GetCurrentDirectory(256,ConfigFile);
	strcat(ConfigFile,"\\Config.ini");

	if(FindFirstFile(ConfigFile,&FindFileData) == INVALID_HANDLE_VALUE )//�ļ��Ƿ����
	{
		printf("Can't Find Config.in!\n");
		return "";
	}

	char buf[256]={0};
	memset(buf,0,256);
	GetPrivateProfileString("SETUP",tag,"",buf,256,ConfigFile);//�õ����´�������
	g_strServerIP = std::string(buf);
	return g_strServerIP;
}

void MyTools::SetStringConfig( const char * tag,std::string str )
{
	g_strServerIP = str;

	char ConfigFile[256] = {0};
	WIN32_FIND_DATA FindFileData;
	GetCurrentDirectory(256,ConfigFile);
	strcat(ConfigFile,"\\Config.ini");

	WritePrivateProfileString("SETUP",tag,str.c_str(),ConfigFile);//��������
}
