#ifdef WIN32
#include <windows.h>
#endif
#include "tclparms.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <map>
#include <string>

//----------------------------------------------------------------------------
#define BUFFER 1024

//----------------------------------------------------------------------------
struct TclData
{
	Mix_Music *music_;
	std::map<std::string, Mix_Chunk*> sounds_;

public:
	TclData(void)
	{
		music_ = NULL;
	}

	~TclData(void)
	{
		if (music_)
		{
			Mix_FreeMusic(music_);
		}
	}

	bool playMusic(const char *filename, int loops)
	{
		// clear current music
		if (music_)
		{
			Mix_FreeMusic(music_);
			music_ = NULL;
		}

		if (!filename) return false; // some weird error

		music_ = Mix_LoadMUS(filename);
		if (!music_)
		{
			return true; // clear music
		}

		if (Mix_PlayMusic(music_, loops) == -1)
		{
			return false; // some weird error
		}

		return true;
	}

	bool loadSound(const char *filename)
	{
		std::map<std::string, Mix_Chunk*>::const_iterator i =
		  sounds_.find(filename);
		if (i != sounds_.end())
			return true;

		Mix_Chunk *chk = Mix_LoadWAV(filename);
		if (!chk)
		{
			return false;
		}

		sounds_.insert(std::make_pair(filename, chk));
		return true;
	}
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

	if (!self->playMusic(p.getStringParm(0), p[1]))
	{
		return TCL_ERROR;
	}
	return TCL_OK;
}

void destructor(ClientData clientData)
{
	delete reinterpret_cast<TclData*>(clientData);
	SDL_Quit();
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

	ns = Tcl_CreateNamespace(interp, "sdl::mix", NULL, NULL);
	Tcl_Export(interp, ns, "*", 0);

	TclData *self = new TclData;
	Tcl_CreateObjCommand(interp, "sdl::mix::music", musicCmd, self, destructor);

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
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}*/

