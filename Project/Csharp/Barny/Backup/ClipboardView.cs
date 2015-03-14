using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace Barny
{
	public partial class ClipboardView : Control
	{
		public ClipboardView()
		{
			InitializeComponent();
			ShowBrowser();
			webBrowser1.DocumentText = "<h1>Empty</h1>";
		}

		public void Show(Irc.IClip clip)
		{
			if (clip.Contents.GetDataPresent(DataFormats.Html, false))
			{
				ShowBrowser();
				string blurb = clip.Contents.GetData(DataFormats.Html).ToString();
				Debug.WriteLine(blurb);
				int start_html = blurb.IndexOf("<html>");

				string html = blurb.Substring(start_html);
				webBrowser1.DocumentText = html;
				webBrowser1.Refresh();
			}
			else
			{
				ShowRichText();

				if (clip.Contents.GetDataPresent(DataFormats.Rtf, true))
					richTextBox1.Rtf = clip.Contents.GetData(DataFormats.Rtf, true).ToString();
				else if (clip.Contents.GetDataPresent(DataFormats.Text, true))
					richTextBox1.Text = clip.Contents.GetData(DataFormats.Text, true).ToString();
				else
					richTextBox1.Text = "<cannot display>";
			}
		}

		private void ShowBrowser()
		{
			SuspendLayout();
			Controls.Remove(richTextBox1);
			Controls.Add(webBrowser1);
			webBrowser1.Dock = DockStyle.Fill;
			ResumeLayout();
		}

		private void ShowRichText()
		{
			SuspendLayout();
			Controls.Remove(webBrowser1);
			Controls.Add(richTextBox1);
			richTextBox1.Dock = DockStyle.Fill;
			ResumeLayout();
		}

		protected override void OnPaint(PaintEventArgs pe)
		{
			base.OnPaint(pe);
		}
	}
}

//EOF
