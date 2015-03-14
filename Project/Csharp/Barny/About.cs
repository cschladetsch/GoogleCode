using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Barny
{
	public partial class About : Form
	{
		public About()
		{
			InitializeComponent();
			richTextBox1.Text = "Visit us at irc://lol.com/#ok";
		}

		private void button1_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}