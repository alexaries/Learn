#include "BasicPropertyRes.h"
#include "ItemResMgr.h"

BasicPropertyRes::BasicPropertyRes(ACE_UINT32 unType)
:BaseRes(unType)
{
	ACE_OS::memset(m_valueList, 0, sizeof(ACE_UINT32) * PLAYER_PROPERTY_MAX);
}

BasicPropertyRes::~BasicPropertyRes()
{

}

int BasicPropertyRes::parse_xml_node(xmlNodePtr node)
{
	BaseRes::parse_xml_node(node);

	if(xmlStrcasecmp(node->name, BAD_CAST"attribute") == 0)
	{
		GameUtils::parse_property_value_list(m_valueList, (char *)xmlNodeGetContent(node));
	}
	
	return 0;
}

void BasicPropertyRes::process_res_id_by_name()
{
}