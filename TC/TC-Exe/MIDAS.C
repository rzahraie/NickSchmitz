/*----------------------------------------------------------------*/
/*
    Demo program to TC2000 data & Calculate Midas Info
	Author: Nick Schmitz
    Date: 5/4/95
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <alloc.h>/**/

#define MAX_LINE_LENGTH  120
#define NC_ROUTE           0


float high,low,cls,vol;
float tprc,lowp,lstp,cumv,obv;
int dbg;

/*----------------------------------------------------------------*/
/*
	Read stock data & compute volume & price data
*/
/*----------------------------------------------------------------*/

void rd_m5_sm(int argc, char *argv[])
{
	 int j,k,m;
	 int r,s;
	 int ln,tmp;
	 char a_str[256], b_str[256], c_str[256];
	 FILE *fp;

/*----------------------------------------------------------------*/
/*     printf("%s \n",*argv);/**/
/*     printf("%s \n",*++argv);/**/

	*++argv;/**/	/* Skip Prog Name */
	lowp=999.9;
	lstp=0.0;
	cumv=0.0;
	obv =0.0;

/*----------------------------------------------------------------*/
/*
	File access stuff - handle errors & exit
*/
	 if(argc==1) {
		  printf("Need file name \n");
/*		  exit(1); /**/
		  }
	 if((argc>1) && (fp = fopen(*argv, "r")) == NULL) {
		  printf("Cannot open file %s\n",*argv);
/*		  exit(1);/**/
		  }

/*----------------------------------------------------------------*/
/*

*/

     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++)
          {
          for (j=0; a_str[j]; j++);
	  for (m=0; a_str[m]==' '; m++);
	  if (dbg>5) printf("!%d %d %d %s ",ln,j,m,a_str);/**/


/*----------------------------------------------------------------*/

            sscanf(a_str, "%f %f %[^\n]s", &high, &low, &b_str);
		if (dbg>7) printf(";  %6.2f * %6.2f * %s\n", high,low,b_str);/**/
            sscanf(b_str, "%f %f %[^\n]s", &cls, &vol, &b_str);
		if (dbg>7) printf(";  %6.2f * %6.0f * %s\n", cls,vol,b_str);/**/

	   tprc=(high+low)/2.0;
	   cumv=cumv+vol;
	   if (tprc>lstp) obv=obv+vol;
	   if (tprc<lstp) obv=obv-vol;
	   if (dbg>5) printf( "@ %6.2f %6.2f %10.0f %10.0f %s \n",
		tprc, lstp, obv, cumv, b_str);

	   lstp=tprc;

        }

/*----------------------------------------------------------------*/
/*	 fclose(fp);  /**/

	if (dbg>2) printf("\n");/**/
	printf( "%4.3f %10.0f %10.0f %3d %s \n",
		obv/cumv, obv, cumv, ln, b_str);
}

/*----------------------------------------------------------------*/
/*   for (m=m; m<j;  m++) if (a_str[m]==',') {b_str[k++]=' ';}/**/
/*   b_str[k++]=0;/**/
/*   printf("%s",b_str);/**/
/*   for (k=0; c_str[k]; k++) a_str[k]=c_str[k];
     a_str[k++]='\n'; a_str[k++]=0;/**/

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
      dbg=0;
      rd_m5_sm(argc, argv);
}
/*----------------------------------------------------------------*/

