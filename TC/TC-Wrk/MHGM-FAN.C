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
   int temp, t2;
   int typ;
   int ln;

     for (k=1; k<=4; k++) for (j=0; j<=99; j++) hgrm[k][j]=0;;
     typ=1;


     for (ln=1; gets(lin); ln++)
       {
/*  puts(lin);/**/
        for (m=0; lin[m]; m++);

        if (lin[0]==9) if (lin[1]==32) {
             k=0;
             for (q=0; (lin[q]==' ' || lin[q]==9) && lin[q]; q++);

             for (j=q;  lin[j]!=9; j++) sfl[k++]=lin[j];
             sfl[k]=0;
             temp=atoi(sfl);/**/

/*
printf("%d \t %d %d \t%d\n",q,j,k);
puts(sfl);/**/
             typ=1;
             if (lin[j+1]=='O') typ=2;
             if (lin[j+1]=='N') typ=3;

             k=0;
             for (q=m; (lin[q]==' ' || lin[q]==9) && q; q--);
             for (j=q-1;  lin[j]!=' ' && lin[j]!=9 && j; j--);

             for (j=j+1;  j<=q; j++) sfl[k++]=lin[j];
             sfl[k]=0;
             t2=atoi(sfl);/**/

/*
printf("%d \t %d %d \t%d\n",q,j,k);
puts(sfl);/**/

/*
printf("%d \t %d \n",t2,temp);/**/

            hgrm[typ][t2]=hgrm[typ][t2]+temp; /**/
            hgrm[4][t2]=hgrm[4][t2]+temp; /**/
            }

      }

   printf("\n");
   for (j=0; j<=35; j++) printf("%d \t",j); printf("\n");
   for (k=1; k<=4; k++) {
       for (j=0; j<=35; j++) printf("%d \t",hgrm[k][j]);
       printf("\n");
     }
}
