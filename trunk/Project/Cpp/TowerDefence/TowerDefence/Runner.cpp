// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "PathFinder.h"
#include "Runner.h"

#include "Tower.h"

#include "CGL/Draw.h"
#include <iostream>

CGL_BEGIN

Runner::Runner()
{
	path_finder = 0;
	health = 100;					///< when this gets to zero, the runner is dead
	current_step = 0;
	total_distance = 0;
}

void Runner::SetGoal(Point goal)
{
	delete path_finder;

	path_finder = new PathFinder();
	path_finder->SetOwner(Self);

	Point loc = GetLocation();
	loc.x = (int)loc.x/10;
	loc.y = (int)loc.y/10;
	path_finder->Search(loc, goal);
	path = path_finder->GetSolution();
	current_step = 0;
	total_distance = 0;
}

void Runner::RecalcPath()
{
	SetGoal(path_finder->GetGoal());
}

void Runner::TakeDamage(float D)
{
	health -= D;
	if (health < 0)
	{
		Delete();
	}
}

void Runner::Update(GameTime time)
{
	total_distance += GetSpeed()*time.DeltaSeconds();

	int index = (int)total_distance;

	// at the end of the path
	if (index >= (int)path.size())
	{
		//this->Delete();
		return;
	}
	int prev = index - 1;
	if (prev < 0)
		prev = 0;
	float delta = total_distance - (int)total_distance;

	Point target = path[index];
	Point prev_point = path[prev];
	
	target *= 10;
	prev_point *= 10;
	Point loc = prev_point + (target - prev_point)*delta;
	SetLocation(loc);
}

void Runner::Draw(Matrix const &)
{
	SDL_Surface *surface = GetGame()->GetSurface();
	Point loc = GetLocation();
	DrawCircle(surface, loc.x, loc.y, GetRadius(), GetColor());
}

CGL_END

//EOF
