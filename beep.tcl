#!/usr/bin/tclsh8.5
load ./sdlmix.dll

sdl::mix::channels 1

set h 0
set m 0
set s 0

switch $argc {
	1 {
		set s [lindex $argv 0]
	}

	2 {
		set m [lindex $argv 0]
		set s [lindex $argv 1]
	}

	3 {
		set h [lindex $argv 0]
		set m [lindex $argv 1]
		set s [lindex $argv 2]
	}

	default {
		puts "Usage $argv0 \[h\] \[m\] <s>"
		exit
	}
}

set t [expr {($h*3600 + $m*60 + $s)*1000}]

proc alarm {} {
	sdl::mix::sound pluck.wav
}

after $t
alarm
after 2000

