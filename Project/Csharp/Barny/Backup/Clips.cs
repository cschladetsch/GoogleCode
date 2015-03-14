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
	public partial class Clips : Form
	{
		public static Clips Instance;

		public Clips()
		{
			InitializeComponent();
			Database.Clips.OnAdd += Add;
			Disposed += new EventHandler(Clips_Disposed);
			Instance = this;
			RefreshClips();
		}

		private void Clips_Disposed(object sender, EventArgs e)
		{
			Database.Clips.OnAdd -= Add;
			Instance = null;
		}

		public void Add(Irc.IClip C)
		{
			string owner = "anon";
			if (C.User!= null)
				owner = C.User.Name;
			string text = string.Format("{0}: {1}: {2}", C.DatePosted.ToShortDateString(), owner, C.Name);
			TreeNode node = new TreeNode(text);
			node.Tag = C;
			treeView1.Nodes.Add(node);
		}

		private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
		{
			clipboardView1.Show(treeView1.SelectedNode.Tag as Clip);
		}

		public void RefreshClips()
		{
			treeView1.Nodes.Clear();
			foreach (KeyValuePair<Guid, IClip> clip in Database.Clips.Dictionary)
				Add(clip.Value);
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}

//EOF
