// (C) 2009 christian.schladetsch@gmail.com

#include "TowerDefence.h"
#include "SplashScreenPhase.h"
#include "PlayPhase.h"

CGL_BEGIN

void SplashScreenPhase::Update(GameTime)
{
}

void SplashScreenPhase::Draw(Matrix const &)
{
	Pointer<Font> font = GetGame()->GetFont();
	char *text = "tower-defence";
	size_t len = strlen(text);
	Matrix matrix = Matrix::Translate(-4*(int)len - 5, -4)*Matrix::Scale(5, 5)*Matrix::Translate(300,200);
	//Matrix matrix;// = Matrix::Translate(300,200);
	font->DrawShadowedText(GetGame()->GetSurface(), matrix, Box(), GetGame()->MakeColor(255,255,0), text);

	Time now = GetGame()->GetTime().Now();
	SDL_Surface *surface = GetGame()->GetSurface();

	font->DrawText(surface, Matrix::Translate(20,4), Box(), GetGame()->MakeColor(100,100,100), "build 0-1");

	// draw 'press space' zooming in and out
	{
		std::string text = "press-space";
		size_t len = text.size();

		float scale = 4 + 1.5*sin(now*5);

		Matrix M =
			Matrix::Translate(-(int)len*4 - 2,-4)
			*Matrix::Scale(scale,scale)
			*Matrix::Translate(300,260);

		font->DrawShadowedText(
			surface
			, M
			, Box(Point(50,50), Point(400,200))
			, GetGame()->MakeColor(255,255,255)
			, text.c_str());
	}

}

bool SplashScreenPhase::InputEvent(SDL_Event const &event)
{
	bool space = event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE;
	if (space)
	{
		GetGame()->PhaseChange(NewGameObject<PlayPhase>());
	}
	return space;
}

void SplashScreenPhase::Enter(Pointer<PhaseBase> previous)
{
}

void SplashScreenPhase::Leave(Pointer<PhaseBase> next)
{
}

CGL_END

//EOF
