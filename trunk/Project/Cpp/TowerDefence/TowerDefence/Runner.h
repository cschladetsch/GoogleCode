// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_RUNNER_H
#	define TD_RUNNER_H

struct PathFinder;

/// attempts to make it to the exit without getting blasted by the towers
struct Runner : GameObject<Runner>
{
	PathFinder *path_finder;	///< used to generate a path to follow
	float health;					///< when this gets to zero, the runner is dead
	int current_step;
	float total_distance;
	std::vector<Point> path;

public:
	Runner();

	void SetGoal(Point);
	void TakeDamage(float);
	void RecalcPath();

	// overrides
	void Update(GameTime);
	void Draw(Matrix const &);
};

TD_GAME_OBJECT(Runner);

#endif // TD_RUNNER_H

//EOF
