using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Barny
{
	public partial class Connecting : Form
	{
		public Connecting()
		{
			InitializeComponent();
			timer1.Interval = 800;
		}

		public Connecting(string title)
		{
			InitializeComponent();
			Text = title;
			timer1.Interval = 800;
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			int progress = progressBar1.Value + 10;
			if (progress > progressBar1.Maximum)
				progress = 0;
			progressBar1.Value = progress;
		}
	}
}

//EOF
