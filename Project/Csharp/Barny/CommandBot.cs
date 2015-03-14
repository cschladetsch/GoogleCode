using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Barny.Irc;

namespace Barny
{
	class CommandBot : IBot
	{
		delegate void CommandHandler(IChannel channel, IUser user, string text);

		Dictionary<string, CommandHandler> commands = new Dictionary<string, CommandHandler>();
		Regex command_regex = new Regex(@"!(?<cmd>\w+) (?<args>.*)");
		IClient client;

		public CommandBot(IClient client)
		{
			this.client = client;

			commands["join"] = JoinChannel;
			commands["part"] = PartChannel;
			commands["quit"] = Quit;
			commands["clear"] = ClearText;
			commands["clear_image"] = ClearImage;
		}

		public bool Handle(IChannel channel, IUser user, string text)
		{
			Match match = command_regex.Match(text);
			if (!match.Success)
				return false;
			string cmd = match.Groups["cmd"].ToString();
			string args = match.Groups["args"].ToString();
			if (!commands.ContainsKey(cmd))
				return false;
			commands[cmd](channel, user, args);
			return true;
		}


		void Quit(IChannel channel, IUser user, string args)
		{
			client.Quit(args);
		}

		void JoinChannel(IChannel channel, IUser user, string args)
		{
			client.JoinChannel(args);
		}

		void PartChannel(IChannel channel, IUser user, string args)
		{
		}

		void ClearText(IChannel channel, IUser user, string args)
		{
		}

		void ClearImage(IChannel channel, IUser user, string args)
		{
		}
	}
}

//EOF

