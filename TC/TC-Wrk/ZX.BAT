@echo off
echo @@echo off >zzz.z
if not x%3x==xx echo if exist zf.log del zf.log >>zzz.z
echo @@>>zzz.z
dir/b %2 | xlnx >>zzz.z
echo @@>>zzz.z
@
echo 1,# r##@Zfind %1  >zzz.eds
echo 1,# r$$@Z @¼ c-x @~@~zf.log >>zzz.eds
echo exit>>zzz.eds
czfix <zzz.eds >zzz.obj
edlin zzz.z <zzz.obj >nul
@
czfix <zzz.z >zzz.bat
@
call zzz
del zzz.*
@
:dne
