#include <tcl.h>
#include <windows.h>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#define BUFFER 1024

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
	Mix_Music *music = Mix_LoadMUS("commndr.mod");
	if (!music)
	{
		return TCL_ERROR;
	}

	if (Mix_PlayMusic(music, -1) == -1)
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

	Tcl_CreateObjCommand(interp, "sdl::music", musicCmd, NULL, NULL);

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

