using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Barny.Irc
{
	public class Clip : IClip
	{
		private Guid id = Guid.NewGuid();
		private IUser user;
		private DateTime date_posted = DateTime.Now;
		private string title;
		private IDataObject contents;

		public Guid Id { get { return id; } }
		public Guid UserId { get { return user.Id; } }
		public IUser User { get { return user; } }
		public IDataObject Contents { get { return contents; } }
		public string Name { get { return title; } set { title = value; } }
		public DateTime DatePosted { get { return date_posted;  } }

		public Clip(IUser A, string T, IDataObject C)
		{
			user = A;
			contents = C;
			title = T;
		}

		public static Clip NewInstance(IUser A, string T, IDataObject G)
		{
			return new Clip(A, T, G);
		}
	}
}

//EOF
