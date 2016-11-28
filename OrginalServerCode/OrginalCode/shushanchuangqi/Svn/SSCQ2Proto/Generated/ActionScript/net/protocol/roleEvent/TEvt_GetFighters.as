/************************************************************************
//  工具自动生成的Flash客户端协议代码(返回函数参数事件类型)
//  File Name:    TEvt_GetFighters.as
//  Purpose:      协议:玩家在GameServer的消息处理, 函数:取得散仙列表
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.roleEvent
{
	import flash.events.Event;

	/**
	 * 函数GetFighters[取得散仙列表]的返回事件
	 */
	public final class TEvt_GetFighters extends Event
	{
		public static const DATA_RETURN:String = "RoleEvent_GetFighters";
		public static const DATA_ERROR:String = "RoleEvent_GetFighters_Error";

		public var ret:int; //0为成功，1为失败，2为超时
		public var retParam:SRetParam_GetFighters; //返回的参数

		public function TEvt_GetFighters(type:String, bubbles:Boolean = false, cancelable:Boolean = false)
		{
			super(type, bubbles, cancelable);
		}

		/**
		 * 转换为XML
		 */
		public function toXML():String
		{
			var topXml:XML = <TEvt_GetFighters ret={ret}/>;
			if(retParam != null)
				topXml.appendChild(retParam.toXML("retParam"));
			return topXml.toXMLString();
		}
	}
}