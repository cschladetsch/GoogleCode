using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Barny.Irc;

namespace Barny
{
	public partial class AddClip : Form
	{
		Clip clip;

		public AddClip(Clip clip)
		{
			this.clip = clip;
			InitializeComponent();
			clipboardView1.Show(clip);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			clip.Name = this.textBox2.Text;
			Database.Clips.Add(clip);
			Close();
		}

		private void button2_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}

//EOF
