// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "PathFinder.h"
#include "Tower.h"
#include "CGL/Draw.h"
#include "CGL/Timer.h"
#include <iostream>

void PathFinder::InitialisePool()
{
	pool.resize(1000);
	pool_top = 1;	// index 0 is reserved to act as null
	start = goal = 0;

	x_dim = 200;
	y_dim = 200;
	which_list.resize(x_dim*y_dim, None);
}

void PathFinder::Initialise(Point start_pos, Point goal_pos)
{
	InitialisePool();

	Clear(open);
	Clear(closed);
	completed = false;

	start = NewNode(start_pos);
	goal = NewNode(goal_pos);

	start->g = 0;
	start->h = GuessCost(start, goal);
	start->f = start->h;

	AddToOpen(start);

	Pointer<World> world = owner->GetGame()->GetWorld();
	towers = world->GetObjectsOfType<Tower>();
}

PathFinder::Path PathFinder::Search(Point start_pos, Point goal_pos)
{
	Initialise(start_pos, goal_pos);
	for (;;)
	{
		Node *current = Step();
		if (!current)
			break;
		if (Completed())
			return solution;
	}
	return Path();
}

PathFinder::Path PathFinder::GetPath() const
{
	if (Completed())
		return solution;
	if (open.empty())
		return Path();
	return ReconstructPath(open.front());
}

void PathFinder::Step(int num_steps)
{
	for (int n = 0; n < num_steps; ++n)
	{
		Step();
		if (Completed())
			break;
	}
}

struct CompareCost
{
	bool operator()(PathFinder::Node *A, PathFinder::Node *B) const
	{
		return A->f > B->f;
	}
};

struct CompareCostMin
{
	bool operator()(PathFinder::Node *A, PathFinder::Node *B) const
	{
		return A->f < B->f;
	}
};

PathFinder::Node *PathFinder::Step()
{
	// check the next best option
	Node *current = BestNextOption();
	
	// search completed
	if (IsGoal(current))
	{
		Complete(current);
		return current;
	}

	// no available alternatives
	if (current == 0)
		return 0;

	//std::cout << "current: " << current->point.x << ", " << current->point.y 
	//	<< "; g=" << current->g
	//	<< "; h=" << current->h
	//	<< "; f=" << current->f
	//	<< "\n";
	
	// add the node to the list of searched items
	AddToClosed(current);

	// search surrounding nodes
	Point offsets[] = 
	{ 
		Point(-1,0), Point(-1,-1), Point(0,-1), Point(1,-1),
		Point(1,0), Point(1,1), Point(0,1), Point(-1,1) 
	};
	foreach (Point offset, offsets)
	{
		Node *node = NewNode(current->point + offset);

		if (OutOfBounds(node) || InClosedSet(node))
		{
			// already looked here
			DeleteNode(node);	
			continue;
		}
		
		// g is the actual cost to node
		// h is a guess of cost from a node to goal,
		// f = g + h
		float distance = 0;
		bool can_reach = Distance(current, node, distance);
		if (!can_reach)
		{
			DeleteNode(node);
			continue;
		}
		
		float g = current->g + distance;
		if (!InOpenSet(node))
		{
			// this node hasn't been searched yet, so
			// add it to the open list of alternatives
			node->parent = current;
			node->h = GuessCost(node, goal);
			node->g = g;
			node->f = node->g + node->h;
			AddToOpen(node);
		}
		else if (g < node->g)
		{
			// this position is already in the open set,
			// but the path through the new node is better
			// than the existing path
			node->parent = current;
			node->g = g;
			node->f = node->g + node->h;
			
			// since we changed the node's cost, need to remake the heap
			std::make_heap(open.begin(), open.end(), CompareCost());
		}
	}
	
	return current;
}

bool PathFinder::OutOfBounds(Node *node) const
{
	float x = node->point.x;
	float y = node->point.y;
	return x < 0 || y < 0 || x >= x_dim || y >= y_dim;
}

