/*----------------------------------------------------------------*/
/*
    Program to rank order stocks from TC2000 data (binary)
    Author: Nick Schmitz
    Date: 5/6/97

Description:

Program builds rank order list of stock prices. Program reads list
of stocks from RANK.DAT & updates it with current ranking (0-99),
based on moving average price changes in week trading period.

Details:

1. Cmd line argument "970505" is used as end date for period of ranking
2. Owned stocks "*" in column 1 - are flagged for extraction using FIND
3. Comments ";" in column are ignored in ranking
4. New rank order is inserted after symbol at start of rank list
5. Sort is simple bubble sort, using 2 index ordering (ptr & loc)
	for bookkeeping
6. INIT reads cmdline for date argument & reference stk (AMD) for
	3 date entries dy_srt, dy_old, dy_new
7. RANK.DAT is read (2 pass) - 1st to get price moving average,
	2nd to post rank orders on output file - RANK.CSV
8. Rank order posting is numeric 0..9999, with output divided by 100

*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <alloc.h>/**/

#define MAX_LINE_LENGTH  120
#define MAX_STK  2999

float high,  low,  cls, vol;            /* tc2k data /**/
float dy_srt, dy_old, dy_new, date;     /* date trigger for period /**/

int   rnk_ptr[MAX_STK],rnk_loc[MAX_STK],nstk;   /* Numeric price db /**/
float prc_rnk[MAX_STK],tprc;

int   dbg;

/*----------------------------------------------------------------*/
/*
	Numeric conversion for Tc2k Stock data
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
/*
	Read stock data & compute volume & price data
*/
/*----------------------------------------------------------------*/
void rd_tc2k(int xx, char *stkx[])
{
     char  cbuf[25];
     int   jbuf[11];
     long  lbuf[6];
     float v[6], ma5, prc_old, prc_new;

     int j,k,m,q;
	 int r,s;
	 int ln, lnc, tmp;
	 char a_str[256], b_str[256], c_str[256];
	 FILE *fp;

/*----------------------------------------------------------------*/
/*   for (k=1; k<60; k++) printf("*");   printf(" \n"); /**/
/*----------------------------------------------------------------*/
/*
	File access stuff - handle errors & exit
*/
         strcpy(a_str,stkx);
	 if((dbg>0) && (fp = fopen(a_str, "rb")) == NULL) {

		  cbuf[0]=a_str[0];  cbuf[1]=0;
		  strcpy(a_str,"c:\\tc2000\\data\\");/**/
		  strcat(a_str,cbuf);/**/
                  strcat(a_str,"\\");
                  strcat(a_str,stkx);
                  strcat(a_str,".ndx");
                  if (dbg>2) printf("%s\n",a_str);

  	          if((dbg>0) && (fp = fopen(a_str, "rb")) == NULL) {
		       printf("Cannot open file %s\n",stkx);
/*		       exit(1);/**/
                      }
                  }
/*----------------------------------------------------------------*/
    ma5=-1.0;   prc_old=ma5;    prc_new=ma5;	tprc=ma5;

    fread(&cbuf,sizeof(cbuf)-1,1,fp);   /* read stock name */
    cbuf[24]=0;
    lnc=0;

    fread(&jbuf,sizeof(jbuf),1,fp); 	/* skip data flags */
    fread(&q,sizeof(q),1,fp);   /* Some off-by-one problem */

    for (ln=1; ln<2554 && feof(fp)==0; ln++)  /* daily data */
       {
    	fread(&lbuf,sizeof(lbuf),1,fp);
        for (q=0; q<=5; q++) v[q]=msb2d(lbuf[q]);

        vol=v[4]*100;		/* Data for analysis /**/
        high=v[1];
        low=v[2];

/*----------------------------------------------------------------*/
   if ((vol>0.0) && (v[0]>=dy_srt)){
       tprc=(high+low)/2.0;
       if (ma5<0.0)  { ma5 =tprc;} else { ma5 =( 3*ma5 +tprc)/ 4.0;}
       if (v[0]<=dy_old) prc_old=ma5;
       if (v[0]<=dy_new) prc_new=ma5;

       lnc++;
/*       if (dbg>2) printf("%6.0f,%9.3f,%6.3f,%6.3f,%6.3f,%6.3f,%6.0f\n",
           v[0],   ma5,     v[5], v[1], v[2],v[3],  v[4]*100);/**/

     } /* vol>0 /**/
   } /* each day/**/

  fclose(fp);
  if (prc_old>0.0) { tprc=100.0*(prc_new-prc_old)/prc_old; }
 	      else { printf("\n Bad data: %s \n",a_str); }

  if (dbg>1) printf("%s,%5.2f,%4.2f,%4.2f\n", a_str,tprc, prc_old, prc_new);/**/

}


