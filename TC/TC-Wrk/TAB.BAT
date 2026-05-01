@echo off

awk -f awk\awk1   ..\log\%1.log  > ..\tab\%1.com
awk -f awk\awk_lc ..\log\%1.log  > ..\tab\%1.tab
type ..\tab\%1.tab
