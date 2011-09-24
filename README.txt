No relation to http://code.google.com/p/tclsdl/
(which hasn't been updated since 2008)

Requires:
1) Tcl
2) SDL

Modify the Makefile to point to your Tcl and SDL installations and libs.

Currently the mixer loads a local mod (not in repo), more functionality coming
soon!

% wish &
% load ./sdlmix.dll
% sdl::music
(will play in a continuous loop)

