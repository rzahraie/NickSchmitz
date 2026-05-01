@echo off
@
echo 1,# r@I@Z >zzz.edj
echo 1,# r  @Z >>zzz.edj
echo 1,# r  @Z >>zzz.edj
echo 1,# r  @Z >>zzz.edj
echo 4,#   rL1@Z1>>zzz.edj
echo 4,#   rL2@Z2>>zzz.edj
echo 4,#   rL3@Z3>>zzz.edj
echo 1,# r-s@Z >>zzz.edj
echo 1,# r-b@Z >>zzz.edj
echo 1,# r-m@Z >>zzz.edj
echo 1,# r-d@Z >>zzz.edj
echo 1,# r  @Z >>zzz.edj
echo 1,# r  @Z >>zzz.edj
echo 1,# r-x@Z 0>>zzz.edj
echo 1,# r1 3  0@Z1 3 0>>zzz.edj
echo 1,# r A @Z  1 >>zzz.edj
echo 1,# r B @Z  2 >>zzz.edj
echo 1,# r C @Z  3 >>zzz.edj
echo 1,# r D @Z  4 >>zzz.edj
echo exit>>zzz.edj
@
copy m5-sws.txt ms.t
czfix <zzz.edj >zzz.obj
edlin ms.t <zzz.obj >nul
