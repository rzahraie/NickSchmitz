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


float high, low, cls, vol;
float tprc, lstp, cumv, obv;
float pmax, pmin, vmax, sone;
float obvr, dist, sprd, rize, gain;
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
    pmax=0.0;
    pmin=999.9;
    lstp=0.0;
    obv=0.0;
    vmax=0.0;
    cumv=0.0;
    tmp=0;

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

/* Spaced Data */
/*            sscanf(a_str, "%f %f %[^\n]s", &high, &low, &b_str);
            sscanf(b_str, "%f %f %[^\n]s", &cls, &vol, &b_str); /**/

/* CSV data */
            sscanf(a_str, "%f%*c%f%[^\n]s", &high, &low, &b_str);
            sscanf(b_str, "%*c%f%*c%f%*c%[^\n]s", &cls, &vol, &b_str);/**/

        if (dbg>9) printf(";  %4.2f * %4.2f * %4.2f * %4.2f * %s\n",
                high, low, cls, vol, b_str);/**/

   if (vol>0.0){
       tprc=(high+low)/2.0;

       if (tprc<pmin) {
            pmin=tprc;
            pmax=0.0;
            lstp=0.0;
            obv=0.0;
            vmax=0.0;
            cumv=0.0;
            ln=1;
            tmp=0;
         }
       if (tprc>pmax) pmax=tprc;

       if (tprc>lstp) obv=obv+vol;
       if (tprc<lstp) obv=obv-vol;
       if (obv>vmax) vmax=obv;

       if (cumv+vol>0) sone=(sone*cumv+vol*tprc)/(cumv+vol);
       if (tprc<sone) tmp=tmp+1;
       cumv=cumv+vol;


       if (dbg>5) printf(
           "@ %4.2f %4.2f %8.0f %8.0f %4.2f %4.2f %8.0f %4.2f %s \n",
            tprc, lstp, obv, cumv, pmin, pmax, vmax, sone, b_str);

	   lstp=tprc;
   }
        }

/*----------------------------------------------------------------*/
/*	 fclose(fp);  /**/

    obvr=0.0;
    dist=0.0;
    sprd=0.0;
    rize=0.0;
    gain=0.0;
    if (vmax!=0.0)  obvr=obv/vmax;
    if (pmax!=pmin) dist=(tprc-sone)/(pmax-pmin);
    if (pmin!=0.0)  sprd=pmax/pmin;
    if (tprc!=0.0)  rize=pmax/tprc;
    if (pmin!=0.0)  gain=tprc/pmin;
/*    if (dist<0) dist=-dist; /**/

    if (dbg>1) printf("\n");/**/

    if (dbg>1) printf( "% 4.3f %10.0f %10.0f ", obv/cumv, obv, cumv);

    printf( "%4d %4d  - % 4.2f % 4.2f %4.2f - %4.2f %4.2f %s ",
        ln, tmp, obvr, dist, sprd, rize, gain,  b_str);

    if (obvr>0.8 && sprd>1.5)
      	{
      		printf( " *");
    		if (dist<0.1 && dist>-0.1) printf( "*");
      		printf( "\n");
    	}else{ printf( " \n");  }
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