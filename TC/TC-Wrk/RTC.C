/* Program to read TC2000 Stock Data

    Author: Nick Schmitz
    Date: 7/23/96
*/

#include <stdio.h>

int   dbg;
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
void rd_tc2k(int argc, char *argv[])
{
     int q,ln;
     FILE *fp;
     char  cbuf[25], a_str[256];
     int   jbuf[11];
     long  lbuf[6];
     float v[6];

/*----------------------------------------------------------------*/
/*     printf("%s \n",*argv);/**/
/*     printf("%s \n",*++argv);/**/
    *++argv;/**/    /* Skip Prog Name */

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
                  printf("%s\n",a_str);

  	          if((argc>1) && (fp = fopen(a_str, "rb")) == NULL) {
		       printf("Cannot open file %s\n",*argv);
/*		       exit(1);/**/
		       }
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
    printf("\"%s\"\n",cbuf);/**/
    printf("\"date\",\"open\",\"high\",\"low\",\"close\",\"vol\"\n");

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

        printf("%6.0f,%6.3f,%6.3f,%6.3f,%6.3f,%6.0f\n",
           v[0],        v[5], v[1], v[2],v[3],  v[4]*100);
/* Format: date-YYMMDD, open, high, low, close, vol */
       }
}

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
      dbg=0;
      rd_tc2k(argc, argv);
}
/*----------------------------------------------------------------*/
