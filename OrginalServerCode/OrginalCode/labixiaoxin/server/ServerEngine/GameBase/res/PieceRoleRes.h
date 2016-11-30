#ifndef __PIECEROLERES_H_
#define __PIECEROLERES_H_
#include "BaseRes.h"

class PieceRoleRes : public BaseRes
{
public:
	PieceRoleRes(ACE_UINT32 unType);
	virtual ~PieceRoleRes();

	virtual int parse_xml_node(xmlNodePtr node);
	
	CostInfoList m_cost;//������Դ����������������Դ��ID�Ͷ�Ӧ���������á�,��������
	ACE_UINT32 m_piece_id;//��ƬID����������Ƭ��ID-��6����Ʒ
	ACE_UINT32 m_use_amount;//��������������ת����ɫ���������
	ACE_UINT32 m_character_id;//Ŀ���ɫID������ת������Ի�õĻ���ID
};

#endif