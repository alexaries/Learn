/************************************************************************
//  工具自动生成的Flash客户端协议代码(结构体类型)
//  File Name:    SEffectStateBuffDec.as
//  Purpose:      战斗通信协议
//  Copyright (c) 2010-2020 上海恺英网络科技有限公司, All rights reserved.
*************************************************************************/

package net.protocol.battleGS
{
	import flash.utils.ByteArray;
	import net.util.*;

	/**
	 * 减状态Buff
	 */
	public final class SEffectStateBuffDec extends SEffectBase
	{
		public var dwBuffKey:uint; //(无符号32位整数)查找这个Buff的唯一ID

		/**
		 * 获取继承类的类型
		 */
		public override function getClassType():uint
		{
			return EType_SEffectBase.eType_SEffectStateBuffDec;
		}

		/**
		 * 转换为XML
		 * @param name_ 此类型作为成员变量时的变量名
		 */
		public override function toXML(name_:String = null):XML
		{
			var topXml:XML
			if(name_ != null && name_ != "")
				topXml = <SEffectStateBuffDec _name_={name_} dwBuffKey={dwBuffKey}/>;
			else
				topXml = <SEffectStateBuffDec dwBuffKey={dwBuffKey}/>;
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
			obj_.dwBuffKey = dwBuffKey;
		}

		/////////////////////以下为序列化函数/////////////////////

		public static function fromByteArray(bytes:ByteArray):SEffectStateBuffDec
		{
			var s_:SEffectStateBuffDec = new SEffectStateBuffDec();
			var base_:SEffectBase = SEffectBase.fromByteArray(bytes);
			base_.copyTo(s_);
			var dwBuffKey:uint = bytes.readUnsignedInt();
			s_.dwBuffKey = dwBuffKey;
			return s_;
		}

		public static function vectorFromByteArray(bytes:ByteArray):Vector.<SEffectStateBuffDec>
		{
			var length:uint = BytesUtil.readVectorLength(bytes);
			var vec:Vector.<SEffectStateBuffDec> = new Vector.<SEffectStateBuffDec>();
			for(var i:uint = 0; i < length; ++i)
			{
				var s:SEffectStateBuffDec = SEffectStateBuffDec.fromByteArray(bytes);
				vec.push(s);
			}
			return vec;
		}

		public override function toByteArray(bytes:ByteArray):void
		{
			super.toByteArray(bytes);
			bytes.writeUnsignedInt(dwBuffKey);
		}

		public static function vectorToByteArray(vec:Vector.<SEffectStateBuffDec>, bytes:ByteArray):void
		{
			BytesUtil.writeVectorLength(bytes, vec.length);
			for each(var s:SEffectStateBuffDec in vec)
				s.toByteArray(bytes);
		}
	}
}