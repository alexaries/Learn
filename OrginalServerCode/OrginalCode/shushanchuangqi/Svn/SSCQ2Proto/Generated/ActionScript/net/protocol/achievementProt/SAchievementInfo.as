/************************************************************************
//  工具自动生成的Flash客户端协议代码(结构体类型)
//  File Name:    SAchievementInfo.as
//  Purpose:      成就相关协议
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.achievementProt
{
	import com.hurlant.math.BigInteger;
	import flash.utils.ByteArray;
	import net.util.*;

	/**
	 * EDataType:eTypeAchievementInfo
	 */
	public final class SAchievementInfo
	{
		public var qwRoleID:BigInteger; //(无符号64位整数)角色ID
		public var wAchievementID:uint; //(无符号16位整数)成就ID
		public var dwCount:uint; //(无符号32位整数)EAchievementProcess类型成就完成进度
		public var dwProcess:uint; //(无符号32位整数)当前进度(持续型事件记录数据)
		public var dwFinishTime:uint; //(无符号32位整数)完成时间

		/**
		 * 辅助创建函数
		 */
		public static function create(qwRoleID:BigInteger, wAchievementID:uint, dwCount:uint, dwProcess:uint, dwFinishTime:uint):SAchievementInfo
		{
			var s_:SAchievementInfo = new SAchievementInfo();
			s_.qwRoleID = qwRoleID;
			s_.wAchievementID = wAchievementID;
			s_.dwCount = dwCount;
			s_.dwProcess = dwProcess;
			s_.dwFinishTime = dwFinishTime;
			return s_;
		}

		/**
		 * 转换为XML
		 * @param name_ 此类型作为成员变量时的变量名
		 */
		public function toXML(name_:String = null):XML
		{
			var topXml:XML
			if(name_ != null && name_ != "")
				topXml = <SAchievementInfo _name_={name_} qwRoleID={qwRoleID} wAchievementID={wAchievementID} dwCount={dwCount} dwProcess={dwProcess} dwFinishTime={dwFinishTime}/>;
			else
				topXml = <SAchievementInfo qwRoleID={qwRoleID} wAchievementID={wAchievementID} dwCount={dwCount} dwProcess={dwProcess} dwFinishTime={dwFinishTime}/>;
			return topXml;
		}

		/**
		 * 拷贝到另一个对象
		 * @param obj_ 另一个对象
		 */
		public function copyTo(obj_:*):void
		{
			obj_.qwRoleID = qwRoleID;
			obj_.wAchievementID = wAchievementID;
			obj_.dwCount = dwCount;
			obj_.dwProcess = dwProcess;
			obj_.dwFinishTime = dwFinishTime;
		}

		/////////////////////以下为序列化函数/////////////////////

		public static function fromByteArray(bytes:ByteArray):SAchievementInfo
		{
			var s_:SAchievementInfo = new SAchievementInfo();
			var qwRoleID:BigInteger = BytesUtil.readUInt64(bytes);
			s_.qwRoleID = qwRoleID;
			var wAchievementID:uint = bytes.readUnsignedShort();
			s_.wAchievementID = wAchievementID;
			var dwCount:uint = bytes.readUnsignedInt();
			s_.dwCount = dwCount;
			var dwProcess:uint = bytes.readUnsignedInt();
			s_.dwProcess = dwProcess;
			var dwFinishTime:uint = bytes.readUnsignedInt();
			s_.dwFinishTime = dwFinishTime;
			return s_;
		}

		public static function vectorFromByteArray(bytes:ByteArray):Vector.<SAchievementInfo>
		{
			var length:uint = BytesUtil.readVectorLength(bytes);
			var vec:Vector.<SAchievementInfo> = new Vector.<SAchievementInfo>();
			for(var i:uint = 0; i < length; ++i)
			{
				var s:SAchievementInfo = SAchievementInfo.fromByteArray(bytes);
				vec.push(s);
			}
			return vec;
		}

		public function toByteArray(bytes:ByteArray):void
		{
			BytesUtil.writeUInt64(bytes, qwRoleID);
			bytes.writeShort(wAchievementID);
			bytes.writeUnsignedInt(dwCount);
			bytes.writeUnsignedInt(dwProcess);
			bytes.writeUnsignedInt(dwFinishTime);
		}

		public static function vectorToByteArray(vec:Vector.<SAchievementInfo>, bytes:ByteArray):void
		{
			BytesUtil.writeVectorLength(bytes, vec.length);
			for each(var s:SAchievementInfo in vec)
				s.toByteArray(bytes);
		}
	}
}