#include <iostream>
#include <boost\bind.hpp>
#include <boost\algorithm\string.hpp>
#include "..\src\tools\TimerDeal\TimerDeal.h"
#include "..\src\asio\ServerMain.h"


#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>  
#include <boost\algorithm\string.hpp>
#include "..\src\asio\ServerApp.h"
#include "..\src\data\ServerConnection.h"
#include "..\src\tools\TimerDeal\TimerDeal.h"
#include "..\src\tools\MyIni.h"
#include <boost\date_time\posix_time\time_formatters.hpp>
#include "..\src\tools\Mytools.h"
#include "..\src\tools\dmp\MyDump.h"

CMyIni tools;
boost::asio::io_service myIoService;

void StartServer()
{
	if (tools.ReadText())
	{
		try
		{
			boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), tools.m_serverPort);//�����õ���inet4
			//�նˣ����Կ���sockaddr_in����ɺ󣬾�Ҫaccept��

			CserverApp sa(myIoService,endPoint);
			myIoService.run();//�����շ��߼�
		}
		catch (std::exception & e)
		{
			std::vector<std::string> ver;
			ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) +"\t"+e.what());
			MyTools::PrintToFile("MsgLog/error.txt",ver,false);
			std::cout << "error��" <<e.what()<< std::endl;//���������
		}
	}
} 



int main(int argc, char *argv[])
{
	MEMORY_CHICK;//�ڴ���

	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);//���ûص�����

	//new int[1024];

	CServerMain::instance()->OnStart();//������ͻ��������߳�

	//��ʼ��ʱ��
	CTimerDeal::instance()->OnStart();

	std::cout<<"start gm !\n";

	std::string str;
	while(1)
	{	
		std::cin>>str;

		if (boost::iequals(str,"quit"))
			break;

// 		std::map<std::string,int>::const_iterator c_it = tools.m_opMap.begin();
// 		while(c_it != tools.m_opMap.end())
// 		{
// 			if (boost::iequals(str,c_it->first))
// 			{
// 				CServerConnectionData::instance()->SendAllMessage(str);
// 				break;
// 			}
// 			c_it++;
// 		}
	}

	//������ʱ��
	CTimerDeal::instance()->OnStop();

	CServerMain::instance()->OnStop();//�ر�����IO

	system("pause");
	return 0;
}
