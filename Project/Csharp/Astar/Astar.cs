// (C) 2010 Christian.Schladetsch@Gmail.com
// Permission to use for any purpose is granted, as long as this notice remains in
// any derived works.

using System;
using System.Drawing;
using System.Collections;

namespace Astar3
{
	public class Search
	{
		public enum States { Unprepared, Prepared, Running, Failed, Succeeded }
        States state;
        States State { get { return state; } }

		Node start, goal;
		ArrayList open, closed;
		Map map;
		Node current;

        // setup to use a new map
		public void Prepare(Map new_map)
		{
			map = Node.map = new_map;
			Node.goal = map.goal;

			start = new Node(null, new Point(map.start.X, map.start.Y));
			goal = new Node(null, new Point(map.goal.X, map.goal.Y));

			open = new ArrayList();
			closed = new ArrayList();

			open.Add(start);
			
			state = States.Prepared;
		}

        // perform a single step of the A* algorithm
		public States Step()
		{
			if (open.Count == 0)
				return state = States.Failed;
			
			SortOpen();
			
			Node node = (Node)open[0];
			current = node;
			open.RemoveAt(0);
			closed.Add(node);
			if (node.SamePosition(goal))
				return state = States.Succeeded;

			ArrayList succ = node.GetSucessors();
			foreach (Node other in succ)
			{
				Node in_open = FindNode(other.GetPosition(), open);
				Node in_closed = FindNode(other.GetPosition(), closed);
				bool on_open = in_open != null;
				bool on_closed = in_closed != null;

				if (on_open || on_closed)
				{
					Node Q = on_open ? in_open : in_closed;
					if (other.GetTotalCost() > Q.GetTotalCost() || other.PathLength() > Q.PathLength())
						continue;
					if (on_open)
						RemoveOccurances(other, open);
					if (on_closed)
						RemoveOccurances(other, closed);
				}
				open.Add(other);
			}
			return state = States.Running;
		}

		void RemoveOccurances(Node node, ArrayList source)
		{
			ArrayList deathrow = new ArrayList();
			int index = 0;
			foreach (Node current in source)
			{
				if (current.SamePosition(node))
					deathrow.Add(index);
				++index;
			}
			foreach (int i in deathrow)
				source.RemoveAt(i);
		}

		class CompareCosts : IComparer
		{
			public int Compare(object A, object B)
			{
				Node P = (Node)A, Q = (Node)B;
				if (P.GetTotalCost() < Q.GetTotalCost())
					return -1;
				if (P.GetTotalCost() > Q.GetTotalCost())
					return 1;
				int N = P.PathLength(), M = Q.PathLength();
				if (N < M)
					return -1;
				if (N > M)
					return 1;
				return 0;
			}
		}

		void SortOpen()
		{
			CompareCosts compare_costs = new CompareCosts();
			open.Sort(compare_costs);
		}

		Node FindNode(Point point, ArrayList source)
		{
			foreach (Node node in source)
			{
				if (point.X == node.GetPosition().X && point.Y == node.GetPosition().Y)
					return node;
			}
			return null;
		}

		public ArrayList GeneratePath()
		{
			ArrayList path = new ArrayList();
			Node node = current;
			path.Add(node.GetPosition());
			while (node.GetParent() != null)
			{
				node = node.GetParent();
				path.Add(node.GetPosition());
			}
			return path;
		}
	}
}

//EOF
