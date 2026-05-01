@echo off
rem --------------------------
rem Dos Script file for JHSCN
rem A: Nick Schmitz
rem D: 10/06/99
rem --------------------------
if x%1x==x.x goto tst
if x%1x==x!x goto tstall
@
if exist quotes.csv goto day
:nite
rem --------------------------
echo Nite run of universe...
jhscn univ + >zzz.z
@
echo ........................>>r-jh.txt
echo Nite.>>r-jh.txt
find "!$" zzz.htm | find "!" >>r-jh.txt
find "!!" zzz.z
find "sell," zzz.z | find "-" | sort /+71 | tail 3
goto dne2
@
:day
rem --------------------------
echo Day run of selected stocks...
jhscn quote + + >zzz.q
@
echo .>>r-jh.txt
echo Day.>>r-jh.txt
find "!$" zzz.htm | find "!" >>r-jh.txt
@
if exist quotes.sav del quotes.sav
ren quotes.csv quotes.sav
goto dne2
@
:tstall
rem --------------------------
echo Testing...
del zzz.z
del zz.*
@
echo @M@Z | czfix >zzz.cr
time <zzz.cr >>zzz.z
jhscn univ + >>zzz.z
time <zzz.cr >>zzz.z
del zzz.cr
@
echo Extraction...
find ",!," zzz.z /v >zzz.x
@
goto wrp
echo Do summary...
echo -------------------->>x.x
echo #Trades>>x.x
find "$!$" zzz.x | find "!"   | sort /+20 | tail -3 >>x.x
find "$!$" zzz.x | find "!"   | sort /+20 | tail  3 >>x.x
@
echo #Day>>x.x
find "$!$" zzz.x | find "!"   | sort /+27 | tail -3 >>x.x
find "$!$" zzz.x | find "!"   | sort /+27 | tail  3 >>x.x
@
echo $Day>>x.x
find "$!$" zzz.x | find "-"/v | sort /+46 | tail  3 >>x.x
@
echo -------------------->>x.x
@
@
echo lmt codes - b,c,d,e >>x.x
find "lmt" zzz.x /c | find "-" >>x.x
find "lmt" zzz.x | find "#b"/c >>x.x
find "lmt" zzz.x | find "#c"/c >>x.x
find "lmt" zzz.x | find "#d"/c >>x.x
find "lmt" zzz.x | find "#e"/c >>x.x
@
echo -------------------->>x.x
echo buy codes - 1,2,3>>x.x
find "buy"  zzz.x /c | find "-" >>x.x
find "buy"  zzz.x | find "#1"/c >>x.x
find "buy"  zzz.x | find "#2"/c >>x.x
find "buy"  zzz.x | find "#3"/c >>x.x
@
echo sell codes - p,v >>x.x
find "sell"  zzz.x /c | find "-" >>x.x
find "sell" zzz.x | find "$p"/c >>x.x
find "sell" zzz.x | find "$v"/c >>x.x
echo -------------------->>x.x
@
find "$!$" zzz.x | find "-"   | sort /+35 | tail 3 >>x.x
find "$!$" zzz.x | find "-"/v | sort /+35 | tail 3 >>x.x
@
:wrp
echo Wrap-up...
find "!!" zzz.x >>x.x
find "sell," zzz.x | find "-" | sort /+71 | tail 3 >>x.x
type x.x
@
:tst
@
jhscn apwr + + +  | find "***" >za.csv
jhscn apwr + +   >zz.a
jhscn sfp  + + +  | find "***" >zs.csv
jhscn sfp  + +   >zz.s
goto dne
jhscn RIMG + + >zz.r
jhscn JAKK + + >zz.j
jhscn DLIA + + >zz.d
jhscn UNFY + + >zz.u
@
goto dne
rem --------------------------
echo lmt codes - l,m,o,a >>x.x
find ",!," zzz.z | find "+l" /c >>x.x
find ",!," zzz.z | find "+m" /c >>x.x
find ",!," zzz.z | find "+o" /c >>x.x
find ",!," zzz.z | find "+a" /c >>x.x
:dne
rem --------------------------
find ":" zzz.z | find "Cur" >>x.x
find ":" zzz.z | find "Cur"
:dne2
echo Done.
