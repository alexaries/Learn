#include "ConfigLoader.h"

ConfigLoader::ConfigLoader()
{

}

ConfigLoader::~ConfigLoader()
{

}

int ConfigLoader::load_config(int config_id, char *path)
{
	xmlDocPtr doc;           //��������ĵ�ָ��
	xmlNodePtr rootNode;      //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�)

	xmlKeepBlanksDefault(0);

	doc = xmlReadFile(path,"UTF-8",XML_PARSE_RECOVER);

	if (NULL == doc)
	{
		//�ĵ��򿪴���
		return -1;
	}

	rootNode = xmlDocGetRootElement(doc); //ȷ���ĵ���Ԫ��
	/*���ȷ�ϵ�ǰ�ĵ��а�������*/
	if (NULL == rootNode)
	{
		//�յ�xml�ļ�
		xmlFreeDoc(doc);
		return -1;
	}

	int nRet = deal_config(config_id, rootNode);

	xmlFreeDoc(doc);

	return nRet;
}