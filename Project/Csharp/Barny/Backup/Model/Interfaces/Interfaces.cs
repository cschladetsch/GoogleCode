using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace Barny
{
	public interface IHasName
	{
		string Name { get; set; }
	}

	public interface IHasGuid
	{
		Guid Id { get; }
	}

	public interface IHasNameAndGuid : IHasName, IHasGuid
	{
	}
}

namespace Barny.Irc
{
	public interface IServer : IHasNameAndGuid
	{
	}

	public interface IChannel : IHasNameAndGuid
	{
		IServer Server { get; }
		List<IUser> Users { get; }
		List<IMessage> Messages { get; }

		void AddUser(IUser user);
		void AddMessage(IMessage message);
	}

	public interface IUser : IHasNameAndGuid
	{
	}

	public interface IClip : IHasNameAndGuid
	{
		IUser User { get; }
		IDataObject Contents { get; }
		DateTime DatePosted { get; }
	}

	public interface IMessage : IHasGuid
	{
		IUser User { get; }
		IChannel Channel { get; }
		DateTime DatePosted { get; }
	}
}

//EOF

