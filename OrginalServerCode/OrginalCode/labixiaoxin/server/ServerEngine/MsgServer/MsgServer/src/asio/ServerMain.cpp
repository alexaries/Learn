#include "ServerMain.h"
#include <boost\date_time\posix_time\time_formatters.hpp>
#include <boost\foreach.hpp>
#include "ServerApp.h"
#include "..\tools\Mytools.h"
#include "..\data\ClientList.h"


void CServerMain::StartServer()
{
	if (tools.ReadText())
	{
		try
		{
			boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), tools.m_serverPort);//�����õ���inet4
			//�նˣ����Կ���sockaddr_in����ɺ󣬾�Ҫaccept��

			CserverApp sa(m_ioService,endPoint);

			std::vector<boost::shared_ptr<boost::thread> > _listThread;//����io�߳�
			//����10����(io)�߳�
			for (int i = 0; i < MAX_IO_THREAD; ++i)
			{
				boost::shared_ptr<boost::thread> pTh(new boost::thread(
					boost::bind(&boost::asio::io_service::run,&m_ioService)));
				_listThread.push_back(pTh);
			}

			m_ioService.run();//�����շ��߼�

			//���������߳�
			BOOST_FOREACH(const boost::shared_ptr<boost::thread> & it,_listThread)
			{
				it->join();//�ȴ��߳̽���
			}
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

void CServerMain::OnStop()
{
	CClientList::instance()->OnStop();

	m_ioService.stop();//ֹͣ
	m_mainThrd.join();
	CClientList::instance()->OnClear();
}

void CServerMain::OnStart()
{
	m_mainThrd = boost::thread(boost::bind(&CServerMain::StartServer,this));//������ͻ��������߳�
	m_mainThrd.yield();//�Զ�����

	CClientList::instance()->OnStart();
}

