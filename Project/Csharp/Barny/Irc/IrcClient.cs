using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Diagnostics;
using System.Text.RegularExpressions;

using Barny.Irc;

namespace Barny.Irc
{
	public class Client : IClient
	{
		#region Events
		public delegate void ServerJoinDelegate(IServer server);
		public delegate void ServerLeaveDelegate(IServer server);
		public delegate void UserJoinDelegate(IServer server, IChannel channel, IUser user);
		public delegate void UserLeaveDelegate(IServer server, IChannel channel, IUser user);
		public delegate void UserSayDelegate(IServer server, IChannel channel, IUser user, IMessage text);
		public delegate void UserEmoteDelegate(IServer server, IChannel channel, IUser user, IMessage text);
		public delegate void UserNotifyDelegate(IServer server, IChannel channel, IUser user, IMessage text);

		public event ServerJoinDelegate OnServerJoin;
		public event ServerLeaveDelegate OnServerLeave;
		public event UserJoinDelegate OnUserJoin;
		public event UserLeaveDelegate OnUserLeave;
		public event UserSayDelegate OnUserSay;
		public event UserEmoteDelegate OnUserEmote;
		public event UserNotifyDelegate OnUserNotify;
		#endregion

		//`readonly public Dictionary<IChannel, List<IBot>> Bots = new Dictionary<IChannel, List<IBot>>();

		public readonly Dictionary<IChannel, List<IBot> > Bots = new Dictionary<IChannel, List<IBot>>();
		public readonly List<IBot> InputBots = new List<IBot>();

		#region Fields
		private const int BufferSize = 4321;
		readonly byte[] buffer = new byte[BufferSize];
		private Socket socket;

		object mutex = new object();
		object receive_mutex = new object();
		List<string> received_text = new List<string>();

		private IUser server_user, user;
		private IServer server;
		private IChannel server_channel;
		private string local_ip_address;

		private delegate void CommandHandler(CommandArgs args);
		private Dictionary<string, CommandHandler> commands = new Dictionary<string, CommandHandler>();

		public IUser User { get { return user; } }
		public IUser ServerUser { get { return server_user; } }
		public IChannel ServerChannel { get { return server_channel; } }
		#endregion

		public Client()
		{
			AddCommandHandlers();
			server_user = null;
		}

		public void Quit(string text)
		{
			SendTextRaw("QUIT :" + text);
		}

		public void AddBot(IBot bot)
		{
			InputBots.Add(bot);
		}

		public void AddBot(IChannel channel, IBot bot)
		{
			if (!Bots.ContainsKey(channel))
				Bots.Add(channel, new List<IBot>());
			Bots[channel].Add(bot);
		}

		public void JoinChannel(string name)
		{
			SendTextRaw("JOIN " + name + "\n");
		}

		public void ChangeNick(string name)
		{
			SendTextRaw("NICK " + name + "\n");
		}

		#region Sockets

		public void Connect()
		{
			Barny.Properties.Settings irc = new Barny.Properties.Settings();
			try
			{
				//connecting = new Connecting();
				//connecting.Show();

				socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

				// TODO: asynchronous connection
				socket.Connect(irc.Server, int.Parse(irc.Port));

				// TODO
				SendTextRaw(string.Format("USER {0} 1 * :Barney the Farmer \n", irc.Botname));

				ChangeNick(irc.Nick);
				JoinChannel(irc.Data);
				JoinChannel(irc.Chat);

				server = Database.Servers.Open(irc.Server, irc.Server);
				server_channel = Database.Channels.Open(irc.Server, server, irc.Server);
				user = Database.Users.Open(irc.Nick, irc.Nick);

				// TODO
					Database.Channels.Open(irc.Data, server, irc.Data);
					Database.Channels.Open(irc.Chat, server, irc.Chat);

				UserJoin(server, server_channel, user);
				
				local_ip_address = irc.Botname;// "..."; // TODO

				AsyncReceive();
			}
			catch (Exception E)
			{
				string text = string.Format("Couldn't connect to IRC server {0}:{1}\n\nException:\n{2}", irc.Server, irc.Port, E.ToString());
				MessageBox.Show(text, "Couldn't Connect", MessageBoxButtons.OK, MessageBoxIcon.Question);
				return;
			}
		}

		public void Disconnect()
		{
			socket.Close();
			//server = null;
			//user = null;
		}

		void AsyncReceive()
		{
			socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, Received, this);
		}

