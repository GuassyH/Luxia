using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuxiaBehaviour
{
    public class LuxTest : ILuxiaBehaviour
	{

        public void Update()
        {
            Luxia.Log.Line(TestString());
        }

        string TestString()
        {
            return "Hello World!";
		}
	}
}
