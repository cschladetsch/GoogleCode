// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "Playfield.h"

void Playfield::Initialise(int x_dim, int y_dim)
{
	this->x_dim = x_dim;
	this->y_dim = y_dim;
	map.resize(x_dim*y_dim, TypeNumbers::Empty);
}

bool Playfield::OutOfBounds(int x, int y) const
{
	return x < 0 || y < 0 || x >= x_dim || y >= y_dim;
}

void Playfield::Clear()
{
	std::fill(map.begin(), map.end(), TypeNumbers::Empty);
}

void Playfield::Set(int x, int y, TypeNumber num)
{
	if (OutOfBounds(x,y))
		return;
	map[x + y*x_dim] = num;
}

TypeNumber Playfield::Get(int x, int y) const
{
	if (OutOfBounds(x,y))
		return TypeNumbers::Empty;
	return map[x + y*x_dim];
}

//EOF
