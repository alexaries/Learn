#pragma once
#include "ExampleSvr.h"

class CExampleS : public NExample::CExampleSvr
{
public:
	//���գ�C->Sʾ������1
	virtual void OnRecv_SampleFunc1(
		INT8 byParam1, //8λ�з�������
		INT16 wParam2, //16λ�з�������
		INT32 dwParam3, //32λ�з�������
		INT64 qwParam4, //64λ�з�������
		UINT8 byParam5, //8λ�޷�������
		UINT16 wParam6, //16λ�޷�������
		UINT32 dwParam7, //32λ�޷�������
		UINT64 qwParam8, //64λ�޷�������
		bool bParam9, //����ֵ
		float fParam10, //�����ȸ�����
		double dParam11, //˫���ȸ�����
		const std::string& strParam12, //�ַ���
		const std::string& strParam13, //AS�ֽ�����
		NExample::EExTest eTest14, //ö��ֵ
		const NExample::TVecExDescend4& vecSesc15 //�̳нṹ���б�
		);

	//���գ�C->S�з���ֵ��ʾ������
	virtual bool OnRecv_SampleRetFunc2(
		UINT8 byParam1, //����1��������
		UINT64 qwParam2, //����2������
		shared_func<SSampleRetFunc2Ack>& fnAck //���ػص�����
		);
};