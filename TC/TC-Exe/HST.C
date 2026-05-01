/*  Creates Histogram of PT sizes from MACH 1x/2x Report file
   Author: Nick Schmitz 11/7/94
*/
#include <stdio.h>

main()
{
   int j,k,m,n;
   int p,q;
   char lin[256], sfl[256];
   int hgrm[256];
   int temp;
   int ln;

     for (j=0; j<=255; j++) hgrm[j]=0;;

     for (ln=1; gets(lin); ln++)
       {
/*puts(lin);/**/
        for (m=0; lin[m]; m++);

        temp=atoi(lin) % 32; /**/

        ++hgrm[temp];
        }

   printf("{ \t"); for (j=0; j<=31; j++) printf("%d\t",      j ); printf("\n");
   printf("{ \t"); for (j=0; j<=31; j++) printf("%d\t", hgrm[j]); printf("\n");
}

