#include "MyQueue.h"
// 
// template<typename TYPE>
// TYPE CMyQueue<TYPE>::pop()
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//������
// 	if (!theQueue_.empty())
// 	{
// 		TYPE tmp = theQueue_.front();
// 		theQueue_.pop();
// 		return tmp;
// 	}
// 	else
// 		return TYPE();//�յķ���Ĭ��
// }
// 
// template<typename TYPE>
// void CMyQueue<TYPE>::push( const TYPE &val )
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//������
// 	theQueue_.push(val);
// }
// 
// template<typename TYPE>
// size_t CMyQueue<TYPE>::size()
// {
// 	//boost::mutex::scoped_lock oLock(m_oMutex);//������
// 	//	size_t size=theQueue_.size();
// 	return theQueue_.size();
// }
// 
// template<typename TYPE>
// bool CMyQueue<TYPE>::empty()
// {
// 	//boost::mutex::scoped_lock oLock(m_oMutex);//������
// 	return theQueue_.empty();
// }
// 
// template<typename TYPE>
// TYPE CMyQueue<TYPE>::Dequeue()
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//������
// 	while(theQueue_.empty())//����ѭ�����������ж�֮����wait()
// 	{
// 		m_cond.wait(m_oMutex);//��ԭ���Ե�unlock m_oMutex ������ȴ�
// 		//waitִ�����֮����Զ����ּ���
// 	}
// 
// 	assert(!theQueue_.empty());
// 
// 	TYPE tmp = theQueue_.front();
// 	theQueue_.pop();
// 	return tmp;
// }
// 
// template<typename TYPE>
// bool CMyQueue<TYPE>::Enqueue(const TYPE & e )
// {
// 	{
// 		boost::mutex::scoped_lock oLock(m_oMutex);//������
// 		theQueue_.push(e);
// 	}
// 
// 	m_cond.notify_one();//��ʾһ������
// }
