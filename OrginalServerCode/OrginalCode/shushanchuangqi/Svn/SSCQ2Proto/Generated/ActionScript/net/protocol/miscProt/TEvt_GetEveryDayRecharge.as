/************************************************************************
//  工具自动生成的Flash客户端协议代码(返回函数参数事件类型)
//  File Name:    TEvt_GetEveryDayRecharge.as
//  Purpose:      协议:不好归类的小协议, 函数:获取每日冲值金额
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.miscProt
{
	import flash.events.Event;

	/**
	 * 函数GetEveryDayRecharge[获取每日冲值金额]的返回事件
	 */
	public final class TEvt_GetEveryDayRecharge extends Event
	{
		public static const DATA_RETURN:String = "MiscProt_GetEveryDayRecharge";
		public static const DATA_ERROR:String = "MiscProt_GetEveryDayRecharge_Error";

		public var ret:int; //0为成功，1为失败，2为超时
		public var retParam:SRetParam_GetEveryDayRecharge; //返回的参数

		public function TEvt_GetEveryDayRecharge(type:String, bubbles:Boolean = false, cancelable:Boolean = false)
		{
			super(type, bubbles, cancelable);
		}

		/**
		 * 转换为XML
		 */
		public function toXML():String
		{
			var topXml:XML = <TEvt_GetEveryDayRecharge ret={ret}/>;
			if(retParam != null)
				topXml.appendChild(retParam.toXML("retParam"));
			return topXml.toXMLString();
		}
	}
}