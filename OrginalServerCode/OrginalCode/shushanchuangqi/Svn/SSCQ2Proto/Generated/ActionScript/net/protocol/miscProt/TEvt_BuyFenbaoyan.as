/************************************************************************
//  工具自动生成的Flash客户端协议代码(返回函数参数事件类型)
//  File Name:    TEvt_BuyFenbaoyan.as
//  Purpose:      协议:不好归类的小协议, 函数:分宝岩购买活动
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.miscProt
{
	import flash.events.Event;

	/**
	 * 函数BuyFenbaoyan[分宝岩购买活动]的返回事件
	 */
	public final class TEvt_BuyFenbaoyan extends Event
	{
		public static const DATA_RETURN:String = "MiscProt_BuyFenbaoyan";
		public static const DATA_ERROR:String = "MiscProt_BuyFenbaoyan_Error";

		public var ret:int; //0为成功，1为失败，2为超时
		public var retParam:SRetParam_BuyFenbaoyan; //返回的参数

		public function TEvt_BuyFenbaoyan(type:String, bubbles:Boolean = false, cancelable:Boolean = false)
		{
			super(type, bubbles, cancelable);
		}

		/**
		 * 转换为XML
		 */
		public function toXML():String
		{
			var topXml:XML = <TEvt_BuyFenbaoyan ret={ret}/>;
			if(retParam != null)
				topXml.appendChild(retParam.toXML("retParam"));
			return topXml.toXMLString();
		}
	}
}