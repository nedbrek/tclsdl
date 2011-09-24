No relation to http://code.google.com/p/tclsdl/
(which hasn't been updated since 2008)

Requires:
1) Tcl (http://www.tcl.tk/)
2) SDL (http://www.libsdl.org/)
3) SDL Mixer (http://www.libsdl.org/projects/SDL_mixer/)

Modify the Makefile to point to your Tcl and SDL installations and libs.

Currently the mixer can play a local mod (not in repo), more functionality
coming soon!

% wish &
% load ./sdlmix.dll
% sdl::music play <file>
(will play in a continuous loop)

