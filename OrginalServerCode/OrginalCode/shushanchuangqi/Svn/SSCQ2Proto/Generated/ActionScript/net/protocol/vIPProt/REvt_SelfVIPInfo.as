/************************************************************************
//  工具自动生成的Flash客户端协议代码(接收函数参数事件类型)
//  File Name:    REvt_SelfVIPInfo.as
//  Purpose:      协议:VIP特权相关协议, 函数:个人VIP特权信息
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.vIPProt
{
	import flash.events.Event;

	/**
	 * 函数SelfVIPInfo[个人VIP特权信息]的接收事件
	 */
	public final class REvt_SelfVIPInfo extends Event
	{
		public static const DATA_RECEIVE:String = "VIPProt_SelfVIPInfo";
		public static const DATA_ERROR:String = "VIPProt_SelfVIPInfo_Error";

		public var stVIPData:SVIPData; //VIP特权信息

		public function REvt_SelfVIPInfo(type:String, bubbles:Boolean = false, cancelable:Boolean = false)
		{
			super(type, bubbles, cancelable);
		}

		/**
		 * 转换为XML
		 */
		public function toXML():XML
		{
			var topXml:XML = <REvt_SelfVIPInfo/>;
			if(stVIPData != null)
				topXml.appendChild(stVIPData.toXML("stVIPData"));
			return topXml;
		}
	}
}