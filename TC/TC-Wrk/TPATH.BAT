@echo off
if x%1x==xx goto dne
@
rem -----------------------------------------------------------
@
echo ----------------------- >%1.p
echo Timing Paths for %1.SDL>>%1.p
echo ----------------------->>%1.p
@
echo 1.On-chip Fmax >>%1.p
find " to " %1.sdl | find    "Clock" | find   "Setup" >f-x.p
f-x <%1.sdl >>%1.p
@
echo . >>%1.p
echo 2.Clock to Out >>%1.p
find " to " %1.sdl | find    "Clock" | find/v "Setup" >f-x.p
f-x <%1.sdl >>%1.p
@
echo . >>%1.p
echo 3.Clock Distribution >>%1.p
find " to " %1.sdl | find   " Clock" | find/v "Setup" >f-x.p
f-x <%1.sdl >>%1.p
@
echo . >>%1.p
echo 4.External Setup >>%1.p
find " to " %1.sdl | find/v  "Clock" | find    " to " >f-x.p
f-x <%1.sdl >>%1.p
@
echo . >>%1.p
echo 5.Tpd - Asynch Propagation (Vectored) >>%1.p
find " to " %1.sdl | find/v  "Clock" | find/v "Setup" | find "<" >f-x.p
f-x <%1.sdl >>%1.p
@
echo . >>%1.p
echo 6.Tpd - Asynch Propagation (Non-Vectored) >>%1.p
find " to " %1.sdl | find/v  "Clo" | find/v "Set" | find " to " | find/v "<" >f-x.p
f-x <%1.sdl >>%1.p
@
rem -----------------------------------------------------------
@
echo __ON_CHIP>%1.tpi
find " to " %1.sdl | find    "Clock" | find   "Setup" >f-x.p
f-x 1  <%1.sdl >>%1.tpi
echo __END>>%1.tpi
@
echo __PAD_CONTROL>>%1.tpi
find " to " %1.sdl | find/v  "Clock" | find "Setup"  >f-x.p
f-x 1  <%1.sdl >>%1.tpi
echo __END>>%1.tpi
@
rem echo __PAD_CLOCK>>%1.tpi
echo __PAD_MINI>>%1.tpi
rem find " to " %1.sdl | find/v  "Clock" | find "Setup"  >f-x.p
find "FF Se" %1.sdl | find "EC" >f-x.p
f-x 1  <%1.sdl >>%1.tpi
echo __END>>%1.tpi
@
echo __CLOCK_OUT>>%1.tpi
find " to " %1.sdl | find "Clock" | find/v "Setup" | find "net" >f-x.p
f-x 1  <%1.sdl >>%1.tpi
echo __END>>%1.tpi
@
echo __PAD_PAD>>%1.tpi
find " to " %1.sdl | find/v  "Clock" | find/v "Setup" | find "<" >f-x.p
f-x 1  <%1.sdl >>%1.tpi
echo __END>>%1.tpi
@
echo __PAD_PAD>>%1.tpi
find/v "Clock" %1.sdl | find/v "Setup" | find/v "<" | find " to ">f-x.p
f-x 1  <%1.sdl >>%1.tpi
echo __END>>%1.tpi
@
echo __END>>%1.tpi
@
rem -----------------------------------------------------------
@
:dne
