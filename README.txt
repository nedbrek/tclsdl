No relation to http://code.google.com/p/tclsdl/
(which hasn't been updated since 2008)

Requires:
1) Tcl (http://www.tcl.tk/)
2) SDL (http://www.libsdl.org/)
3) SDL Mixer (http://www.libsdl.org/projects/SDL_mixer/)

The version of SDL built with MinGW requires extra files when linked with C++
code.

So, either you need to run wish from a MinGW term (irritating), or you need
two additional files in the current dir, or path.  They are:
libgcc_s_dw2-1.dll
libstdc++-6.dll

With the addition of more C++ code, I am unable to compile in Cygwin,
only MinGW...

Modify the Makefile to point to your Tcl and SDL installations and libs.

Currently the mixer can play a local mod (not in repo), more functionality
coming soon!

% wish &
% load ./sdlmix.dll
% sdl::mix::music <file>
(will play in a continuous loop)
% sdl::mix::channels 16
% sdl::mix::sound <file>
(plays once)
% sdl::mix::sound <file> -1 1
(channel=-1 (first free), 1 loop (play twice))

