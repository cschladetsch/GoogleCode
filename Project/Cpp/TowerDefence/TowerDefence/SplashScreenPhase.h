// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_SPLASH_SCREEN_PHASE_H
#	define TD_SPLASH_SCREEN_PHASE_H

CGL_BEGIN

struct SplashScreenPhase : Reflected<SplashScreenPhase>, PhaseBase
{
	// over-rides for ActiveObject
	void Update(GameTime);
	void Draw(Matrix const &);

	// over-rrides for PhaseBase
	bool InputEvent(SDL_Event const &);
	void Enter(Pointer<PhaseBase> previous);
	void Leave(Pointer<PhaseBase> next);
};

CGL_TRAITS_DERIVED_REFLECTED(SplashScreenPhase, TypeNumbers::SplashScreenPhase, PhaseBase);

CGL_END

#endif // TD_SPLASH_SCREEN_PHASE_H

//EOF
