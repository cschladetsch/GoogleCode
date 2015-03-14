// (C) 2009 christian.schladetsch@gmail.com

#include "CGL/Precompiled.h"
#include "CGL/World.h"
#include <algorithm>

CGL_BEGIN

World::~World()
{
	foreach (Object object, objects)
	{
		object.Delete();
	}
}

void World::Update(GameTime time)
{
	// copy the object list, then update the objects
	std::vector<Pointer<ObjectBase> > tmp(objects.begin(), objects.end());
	foreach (Pointer<ObjectBase> object, tmp)
	{
//		object.Method(&ObjectBase::Update, time);
		if (object.Exists())
			object->Update(time);
	}

	// remove deleted objects
	Objects::iterator object = objects.begin();
	while (object != objects.end())
	{
		if (!object->Exists())
		{
			object = objects.erase(object);
		}
		else
		{
			++object;
		}
	}
}

void World::Draw(Matrix const &transform)
{
//	SDL_Surface *surface = GetGame()->GetSurface();
	foreach (Pointer<ObjectBase> object, objects)
	{
		ObjectBase *ptr = &*object;
		ptr->Draw(transform);
	}
}

void World::AddObject(Object object)
{
	if (object.Exists())
		objects.push_back(object);
}

/*
 function A*(start,goal)
     closedset := the empty set                 % The set of nodes already evaluated.
     openset := set containing the initial node % The set of tentative nodes to be evaluated.
     g_score[start] := 0                        % Distance from start along optimal path.
     h_score[start] := heuristic_estimate_of_distance(start, goal)
     f_score[start] := h_score[start]           % Estimated total distance from start to goal through y.
     while openset is not empty
         x := the node in openset having the lowest f_score[] value
         if x = goal
             return reconstruct_path(came_from,goal)
         remove x from openset
         add x to closedset
         foreach y in neighbor_nodes(x)
             if y in closedset
                 continue
             tentative_g_score := g_score[x] + dist_between(x,y)
             tentative_is_better := false
             if y not in openset
                 add y to openset
                 h_score[y] := heuristic_estimate_of_distance(y, goal)
                 tentative_is_better := true
             elseif tentative_g_score < g_score[y]
                 tentative_is_better := true
             if tentative_is_better = true
                 came_from[y] := x
                 g_score[y] := tentative_g_score
                 f_score[y] := g_score[y] + h_score[y]
     return failure
*/

CGL_END

//EOF
