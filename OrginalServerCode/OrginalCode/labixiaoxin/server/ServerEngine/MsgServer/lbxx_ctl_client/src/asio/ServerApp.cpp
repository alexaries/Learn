#include "ServerApp.h"
#include "..\data\ServerConnection.h"

CserverApp::CserverApp(boost::asio::io_service& ioservice, boost::asio::ip::tcp::endpoint& endpoint)
	:m_ioservice(ioservice),
	acceptor_(ioservice, endpoint/*,false*/)//false ���ö˿ڲ��ɸ���
{
	std::cout<<"strat!"<<std::endl;
	std::cout<<"listen [ip:port]:"<<endpoint<<std::endl;
	OnAccept();
}

CserverApp::~CserverApp()
{

}

void CserverApp::Handle_accept(const boost::system::error_code& error, Session_ptr& session)
{
	if (!error)
	{
		session->OnConnect();//֪ͨ����
		//ʵ�ֶ�ÿ���ͻ��˵����ݴ���
		session->Start();

		OnAccept();

		CServerConnectionData::instance()->AddOneConnection(session);
	}
}

void CserverApp::OnAccept()
{
	//ÿһ��session����һ���ͻ���
	Session_ptr new_session(new CMyClient(m_ioservice));
	acceptor_.async_accept(new_session->Socket(),
		boost::bind(&CserverApp::Handle_accept, this, boost::asio::placeholders::error,
		new_session));
}


