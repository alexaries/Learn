#include <iostream>  
#include "..\include\jwsmtp.h" 
#include "Mytools.h"

void printErrorInfo(int errorid)
{
	switch(errorid)
	{
	case  500:
		{
			std::cout<<"��ʶ�𣨴˴���Ҳ���������й�����"<<std::endl;
		}break; 
	case  501:
		{
			std::cout<<"������ʽ����"<<std::endl;
		}break; 
	case  502:
		{
			std::cout<<"�����ʵ�� "<<std::endl;
		}break; 

	case  503:
		{
			std::cout<<"�������������"<<std::endl;
		}break; 

	case  504:
		{
			std::cout<<"�����������ʵ��"<<std::endl;
		}break; 

	case  211:
		{
			std::cout<<"ϵͳ״̬��ϵͳ������Ӧ"<<std::endl;
		}break; 

	case  214:
		{
			std::cout<<"������Ϣ"<<std::endl;
		}break; 

	case  220:
		{
			std::cout<<" �������"<<std::endl;
		}break; 

	case  221:
		{
			std::cout<<"����رմ����ŵ�"<<std::endl;
		}break; 

	case  421:
		{
			std::cout<<"����δ�������رմ����ŵ���������ر�ʱ����Ӧ�������Ϊ���κ��������Ӧ��"<<std::endl;
		}break; 

	case  250:
		{
			std::cout<<"Ҫ����ʼ��������"<<std::endl;
		}break; 

	case  251:
		{
			std::cout<<"�û��Ǳ��أ���ת����"<<std::endl;
		}break; 

	case  450:
		{
			std::cout<<"Ҫ����ʼ�����δ��ɣ����䲻���ã����磬����æ��"<<std::endl;
		}break; 

	case  550:
		{
			std::cout<<"Ҫ����ʼ�����δ��ɣ����䲻���ã����磬����δ�ҵ����򲻿ɷ��ʣ�"<<std::endl;
		}break; 
	case  451:
		{
			std::cout<<"����Ҫ��Ĳ�������������г���  "<<std::endl;
		}break; 
	case  551:
		{
			std::cout<<"�û��Ǳ��أ��볢��"<<std::endl;
		}break; 
	case  452:
		{
			std::cout<<"ϵͳ�洢���㣬Ҫ��Ĳ���δִ��"<<std::endl;
		}break; 
	case  552:
		{
			std::cout<<"�����Ĵ洢���䣬Ҫ��Ĳ���δִ��"<<std::endl;
		}break; 
	case  553:
		{
			std::cout<<" �����������ã�Ҫ��Ĳ���δִ�У����������ʽ����"<<std::endl;
		}break; 
	case  354:
		{
			std::cout<<"��ʼ�ʼ����룬��.����"<<std::endl;
		}break; 
	case  554:
		{
			std::cout<<"����ʧ��"<<std::endl;
		}break; 
	case  535:
		{
			std::cout<<"�û���֤ʧ��  "<<std::endl;
		}break; 
	case  235:
		{
			std::cout<<"�û���֤�ɹ�"<<std::endl;
		}break; 
	case  334:
		{
			std::cout<<"�ȴ��û�������֤��"<<std::endl;
		}break; 
	default:
		{
			std::cout<<"δ֪����"<<std::endl;
		}break;
	}
}
//ע��utf8����
int main(int argc, char* argv[])  
{  
	CMytools tools;
	if (tools.ReadText())
	{
		jwsmtp::mailer mail(tools.m_verTOaddress.at(0).c_str()/*������*/
			,tools.FROMaddress.c_str()/*������*/
			,tools.Subject.c_str()/*"������д�ʼ�����"*/
			,tools.Message.c_str()
			,tools.server.c_str()
			,jwsmtp::mailer::SMTP_PORT, false);  

		//��Ӷ��������
		for (int i=1;i<tools.m_verTOaddress.size();i++)
		{
			mail.addrecipient(tools.m_verTOaddress.at(i).c_str());
		}
		// 	mail.addrecipient("testjwstmp@163.com");  
		// 	mail.addrecipient("testjwstmp@gmail.com");  
		// 	mail.addrecipient("testjwstmp@qq.com");  
		// 
		// 	//���HTML�ķ������ݣ������滻���캯���еġ�������д�ʼ����ݡ�  
		// 	mail.setmessageHTML(html);  

		//�������ԣ�163֧�ֵ�auth��֤��PLAINģʽ  
		mail.authtype(jwsmtp::mailer::PLAIN);  

		//����������֤�û�����ע��Ŷ����Ҫ��***@163.com���û���  
		mail.username(tools.FROMaddress.c_str());  
		//������������  
		mail.password(tools.FROPassword.c_str());
		mail.send(); // ���﷢���ʼ�����Ҫע����ǣ�������ͬ��ģʽŶ��
		std::string str = mail.response();
		int errorinfo = atoi(str.substr(0,3).c_str());
		printErrorInfo(errorinfo);

		std ::cout << mail.response() << std::endl;//���ﷵ���Ƿ�ɹ���250�������ʼ��ɹ�; 
	}

	Sleep(10000);
	
	//system("pause");  
	return 0;  
}  

/*
���Ϸ���Email�ķ�����

�ʼ����񷵻ش��뺬��  
500   ��ʽ���������ʶ�𣨴˴���Ҳ���������й�����  
501   ������ʽ����  
502   �����ʵ��  
503   �������������  
504   �����������ʵ��  
211   ϵͳ״̬��ϵͳ������Ӧ  
214   ������Ϣ  
220     �������  
221     ����رմ����ŵ�  
421     ����δ�������رմ����ŵ���������ر�ʱ����Ӧ�������Ϊ���κ��������Ӧ��  
250   Ҫ����ʼ��������  
251   �û��Ǳ��أ���ת����  
450   Ҫ����ʼ�����δ��ɣ����䲻���ã����磬����æ��  
550   Ҫ����ʼ�����δ��ɣ����䲻���ã����磬����δ�ҵ����򲻿ɷ��ʣ�  
451   ����Ҫ��Ĳ�������������г���  
551   �û��Ǳ��أ��볢��  
452   ϵͳ�洢���㣬Ҫ��Ĳ���δִ��  
552   �����Ĵ洢���䣬Ҫ��Ĳ���δִ��  
553   �����������ã�Ҫ��Ĳ���δִ�У����������ʽ����  
354   ��ʼ�ʼ����룬��.����  
554   ����ʧ��  
535   �û���֤ʧ��  
235   �û���֤�ɹ�  
334   �ȴ��û�������֤��
*/