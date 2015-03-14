using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Diagnostics;

namespace Barny
{
	public class Factory
	{
		static Factory factory = new Factory();
		Dictionary<Type, MethodInfo> ctors = new Dictionary<Type, MethodInfo>();

		public static void BindType<Iface, Concrete>()
		{
			MethodInfo[] methods = typeof(Concrete).GetMethods();
			foreach (MethodInfo method in methods)
			{
				if (method.Name == "NewInstance")
				{
					factory.ctors[typeof(Iface)] = method;
					return;
				}
			}
			throw new Exception("No static creation method for type " + typeof(Iface).Name);
		}

		public static T Create<T>(object[] args)
		{
			return (T)factory.ctors[typeof(T)].Invoke(null, args);
		}
	}
}

//EOF
