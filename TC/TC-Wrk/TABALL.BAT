cd ..\tab
move taball.1 taball.0
move taball.2 taball.1
move taball   taball.2
grep Total *.tab              >  ..\run\taball.tmp
cd ..\run
awk -f awk\awk9  taball.tmp  >  ..\tab\taball
del  taball.tmp
type ..\tab\taball
