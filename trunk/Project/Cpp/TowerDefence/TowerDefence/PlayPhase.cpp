// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "CGL/Timer.h"
#include "CGL/Draw.h"

#include "SplashScreenPhase.h"
#include "PlayPhase.h"
#include "PathFinder.h"
#include "Runner.h"
#include "Tower.h"
#include "Playfield.h"

#include <iostream>

PlayPhase::PlayPhase()
	: playfield(new Playfield())
{
}

void PlayPhase::Enter(Pointer<PhaseBase> previous)
{
	Timer::Initialise();

	GetGame()->SetWorld(NewGameObject<World>());

	playfield->Initialise(60,40);
}

void PlayPhase::Leave(Pointer<PhaseBase> next)
{
	GetGame()->GetWorld()->Delete();
	GetGame()->SetWorld(0);
}

void PlayPhase::Update(GameTime time)
{
	GetGame()->GetWorld()->Update(time);
	//if (!path_finder->Completed())
	//	path_finder->Step();
}

void PlayPhase::Draw(Matrix const &transform)
{
	DrawGrid();

	GetGame()->GetWorld()->Draw(transform);
//	path_finder->Draw(GetGame()->GetSurface());

}

void PlayPhase::DrawGrid() const
{
	SDL_Surface *surface = GetGame()->GetSurface();
	Color color = GetGame()->MakeColor(60,60,80);
	int width = 600;
	int height = 400;
	int spacing = 10;
	for (int x = 0; x < width; x += spacing)
	{
		DrawLineSegment(surface, LineSegment(Point(x,0), Point(x,height - 1)), color);
	}
	for (int y = 0; y < height; y += spacing)
	{
		DrawLineSegment(surface, LineSegment(Point(0,y), Point(width-1,y)), color);
	}
}

void PlayPhase::AddTower(Point where)
{
	Pointer<Tower> tower = NewGameObject<Tower>();
	tower->SetLocation(where);
	tower->SetRadius(10);
	GetGame()->GetWorld()->AddObject(tower);

	GetGame()->GetWorld()->ForEach(&Runner::RecalcPath);
}

bool PlayPhase::InputEvent(SDL_Event const &event)
{
	int window_width = 600;
	int window_height = 400;

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			Point where(event.button.x, event.button.y);
			if (where.x >= window_width || where.y >= window_height)
			{
				break;
			}
			where.x = (int)(where.x/20);
			where.y = (int)(where.y/20);
			where *= 20;
			AddTower(where);
		}
		break;
	case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_p:
				//delete path_finder;
				//path_finder = new PathFinder();
				//path_finder->SetOwner(Self);
				//path_finder->Initialise(Point(0,0), Point(60,40));
				return true;;
			case SDLK_ESCAPE:
				{
					GetGame()->PhaseChange(NewGameObject<SplashScreenPhase>());
					return true;
				}
				break;
			case SDLK_SPACE:
				{
//					path_finder->Initialise(Point(0,0), Point(5,5));
					Pointer<Runner> runner = NewGameObject<Runner>();
					runner->SetLocation(Point(600,400));
					runner->SetSpeed(5);
					runner->SetRadius(10);
					runner->SetColor(GetGame()->MakeColor(255,255,255));
					Timer timer;
					timer.Start();
					runner->SetGoal(Point(0,0));
					timer.Stop();
					std::cout << "pathfind took " << timer.GetElapsedMillis() << "ms\n";
					GetGame()->GetWorld()->AddObject(runner);
				}
				return true;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

//EOF
