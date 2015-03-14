using System;
using System.Collections.Generic;
using System.Text;

namespace Barny.Interfaces
{
	public interface IMessageSink
	{
		void AddMessage(Irc.Message message);
	}
}

//EOF
