using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace LuxiaBehaviour.Luxia
{
	public class Log
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static void Line(string message);
	}
}