/*----------------------------------------------------------------*/
/*
	Read stock names RANK.Dat & get price deltas
*/
/*----------------------------------------------------------------*/
void rd_fil(int xx)
{
     int j,k,m,q;
     int ln,lx, lnc;
     char  cbuf[25], tch;
     char a_str[256],b_str[256],c_str[256];
     FILE *fx;

     strcpy(a_str,"rank.dat");/**/
     strcpy(b_str,".");/**/
     strcpy(c_str,".");/**/
     lnc=-1;

     if ((fx = fopen(a_str, "r")) == NULL) {
        printf("Cannot open file %s\n",a_str);
/*      exit(1);/**/
       } else {

    lnc=0;
    for (ln=1; ln<5 && feof(fx)==0; ln++)
       {
     for (lx=1; fgets(c_str,MAX_LINE_LENGTH,fx); lx++) {
        sscanf(c_str, "%c %[^,]%[^\n]", &tch,&a_str,&b_str);

        if (dbg>5) printf("%c!%s*%s*%s\n",tch,a_str,b_str,c_str);

        if (tch!=0 && tch!=';') {
        	if (dbg>5) printf("%s\n",a_str);
	       	rd_tc2k(dbg, a_str);/**/
		prc_rnk[lnc]=tprc;
		if ((dbg==1) && (fmod(lnc,500)<0.0001)) printf("\n");
		if ((dbg==1) && (fmod(lnc,10)<0.1)) printf(".");
		lnc++;
            }

        }
       }/* feof */
     fclose(fx);
     if (dbg==1) printf("\n");
     } /* file open */
  nstk=lnc-1;
}

/*----------------------------------------------------------------*/
/*
	Order delta prices, convert to rank order 0..9999
*/
/*----------------------------------------------------------------*/
void or_rnk(int xx)
{
     int j,k,m,q;
     float tmp,dtp;

     printf("# of Stocks:%6d\n",nstk+1);
/*     if (dbg>2) for (j=0; j<=nstk; j++)
     	printf("%6d,%6d,%6.2f\n",j,rnk_ptr[j],prc_rnk[rnk_ptr[j]]);/**/

     for (j=0; j<=nstk; j++) {		/* bubble sort /**/
        for (k=j+1; k<=nstk; k++) {
            if (prc_rnk[k]<prc_rnk[j]) {

          tmp=prc_rnk[j];		/* swap data /**/
          prc_rnk[j]=prc_rnk[k];
          prc_rnk[k]=tmp;

          q=rnk_ptr[rnk_loc[j]];	/* swap PTR's /**/
          rnk_ptr[rnk_loc[j]]=rnk_ptr[rnk_loc[k]];
          rnk_ptr[rnk_loc[k]]=q;/**/

          q=rnk_loc[j];			/* swap LOC's /**/
          rnk_loc[j]=rnk_loc[k];
          rnk_loc[k]=q;/**/
/*
        printf("%6d,%6d,%6d,%6d,%7.2f,%6.2f\n",j,k,
        rnk_ptr[j], rnk_ptr[k],prc_rnk[j], prc_rnk[k]);/**/
          }}}

     tmp=0.0;  dtp=9900.0/nstk;
     for (j=0; j<=nstk; j++) {	/* rank order /**/
     	prc_rnk[j]=tmp;
        tmp=tmp+dtp;
        }/**/

     if (dbg>2) for (j=0; j<=nstk; j++)
     	printf("%6d,%6d,%7.2f,%6.2f\n",j,rnk_ptr[j],
     	prc_rnk[j], prc_rnk[rnk_ptr[j]]);
}

