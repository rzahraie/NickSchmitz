/*----------------------------------------------------------------*/
/*  Daily stock alert processing
	Author: Nick Schmitz
    2/10/98 : First Build
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#define MAX_LINE_LENGTH  120

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     int j,k,m,n,p;
     int ln;
     FILE *fp;
     char a_str[256],b_str[256],c_str[256],d_str[256],e_str[256];
     char ch;
     char dbs[12][12];
     int dbi[10];
     int val[25], avg[5];

     j=0;   k=0;    m=0;    n=0;   p=0;
     for (j=1; j<=10; j++) {dbs[j][0]=0; dbi[j]=0;}

     if(argc>1){  /* pick up date from command line /**/
        *++argv;/**/    /* Skip stk sym */
/*        printf("C %s %f  \n ",*argv);/**/
     }

/**/

     for (ln=1; gets(a_str); ln++) {
/* printf("%d*%s*\n", ln, a_str);/**/

    m=0;
    sscanf(a_str,"%[^.]%c%[^\n]", &e_str, &ch, &d_str);/**/
/* printf("%s*%s*\n", e_str, d_str);/**/

     for (j=1; j<24; j++) {
	     sscanf(d_str,"%[^,]%c%[^\n]s", &b_str, &ch, &d_str);/**/
	     sscanf(b_str,"%d", &val[j]);/**/
/* printf("%d*%d*%s*%s\n", j,val[j], b_str, d_str);/**/
     }
/* printf("\n");/**/

     m=0;
     for (j=1; j<=4; j++) {m=m+val[j];}
     m=m/4;
     for (k=1; k<4; k++) {avg[k]=0;
     	for (j=0; j<3; j++) {avg[k]=avg[k]+val[k+j];}
     	avg[k]=avg[k]/3;
       }

     k=-1;
     for (j=1; j<=10; j++) {if (dbi[j]<=m) k=j;}
     if (k>0) {
     	for (j=1; j<k; j++) {dbi[j]=dbi[j+1];
     		for (n=0; n<8; n++) dbs[j][n]=dbs[j+1][n];
	     	}
     	dbi[k]=m;
     	for (n=0; n<8; n++) dbs[k][n]=e_str[n];
      }
/*
 printf("%s*%d*%d", e_str, m, k);
 for (p=1; p<4; p++) printf("*%d", avg[p]);
 printf("\n");/**/

     if (e_str[0]=='+')
 	if (avg[3]>avg[2]+10 && avg[2]>avg[1]+10) printf("%s *!*\n",e_str);/**/
 	if (avg[3]>avg[2]+23 && avg[2]>avg[1]+23) printf("%s *-*\n",e_str);/**/
 	if (avg[2]>avg[3]+23 && avg[1]>avg[2]+23) printf("%s *+*\n",e_str);/**/

  }
     for (p=1; p<=10; p++) if (dbi[p]>1){
/*     	printf("%d*%d*", p, dbi[p] );/**/
     	for (n=0; n<8; n++) printf("%c",dbs[p][n]);
     	printf(" *$*\n");     }
/*----------------------------------------------------------------*/
} /* main/**/
