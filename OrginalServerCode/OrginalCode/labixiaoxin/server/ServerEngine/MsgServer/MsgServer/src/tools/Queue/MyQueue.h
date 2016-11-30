/********************************************************************
	����ʱ�䣺	2014/08/15  16:26:54 
	�ļ�����		MyQueue.h
	���ߣ�		лС��
	
	���ܣ�		1. �̰߳�ȫ����
				2.
				
	˵����	    1. ���߳�״̬�µİ�ȫ����
				2. 
*********************************************************************/
#ifndef MYQUEUE_H_
#define MYQUEUE_H_
#include <queue>//std queue
#include <vector>
//#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

template<typename TYPE>
class CMyQueue
{
private:
	std::queue<TYPE> theQueue_;
	boost::mutex m_oMutex;
	boost::condition_variable_any m_cond;//��������
public:
	CMyQueue(){};
	~CMyQueue(){};
public:
	void Dequeue(std::vector<TYPE> & ver);
	TYPE Dequeue();//���������õ�һ����Ϣ
	void Enqueue(const TYPE & e);//����һ��Ԫ��
protected:
// 	TYPE pop();//��һ������
// 	void push( const TYPE &val );//ѹ��һ������
	size_t size();//���г���
	bool empty();//�п�
};


template<typename TYPE>
size_t CMyQueue<TYPE>::size()
{
	//boost::mutex::scoped_lock oLock(m_oMutex);//������
	//	size_t size=theQueue_.size();
	return theQueue_.size();
}

template<typename TYPE>
bool CMyQueue<TYPE>::empty()
{
	//boost::mutex::scoped_lock oLock(m_oMutex);//������
	return theQueue_.empty();
}

template<typename TYPE>
TYPE CMyQueue<TYPE>::Dequeue()
{
	boost::mutex::scoped_lock oLock(m_oMutex);//������
	while(theQueue_.empty())//����ѭ�����������ж�֮����wait()
	{
		m_cond.wait(m_oMutex);//��ԭ���Ե�unlock m_oMutex ������ȴ�
		//waitִ�����֮����Զ����ּ���
	}

	assert(!theQueue_.empty());

	TYPE tmp = theQueue_.front();
	theQueue_.pop();
	return tmp;
}

template<typename TYPE>
void CMyQueue<TYPE>::Dequeue( std::vector<TYPE> & ver )
{
	boost::mutex::scoped_lock oLock(m_oMutex);//������
	while(theQueue_.empty())//����ѭ�����������ж�֮����wait()
	{
		m_cond.wait(m_oMutex);//��ԭ���Ե�unlock m_oMutex ������ȴ�
		//waitִ�����֮����Զ����ּ���
	}

	assert(!theQueue_.empty());

	ver.clear();
	int n =0 ;
	while(!theQueue_.empty() && n < MAX_SEND_COUNT)
	{
		ver.push_back(theQueue_.front());
		theQueue_.pop();
		++n;
	}
}

template<typename TYPE>
void CMyQueue<TYPE>::Enqueue(const TYPE & e )
{
	{
		boost::mutex::scoped_lock oLock(m_oMutex);//������
		theQueue_.push(e);
	}

	m_cond.notify_one();//��ʾһ������
}

#endif