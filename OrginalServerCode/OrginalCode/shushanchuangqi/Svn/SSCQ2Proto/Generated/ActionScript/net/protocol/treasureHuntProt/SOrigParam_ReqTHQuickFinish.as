/************************************************************************
//  工具自动生成的Flash客户端协议代码(结构体类型)
//  File Name:    SOrigParam_ReqTHQuickFinish.as
//  Purpose:      请求寻宝立即完成
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.treasureHuntProt
{
	/**
	 * 请求寻宝立即完成的原来的参数组成的结构体
	 */
	public final class SOrigParam_ReqTHQuickFinish
	{
		public var callback:Function;

		/**
		 * 转换为XML
		 * @param name_ 此类型作为成员变量时的变量名
		 */
		public function toXML(name_:String = null):XML
		{
			var topXml:XML
			if(name_ != null && name_ != "")
				topXml = <SOrigParam_ReqTHQuickFinish _name_={name_}/>;
			else
				topXml = <SOrigParam_ReqTHQuickFinish/>;
			return topXml;
		}
	}
}