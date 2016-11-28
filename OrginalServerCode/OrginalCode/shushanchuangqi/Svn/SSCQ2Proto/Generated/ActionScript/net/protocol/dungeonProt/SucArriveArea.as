/************************************************************************
//  工具自动生成的Flash客户端协议代码(结构体类型)
//  File Name:    SucArriveArea.as
//  Purpose:      副本相关协议
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.dungeonProt
{
	import flash.utils.ByteArray;
	import net.util.*;

	/**
	 * 到达某个区域
	 */
	public final class SucArriveArea extends DgnSucCond
	{
		public var wAreaID:uint; //(无符号16位整数)区域ID

		/**
		 * 获取继承类的类型
		 */
		public override function getClassType():uint
		{
			return EType_DgnSucCond.eType_SucArriveArea;
		}

		/**
		 * 转换为XML
		 * @param name_ 此类型作为成员变量时的变量名
		 */
		public override function toXML(name_:String = null):XML
		{
			var topXml:XML
			if(name_ != null && name_ != "")
				topXml = <SucArriveArea _name_={name_} wAreaID={wAreaID}/>;
			else
				topXml = <SucArriveArea wAreaID={wAreaID}/>;
			topXml.appendChild(super.toXML(name_));
			return topXml;
		}

		/**
		 * 拷贝到另一个对象
		 * @param obj_ 另一个对象
		 */
		public override function copyTo(obj_:*):void
		{
			super.copyTo(obj_);
			obj_.wAreaID = wAreaID;
		}

		/////////////////////以下为序列化函数/////////////////////

		public static function fromByteArray(bytes:ByteArray):SucArriveArea
		{
			var s_:SucArriveArea = new SucArriveArea();
			var base_:DgnSucCond = DgnSucCond.fromByteArray(bytes);
			base_.copyTo(s_);
			var wAreaID:uint = bytes.readUnsignedShort();
			s_.wAreaID = wAreaID;
			return s_;
		}

		public static function vectorFromByteArray(bytes:ByteArray):Vector.<SucArriveArea>
		{
			var length:uint = BytesUtil.readVectorLength(bytes);
			var vec:Vector.<SucArriveArea> = new Vector.<SucArriveArea>();
			for(var i:uint = 0; i < length; ++i)
			{
				var s:SucArriveArea = SucArriveArea.fromByteArray(bytes);
				vec.push(s);
			}
			return vec;
		}

		public override function toByteArray(bytes:ByteArray):void
		{
			super.toByteArray(bytes);
			bytes.writeShort(wAreaID);
		}

		public static function vectorToByteArray(vec:Vector.<SucArriveArea>, bytes:ByteArray):void
		{
			BytesUtil.writeVectorLength(bytes, vec.length);
			for each(var s:SucArriveArea in vec)
				s.toByteArray(bytes);
		}
	}
}