using System;
using System.Collections.Generic;
using System.Text;

namespace Barny.Irc
{
	public class Database
	{
		private static Database database = new Database();

		private TwoWayRepository<IServer> servers = new TwoWayRepository<IServer>();
		private TwoWayRepository<IChannel> channels = new TwoWayRepository<IChannel>();
		private TwoWayRepository<IUser> users = new TwoWayRepository<IUser>();
		private TwoWayRepository<IClip> clips = new TwoWayRepository<IClip>();
		private Repository<IMessage> messages = new Repository<IMessage>();

		public static TwoWayRepository<IUser> Users { get { return Get().users; } }
		public static TwoWayRepository<IClip> Clips { get { return Get().clips; } }
		public static TwoWayRepository<IServer> Servers { get { return Get().servers; } }
		public static TwoWayRepository<IChannel> Channels { get { return Get().channels; } }
		public static Repository<IMessage> Messages { get { return Get().messages; } }

		public static Database Get()
		{
			if (database == null)
				database = new Database();
			return database;
		}
	}
}

//EOF
