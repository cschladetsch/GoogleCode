// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "SplashScreenPhase.h"
#include "PlayPhase.h"
#include "PathFinder.h"
#include "Runner.h"
#include "Tower.h"
#include "ToolBar.h"
#include "Player.h"
#include "Bullet.h"

/// @mainpage TowerDefence
///
/// A... tower defence game

//using namespace cgl;

void RegisterTypes(Factory &factory)
{
	factory.AddClass<Font>();
	factory.AddClass<Game>();
	factory.AddClass<SplashScreenPhase>();
	factory.AddClass<PlayPhase>();
	factory.AddClass<Tower>();
	factory.AddClass<Runner>();
	factory.AddClass<ToolBar>();
	factory.AddClass<World>();
	factory.AddClass<Player>();
	factory.AddClass<Bullet>();
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	Factory factory;
	RegisterTypes(factory);

	Pointer<Game> game = factory.New<Game>();
	Game *gg = &*game;
	game->Create(600, 400);
	if (!game->Initialised())
		return 1;
	game->SetTitle("Tower Defence");
	game->PhaseChange(game->NewGameObject<SplashScreenPhase>());

	GameTime time;
	Matrix transform;

	Pointer<Bullet> B = game->NewGameObject<Bullet>();
	Pointer<Tower> T = game->NewGameObject<Tower>();

	Pointer<ObjectBase> OB = B;
	Pointer<ObjectBase> OT = T;

	ObjectBase *PB = &*OB;
	ObjectBase *PT = &*OT;


	while (!game->Finished())
	{
    	time.StartFrame();
		game->Update(time);
    	game->Draw(transform);
	}
    return 0;
}

//EOF