void PathFinder::Complete(Node *node)
{
	completed = true;
	solution = ReconstructPath(node);
}

PathFinder::Node *PathFinder::BestNextOption()
{
	if (open.empty())
		return 0;
	std::pop_heap(open.begin(), open.end(), CompareCost());
	Node *best = open.back();
	open.pop_back();
	return best;
}

bool PathFinder::IsGoal(Node *node) const
{
	return node && goal && node->point == goal->point;
}

bool PathFinder::Distance(Node *from, Node *to, float &dist)
{
	if (OutOfBounds(to))
		return false;

	foreach (Pointer<Tower> tower, towers)
	{
		Point pos = tower->GetLocation();
		pos /= 10.0f;
		float distance = (to->point - pos).Length();
		if (distance < (owner->GetRadius() + tower->GetRadius())/10)
			return false;
	}
	dist = (from->point - to->point).Length();
	return true;
}

template <class Container>
void PathFinder::Clear(Container &cont)
{
	foreach (PathFinder::Node *node, cont)
	{
		delete node;
	}
	cont.clear();
}

float PathFinder::GuessCost(Node *from, Node *to) const
{
	return (from->point - to->point).Length();
}

PathFinder::Path PathFinder::ReconstructPath(Node *node) const
{
	// determine the length of the path
	int length = 0;
	for (Node *current = node; current != 0; current = current->parent)
		++length;

	// create the path from last to first
	Path path(length);
	for (int n = length - 1; node != 0; node = node->parent, --n)
	{
		path[n] = node->point;
	}
	return path;
}

PathFinder::Node *PathFinder::NewNode(Point const &pos)
{
	return new Node(pos);
}

void PathFinder::DeleteNode(Node *node)
{
	delete node;
}

void PathFinder::AddToOpen(Node *node)
{
	node->in_open = true;
	open.push_back(node);
	std::push_heap(open.begin(), open.end(), CompareCost());
	Which(node) = InOpen;
}

void PathFinder::AddToClosed(Node *node)
{
	node->in_open = false;
	closed.push_back(node);
	Which(node) = InClosed;
}

bool PathFinder::InClosedSet(Node *node) const
{
	return Which(node) == InClosed;
}

bool PathFinder::InOpenSet(Node *node) const
{
	return Which(node) == InOpen;
}

PathFinder::WhichList &PathFinder::Which(Node *node)
{
	int index = node->point.x + node->point.y*y_dim;
	return which_list[index];
}

PathFinder::WhichList PathFinder::Which(Node *node) const
{
	int index = node->point.x + node->point.y*y_dim;
	return which_list[index];
}

void PathFinder::Draw(SDL_Surface *surface) const
{
	if (open.empty())
		return;
	Path path;
	if (Completed())
	{
		path = solution;
	}
	else
	{
		//Node *top = std::min_element(open.begin(), open.end(), CompareCost());//open.front();
		Node *top = open.front();
		path = ReconstructPath(top);
	}
//	std::cout << "\npath:\n";

	Color white = SDL_MapRGB(surface->format, 255,255,255);
	Color red = SDL_MapRGB(surface->format, 255,0,0);
	Color green = SDL_MapRGB(surface->format, 0,255,0);

	Point origin(0,0);
	foreach (Point point, path)
	{
		Point local(point.x*10, point.y*10);
		Point p = origin + local;
		DrawCircle(surface, p.x, p.y, 5, white);
	}

	foreach (Node *node, closed)
	{
		Point local(node->point.x*10, node->point.y*10);
		Point p = origin + local;
		DrawCircle(surface, p.x, p.y, 3, red);
	}
	foreach (Node *node, open)
	{
		Point local(node->point.x*10, node->point.y*10);
		Point p = origin + local;
		DrawCircle(surface, p.x, p.y, 3, green);
	}
}

//EOF
