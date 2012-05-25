// (C) 2010 Christian.Schladetsch@Gmail.com
// Permission to use for any purpose is granted, as long as this notice remains in
// any derived works.

using System;
using System.Drawing;
using System.Collections;

namespace Astar3
{
	public class Node
	{
		Point location;
		Node parent;
		float local_cost, total_cost;

		public static Point goal;
		public static Map map;

		public Node(Node P, Point L)
		{
			parent = P;
			location = L;
			if (parent != null)
			{
				float traversal_cost = map.TraversalCost(parent.location, location);
				local_cost = parent.local_cost + traversal_cost;
				total_cost = local_cost + GuessCost(location, goal);
			}
			else
				total_cost = 0;
		}

		float GuessCost(Point P, Point G)
		{
			Point D = new Point(G.X - P.X, G.Y - P.Y);
			return (float)Math.Sqrt(D.X*D.X + D.Y*D.Y);
		}

		public bool SamePosition(Node P)
		{
			return location.X == P.location.X && location.Y == P.location.Y;
		}

		public float GetTotalCost()
		{
			return total_cost;
		}

		public int PathLength()
		{
			int N = 0;
			Node P = parent;
			while (P != null)
			{
				++N;
				P = P.parent;
			}
			return N;
		}

		public Node GetParent()
		{
			return parent;
		}
		
		public Point GetPosition()
		{
			return location;
		}

		public ArrayList GetSucessors()
		{
			ArrayList S = new ArrayList();
			int X = location.X, Y = location.Y;
			AddSuccessor(S,X-1,Y  );
			AddSuccessor(S,X-1,Y-1);
			AddSuccessor(S,X  ,Y-1);
			AddSuccessor(S,X+1,Y-1);
			AddSuccessor(S,X+1,Y  );
			AddSuccessor(S,X+1,Y+1);
			AddSuccessor(S,X  ,Y+1);
			AddSuccessor(S,X-1,Y+1);
			return S;
		}

		void AddSuccessor(ArrayList S, int X, int Y)
		{
			if (!map.ValidLocation(X,Y))
				return;
			if (parent != null 
				&& parent.location.X == X 
				&& parent.location.Y == Y)
				return;
			S.Add(new Node(this, new Point(X, Y)));
		}
	}
}

//EOF
