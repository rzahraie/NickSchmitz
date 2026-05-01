rem @echo off
@
find "EXT," %1.xnf | find "I," >zzz.pin
@
echo 5,# d>zzz.eds
echo exit>>zzz.eds
@
czfix <zzz.eds >zzz.obj
edlin zzz.pin <zzz.obj >nul
@
find "EXT," %1.xnf | find "O," >>zzz.pin
@
echo 9,# d>zzz.eds
@
rem Ckt Inputs
echo 1,1 rEXT, @@Z1,# rd@@@¼0@~@@@@Z>>zzz.eds
echo 2,2 rEXT, @@Z1,# rd@@@¼1@~@@@@Z>>zzz.eds
echo 3,3 rEXT, @@Z1,# rd@@@¼2@~@@@@Z>>zzz.eds
echo 4,4 rEXT, @@Z1,# rd@@@¼3@~@@@@Z>>zzz.eds
@
rem Ckt Outputs
echo 5,5 rEXT, @@Z1,# rs@@@¼0@~@@@@Z>>zzz.eds
echo 6,6 rEXT, @@Z1,# rs@@@¼1@~@@@@Z>>zzz.eds
echo 7,7 rEXT, @@Z1,# rs@@@¼2@~@@@@Z>>zzz.eds
echo 8,8 rEXT, @@Z1,# rs@@@¼3@~@@@@Z>>zzz.eds
@
echo 1,# r, I,@@Z>>zzz.eds
echo 1,# r, O,@@Z>>zzz.eds
echo exit>>zzz.eds
@
czfix <zzz.eds >zzz.obj
czfix <zzz.obj >zzz.obk
edlin zzz.pin <zzz.obk >nul
@
echo exit>>zzz.pin
czfix <zzz.pin >zzz.obx
copy filler.xnf filler.xxx
edlin filler.xxx <zzz.obx >nul
@
del zzz.*
@
