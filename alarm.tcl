package require Tk
load ./sdlmix.dll

set ::ct 0

sdl::mix::channels 16

### timer
proc decList {} {
	for {set i 0} {$i < $::ct} {incr i} {
		set t [.tMain.f$i.lC cget -text]
		if {$t > 0} {
			incr t -1
			if {$t == 0} {
				sdl::mix::sound pluck.wav
			}

			.tMain.f$i.lC configure -text $t
		}
	}
}

proc timerCB {} {
	set t [expr [clock milliseconds] + 1000]
	decList
	after [expr {$t - [clock milliseconds]}] timerCB
}

### gui helpers
proc entryGet {w} {
	set ret [$w get]
	if {[string is integer -strict $ret]} {
		return $ret
	}
	return 0
}

proc setTm {ct} {
	set w .tMain.f$ct.lC
	if {[$w cget -text] != 0} {
		if {[tk_messageBox -message "Overwrite existing timer?" -type okcancel] ne "ok"} {
			return
		}
	}

	set h [entryGet .tMain.f$ct.eH]
	set m [entryGet .tMain.f$ct.eM]
	set s [entryGet .tMain.f$ct.eS]
	set t [expr {$h*3600 + $m*60 + $s}]

	$w configure -text $t
}

#### gui
proc addLine {txt} {
	pack [frame .tMain.f$::ct] -side top -expand 1 -fill x
	pack [label .tMain.f$::ct.lT -text $txt] -side left
	pack [label .tMain.f$::ct.lC -text "0"] -side left
	pack [entry .tMain.f$::ct.eH -width 3] -side left
	pack [entry .tMain.f$::ct.eM -width 3] -side left
	pack [entry .tMain.f$::ct.eS -width 3] -side left
	pack [button .tMain.f$::ct.bSet -text "Set" -command [list setTm $::ct]] -side left

	incr ::ct
}

wm withdraw .
toplevel .tMain


