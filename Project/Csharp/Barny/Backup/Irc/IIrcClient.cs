using System;
using System.Collections.Generic;
using System.Text;

namespace Barny.Irc
{
	public interface IClient
	{
		IUser User { get; }

		void Quit(string text);
		void SendText(IChannel channel, string text);
		void JoinChannel(string name);
		void ChangeNick(string nick);
	}

	public interface IBot
	{
		bool Handle(IChannel channel, IUser from, string text);
	}
}

//EOF
