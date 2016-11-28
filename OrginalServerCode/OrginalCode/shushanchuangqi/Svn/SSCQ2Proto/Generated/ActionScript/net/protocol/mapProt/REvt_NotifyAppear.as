/************************************************************************
//  工具自动生成的Flash客户端协议代码(接收函数参数事件类型)
//  File Name:    REvt_NotifyAppear.as
//  Purpose:      协议:地图相关功能的协议, 函数:通知自己的玩家角色出现
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.mapProt
{
	import flash.events.Event;
	import com.hurlant.math.BigInteger;

	/**
	 * 函数NotifyAppear[通知自己的玩家角色出现]的接收事件
	 */
	public final class REvt_NotifyAppear extends Event
	{
		public static const DATA_RECEIVE:String = "MapProt_NotifyAppear";
		public static const DATA_ERROR:String = "MapProt_NotifyAppear_Error";

		public var qwRoleID:BigInteger; //(无符号64位整数)角色ID
		public var rPlayerAppear:SPlayerAppear; //玩家的外观
		public var rPosition:SPosition; //出现位置
		public var fDir:Number; //(Float)方向(弧度)
		public var dwAppearState:uint; //(无符号32位整数)当前状态(EAppearState按位或的结果)

		public function REvt_NotifyAppear(type:String, bubbles:Boolean = false, cancelable:Boolean = false)
		{
			super(type, bubbles, cancelable);
		}

		/**
		 * 转换为XML
		 */
		public function toXML():XML
		{
			var topXml:XML = <REvt_NotifyAppear qwRoleID={qwRoleID} fDir={fDir} dwAppearState={dwAppearState}/>;
			if(rPlayerAppear != null)
				topXml.appendChild(rPlayerAppear.toXML("rPlayerAppear"));
			if(rPosition != null)
				topXml.appendChild(rPosition.toXML("rPosition"));
			return topXml;
		}
	}
}