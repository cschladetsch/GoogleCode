namespace Barny
{
	partial class BarnyWindow
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BarnyWindow));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.mainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.seToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.usersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.paintToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.clipsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.actionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.postFromClipboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.lOLToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.richTextBox1 = new System.Windows.Forms.RichTextBox();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.showTimestampsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.outputWindow1 = new Barny.OutputWindow();
			this.menuStrip1.SuspendLayout();
			this.statusStrip1.SuspendLayout();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.actionToolStripMenuItem,
            this.helpToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(682, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// mainToolStripMenuItem
			// 
			this.mainToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.seToolStripMenuItem,
            this.connectToolStripMenuItem,
            this.exitToolStripMenuItem});
			this.mainToolStripMenuItem.Name = "mainToolStripMenuItem";
			this.mainToolStripMenuItem.Size = new System.Drawing.Size(46, 20);
			this.mainToolStripMenuItem.Text = "&Main";
			// 
			// seToolStripMenuItem
			// 
			this.seToolStripMenuItem.Name = "seToolStripMenuItem";
			this.seToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.I)));
			this.seToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
			this.seToolStripMenuItem.Text = "&Irc Settings";
			this.seToolStripMenuItem.Click += new System.EventHandler(this.seToolStripMenuItem_Click);
			// 
			// connectToolStripMenuItem
			// 
			this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
			this.connectToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.C)));
			this.connectToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
			this.connectToolStripMenuItem.Text = "&Connect";
			this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.X)));
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
			this.exitToolStripMenuItem.Text = "E&xit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// viewToolStripMenuItem
			// 
			this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.usersToolStripMenuItem,
            this.paintToolStripMenuItem,
            this.clipsToolStripMenuItem,
            this.showTimestampsToolStripMenuItem});
			this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
			this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.viewToolStripMenuItem.Text = "&View";
			// 
			// usersToolStripMenuItem
			// 
			this.usersToolStripMenuItem.Name = "usersToolStripMenuItem";
			this.usersToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.U)));
			this.usersToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
			this.usersToolStripMenuItem.Text = "User list";
			this.usersToolStripMenuItem.Click += new System.EventHandler(this.usersToolStripMenuItem_Click);
			// 
			// paintToolStripMenuItem
			// 
			this.paintToolStripMenuItem.Name = "paintToolStripMenuItem";
			this.paintToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.P)));
			this.paintToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
			this.paintToolStripMenuItem.Text = "&Paint";
			this.paintToolStripMenuItem.Click += new System.EventHandler(this.paintToolStripMenuItem_Click);
			// 
			// clipsToolStripMenuItem
			// 
			this.clipsToolStripMenuItem.Name = "clipsToolStripMenuItem";
			this.clipsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.L)));
			this.clipsToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
			this.clipsToolStripMenuItem.Text = "&Clips";
			this.clipsToolStripMenuItem.Click += new System.EventHandler(this.clipsToolStripMenuItem_Click);
			// 
			// actionToolStripMenuItem
			// 
			this.actionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.postFromClipboardToolStripMenuItem});
			this.actionToolStripMenuItem.Name = "actionToolStripMenuItem";
			this.actionToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
			this.actionToolStripMenuItem.Text = "&Action";
			// 
			// postFromClipboardToolStripMenuItem
			// 
			this.postFromClipboardToolStripMenuItem.Name = "postFromClipboardToolStripMenuItem";
			this.postFromClipboardToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
						| System.Windows.Forms.Keys.V)));
			this.postFromClipboardToolStripMenuItem.Size = new System.Drawing.Size(199, 22);
			this.postFromClipboardToolStripMenuItem.Text = "Paste &Clip";
			this.postFromClipboardToolStripMenuItem.Click += new System.EventHandler(this.postFromClipboardToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lOLToolStripMenuItem,
            this.aboutToolStripMenuItem});
			this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
			this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.helpToolStripMenuItem.Text = "&Help";
			// 
			// lOLToolStripMenuItem
			// 
			this.lOLToolStripMenuItem.Name = "lOLToolStripMenuItem";
			this.lOLToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
			this.lOLToolStripMenuItem.Text = "&Readme";
			this.lOLToolStripMenuItem.Click += new System.EventHandler(this.lOLToolStripMenuItem_Click);
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
			this.aboutToolStripMenuItem.Text = "&About";
			this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
			this.statusStrip1.Location = new System.Drawing.Point(0, 431);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(682, 22);
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
			this.toolStripStatusLabel1.Size = new System.Drawing.Size(39, 17);
			this.toolStripStatusLabel1.Text = "Ready";
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 24);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.richTextBox1);
			this.splitContainer1.Size = new System.Drawing.Size(682, 407);
			this.splitContainer1.SplitterDistance = 334;
			this.splitContainer1.TabIndex = 2;
			// 
			// tabControl1
			// 
			this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Bottom;
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Location = new System.Drawing.Point(0, 0);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(682, 334);
			this.tabControl1.TabIndex = 1;
			// 
			// richTextBox1
			// 
			this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.richTextBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.richTextBox1.Location = new System.Drawing.Point(0, 0);
			this.richTextBox1.Name = "richTextBox1";
			this.richTextBox1.Size = new System.Drawing.Size(682, 69);
			this.richTextBox1.TabIndex = 0;
			this.richTextBox1.Text = "";
			this.richTextBox1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.richTextBox1_KeyDown);
			// 
			// tabPage1
			// 
			this.tabPage1.Location = new System.Drawing.Point(0, 0);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Size = new System.Drawing.Size(200, 100);
			this.tabPage1.TabIndex = 0;
			// 
			// timer1
			// 
			this.timer1.Enabled = true;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// showTimestampsToolStripMenuItem
			// 
			this.showTimestampsToolStripMenuItem.Checked = true;
			this.showTimestampsToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
			this.showTimestampsToolStripMenuItem.Name = "showTimestampsToolStripMenuItem";
			this.showTimestampsToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
			this.showTimestampsToolStripMenuItem.Text = "&Show timestamps";
			this.showTimestampsToolStripMenuItem.Click += new System.EventHandler(this.showTimestampsToolStripMenuItem_Click);
			// 
			// outputWindow1
			// 
			this.outputWindow1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.outputWindow1.Location = new System.Drawing.Point(3, 3);
			this.outputWindow1.Name = "outputWindow1";
			this.outputWindow1.ShowTimestamps = false;
			this.outputWindow1.Size = new System.Drawing.Size(498, 416);
			this.outputWindow1.TabIndex = 0;
			this.outputWindow1.Text = "outputWindow1";
			this.outputWindow1.Click += new System.EventHandler(this.outputWindow1_Click);
			// 
			// BarnyWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(682, 453);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.menuStrip1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "BarnyWindow";
			this.Text = "Barny 0.6a";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem mainToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem usersToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem paintToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem clipsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.RichTextBox richTextBox1;
		private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
		private System.Windows.Forms.TabPage tabPage1;
		private OutputWindow outputWindow1;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.ToolStripMenuItem actionToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem postFromClipboardToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem lOLToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem seToolStripMenuItem;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.ToolStripMenuItem showTimestampsToolStripMenuItem;
	}
}

