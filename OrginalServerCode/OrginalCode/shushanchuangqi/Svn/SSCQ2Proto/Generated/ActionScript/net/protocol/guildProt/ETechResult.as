/************************************************************************
//  工具自动生成的Flash客户端协议代码(枚举类型)
//  File Name:    ETechResult.as
//  Purpose:      帮派相关协议
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.guildProt
 {
	import net.protocol.Message.ProtocolMessageProt16;
	/**
	 * 帮派科技操作返回结果
	 */
	public final class ETechResult
	{
		public static const eGTResultSuccess:uint = 0; //操作成功
		public static const eGTResultFailed:uint = 1; //操作失败
		public static const eGTResultNoGuild:uint = 2; //不存在帮派
		public static const eGTResultLackofSilver:uint = 3; //帮贡不足
		public static const eGTResultLevelMax:uint = 4; //已经是最大等级
		public static const eGTResultNoAuthority:uint = 5; //权限不足
		public static const eGTResultBuildingLevelTooLow:uint = 6; //帮派建筑等级过低
		public static const eGTResultUserLevelTooLow:uint = 7; //玩家等级过低
		public static const end:uint = eGTResultUserLevelTooLow;

		/////////////////////以下为辅助函数/////////////////////

		public static function getDescription(value:uint):String
		{
			switch(value)
			{
				case eGTResultSuccess:
					return ProtocolMessageProt16.eGTResultSuccess;
				case eGTResultFailed:
					return ProtocolMessageProt16.eGTResultFailed;
				case eGTResultNoGuild:
					return ProtocolMessageProt16.eGTResultNoGuild;
				case eGTResultLackofSilver:
					return ProtocolMessageProt16.eGTResultLackofSilver;
				case eGTResultLevelMax:
					return ProtocolMessageProt16.eGTResultLevelMax;
				case eGTResultNoAuthority:
					return ProtocolMessageProt16.eGTResultNoAuthority;
				case eGTResultBuildingLevelTooLow:
					return ProtocolMessageProt16.eGTResultBuildingLevelTooLow;
				case eGTResultUserLevelTooLow:
					return ProtocolMessageProt16.eGTResultUserLevelTooLow;
				default:
					return "Unknown Error";
			}
		}
	}
}