/*----------------------------------------------------------------*/
/*
	Output rank order to file
*/
/*----------------------------------------------------------------*/
void rk_fil(int xx)
{
     int j,k,m,q;
     int ln,lx, lnc;
     char  cbuf[25], tch;
     char a_str[256],b_str[256],c_str[256];
     FILE *fx, *fy;

     strcpy(a_str,"rank.dat");/**/
     strcpy(b_str,".");/**/
     strcpy(c_str,".");/**/
     lnc=-1;

     if ((lnc<0) && (fx = fopen(a_str, "r")) == NULL) {
        printf("Cannot open file %s\n",a_str);
/*      exit(1);/**/
       } else {
        strcpy(a_str,"rank.csv");/**/

/**/
        if ((lnc<0) && (fy = fopen(a_str, "w+")) == NULL) {
           printf("Cannot open file %s\n",a_str);
          } else {/**/

          lnc=0;
          for (ln=1; ln<5 && feof(fx)==0; ln++)
            {
            for (lx=1; fgets(c_str,MAX_LINE_LENGTH,fx); lx++) {
            sscanf(c_str, "%c %[^,]%[^\n]", &tch,&a_str,&b_str);

            if (b_str[1]=='.') {
            	sscanf(b_str, ",.%[^\n]",&c_str);
		strcpy(b_str,",");/**/
		strcat(b_str,c_str);/**/
            }

            if (dbg>5) printf("%c!%s*%s*%s\n",tch,a_str,b_str,c_str);

            if (tch!=0 && tch!=';') {
            }
            if (dbg>5) printf("!%s*%s*%s\n",a_str,b_str,c_str);

            b_str[66]=0; /* truncate long lines /**/

            if (tch!=0 && tch!=';') {
          	 if (dbg>5) printf("%s\n",a_str);
            	 fprintf(fy,"%c%6s,.%2.0f%s\n",tch,a_str,
            		prc_rnk[rnk_ptr[lnc]]/100.0,b_str);/**/
		 lnc++;/**/
              } else {
           	 if (a_str[0]==';') fprintf(fy,"%s",a_str);
              }

            }
         }/* feof */
       fclose(fy);
       } /* file open */
     fclose(fx);
     } /* file open */
}

/*----------------------------------------------------------------*/
/*
	Read cmd line for date parameter,
	pick up real dates from reference stk - AMD
*/
/*----------------------------------------------------------------*/
void init(int argc, char *argv[])
{
     char  cbuf[25];
     int   jbuf[11];
     long  lbuf[6];
     float v[6];

     int j,k,ln,q;
     char a_str[256];
     FILE *fp;

     for (j=0; j<MAX_STK; j++) rnk_ptr[j]=j; /* Set up bookkeeping ptr's /**/
     for (j=0; j<MAX_STK; j++) rnk_loc[j]=j;
     for (j=0; j<MAX_STK; j++) prc_rnk[j]=0.0;

     date=-1.0;
     if(argc>1){  /* pick up date from command line /**/
        *++argv;/**/    /* Skip stk sym */
        sscanf(*argv, "%f %[^\n]s", &date, &a_str); /**/
/*        printf("C %s %f  \n ",*argv, date);/**/
     }

     strcpy(a_str,"c:\\tc2000\\data\\a\\amd.ndx");/**/
     if (dbg>3) printf("%s\n",a_str);

     if((j>0) && (fp = fopen(a_str, "rb")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

    fread(&cbuf,sizeof(cbuf)-1,1,fp); 	/* read stock name */
    cbuf[24]=0;

    fread(&jbuf,sizeof(jbuf),1,fp); 	/* skip data flags */
    fread(&q,sizeof(q),1,fp);   /* Some off-by-one problem */

    for (ln=1; ln<2554 && feof(fp)==0; ln++)  /* daily data */
       {
    	fread(&lbuf,sizeof(lbuf),1,fp);
        v[0]=msb2d(lbuf[0]);

        for (j=50; j>=1; j--) prc_rnk[j]=prc_rnk[j-1];
        prc_rnk[0]=v[0];

        if ((date>1.0) && (v[0]>=date)){
            dy_srt=prc_rnk[20];
            dy_old=prc_rnk[05];
            dy_new=prc_rnk[00];/**/
            date=0.1;
          }

      } /* each day/**/

       if (date<0.0) { /* no date on cmd line /**/
         dy_srt=prc_rnk[20]; dy_old=prc_rnk[05]; dy_new=prc_rnk[00];/**/
         }

     printf("Trigger dates: %5.0f,%5.0f,%5.0f\n",dy_srt, dy_old, dy_new);
     for (j=0; j<MAX_STK; j++) prc_rnk[j]=0.0;
     fclose(fp);
    } /* file open/**/
}

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
      dbg=1;
      init(argc, argv);
      rd_fil(dbg);
      or_rnk(dbg);
      rk_fil(dbg);
}
/*----------------------------------------------------------------*/
