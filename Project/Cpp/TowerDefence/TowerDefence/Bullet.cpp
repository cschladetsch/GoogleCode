// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "Bullet.h"
#include "CGL/Draw.h"

CGL_BEGIN

void Bullet::Update(GameTime time)
{
	if (time.Now() > time_to_die)
	{
		Delete();
	}
}

void Bullet::Draw(Matrix const &)
{
	if (!target.Exists())
	{
		Delete();
		return;
	}
	Point pos = GetLocation();
	Point target_pos = target->GetLocation();
	Color color = GetGame()->MakeColor(255,0,255);
	SDL_Surface *surface = GetGame()->GetSurface();
	DrawLineSegment(surface, LineSegment(pos, target_pos), color);
}

CGL_END

//EOF
