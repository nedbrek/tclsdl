#ifdef WIN32
#include <windows.h>
#endif
#include "tclparms.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

//----------------------------------------------------------------------------
#define BUFFER 1024

//----------------------------------------------------------------------------
struct TclData
{
	Mix_Music *music_;

	void init(void) { music_= NULL; }
};

//----------------------------------------------------------------------------
/*void print_init_flags(int flags)
{
#define PFLAG(a) if(flags&MIX_INIT_##a) printf(#a " ")
   PFLAG(FLAC);
   PFLAG(MOD);
   PFLAG(MP3);
   PFLAG(OGG);

   if (!flags)
      printf("None");

   printf("\n");
}*/

static
int musicCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	if (objc == 1) return TCL_ERROR;

	TclData *self = reinterpret_cast<TclData*>(cdata);

	Parms p(interp, objv, unsigned(objc));
	const char *subcmd = p.getStringParm(0);
	if (strcmp(subcmd, "play") != 0)
		return TCL_ERROR;

	if (self->music_)
	{
		Mix_FreeMusic(self->music_);
		self->music_ = NULL;
	}

	const char *file = p.getStringParm(1);
	if (!file) return TCL_ERROR;

	self->music_ = Mix_LoadMUS(file);
	if (!self->music_)
	{
		return TCL_ERROR;
	}

	if (Mix_PlayMusic(self->music_, -1) == -1)
	{
		return TCL_ERROR;
	}

	return TCL_OK;
}

extern "C" int Sdlmix_Init(Tcl_Interp *interp);

int Sdlmix_Init(Tcl_Interp *interp)
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		return TCL_ERROR;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, BUFFER) < 0)
	{
		SDL_Quit();
		return TCL_ERROR;
	}

	// deallocate default channels
	Mix_AllocateChannels(0);

	Tcl_Namespace *ns = Tcl_FindNamespace(interp, "sdl", NULL, 0);
	if (!ns)
	{
		ns = Tcl_CreateNamespace(interp, "sdl", NULL, NULL);
	}

	Tcl_Export(interp, ns, "*", 0);

	TclData *self = (TclData*)malloc(sizeof(TclData));
	self->init();
	Tcl_CreateObjCommand(interp, "sdl::music", musicCmd, self, NULL);

	return TCL_OK;
}

/*
	// determine caps
	int initted = Mix_Init(0);
	print_init_flags(initted);
	initted=Mix_Init(~0);
	print_init_flags(initted);
	Mix_Quit();
*/

/*int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	while (1)
	{
		while(Mix_PlayingMusic() || Mix_PausedMusic())
		{
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
					cleanExit();
			}
			SDL_Delay(0);
		}

		Mix_PlayMusic(music, 1);
	}

	Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}*/

