using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

using Barny.Irc;

namespace Barny
{
	public partial class BarnyWindow : Form
	{
		Barny.Properties.Settings settings = new Barny.Properties.Settings();
		Irc.Client irc_client;
		BarnyBot barny;
		CommandBot commands;

		public BarnyWindow()
		{
			InitializeComponent();
			RegisterTypes();
		}

		public void StartUp()
		{
			settings.Reload();
			if (settings.Version != "0.6a")
			{
				settings.Version = "0.6a";
				settings.Save();
				new Readme(this, true).Show();
			}
			else if (string.IsNullOrEmpty(settings.Nick))
			{
				MessageBox.Show("We need to setup your Barnage for the first time", "Welcome to the Barn");
				new IrcSettings(this).Show();
			}
			else if (MessageBox.Show(string.Format("Connect to {0}/{2} as {1} now?", settings.Server, settings.Nick, settings.Chat), "Connect to Server", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
				Connect();
		}

		void RegisterTypes()
		{
			Factory.BindType<IServer, Server>();
			Factory.BindType<IChannel, Channel>();
			Factory.BindType<IUser, User>();
			Factory.BindType<IMessage, Irc.Message>();
			Factory.BindType<IClip, Clip>();
		}


		Connecting connecting;
		public void Connect()
		{
			if (connecting != null)
				connecting.Dispose();

			settings.Reload();
			connecting = new Connecting(string.Format("Entering the Barn as {0}", settings.Nick));
			connecting.Show();
			toolStripStatusLabel1.Text = "Connecting to " + settings.Server;

			if (irc_client != null)
				irc_client.Disconnect();

			irc_client = new Irc.Client();

			irc_client.OnServerJoin += new Barny.Irc.Client.ServerJoinDelegate(irc_client_OnServerJoin);
			irc_client.OnUserJoin += new Barny.Irc.Client.UserJoinDelegate(irc_client_OnUserJoin);
			irc_client.OnUserLeave += new Barny.Irc.Client.UserLeaveDelegate(irc_client_OnUserLeave);

			try
			{
				irc_client.Connect();
			}
			catch (Exception E)
			{
				MessageBox.Show("Failed to connect to the Barn:\n\n " + E.ToString(), "No Barn for you", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
			}
			Channel channel = (Channel)irc_client.ServerChannel;
			channel.OnAddMessage += outputWindow1.AddMessage;

			//connectToolStripMenuItem.Text = "&Disconnect";
		}

		#region User Interface Events
		private void connectToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Connect();
		}

		void irc_client_OnUserLeave(IServer server, IChannel channel, IUser user)
		{
			Debug.WriteLine("irc_client_OnUserLeave");
		}

		void irc_client_OnUserJoin(IServer server, IChannel channel, IUser user)
		{
			if (user.Name != settings.Nick)
				return;

			TabPage page = new TabPage(channel.Name);
			OutputWindow output = new OutputWindow();
			Channel c = (Channel)channel;

			c.OnAddMessage += output.AddMessage;
			page.Controls.Add(output);
			page.Tag = c;
			tabControl1.TabPages.Add(page);
			tabControl1.SelectedTab = page;
			output.RefreshOutput();
			page.Refresh();
			tabControl1.Refresh();
			Database.Messages.Add(new Irc.Message(c, irc_client.ServerUser, "You have joined " + channel.Name));
		}

		void irc_client_OnServerJoin(IServer server)
		{
			Debug.WriteLine("irc_client_OnServerJoin");

			connecting.Dispose();
			connecting = null;
			toolStripStatusLabel1.Text = "In the barn";

			IChannel chat = Database.Channels[settings.Chat];
			IChannel data = Database.Channels[settings.Data];

			barny = new BarnyBot(this, irc_client, chat, Database.Channels[settings.Data]);
			commands = new CommandBot(irc_client);

			irc_client.AddBot(data, barny);
			irc_client.AddBot(commands);

			Database.Clips.OnAdd += new TwoWayRepository<IClip>.OnAddDelegate(barny.SendClip);
		}

		public void NewClip(IClip clip)
		{
			IUser user = clip.User;
			string msg = string.Format("{0} added a clip called '{1}'. View clips using Alt-L or View/Clips. Show the clip now?", user.Name, clip.Name);
			if (MessageBox.Show(msg, "New clip from " + user.Name, MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
				new ShowClip(clip).Show();
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
		{
		}

		private void outputWindow1_Click(object sender, EventArgs e)
		{
		}

		Channel GetCurrentChannel()
		{
			TabPage page = tabControl1.SelectedTab;
			if (page == null)
				return null;
			return (Channel)page.Tag;
		}

		private void richTextBox1_KeyDown(object sender, KeyEventArgs e)
		{
			if (irc_client != null && e.KeyCode == Keys.Enter)
			{
				irc_client.SendText(GetCurrentChannel(), richTextBox1.Text);
				richTextBox1.Clear();
				e.Handled = true;
			}
		}
		private void clipsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (Clips.Instance == null)
				new Clips().Show();
			else
				Clips.Instance.Dispose();
		}

		private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			new About().Show();
		}

		private void postFromClipboardToolStripMenuItem_Click(object sender, EventArgs e)
		{
			IDataObject Q = Clipboard.GetDataObject();
			if (Q == null)
				return;
			IUser user = null;
			if (irc_client != null)
				user = irc_client.User;
			new AddClip(new Clip(user, "unnamed", Q)).Show();
		}

		private void seToolStripMenuItem_Click(object sender, EventArgs e)
		{
			new IrcSettings(this).Show();
		}
		#endregion

		private void timer1_Tick(object sender, EventArgs e)
		{
			if (irc_client != null)
			{
				try
				{
					irc_client.ProcessPendingInput();
				}
				catch (Exception E)
				{
					MessageBox.Show("EXCEPTIONAL WORK!\n\n" + E.ToString(), "Awesome", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
			}
		}

		private void usersToolStripMenuItem_Click(object sender, EventArgs e)
		{
			new UserlistWindow(GetCurrentChannel()).Show();
		}

		private void paintToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Not implemented");
		}

		private void lOLToolStripMenuItem_Click(object sender, EventArgs e)
		{
			new Readme(this, false).Show();
		}

		private void showTimestampsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			outputWindow1.SetTimestamps(!outputWindow1.ShowTimestamps);
		}
	}
}

//EOF
