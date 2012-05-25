// (C) 2009 christian.schladetsch.net

#include "CGL/Precompiled.h"

CGL_BEGIN

void Game::Create(int width, int height)
{
	this->game = this;

	initialised = false;
	screen = 0;

	if (!InitialiseSDL(width, height))
		return;

	font = Self->New<Font>();
	bool loaded = font->FromFile("font");
	assert(loaded);

	initialised = true;
	finished = false;
}

Game::~Game()
{
	phase.Delete();
	next_phase.Delete();
}

SDL_Surface *Game::GetSurface() const
{
	return screen;
}

GameTime Game::GetTime() const
{
	return time;
}

Color Game::MakeColor(int r, int g, int b) const
{
	return SDL_MapRGB(GetSurface()->format, r, g, b);
}

Time Game::TimeNow() const
{
	return time.TotalElapsedSeconds();
}

bool Game::Transitioning() const
{
	return next_phase.Exists() && time.IsAfter(transition_ends);
}

void Game::Transist()
{
	if (time.IsAfter(transition_ends))
	{
		EndTransition();
	}
}

void Game::EndTransition()
{
	if (phase.Exists())
	{
		phase->Leave(next_phase);
	}
	if (next_phase.Exists())
	{
		next_phase->Enter(phase);
	}

	phase.Delete();
	phase = next_phase;
	next_phase = 0;
	transition_ends = 0;
}

void Game::PhaseChange(Pointer<PhaseBase> next, Time transition_time)
{
	if (!next.Exists())
		return;
	if (Transitioning())
		EndTransition();
	transition_ends = TimeNow() + transition_time;
	next_phase = next;
}

void Game::Update(GameTime)
{
	time.StartFrame();

	SDL_LockAudio();
	{
		ParseInput();
		if (Transitioning())
		{
			Transist();
		}
		else if (phase.Exists())
		{
			phase->Update(time);
		}
	}
	SDL_UnlockAudio();

	Self->GetFactory()->Purge();

	if (finished)
	{
		SDL_CloseAudio();
	}
}

void Game::ParseInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (phase.Exists() && phase->InputEvent(event))
			continue;
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			finished = true;
		if (event.type == SDL_QUIT)
			finished = true;
	}
}

void Game::Draw(Matrix const &transform)
{
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 30, 30, 50));
	if (phase.Exists())
		phase->Draw(transform);
	SDL_Flip(screen);
}

float t_last;
bool first = true;
double target_frequency;
double frequency;

void AudioCallback(void *userdata, Uint8 *stream, int len)
{
//	Game *game = (Game *)userdata;
//	Phase::Play *play = game->GetPhase<Phase::Play>();
//	if (!play)
//	{
//		return;
//	}
//
//	typedef Sint8 Sample;
//	double playback_rate = 8000;
//	double dt = 1.0f/playback_rate;
//	int sample_num_bytes = sizeof(Sample);
//
//	target_frequency = 200;
//
//	Player *player = game->GetWorld()->GetPlayer();
//	Direction dir = player->GetDirection();
//
//	if (dir == Direction::None)
//	{
//		target_frequency = 0;
//	}
//
//	target_frequency += dir.value*200;
//	frequency += (target_frequency - frequency)/5;
//
//	Sample *buffer = (Sample *)stream;
//	int num_samples = len/sample_num_bytes;
//
//	double t = t_last;
//	for (int n = 0; n < num_samples; ++n)
//	{
//		double value = sin(t*frequency);
//		Sample sample = 127*value;
//		buffer[n] = sample;
//		t += dt;
//
////		// half-way through, move closer to target
////		if (n > num_samples/2)
////		{
////			frequency += (target_frequency - frequency)/2;
////		}
//	}
//	t_last = t;
}

bool Game::InitialiseSDL(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return false;
    }

    // make sure SDL clseans up before exit
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(width, height, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!screen)
    {
        printf("Unable to create window: %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec desired, obtained;

	// latency is freq/samples
    desired.freq = 8000;
    desired.format = AUDIO_S8;
    desired.samples = 256;
    desired.channels = 1;
    desired.callback = AudioCallback;
    desired.userdata = this;
    if (SDL_OpenAudio(&desired, &obtained) < 0)
    {
        printf("Unable to open audio device: %s\n", SDL_GetError());
        return 1;
    }

	printf("audio: freq=%d; samples=%d; channels=%d\n", obtained.freq, obtained.samples, obtained.channels);

	// unpause the audio :S
	SDL_PauseAudio(0);

	SetTitle("CGL Applicaiton");
	return true;
}

void Game::SetTitle(const char *text) const
{
	SDL_WM_SetCaption(text, "CGL-app");
}

CGL_END

//EOF
