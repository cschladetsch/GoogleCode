using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Window
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
			LoadConfigs();
		}

		void LoadConfigs()
		{
		//	System.IO.Directory.SetCurrentDirectory(Program.StartPath);
			Debug.Print("pwd={0}", System.IO.Directory.GetCurrentDirectory());
			System.IO.Directory.SetCurrentDirectory(@"..\..\");

			var files = System.IO.Directory.GetFiles("cfgs", "*.cfg", System.IO.SearchOption.TopDirectoryOnly);
			foreach (var file in files)
			{
				var name = System.IO.Path.GetFileNameWithoutExtension(file);
				listBox1.Items.Add(name);
			}
		}

		private void listBox1_LocationChanged(object sender, EventArgs e)
		{
		}

		private void toolStripButton1_Click(object sender, EventArgs e)
		{
			var sel = listBox1.SelectedItem;
			if (sel == null)
				return;
			var cfgname = (string)sel;
			var filename = string.Format("cfgs\\{0}.cfg", cfgname);
			Debug.Print("loading {0}", filename);
		}
	}
}
