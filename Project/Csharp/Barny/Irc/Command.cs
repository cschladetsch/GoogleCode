using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace Barny.Irc
{
	internal class CommandArgs
	{
		string text;
		string cmd;
		string prefix;
		string nick;
		string host;
		string payload;
		private string raw_args;
		private List<string> args;

		public string Prefix { get { return prefix; } }
		public string Command { get { return cmd; } }
		public string Nick { get { return nick; } }
		public string Machine { get { return host; } }
		public List<string> Args { get { return args; } }
		public string FullText { get { return text; } }
		public string RawArgs { get { return raw_args; } }
		public string Payload { get { return payload; } }
		
		static Regex regex = new Regex(@"(:(?<prefix>\S+)\s+)?(?<cmd>\S+)\s*(?<args>(.*))");
		static Regex prefix_regex = new Regex(@"(?<nick>[a-zA-Z0-9])(!(?<ident>\S+))?");

		public CommandArgs(string text)
		{
			Parse(text);
		}

		void Parse(string text)
		{
			this.text = text;
			Match match = regex.Match(text);
			if (!match.Success)
			{
				Debug.WriteLine("CommandArgs:Parse: FAIL with '" + text + "'");
				return;
			}
			prefix = GetString(match, "prefix");
			if (!string.IsNullOrEmpty(prefix))
			{
				Match ident = prefix_regex.Match(prefix);
				if (!ident.Success)
				{
					Debug.WriteLine("CommandArgs:Parse: PREFIX FAIL with '" + prefix + "'");
					return;
				}
				nick = GetString(ident, "nick");
				host = GetString(ident, "ident");
			}
			cmd = GetString(match, "cmd");
			raw_args = GetString(match, "args");

			args = new List<string>();

			string last = "";
			if (raw_args.Contains(":"))
			{
				payload = last = raw_args.Substring(raw_args.IndexOf(':') + 1);
				raw_args = raw_args.Substring(0, raw_args.Length - last.Length - 1);
			}

			string[] rest = raw_args.Split(' ');
			foreach (string arg in rest)
			{
				if (!string.IsNullOrEmpty(arg))
					args.Add(arg);
			}
			
			if (last != "")
				args.Add(last);
		}

		public static void Test()
		{
			CommandArgs c;
			c = new CommandArgs("PING");
			c = new CommandArgs(":anick PING");
			c = new CommandArgs(":foo_w!machine.name.com PING");
			c = new CommandArgs("PING arg0");
			c = new CommandArgs("PING arg0 arg1");
			c = new CommandArgs(":ni23k!machine.come PING");
			c = new CommandArgs(":ni23k!machine.come PING :");
			c = new CommandArgs(":ni23k!machine.come PING :this is text");
			c = new CommandArgs(":ni23k!machine.come PING :this is text");
			c = new CommandArgs(":ni23k!machine.come PING :");
			c = new CommandArgs(":ni23k!machine.come PING arg0 :arg1 arg2 ");
			c = new CommandArgs(":ni23k!machine.come PING test :arg1 arg2 ");
			c = new CommandArgs(":ni23k!machine.come PING test :arg1 arg2 ");
			c = new CommandArgs(":ni23k!machine.come PING test foo bar :arg1 arg2 ");
		}
		static string GetString(Match m, string n)
		{
			object Q = m.Groups[n];
			if (Q == null)
				return "";
			return Q.ToString();
		}
	}
}

//EOF

