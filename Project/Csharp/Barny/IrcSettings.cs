using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Barny
{
	public partial class IrcSettings : Form
	{
		Barny.Properties.Settings settings = new Barny.Properties.Settings();
		BarnyWindow window;

		public IrcSettings(BarnyWindow W)
		{
			InitializeComponent();
			window = W;

			Server.Text = settings.Server;
			Port.Text = settings.Port.ToString();
			Nick.Text = settings.Nick;
			Chat.Text = settings.Chat;
			Data.Text = settings.Data;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			settings.Server = Server.Text;
			settings.Port = Port.Text;
			settings.Nick = Nick.Text;
			settings.Chat = Chat.Text;
			settings.Data = Data.Text;
			settings.Save();

			if (MessageBox.Show(string.Format("Connect to {0} as {1} now?", settings.Server, settings.Nick), "Connect to Server", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
				window.Connect();

			Close();
		}

		private void Cancel_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}