		private void Received(IAsyncResult ar)
		{
			int num_bytes_received = 0;
			try
			{
				num_bytes_received = socket.EndReceive(ar);
			}
			catch (Exception e)
			{
				if (MessageBox.Show(e.ToString(), "Exception during Receive", MessageBoxButtons.AbortRetryIgnore, MessageBoxIcon.Error) == DialogResult.Abort)
					Application.Exit();
			}

			char[] chars = new char[num_bytes_received + 1];
			System.Text.Encoding.UTF8.GetDecoder().GetChars(buffer, 0, num_bytes_received, chars, 0);

			lock (receive_mutex)
			{
				received_text.Add(new String(chars));
			}

			AsyncReceive();
		}

		void SendTextRaw(string text)
		{
			if (string.IsNullOrEmpty(text))
				return;
			Debug.WriteLine("SendTextRaw: '" + text + "'");
			byte[] bytes = new ASCIIEncoding().GetBytes(text);
			socket.Send(bytes, bytes.Length, SocketFlags.None);
		}

		void ReceiveTextRaw(string raw_text)
		{
			Debug.WriteLine("ReceiveTextRaw: <<" + raw_text + ">>");

			if (string.IsNullOrEmpty(raw_text))
				return;

			string[] cmds = raw_text.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
			foreach (string text in cmds)
			{
				try
				{
					if (!string.IsNullOrEmpty(text) && text.Length > 1)
						ProcessCommand(new CommandArgs(text));
				}
				catch (Exception E)
				{
					string msg = string.Format("Error processing command {0}:\n\n{1}", text, E.ToString());
					Debug.WriteLine(msg);
				}
			}
		}

		public void ProcessPendingInput()
		{
			lock (receive_mutex)
			{
				foreach (string text in received_text)
				{
					ReceiveTextRaw(text);
				}
				received_text.Clear();
			}
		}

		#endregion

		void ProcessCommand(CommandArgs cmd)
		{
			if (commands.ContainsKey(cmd.Command))
			{
				commands[cmd.Command](cmd);
			}
			else
			{
				Debug.WriteLine("Unprocessed Command: " + cmd.FullText);
				if (!string.IsNullOrEmpty(cmd.Payload))
					Database.Messages.Add(new Irc.Message(server_channel, server_user, cmd.Payload));
			}
		}

		public void SendText(IChannel channel, string text)
		{
			foreach (IBot bot in InputBots)
			{
				if (bot.Handle(channel, user, text))
					return;
			}

			SendPrivateMessage(channel, text);
		}

		public void SendPrivateMessage(IChannel channel, string text)
		{
			Database.Messages.Add(new Irc.Message(channel, user, text));

			if (channel == server_channel)
			{
				SendTextRaw(text);
				return;
			}

			string header = BuildMessageHeaderPrefix("PRIVMSG");
			string message = ":" + header + channel.Name + " :" + text + "\r\n";

			SendTextRaw(message);
		}

		string BuildMessageHeaderPrefix(string cmd)
		{
			return user.Name + "!" + local_ip_address + " " + cmd + " ";
		}

		#region Commands

		void AddServerMessage(string text)
		{
			AddServerMessage(server_channel, text);
		}

		void AddServerMessage(IChannel channel, string text, string fore, string back)
		{
			Database.Messages.Add(new Irc.Message(channel, server_user, text, fore, back));
		}

		void AddServerMessage(IChannel channel, string text)
		{
			Database.Messages.Add(new Irc.Message(channel, server_user, text));
		}

		void AddCommandHandlers()
		{
			commands["PING"] = CommandPing;
			commands["NOTICE"] = CommandNotice;
			commands["JOIN"] = CommandJoin;
			commands["PART"] = CommandPart;
			commands["PRIVMSG"] = CommandPrivateMessage;
			commands["NICK"] = CommandNick;
			commands["MODE"] = CommandMode;

			commands["001"] = CommandConnected; // TODO: seperate system for numeric commands
			commands["353"] = CommandUserList;
			commands["375"] = CommandStartMOTD;
			commands["372"] = CommandAddMOTD;
			commands["376"] = CommandEndMOTD;
			commands["004"] = CommandEat;
			commands["005"] = CommandEat;
			commands["366"] = CommandUserListEnd;
			commands["433"] = CommandNickAlreadyInUse;
		}

		void CommandNickAlreadyInUse(CommandArgs cmd)
		{
			// TODO
			MessageBox.Show("Nick already in use");
			return;
		}

		void CommandUserListEnd(CommandArgs cmd)
		{
			IChannel channel = Database.Channels[cmd.Args[1]];
			StringBuilder users = new StringBuilder();
			foreach (IUser user in channel.Users)
				users.Append(user.Name + " ");
			AddServerMessage(channel, users.ToString() + " are also here");
		}
		
