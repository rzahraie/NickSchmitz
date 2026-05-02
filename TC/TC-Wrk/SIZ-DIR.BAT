@if x%3x==xx echo off
rem Find out sizes of each directory on disk
rem Author: Nick Schmitz - 10/7/94
@
echo Checking Size on all top level dir's.
echo Working...
dir \*. /b | xlnx >zzz.bat
find/c "#" zzz.bat
@
echo echo @e1 # @~@~z-z.doc >zzz.eds
echo echo @e1 ... >>zzz.eds
echo dir/s @e1 @?? tail @?? find "(" @~zzz.tmp>>zzz.eds
echo edlin zzz.tmp @@@??zzz.obk @~nul >>zzz.eds
echo type zzz.tmp @~@~z-z.doc >>zzz.eds
@
czfix <zzz.eds >zzz.obl
czfix <zzz.obl >zzz1.bat
@
echo 1,# r##@Zcall zzz1 >zzz.eds
echo exit >>zzz.eds
@
echo 1,1 d >zzz.obk
echo exit >>zzz.obk
@
czfix <zzz.eds >zzz.obj
edlin zzz.bat <zzz.obj >nul
@
call zzz
@
del zzz*.*
@
echo TOP # >>\z-z.doc
dir \ | find "(" >>\z-z.doc
echo Done.
@
type \z-z.doc
:dne
