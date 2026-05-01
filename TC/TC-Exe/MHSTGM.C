/*  Creates Histogram of PT sizes from MACH 1x/2x Report file
   Author: Nick Schmitz 11/7/94
*/
#include <stdio.h>

main()
{
   int j,k,m,n;
   int p,q;
   char lin[256], sfl[256];
   int hgrm[100];
   int symh[100];
   int temp;
   int cnt;
   int typ;
   int ln;
   int ptx,pts,pt4,gts;

     for (j=0; j<=99; j++) hgrm[j]=0;;
     for (j=0; j<=99; j++) symh[j]=0;;

     n=0;
     pts=0; pt4=0; gts=0;

     for (ln=1; gets(lin); ln++)
       {
/*puts(lin);/**/
        for (m=0; lin[m]; m++);

        for (p=0; lin[p]!='"' && (p<m); p++);

       if (lin[p]=='"') {
             k=0;
             for (q=p+2; lin[q]!=' ' && (q<m); q++);
             for (j=p+2; j<=q-1; j++)  sfl[k++]=lin[j];
             sfl[k]=0;
             temp=atoi(sfl);/**/

             if (temp>=0) if (temp<=99) {
                      ++hgrm[temp];
                      if (pt4<=9999) {
                         pts=pts+temp;
                         pt4=pt4+4*(((temp-1) / 4)+1);
                      }
                      ptx=temp;
                      n=n+1;
                  }
          }

          for (p=p; lin[p]!=',' && (p<m); p++);

          if (lin[p]==',') {
             k=0;
             for (q=p+2;  lin[q]!=' ' && (q<m); q++);
             for (j=p+2; j<=q-1; j++)  sfl[k++]=lin[j];
             sfl[k]=0;
             temp=atoi(sfl);/**/

             if (temp>=0) if (temp<=99) {
                   ++symh[temp];
                   if (pt4<=9999)
                         gts=gts + 1 + (2*ptx*temp) / 5;

/*   printf("%d \t %d \t %d \n",pts,pt4,gts);/**/
                      n=n+1;
                }
        }
   }

   printf(". .: %d \t %d \t %d \n",pts,pt4,gts);/**/

   printf("{ \t"); for (j=0; j<=25; j++) printf("%d\t",      j ); printf("\n");
   printf("{ \t"); for (j=0; j<=25; j++) printf("%d\t", hgrm[j]); printf("\n");
   printf("{ \t"); for (j=0; j<=25; j++) printf("%d\t", symh[j]); printf("\n");
}

