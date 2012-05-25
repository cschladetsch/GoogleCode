// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "Tower.h"
#include "Runner.h"
#include "Bullet.h"
#include "CGL/Draw.h"

CGL_BEGIN

Tower::Tower()
{
	firing_rate = 1.f;
	next_fire_time = 0;
	range = 50;
	damage = 50;
}

void Tower::Update(GameTime time)
{
	if (time.Now() < next_fire_time)
		return;
	Pointer<World> world = GetGame()->GetWorld();
	std::vector<Pointer<Runner> > runners = world->GetObjectsOfType<Runner>();
	foreach (Pointer<Runner> runner, runners)
	{
		Vector v = runner->GetLocation() - GetLocation();
		if (v.Length() > range)
			continue;
		
		next_fire_time = time.Now() + firing_rate;
		runner->TakeDamage(damage);
		Pointer<Bullet> bullet = NewGameObject<Bullet>();
		bullet->SetLocation(GetLocation());
		bullet->SetTrajectory(v);
		bullet->SetTimeToDie(time.Now() + 0.300);
		bullet->SetDamage(damage);
		bullet->SetTarget(runner);
		world->AddObject(bullet);

		return;
	}
}

void Tower::Draw(Matrix const &)
{
	SDL_Surface *surface = GetGame()->GetSurface();
	Point pos = GetLocation();
	Color color = GetGame()->MakeColor(255,255,0);
	DrawCircle(surface, pos.x, pos.y, GetRadius(), color);
}

CGL_END

//EOF
