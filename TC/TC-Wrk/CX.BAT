@echo off
find " " cf.c/c | find " " | cf
@
if errorlevel=1 echo 1
if not errorlevel=1 echo space
@
rem if errorlevel=0 echo 0
rem if errorlevel=1 echo 1
rem if errorlevel=2 echo 2
@
find "main" cf.c/c | find " " | cf
@
if errorlevel=1 echo 1
if not errorlevel=1 echo main
@
find "xain" cf.c/c | find " " | cf
@
if errorlevel=1 echo 1
@
