@
if x%1x==xx goto dne
@
del x.x
@
rank 960901 >>x.x
copy rank.csv rank.dat
rank 960915 >>x.x
copy rank.csv rank.dat
@
rank 961001 >>x.x
copy rank.csv rank.dat
rank 961015 >>x.x
copy rank.csv rank.dat
@
rank 961101 >>x.x
copy rank.csv rank.dat
rank 961115 >>x.x
copy rank.csv rank.dat
@
rank 961201 >>x.x
copy rank.csv rank.dat
rank 961215 >>x.x
copy rank.csv rank.dat
@
rank 970101 >>x.x
copy rank.csv rank.dat
rank 970115 >>x.x
copy rank.csv rank.dat
@
rank 970201 >>x.x
copy rank.csv rank.dat
rank 970215 >>x.x
copy rank.csv rank.dat
@
rank 970301 >>x.x
copy rank.csv rank.dat
rank 970315 >>x.x
copy rank.csv rank.dat
@
rank 970401 >>x.x
copy rank.csv rank.dat
rank 970415 >>x.x
copy rank.csv rank.dat
@
rank 970501 >>x.x
copy rank.csv rank.dat
@
rank 970515 >>x.x
copy rank.csv rank.dat
@
:dne
rank
del x.txt
del x.z
echo ------------------------>>x.txt
echo Owned:>>x.txt
Find "*" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo Tech:>>x.txt
Find "!" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo Semis:>>x.txt
Find "@" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo S/W:>>x.txt
Find "#" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo Biggies:>>x.txt
Find "$" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo Ramps:>>x.txt
Find "^" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo Osc:>>x.txt
Find "%%" rank.csv | find ",." | sort /+9 >>x.txt
echo ------------------------>>x.txt
echo Highest:>>x.txt
Find ".9" rank.csv | find ",." | sort /+12 | tail >>x.txt
echo ------------------------>>x.txt
Find "*" rank.csv >>x.z
Find "!" rank.csv >>x.z
Find "@" rank.csv >>x.z
Find "#" rank.csv >>x.z
Find "$" rank.csv >>x.z
Find "^" rank.csv >>x.z
Find "%%" rank.csv >>x.z
@
echo Highest Selected:>>x.txt
find ",." x.z | find ",." | sort   /+9 | tail >>x.txt
echo ------------------------>>x.txt
echo Lowest:>>x.txt
find ",." x.z | find ",." | sort/r /+9 | tail >>x.txt
echo ------------------------>>x.txt
@
