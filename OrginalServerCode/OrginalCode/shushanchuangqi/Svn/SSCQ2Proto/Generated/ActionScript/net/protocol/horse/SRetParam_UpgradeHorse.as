/************************************************************************
//  工具自动生成的Flash客户端协议代码(结构体类型)
//  File Name:    SRetParam_UpgradeHorse.as
//  Purpose:      坐骑进阶
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.horse
{
	/**
	 * 坐骑进阶的返回的参数组成的结构体
	 */
	public final class SRetParam_UpgradeHorse
	{
		public var dwRet:uint; //(无符号32位整数)结果 0成功其他错误码

		/**
		 * 转换为XML
		 * @param name_ 此类型作为成员变量时的变量名
		 */
		public function toXML(name_:String = null):XML
		{
			var topXml:XML
			if(name_ != null && name_ != "")
				topXml = <SRetParam_UpgradeHorse _name_={name_} dwRet={dwRet}/>;
			else
				topXml = <SRetParam_UpgradeHorse dwRet={dwRet}/>;
			return topXml;
		}
	}
}