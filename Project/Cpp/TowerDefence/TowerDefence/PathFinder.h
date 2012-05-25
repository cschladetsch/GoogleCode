// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_PATH_FINDER_H
#	define TD_PATH_FINDER_H

struct Tower;

/// @brief perform a search from a given location to a goal
///
/// this is an A-star implementation.
/// can be used as a generator, or as a blocking call. that is,
/// after initialisation, you can step the path-find algorithm
/// to return intermediate results, or call it to find the entire
/// path as required.
struct PathFinder
{
	/// a node in the path-finding system
	struct Node
	{
		Node *parent;
		float f;		///< estimated total distance from start to goal through this node
		float g;		///< distance of this node from start along optimal path
		float h;		///< heuristic estimate of distance from this node to goal
		Point point;	///< the location of this node
		int index;		///< the node number: index into pool
		bool in_open;	///< if true, node is in the open list, else in closed

		Node() : parent(0), f(0), g(0), h(0), point(0,0) { }
		Node(Point const &P) : parent(0), f(0), g(0), h(0), point(P) { }
	};

	enum WhichList
	{
		InOpen, InClosed, None
	};
	
	typedef std::vector<Node *> ClosedSet;
	typedef std::vector<Node *> OpenSet;
	typedef std::vector<Point> Path;
	typedef std::vector<WhichList> WhichListArray;

private:
	OpenSet open;
	ClosedSet closed;
	Node *start, *goal;
	bool completed;
	Path solution;
	WhichListArray which_list;
	int x_dim, y_dim;

	Pointer<ObjectBase> owner;

	std::vector<Node> pool;
	size_t pool_top;

	std::vector<Pointer<Tower> > towers;		///< the set of towers in the game

public:
	void SetOwner(Object object) { owner = object; }
	
	/// set the start and goal positions for a new search
	void Initialise(Point start_pos, Point goal_pos);

	/// perform a complete search
	Path Search(Point start_pos, Point goal_pos);

	/// incremental step of the path-search
	Node *Step();

	/// perform at most num_steps steps of the algorithm
	void Step(int num_steps);

	/// return true if the search has completed
	bool Completed() const { return completed; }

	/// get the current best solution
	Path GetPath() const;

	/// get the complete solution
	Path const &GetSolution() const { return solution; }

	/// for debugging: draw the current best path
	void Draw(SDL_Surface *) const;

	Point GetGoal() const { return goal->point; }

protected:
	void InitialisePool();

	// node management
	Node *NewNode(Point const &pos);
	void DeleteNode(Node *node);

	// list management
	void AddToOpen(Node *node);
	void AddToClosed(Node *node);
	bool InClosedSet(Node *node) const;
	bool InOpenSet(Node *node) const;

	Node *BestNextOption();

	/// path found via the given node; complete the search
	void Complete(Node *node);

	/// return true if the given node is the goal
	bool IsGoal(Node *node) const;

	/// return true if node A can reach node B, and on return distance is set
	bool Distance(Node *A, Node *B, float &distance);

	/// estimate the cost of traveling between two nodes 
	float GuessCost(Node *from, Node *to) const;
	
	/// reverse the path taken to reach the given node
	Path ReconstructPath(Node *node) const;
	
	void SetDimension(int x_dim, int y_dim);

	// returns true if the node exists within the container
	template <class Container>
	bool Contains(PathFinder::Node *node, Container const &cont);
	
	template <class Container>
	void Clear(Container &cont);

	WhichList &Which(Node *);
	WhichList Which(Node *) const;

	bool OutOfBounds(Node *) const;
};

#endif // TD_PATH_FINDER_H

//EOF
