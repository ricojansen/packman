; Packman.krl Kludge Replacement List for PackMan
; Used by Packman to tell what to do with kludges (not listed is nothing)
;
; Use origin reroute table on address in kludge
PATH: %o
; same
PTH: %o
; Use destination reroute table on address in kludge
DESTADDR:%d
; Remove these kludges
EID:
FMPT
; Use destination reroute table on address in kludge
MSGTO: %d
; Change PID to PackMan
PID: PackMan 1.20
; Use origin reroute table on address in kludge and copy the rest
MSGID: %o blah
; Use destination reroute table on address in kludge and copy the rest
REPLY: %d blahto
; Remove these kludges
TOPT
XH3-75
XID:
