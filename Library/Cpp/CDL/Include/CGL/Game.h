// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_GAME_H_INCLUDED
#	define CGL_GAME_H_INCLUDED

CGL_BEGIN

struct World;

/// @brief the toplevel game object.
///
/// stores the object factory for all other objects in the system.
/// manages the phase transitions and controls the main processing loop
struct Game : GameObject<Game>
{
	GameTime time;

	SDL_Surface *screen;
	bool initialised;
	bool finished;

	Pointer<PhaseBase> phase;			///< the current phase of the game
	Pointer<PhaseBase> next_phase;		///< the phase to transition to
	Time transition_ends;				///< when the current transition ends

	Pointer<Font> font;
	Pointer<World> world;

public:
	typedef Event<Game *, ObjectBase *> CreateEvent;
	CreateEvent OnCreate;

	void Create(int width, int height);
	~Game();

	/// top-level update method
	void Update(GameTime);

	/// top-level draw method
	void Draw(Matrix const &);

	/// returns true if the application is intending to close
	bool Finished() const { return finished; }

	/// returns true if all subsystems initialised correctly
	bool Initialised() const { return initialised; }

	/// returns the default font
	Pointer<Font> GetFont() const { return font; }

	/// returns the game-time structure
	GameTime GetTime() const;

	/// returns the time at the start of this frame
	Time TimeNow() const;

	SDL_Surface *GetSurface() const;
	Color MakeColor(int r, int g, int b) const;

	/// move to a new phase
	void PhaseChange(Pointer<PhaseBase> next_phase, Time transition_time = 0);

	void SetWorld(Pointer<World> W) { world = W; }
	Pointer<World> GetWorld() const { return world; }

	///// return true if currently in the given phase
	template <class T>
	bool InPhase() const { return phase.Exists() && phase.IsType<T>(); }

	/// return the current game phase base
	Pointer<PhaseBase> GetCurrentPhase() const { return phase; }

	/// set the window title
	void SetTitle(const char *) const;
	
protected:
	void ParseInput();

	/// returns true iff the game is currently moving between phases
	bool Transitioning() const;

	/// perform transition between levels
	void Transist();

	/// end the current transition now
	void EndTransition();

	/// prepare the SDL system
	bool InitialiseSDL(int width, int height);
};

CGL_TRAITS_DERIVED_REFLECTED(Game, CGL_TypeNumbers::Game, ObjectBase);

CGL_END

#endif // GAME_H_INCLUDED

//EOF
