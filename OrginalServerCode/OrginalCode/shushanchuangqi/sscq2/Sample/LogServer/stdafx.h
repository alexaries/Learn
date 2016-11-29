﻿
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifdef _WIN32

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#endif // _WIN32

//include from STL
#include <set>
#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

#ifndef MYSQLPP_NO_DLL
#define MYSQLPP_NO_DLL
#endif
#ifndef MYSQLPP_EXPORT
#define MYSQLPP_EXPORT
#endif
//include from MySQL++
#include <mysql++.h>

//include from Network
#include "Interface.h"

//include from SvrLib
#include "GateHandler.h"
#include "SvrHandler.h"
#include "SessionMgr.h"
#include "ServerID.h"

//include from Utility
#include "ObjPool.h"
#include "Thread.h"
#include "Lock.h"
#include "Directory.h"
#include "Random.h"
#include "UtilFunc.h"
#include "Encode.h"

//Basic include
#include "LogConfig.h"
#include "ServerDefine.h"
#include "GlobalDefine.h"
#include "LogNetMgr.h"
#include "AllTimers.h"
#include "Protocols.h"

//DB logic include
//#include "InitQuery.h"
#include "DBMgr.h"
