// (C) 2010 Christian.Schladetsch@Gmail.com

using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;

namespace Astar3
{
	public class Form1 : System.Windows.Forms.Form
	{
		public Form1()
		{
			InitializeComponent();
		}
		protected override void Dispose( bool disposing )
		{
			base.Dispose( disposing );
		}
		#region Windows Form Designer generated code
		private System.Windows.Forms.ToolBar toolBar1;
		private System.Windows.Forms.ToolBarButton toolBarButton1;
		private System.Windows.Forms.ToolBarButton toolBarButton2;
		private System.Windows.Forms.ToolBarButton toolBarButton3;
		private System.Windows.Forms.ToolBarButton toolBarButton4;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.ToolBarButton toolBarButton8;
		private System.Windows.Forms.ToolBarButton toolBarButton9;
		private System.Windows.Forms.ToolBarButton toolBarButton11;
		private System.Windows.Forms.ToolBarButton toolBarButton12;
		private System.Windows.Forms.ToolBarButton toolBarButton10;
		private System.Windows.Forms.ToolBarButton toolBarButton13;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.SaveFileDialog saveFileDialog1;
		private System.ComponentModel.IContainer components;
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.toolBar1 = new System.Windows.Forms.ToolBar();
            this.toolBarButton10 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton13 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton1 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton2 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton3 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton4 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton8 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton9 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton11 = new System.Windows.Forms.ToolBarButton();
            this.toolBarButton12 = new System.Windows.Forms.ToolBarButton();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // toolBar1
            // 
            this.toolBar1.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
            this.toolBarButton10,
            this.toolBarButton13,
            this.toolBarButton1,
            this.toolBarButton2,
            this.toolBarButton3,
            this.toolBarButton4,
            this.toolBarButton8,
            this.toolBarButton9,
            this.toolBarButton11,
            this.toolBarButton12});
            this.toolBar1.DropDownArrows = true;
            this.toolBar1.ImageList = this.imageList1;
            this.toolBar1.Location = new System.Drawing.Point(0, 0);
            this.toolBar1.Name = "toolBar1";
            this.toolBar1.ShowToolTips = true;
            this.toolBar1.Size = new System.Drawing.Size(455, 28);
            this.toolBar1.TabIndex = 0;
            this.toolBar1.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.toolBar1_ButtonClick);
            // 
            // toolBarButton10
            // 
            this.toolBarButton10.ImageIndex = 9;
            this.toolBarButton10.Name = "toolBarButton10";
            // 
            // toolBarButton13
            // 
            this.toolBarButton13.ImageIndex = 10;
            this.toolBarButton13.Name = "toolBarButton13";
            // 
            // toolBarButton1
            // 
            this.toolBarButton1.ImageIndex = 1;
            this.toolBarButton1.Name = "toolBarButton1";
            // 
            // toolBarButton2
            // 
            this.toolBarButton2.ImageIndex = 0;
            this.toolBarButton2.Name = "toolBarButton2";
            // 
            // toolBarButton3
            // 
            this.toolBarButton3.ImageIndex = 2;
            this.toolBarButton3.Name = "toolBarButton3";
            // 
            // toolBarButton4
            // 
            this.toolBarButton4.ImageIndex = 3;
            this.toolBarButton4.Name = "toolBarButton4";
            // 
            // toolBarButton8
            // 
            this.toolBarButton8.ImageIndex = 4;
            this.toolBarButton8.Name = "toolBarButton8";
            // 
            // toolBarButton9
            // 
            this.toolBarButton9.ImageIndex = 5;
            this.toolBarButton9.Name = "toolBarButton9";
            // 
            // toolBarButton11
            // 
            this.toolBarButton11.ImageIndex = 7;
            this.toolBarButton11.Name = "toolBarButton11";
            // 
            // toolBarButton12
            // 
            this.toolBarButton12.ImageIndex = 8;
            this.toolBarButton12.Name = "toolBarButton12";
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "");
            this.imageList1.Images.SetKeyName(1, "");
            this.imageList1.Images.SetKeyName(2, "");
            this.imageList1.Images.SetKeyName(3, "");
            this.imageList1.Images.SetKeyName(4, "");
            this.imageList1.Images.SetKeyName(5, "");
            this.imageList1.Images.SetKeyName(6, "");
            this.imageList1.Images.SetKeyName(7, "");
            this.imageList1.Images.SetKeyName(8, "");
            this.imageList1.Images.SetKeyName(9, "");
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "\"Astar Maps|*.astar\"";
            this.openFileDialog1.InitialDirectory = "\\storage card\\projects\\astar\\maps";
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.FileName = "doc1";
            this.saveFileDialog1.Filter = "\"Astar Maps|*.astar\"";
            this.saveFileDialog1.InitialDirectory = "\\storage card\\projects\\astar\\maps";
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(455, 362);
            this.Controls.Add(this.toolBar1);
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "Sp4m\'s old Astar Demo";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.OnMouseUp);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnMouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnMouseMove);
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		static void Main() 
		{
			Application.Run(new Form1());
		}

		Map map;
		const int grid_width = 10;

		private void OnPaint(object sender, PaintEventArgs e)
		{
			if (map == null)
			{
				// do this now rather than in ctor cos i dont know how to get the clip rect
				int width = e.ClipRectangle.Width/grid_width;
				int height = e.ClipRectangle.Height/grid_width;
				map = new Map(width, height);
			}

			//DrawGrid(e);
			
			map.Draw(e, grid_width);
			if (solution != null)
				DrawSolution(e);
		}

		void DrawGrid(PaintEventArgs e)
		{
			Pen pen = new Pen(Color.Black);

			int height = e.ClipRectangle.Height;
			int width = e.ClipRectangle.Width;

			for (int X = 0; X < width; X += grid_width)
				e.Graphics.DrawLine(pen, X, 0, X, height);

			for (int Y = 0; Y < height; Y += grid_width)
				e.Graphics.DrawLine(pen, 0, Y, width, Y);
		}

		public Point ToMapSpace(int X, int Y)
		{
			return new Point(X/grid_width, Y/grid_width);
		}

		bool mouse_down = false;
		bool place_start = false;
		bool place_goal = false;

		private void OnMouseDown(object sender, MouseEventArgs e)
		{
			Point P = ToMapSpace(e.X, e.Y);
			if (place_start)
			{
				map.start = P;
				place_start = false;
				Refresh();
				return;
			}
			if (place_goal)
			{
				map.goal = P;
				place_goal = false;
				Refresh();
				return;
			}
			mouse_down = true;
			map.SetHardship(P.X, P.Y, solidity);
			Refresh();
		}

		private void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			Point P = ToMapSpace(e.X, e.Y);
			if (mouse_down)
			{
				map.SetHardship(P.X, P.Y, solidity);
				Refresh();
			}
		}

		private void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			mouse_down = false;
		}

		int solidity = 100;

		private void toolBar1_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
		{
			if (e.Button == toolBarButton1)
				place_start = true;
			if (e.Button == toolBarButton2)
				place_goal = true;
			if (e.Button == toolBarButton3)
				Run();
			if (e.Button == toolBarButton4)
				solidity = 100;
			if (e.Button == toolBarButton8)
				solidity = 75;
			if (e.Button == toolBarButton9)
				solidity = 50;
			if (e.Button == toolBarButton11)
				solidity = 25;
			if (e.Button == toolBarButton12)
				solidity = 0;
		}

		ArrayList solution;

		void Run()
		{
			Search search = new Search();
			search.Prepare(map);
			while (search.Step() == Search.States.Running)
			{
				solution = search.GeneratePath();
				Refresh();
			}
			solution = search.GeneratePath();
			Refresh();
		}

		void DrawSolution(PaintEventArgs e)
		{
			Brush pen = new SolidBrush(Color.Purple);
			foreach (Point P in solution)
			{
				e.Graphics.FillRectangle(pen, P.X*grid_width,P.Y*grid_width,grid_width,grid_width);
			}
		}
		
	}	
}

//EOF