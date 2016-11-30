#pragma once

#include "ace\ace.h"

/*
#define SERVICE_MCAST_ADDR "232.1.1.1"
#define SERVICE_MCAST_PORT 1000
#define SERVICE_DAEMON_PORT 1001
#define SERVICE_MGR_LISTEN_PORT 1005

#define LOGIN_PORT 1500
#define GATEWAY_PORT 1510
#define PHP_PROXY_PORT 1520
#define USER_INFO_PORT 1530

#define TEST_WORLD_MANAGER_PORT 2001
#define TEST_LOGIN_SERVER_PORT  3001
#define TEST_GATEWAY_PORT 4001
#define TEST_DB_PORT 5001
#define TEST_MAP_SERVER_PORT 7001
#define TEST_PHP_PROXY_SERVER_PORT 8001
#define TEST_USER_INFO_SERVER_PORT 9001

#define TEST_SERVICE_ADDRESS "192.168.1.43"

enum
{
	SERVICE_TYPE_SERVICE_MGR = 0x01,
	SERVICE_TYPE_SERVICE_DAEMON,
	SERVICE_TYPE_SERVICE_LOGIN,
	SERVICE_TYPE_SERVICE_GATEWAY,
	SERVICE_TYPE_SERVICE_DB,
	SERVICE_TYPE_SERVICE_MAP,
	SERVICE_TYPE_SERVICE_PHP_PROXY,
	SERVICE_TYPE_SERVICE_USER_INFO
};

*/

#define GET_SERVICE_TYPE(s) ((s >> 24) & 0xff)
#define MAKE_SERVICE_ID(t, i) ((t << 24) | i)

#define SERVICE_ID_NONE 0x00

/*
#define SERVICE_MANAGER_ID ((SERVICE_TYPE_SERVICE_MGR << 24) | 0x00000000)
#define GAME_SERVER_ID SERVICE_MANAGER_ID
#define SERVICE_DB_ID ((SERVICE_TYPE_SERVICE_DB << 24) | 0x00000001)
#define TEST_LOGIN_SERVER_ID ((SERVICE_TYPE_SERVICE_LOGIN << 24) | 0x00000001)
#define TEST_GATEWAY_ID ((SERVICE_TYPE_SERVICE_GATEWAY << 24) | 0x00000001)
#define TEST_MAP_SERVICE_ID ((SERVICE_TYPE_SERVICE_MAP << 24) | 0x00000001)
#define TEST_PHP_PROXY_SERVICE_ID ((SERVICE_TYPE_SERVICE_PHP_PROXY << 24) | 0x00000001)
#define TEST_USER_INFO_SERVICE_ID ((SERVICE_TYPE_SERVICE_USER_INFO << 24) | 0x00000001)

*/

//#define TEST_WORLD_MANAGER_ID ((SERVICE_TYPE_WORLD_MGR << 24) | 0x00000001)


enum
{
	ADAPTER_GAME_SERVER_MGR = 1,
	ADAPTER_MAP_MGR,
	ADAPTER_GATEWAY_MGR,
	ADAPTER_LOGIN_MGR,
	ADAPTER_DB_HOME_MGR,
	ADAPTER_DB_LOGIN_MGR,
	ADAPTER_DB_USER_MGR,
	ADAPTER_DB_FRIEND_INFO_MGR,
	ADAPTER_PHP_PROXY_MGR,
	ADAPTER_USER_INFO_MGR,
	ADAPTER_ADMIN_COMMAND_MGR,
	ADAPTER_LOGGER_SERVER_MGR,
	ADAPTER_TEST_MGR,

	////////////////////////////////
	ADAPTER_DB_LOG_MGR
};

typedef struct tagServerBasicInfo
{
	ACE_UINT32 unServiceID;
	ACE_UINT32 unIP;
	ACE_UINT32 unPort;
}ServerBasicInfo;