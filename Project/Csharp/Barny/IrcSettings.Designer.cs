namespace Barny
{
	partial class IrcSettings
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.Server = new System.Windows.Forms.TextBox();
			this.Port = new System.Windows.Forms.TextBox();
			this.Nick = new System.Windows.Forms.TextBox();
			this.Chat = new System.Windows.Forms.TextBox();
			this.Data = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.Cancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 21);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(38, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Server";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 51);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(26, 13);
			this.label2.TabIndex = 1;
			this.label2.Text = "Port";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 75);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(29, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Nick";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 103);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(29, 13);
			this.label4.TabIndex = 3;
			this.label4.Text = "Chat";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(12, 132);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(30, 13);
			this.label5.TabIndex = 4;
			this.label5.Text = "Data";
			// 
			// Server
			// 
			this.Server.Location = new System.Drawing.Point(74, 21);
			this.Server.Name = "Server";
			this.Server.Size = new System.Drawing.Size(100, 20);
			this.Server.TabIndex = 5;
			// 
			// Port
			// 
			this.Port.Location = new System.Drawing.Point(74, 47);
			this.Port.Name = "Port";
			this.Port.Size = new System.Drawing.Size(100, 20);
			this.Port.TabIndex = 5;
			// 
			// Nick
			// 
			this.Nick.Location = new System.Drawing.Point(74, 73);
			this.Nick.Name = "Nick";
			this.Nick.Size = new System.Drawing.Size(100, 20);
			this.Nick.TabIndex = 1;
			// 
			// Chat
			// 
			this.Chat.Location = new System.Drawing.Point(74, 99);
			this.Chat.Name = "Chat";
			this.Chat.Size = new System.Drawing.Size(100, 20);
			this.Chat.TabIndex = 5;
			// 
			// Data
			// 
			this.Data.Location = new System.Drawing.Point(74, 125);
			this.Data.Name = "Data";
			this.Data.Size = new System.Drawing.Size(100, 20);
			this.Data.TabIndex = 5;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(15, 174);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 6;
			this.button1.Text = "OK";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// Cancel
			// 
			this.Cancel.Location = new System.Drawing.Point(123, 174);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 7;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
			// 
			// IrcSettings
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(214, 220);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.Data);
			this.Controls.Add(this.Chat);
			this.Controls.Add(this.Nick);
			this.Controls.Add(this.Port);
			this.Controls.Add(this.Server);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Name = "IrcSettings";
			this.Text = "IrcSettings";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox Server;
		private System.Windows.Forms.TextBox Port;
		private System.Windows.Forms.TextBox Nick;
		private System.Windows.Forms.TextBox Chat;
		private System.Windows.Forms.TextBox Data;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button Cancel;
	}
}