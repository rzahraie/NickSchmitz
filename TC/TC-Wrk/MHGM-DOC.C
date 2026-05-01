/* Creates Histogram of fanout from MINC DOC file
   Author: Nick Schmitz 11/7/94
*/
#include <stdio.h>

main()
{
#define inp 1
#define out 2
#define all 3

   int j,k,m,n;
   int p,q;
   char lin[256], sfl[256];
   int hgrm[5][100];
   int temp;
   int cnt;
   int typ;
   int ln;

     for (k=1; k<=4; k++) for (j=0; j<=99; j++) hgrm[k][j]=0;;
     cnt=-99;
     typ=1;
     temp=5;


     for (ln=1; gets(lin); ln++)
       {
/* puts(lin);/**/
        for (m=0; lin[m]; m++);

          if (lin[0]!='-')
            if (lin[5]!=' ') {
              if ((cnt>=0) && (cnt<=99)) {
                   ++hgrm[3][cnt];
                   if (typ==1) ++hgrm[1][cnt];
                       else ++hgrm[2][cnt];
                  }
              cnt=0;
              typ=1;
             } else ++cnt;

        if (lin[0]!='-')
          if (m>=73) {
             k=0;
             p=72;
             for (q=p; lin[q]!=' ' && q; q--);

             for (j=q+1;  j<=p; j++) sfl[k++]=lin[j];
             sfl[k]=0;
             temp=atoi(sfl);/**/
/*
printf("%d \t %d %d \t%d\n",q,p,j,k);
puts(sfl);/**/
             if (temp>=10) typ=2;

            }
      lin[5]=' ';
      }

   printf("\n");
   for (j=0; j<=35; j++) printf("%d \t",j); printf("\n");
   for (k=1; k<=3; k++) {
       for (j=0; j<=35; j++) printf("%d \t",hgrm[k][j]);
       printf("\n");
     }
}