		void CommandEat(CommandArgs cmd)
		{
			// nom nom nom...
		}
		
		void CommandMode(CommandArgs cmd)
		{
			// TODO
		}
		
		void CommandNick(CommandArgs cmd)
		{
			Debug.WriteLine("New nick for '" + cmd.Nick + "' is '" + cmd.Payload + "'");
			Database.Users[cmd.Nick].Name = cmd.Payload;
		}
		
		void CommandUserList(CommandArgs cmd)
		{
			Debug.WriteLine("CommandUserList: '" + cmd.Payload + "'");
			string[] nicks = cmd.Payload.Split(' ');
			foreach (string nick in nicks)
			{
				string n = nick;
				if (string.IsNullOrEmpty(n))
					continue;
				
				// TODO: something with ops
				if (n[0] == '@')
					n = n.Substring(1);

				Debug.WriteLine("Added user: " + n);
				User user = (User)Database.Users.Open(n, new object[] { n });
				Channel channel = (Channel)Database.Channels[cmd.Args[2]];
				channel.AddUser(user);
			}
		}
		
		void CommandPrivateMessage(CommandArgs cmd)
		{
			string text = cmd.Args[1];
			string channel_name = cmd.Args[0];
			string nick = cmd.Nick;
			
			IChannel channel = Database.Channels.Get(channel_name);
			IUser user = Database.Users.Get(nick);

			if (Bots.ContainsKey(channel))
			{
			    foreach (IBot bot in Bots[channel])
			    {
					if (bot.Handle(channel, user, text))
						return;
			    }
			}

			UserSay(server, channel, user, Database.Messages.Add(new Irc.Message(channel, user, text)));
		}

		void CommandJoin(CommandArgs cmd)
		{
			string channel_name = cmd.Args[0];
			string user_name = cmd.Nick;

			Channel channel = (Channel)Database.Channels.Open(channel_name).OrCreate(server, channel_name).Value;
			IUser user = Database.Users.Open(user_name).OrCreate(user_name).Value;

			channel.AddUser(user);

			AddServerMessage(channel, string.Format("{0} joined", cmd.Nick), "#e0ffe0", "#108010");
			UserJoin(server, channel, user);
		}

		void CommandPart(CommandArgs cmd)
		{
			string channel_name = cmd.Args[0];
			string user_name = cmd.Nick;

			Channel channel = (Channel)Database.Channels.Open(cmd.Args[0]).Value;
			IUser user = Database.Users.Open(cmd.Nick).Value;

			channel.RemoveUser(user);

			AddServerMessage(channel, string.Format("{0} left", cmd.Nick), "#ffe0e0", "#801010");
			UserLeave(server, channel, user);
		}

		void CommandConnected(CommandArgs args)
		{
			AddServerMessage(args.RawArgs);
			
			ServerJoin(Database.Servers[args.Prefix]);
		}

		string motd;
		void CommandStartMOTD(CommandArgs args)
		{
			motd = args.RawArgs;
		}
		void CommandAddMOTD(CommandArgs args)
		{
			motd += args.RawArgs;
		}
		void CommandEndMOTD(CommandArgs args)
		{
			//MessageBox.Show(motd, "Message of the Day", MessageBoxButtons.OK);
		}

		void CommandPing(CommandArgs cmd)
		{
			SendTextRaw("PONG " + cmd.Payload + "\r\n");
		}

		void CommandNotice(CommandArgs cmd)
		{
			AddServerMessage(cmd.Payload);
		}

		#endregion

		#region Events

		void ServerJoin(IServer server)
		{
			if (OnServerJoin != null)
				OnServerJoin(server);
		}

		void ServerLeave(IServer server)
		{
			if (OnServerLeave != null)
				OnServerLeave(server);
		}

		void UserJoin(IServer server, IChannel channel, IUser user)
		{
			if (OnUserJoin != null)
				OnUserJoin(server, channel, user);
		}

		void UserLeave(IServer server, IChannel channel, IUser user)
		{
			if (OnUserLeave != null)
				OnUserLeave(server, channel, user);
		}

		void UserSay(IServer server, IChannel channel, IUser user, IMessage message)
		{
			if (OnUserSay != null)
				OnUserSay(server, channel, user, message);
		}

		void UserEmote(IServer server, IChannel channel, IUser user, IMessage message)
		{
			if (OnUserEmote != null)
				OnUserEmote(server, channel, user, message);
		}

		void UserNotify(IServer server, IChannel channel, IUser user, IMessage message)
		{
			if (OnUserNotify != null)
				OnUserNotify(server, channel, user, message);
		}

		#endregion
	}
}

//EOF
