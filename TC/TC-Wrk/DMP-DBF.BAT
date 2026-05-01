@echo off
rem build & execute batch file for contents of TC2k watch list
if not x%1x==xx goto bld
@
dir \tc2000\watch\???.dbf /o:d | tail
echo q File Date
goto dne
@
:bld
echo @@echo off >zzz.z
echo if exist zzz.r del zzz.r >>zzz.z
echo if exist zzz.s del zzz.s >>zzz.z
echo @@>>zzz.z
dbf \tc2000\watch\%1.dbf | xlnx >>zzz.z
echo @@>>zzz.z
echo find "*r*" zzz.r @¼ find "$" @¼ sort/+7 @~zzz.s >>zzz.z
echo type zzz.s >>zzz.z
@
czfix <zzz.z >zzz.bat
@
echo 1,# r##@Zstkscn>>zzz.eds
echo 1,# r,0.0 $$@Z %2 2.9 @~@~zzz.r>>zzz.eds
echo exit>>zzz.eds
czfix <zzz.eds >zzz.obj
edlin zzz.bat <zzz.obj >nul
call zzz
@
if not x%3x==xx del zzz.*
set CMDLINE=q %1 %2 %3
:dne
