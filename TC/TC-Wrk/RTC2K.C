/*----------------------------------------------------------------*/
/*
    Program to read TC2000 data (binary)
    Author: Nick Schmitz
    Date: 11/23/96
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <alloc.h>/**/

#define MAX_LINE_LENGTH  120	/* unused /**/


float high,  low,  cls, vol;            /* tc2k data /**/
int   dbg;

/*----------------------------------------------------------------*/
/*
	Read stock data & compute volume & price data
*/
/*----------------------------------------------------------------*/

double msb2d (long ii) /* Convert number format /**/
{
    char foo[4];
    char *p;
    long expo,j,k;
    int  sign;
    double x,rslt;

    p=&ii;
    expo=p[3]+104; /* -$98 /**/
    if (expo>128) expo=expo-256;
    if (expo==104) expo=-152;
    k=1;
    sign=1;
    if (ii & 1<<23) sign=-1;

    j=ii | 1<<23; 		/* $0.80.00.00 /**/
    j= j & ((1<<24)-1); 	/* $0.FF.FF.FF /**/

    if (dbg>7) printf(" %d %d %d %d \n",p[0], p[1], p[2], p[3]);/**/
    if (dbg>7) printf(" %12ld %12ld    %d %d ",ii, j, sign, expo);/**/

    if ((expo<=32) && (expo>-32))
        {
        if (expo>=0) x=k<< expo;
                else x=k<<-expo;
        rslt=sign*j/x;
        }
      else
        {
/*        x=exp(expo*log(2.0));
        rslt=sign*j*x;/**/
        rslt=0.0;
        }
     if (dbg>7) printf("%12.3f %12.3f \n",x,rslt);/**/
     return(rslt);
}

/*----------------------------------------------------------------*/
void rd_tc2k(int argc, char *argv[])
{
     char  cbuf[25];
     int   jbuf[11];
     long  lbuf[6];
     float v[6], tprc, lstp, date;

     int j,k,m,q;
	 int r,s;
	 int ln, lnc, tmp;
	 char a_str[256], b_str[256], c_str[256];
	 FILE *fp;

/*----------------------------------------------------------------*/
/*     printf("%s \n",*argv);/**/
/*     printf("%s \n",*++argv);/**/
    *++argv;/**/    /* Skip Prog Name */

/*----------------------------------------------------------------*/
/*   for (k=1; k<60; k++) printf("*");   printf(" \n"); /**/
/*----------------------------------------------------------------*/
/*
	File access stuff - handle errors & exit
*/
	 if(argc==1) {
		  printf("Need file name \n");
/*		  exit(1); /**/
		  }
         strcpy(a_str,*argv);
	 if((argc>1) && (fp = fopen(a_str, "rb")) == NULL) {

		  cbuf[0]=a_str[0];  cbuf[1]=0;
		  strcpy(a_str,"c:\\tc2000\\data\\");/**/
		  strcat(a_str,cbuf);/**/
                  strcat(a_str,"\\");
                  strcat(a_str,*argv);
                  strcat(a_str,".ndx");
                  if (dbg>3) printf("%s\n",a_str);

  	          if((argc>1) && (fp = fopen(a_str, "rb")) == NULL) {
		       printf("Cannot open file %s\n",*argv);
/*		       exit(1);/**/
                      }
 if(argc>2){
    *++argv;/**/    /* Skip stk sym */
   sscanf(*argv, "%f %[^\n]s", &date, &b_str); /**/
} else {date=0.0;}
/*   printf("C %s %f  \n ",*argv, date);/**/
                  }
/*----------------------------------------------------------------*/

/*
    printf("C %d   ",sizeof(cbuf));
    printf("J %d   ",sizeof(jbuf));
    printf("L %d \n",sizeof(lbuf));/**/

/*
    for (q=0; q<=24; putch('a'+q++)) {};
    printf("\n");/**/

    fread(&cbuf,sizeof(cbuf)-1,1,fp); 	/* read stock name */
    cbuf[24]=0;
    lnc=0;
    if (dbg>2) printf("\"%s\"\n",cbuf);/**/
    if (dbg>2) printf("\"date\",\"open\",\"high\",\"low\",\"close\",\"vol\"\n");

    fread(&jbuf,sizeof(jbuf),1,fp); 	/* skip data flags */
    fread(&q,sizeof(q),1,fp);   /* Some off-by-one problem */

    if (dbg>7) for (q=0; q<=10; q++) printf("%8d ",jbuf[q]);
    if (dbg>7) printf("\n");

    for (ln=1; ln<2510 && feof(fp)==0; ln++)  /* daily data */
       {
    	fread(&lbuf,sizeof(lbuf),1,fp);

        if (dbg>7) for (q=0; q<=5; q++) printf("%10ld",lbuf[q]);
        if (dbg>7) printf("%d \n",ln);/**/

        for (q=0; q<=5; q++) v[q]=msb2d(lbuf[q]);

/*
        if (dbg>2) printf("%6.0f,%6.3f,%6.3f,%6.3f,%6.3f,%6.0f\n",
           v[0],        v[5], v[1], v[2],v[3],  v[4]*100);
/* Format: date-YYMMDD, open, high, low, close, vol */

        vol=v[4]*100;		/* Data for analysis /**/
        high=v[1];
        low=v[2];
        strcpy(b_str,cbuf);
/*----------------------------------------------------------------*/
   if ((vol>0.0) && (v[0]>date)){
       tprc=(high+low)/2.0;
       lnc++;
        if (dbg>2) printf("%6.0f,%6.3f,%6.3f,%6.3f,%6.3f,%6.0f\n",
           v[0],        v[5], v[1], v[2],v[3],  v[4]*100);

/*       if (dbg>-5) printf(b_str);/**/

       lstp=tprc;
     } /* vol>0 /**/
   } /* each day/**/
tprc=lstp;
/*  printf("%5.2f %4d %4d", temp, lnc, ncrs);/**/

/*----------------------------------------------------------------*/
/* Main output display /**/
/*----------------------------------------------------------------*/
 if(argc>2){
    printf("....\n");
} else {
    printf( "\n");
}
/*----------------------------------------------------------------*/
/**/
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
      dbg=3;
      rd_tc2k(argc, argv);
}
/*----------------------------------------------------------------*/
