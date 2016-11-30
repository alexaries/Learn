/********************************************************************
	����ʱ�䣺	2015/04/24  23:58:33
	�ļ�����	ClientSession.h
	���ߣ�		лС��
	
	���ܣ�		1. ��Ҫ���ڸ��ͻ��˽���
				2.һ������һ��ʵ��
				3.��Ҫ���������������ݶ�дʹ�ã������������ƣ�ճ�����������ݻ���
				
	˵����	    1. 
				2. 
*********************************************************************/
#ifndef __CLIENTSESSION_H_
#define __CLIENTSESSION_H_
#include <iostream>
#include <boost/enable_shared_from_this.hpp>//�ɽ��а�ȫ��share_ptrת��
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../data/IGameBase.h"
#include "MyAsioTools.h"
#include "../data/IModelBase.h"

#define max_len 1024

class ClientSession :public boost::enable_shared_from_this<ClientSession>,public IGameBase,public IModelBase
{
public:
	ClientSession(boost::asio::io_service & ioservice);
	~ClientSession();
public:
	boost::asio::ip::tcp::socket& Socket(){ return m_socket;}
	void CloseSocket();
	void Start();//��ʼ������Ϣ
	void WriteSomeSomething(const boost::scoped_array<char> & str,MY_UINT32 len);//дһЩ������app
	void WriteResult(int nResult);//���Ͳ��������Ϣ
	void ReadSomeSomething();//��ȡ����
private:
	void handle_write(const boost::system::error_code& error);//д��ɻص�
	void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);//����ɻص�
	void _DoTry(std::size_t bytes_transferred);
private:
	boost::asio::ip::tcp::socket m_socket;
	char _data[max_len];
	CMyAsioTools m_myAsioTools;
};

#endif // !__CLIENTSESSION_H_
