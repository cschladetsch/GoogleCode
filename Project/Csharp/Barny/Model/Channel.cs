using System;
using System.Collections.Generic;
using System.Text;

namespace Barny.Irc
{
	public class Channel : IChannel
	{
		string name;
		IServer server;
		Guid id = Guid.NewGuid();
		List<IUser> users = new List<IUser>();
		List<IMessage> messages = new List<IMessage>();

		public delegate void AddUserDelegate(IUser user);
		public delegate void RemoveUserDelegate(IUser user);
		public delegate void AddMessageDelegate(IMessage message);

		public event AddUserDelegate OnAddUser;
		public event RemoveUserDelegate OnRemoveUser;
		public event AddMessageDelegate OnAddMessage;
		
		public string Name { get { return name; } set { name = value; } }
		public IServer Server { get { return server; } }
		public Guid Id { get { return id; } }
		public List<IUser> Users { get { return users; } }
		public List<IMessage> Messages { get { return messages;  } }
		
		public Channel(IServer S, string N)
		{
			name = N;
			server = S;
		}

		public static Channel NewInstance(IServer S, string N)
		{
			return new Channel(S, N);
		}

		public void AddUser(IUser user)
		{
			if (users.Contains(user))
				return;
			users.Add(user);
			if (OnAddUser != null)
				OnAddUser(user);
		}

		public void RemoveUser(IUser user)
		{
			users.Remove(user);
			if (OnRemoveUser != null)
				OnRemoveUser(user);
		}

		public void AddMessage(IMessage message)
		{
			messages.Add(message);
			if (OnAddMessage != null)
				OnAddMessage(message);
		}
	}
}

//EOF
