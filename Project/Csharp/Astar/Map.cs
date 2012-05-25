// (C) 2010 Christian.Schladetsch@Gmail.com
// Permission to use for any purpose is granted, as long as this notice remains in
// any derived works.

using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;

namespace Astar3
{
	public class Map
	{
		int[,] cells;
		int width, height;

		public Point start, goal; 
		Brush start_pen = new SolidBrush(Color.Green);
		Brush goal_pen = new SolidBrush(Color.Red);
		Brush wall_pen = new SolidBrush(Color.Black);
			
		Brush pen_100 = new SolidBrush(Color.FromArgb(MakeGrey(1.00f)));
		Brush pen_75 = new SolidBrush(Color.FromArgb(MakeGrey(0.75f)));
		Brush pen_50 = new SolidBrush(Color.FromArgb(MakeGrey(0.50f)));
		Brush pen_25 = new SolidBrush(Color.FromArgb(MakeGrey(0.25f)));
		Brush pen_0 = new SolidBrush(Color.FromArgb(MakeGrey(0.00f)));

		Brush pen_wtf = new SolidBrush(Color.HotPink);

		public Map(int W, int H)
		{
			cells = new int[W,H];
			width = W;
			height = H;
		}

		public float TraversalCost(Point A, Point B)
		{
			return TraversalCost(A.X, A.Y, B.X, B.Y);
		}
		public float TraversalCost(int X0, int Y0, int X1, int Y1)
		{
			float dx = X1 - X0;
			float dy = Y1 - Y0;
			float distance = (float)Math.Sqrt(dx*dx + dy*dy);
			float hardship = (cells[X0,Y0] + cells[X1,Y1])/2.0f;
			hardship *= 0.2f;
			return distance*hardship;
		}

		public void SetHardship(int X, int Y, int H)
		{
			if (!ValidLocation(X,Y))
				return;
			cells[X,Y] = H;
		}
			
		public bool ValidLocation(int X, int Y)
		{
			return X > 0 && Y > 0 && X < width && Y < height;
		}

		public bool ValidLocation(Point P)
		{
			return ValidLocation(P.X, P.Y);
		}

		static int MakeGrey(float F)
		{
			char C = (char)(255*(1.0f - F));
			return 255 << 24 | C << 16 | C << 8 | C;
		}

		public void Draw(PaintEventArgs e, int grid_width)
		{
			for (int X = 0; X < width; ++X)
			{
				for (int Y = 0; Y < height; ++Y)
				{
					Brush B;
					switch (cells[X,Y])
					{
						case 100: B = pen_100; break;
						case 75: B = pen_75; break;
						case 50: B = pen_50; break;
						case 25: B = pen_25; break;
						case 0: B = null; break;
						default: B = pen_wtf; break;
					}
					if (B != null)
						e.Graphics.FillRectangle(B, X*grid_width,Y*grid_width,grid_width,grid_width);
				}
			}

			e.Graphics.FillRectangle(start_pen, start.X*grid_width,start.Y*grid_width,grid_width,grid_width);
			e.Graphics.FillRectangle(goal_pen, goal.X*grid_width,goal.Y*grid_width,grid_width,grid_width);
		}
	}
}

//EOF
