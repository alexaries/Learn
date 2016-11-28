﻿/************************************************************************
//  工具自动生成的服务器协议代码(UTF-8 With BOM)
//  File Name:    TripodProt.h
//  Purpose:      九疑鼎相关协议
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

#pragma once
#include "TripodProt.h"

namespace NTripodProt
{

//协议类：九疑鼎相关协议
class CTripodProtSvr : public IProtocol
{
public:
	/////////////////////以下为发送函数/////////////////////
	//发送：更新玩家的九疑鼎数据
	bool Send_UpdateTripod( //发送给当前协议处理的客户端
		const STripodData& oTripodData, //九疑鼎数据列表
		UINT8 byShanfeng, //玩家今日已扇风次数
		UINT8 byHelpTimes //玩家今日已协助次数(玩家协助)
	);
	bool SendClt_UpdateTripod(const UINT64* pUsrID, UINT16 wCnt, //发送给多个客户端
		const STripodData& oTripodData, //九疑鼎数据列表
		UINT8 byShanfeng, //玩家今日已扇风次数
		UINT8 byHelpTimes //玩家今日已协助次数(玩家协助)
	);
	std::string& BuildPkg_UpdateTripod( //只组包不发送
		const STripodData& oTripodData, //九疑鼎数据列表
		UINT8 byShanfeng, //玩家今日已扇风次数
		UINT8 byHelpTimes //玩家今日已协助次数(玩家协助)
	);

	/////////////////////以下的接收函数需要重载/////////////////////
	struct SGetAllTripodsAck : public CClientFuncAck
	{
		STripodData oTripodData; //九疑鼎数据列表
		UINT8 byShanfeng; //玩家今日已扇风次数
		UINT8 byHelpTimes; //玩家今日已协助次数(玩家协助)

		SGetAllTripodsAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：取得玩家所有九疑鼎信息
	virtual bool OnRecv_GetAllTripods(
		shared_func<SGetAllTripodsAck>& fnAck //返回回调函数
	) = 0;

	struct SGetAllFiresAck : public CClientFuncAck
	{
		TVecFireInfo vecFires; //九疑鼎信息列表

		SGetAllFiresAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：取得玩家所有九疑鼎的火种信息
	virtual bool OnRecv_GetAllFires(
		shared_func<SGetAllFiresAck>& fnAck //返回回调函数
	) = 0;

	struct SGetTripodDataAck : public CClientFuncAck
	{
		STripodData oTripodData; //九疑鼎数据列表

		SGetTripodDataAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：取得其他玩家所有九疑鼎信息
	virtual bool OnRecv_GetTripodData(
		UINT64 qwRoleID, //玩家ID(九疑鼎所属者)
		shared_func<SGetTripodDataAck>& fnAck //返回回调函数
	) = 0;

	struct SActiveTripodAck : public CClientFuncAck
	{
		UINT8 byTripodID; //九疑鼎ID
		ETripodType eResult; //结果

		SActiveTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：激活九疑鼎
	virtual bool OnRecv_ActiveTripod(
		UINT8 byTripodID, //九疑鼎ID
		shared_func<SActiveTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SUpgradeTripodAck : public CClientFuncAck
	{
		TVecTripodInfo vecTripods; //九疑鼎信息列表
		ETripodType eResult; //结果

		SUpgradeTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：升级九疑鼎
	virtual bool OnRecv_UpgradeTripod(
		UINT8 byTripodID, //九疑鼎ID
		UINT8 byOneKey, //是否一键
		shared_func<SUpgradeTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SMakeFireTripodAck : public CClientFuncAck
	{
		TVecTripodInfo vecTripods; //九疑鼎信息列表
		ETripodType eResult; //结果

		SMakeFireTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：九疑鼎祈火(炼丹)
	virtual bool OnRecv_MakeFireTripod(
		UINT8 byTripodID, //九疑鼎ID
		UINT16 wFireID, //火种ID
		shared_func<SMakeFireTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SClearFireTripodAck : public CClientFuncAck
	{
		UINT8 byTripodID; //九疑鼎ID
		ETripodType eResult; //结果

		SClearFireTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：清除九疑鼎祈火记录
	virtual bool OnRecv_ClearFireTripod(
		UINT8 byTripodID, //九疑鼎ID
		shared_func<SClearFireTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SGetTripodAwardAck : public CClientFuncAck
	{
		TVecTripodInfo vecTripods; //九疑鼎信息列表
		ETripodType eResult; //结果

		SGetTripodAwardAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：获取九疑鼎炼丹果实
	virtual bool OnRecv_GetTripodAward(
		UINT8 byTripodID, //九疑鼎ID
		shared_func<SGetTripodAwardAck>& fnAck //返回回调函数
	) = 0;

	struct SSpeedTripodAck : public CClientFuncAck
	{
		TVecTripodInfo vecTripods; //九疑鼎信息列表
		ETripodType eResult; //结果

		SSpeedTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：加速九疑鼎炼丹(道具入鼎)
	virtual bool OnRecv_SpeedTripod(
		UINT8 byTripodID, //九疑鼎ID
		const NItemProt::TVecItemCount& vecItems, //入鼎道具列表
		shared_func<SSpeedTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SAddTripodExtraSoulAck : public CClientFuncAck
	{
		TVecTripodInfo vecTripods; //所属者的九疑鼎信息列表
		ETripodType eResult; //结果

		SAddTripodExtraSoulAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：扇火九疑鼎
	virtual bool OnRecv_AddTripodExtraSoul(
		UINT64 qwRoleID, //玩家ID(九疑鼎所属者)
		UINT8 byTripodID, //九疑鼎ID
		shared_func<SAddTripodExtraSoulAck>& fnAck //返回回调函数
	) = 0;

	struct SFighterHelpTripodAck : public CClientFuncAck
	{
		SFighterHelpInfo oFighterHelp; //散仙协助信息
		ETripodType eResult; //结果

		SFighterHelpTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：散仙协助九疑鼎
	virtual bool OnRecv_FighterHelpTripod(
		UINT16 wFighterID, //自己的散仙ID
		UINT8 byPos, //协助位置
		shared_func<SFighterHelpTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SUserHelpTripodAck : public CClientFuncAck
	{
		SPlayerHelpInfo oPlayerHelp; //玩家协助信息
		ETripodType eResult; //结果

		SUserHelpTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：玩家协助九疑鼎
	virtual bool OnRecv_UserHelpTripod(
		UINT64 qwRoleID, //对方玩家ID
		UINT8 byPos, //协助位置
		shared_func<SUserHelpTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SShotOffHelpTripodAck : public CClientFuncAck
	{
		ETripodType eResult; //结果
		UINT8 byPos; //协助位置(0,1,2)
		UINT8 byType; //0散仙 1玩家

		SShotOffHelpTripodAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：踢出九疑鼎协助者
	virtual bool OnRecv_ShotOffHelpTripod(
		UINT8 byPos, //协助位置(0,1,2)
		UINT8 byType, //0散仙 1玩家
		shared_func<SShotOffHelpTripodAck>& fnAck //返回回调函数
	) = 0;

	struct SGetTripodStatusAck : public CClientFuncAck
	{
		TVecTripodStatus vecStatus; //玩家九疑鼎的扇火、协助状态

		SGetTripodStatusAck(IProtocol& rProtocol, UINT64 qwUsrID, UINT32 dwSessionID);
	private:
		virtual void SendReturn(bool bRet) override;
	};
	//接收：请求玩家九疑鼎的能被扇火、协助信息
	virtual bool OnRecv_GetTripodStatus(
		const TVecUINT64& vecRoleID, //好友玩家ID列表
		shared_func<SGetTripodStatusAck>& fnAck //返回回调函数
	) = 0;

	/////////////////////以下为协议接口函数/////////////////////
	virtual bool HandleMessage(const char* pBuf, UINT32 dwLen) override;

	virtual void OnTimeOut(UINT8, UINT32) override { }

	virtual UINT8 GetProtID() const override { return 63; }

	virtual bool IsForClient() const override { return true; }

	std::string& GetEmptyPkg() { _strPkg.clear(); return _strPkg; }

protected:
	/////////////////////协议处理函数/////////////////////
	bool _DoRecv_GetAllTripods(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_GetAllFires(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_GetTripodData(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_ActiveTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_UpgradeTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_MakeFireTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_ClearFireTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_GetTripodAward(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_SpeedTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_AddTripodExtraSoul(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_FighterHelpTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_UserHelpTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_ShotOffHelpTripod(const char* pBuf, UINT32 dwLen);
	bool _DoRecv_GetTripodStatus(const char* pBuf, UINT32 dwLen);

private:
	typedef bool (CTripodProtSvr::*THandleFunc)(const char* pBuf, UINT32 dwLen);
	static THandleFunc _HandleFuncs[];
	std::string _strPkg;

protected:
	CInArchive _oInArchive;
};

} //namespace NTripodProt