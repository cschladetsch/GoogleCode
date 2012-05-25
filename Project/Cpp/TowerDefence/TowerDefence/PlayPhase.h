// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_PLAY_PHASE_H
#	define TD_PLAY_PHASE_H

CGL_BEGIN

struct PathFinder;
struct Playfield;

/// the main game-play phase
struct PlayPhase : Reflected<PlayPhase>, PhaseBase
{
private:
	std::auto_ptr<PathFinder> path_finder;
	std::auto_ptr<Playfield> playfield;

public:
	PlayPhase();

	// over-rrides for PhaseBase
	void Enter(Pointer<PhaseBase> previous);
	bool InputEvent(SDL_Event const &);
	void Leave(Pointer<PhaseBase> next);

	// over-rides for ActiveObject
	void Update(GameTime);
	void Draw(Matrix const &);

protected:
	void AddTower(Point where);
	void DrawGrid() const;
};

CGL_TRAITS_DERIVED_REFLECTED(PlayPhase, CGL_TypeNumbers::PhaseBase, PhaseBase);

CGL_END

#endif // TD_PLAY_PHASE_H

//EOF
