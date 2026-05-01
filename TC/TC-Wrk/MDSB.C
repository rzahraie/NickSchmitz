/*----------------------------------------------------------------*/
/*
    Program to read TC2000 data (csv) & Calculate Midas Info
    Author: Nick Schmitz
    Date: 5/4/95
    mod: 2/27/96 - Osc detection
    mod: 7/22/96 - Binary tc2k data
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <alloc.h>/**/

#define MAX_LINE_LENGTH  120	/* unused /**/
#define NC_ROUTE           0


float high,  low,  cls, vol;            /* tc2k data /**/
float tprc, lstp, cumv, obv;		/* Midas Var's /**/
float pmax, pmin, vmax, sone;
float obvr, dist, sprd, rize, gain;

float cmvd, obvd;			/* Down ramps /**/
float pmxd, pmnd, vmxd, sond;
float obrd, dstd, gand;

float dstu, dstd, rizu, rizd;

float dyab, dybl, mva5, mvav, frac, frcm; /* Osc Var's /**/
float gmax, gmin, rmax, rmin, rgan, temp;
int   ncrs, abv;

int   dbg;
float extrm[141], prchst[141];
float dstx[141],  rizx[141];
int   dates[141];

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
     float v[6];

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
    pmax=0.0;   gmax=0.0;   rmax=0.0;	pmxd=0.0;
    pmin=999.9; gmin=999.9; rmin=999.9; pmnd=999.9;

    lstp=0.0;   obv=0.0;    sone=0.0;
    vmax=0.0;   cumv=0.0;   tmp=0;

    obvd=0.0;   sond=0.0;
    vmxd=0.0;   cmvd=0.0;

    dyab=0;	dybl=0;
    ncrs=0;     lnc=0;

    for (k=1; k<40; k++) extrm[k]=0.0;
    prchst[40]=999.9;

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

        if (dbg>2) printf("%6.0f,%6.3f,%6.3f,%6.3f,%6.3f,%6.0f\n",
           v[0],        v[5], v[1], v[2],v[3],  v[4]*100);
/* Format: date-YYMMDD, open, high, low, close, vol */

        vol=v[4]*100;		/* Data for analysis /**/
        high=v[1];
        low=v[2];
        strcpy(b_str,cbuf);
/*----------------------------------------------------------------*/
   if (vol>0.0){
       tprc=(high+low)/2.0;
       lnc++;

       if (prchst[40]>900){ 	/* Initalize moving average /**/
          for (k=1; k<=40; k++) prchst[k]=tprc;
          extrm[0]=tprc;	dates[0]=lnc;
	  dstx[0]=0.0;		rizx[0]=0.0;
          abv=0;
          };

       for (k=40; k>1; k--) prchst[k]=prchst[k-1]; /* Age data /**/
       prchst[1]=tprc;

       mva5=0;
       for (k=1; k<=5; k++) mva5=mva5+prchst[k];    /* 5 day avg /**/
       mva5=mva5/5.0;

       mvav=0;
       for (k=1; k<=40; k++) mvav=mvav+prchst[k];   /* 40 day avg /**/
       mvav=mvav/40.0;

       if (mva5>mvav) dyab++;		/* up/dn day counts /**/
       if (mva5<mvav) dybl++;
       if (mva5>mvav && abv==0 && extrm[ncrs]>0.01) ncrs++;
       if (mva5<mvav && abv==1 && extrm[ncrs]>0.01) ncrs++;

       abv=(mva5>mvav);

	if (dbg>3) printf("%4d %4d  %5.2f  %5.2f %5.2f - ",
    		lnc, ncrs, tprc, mva5, mvav);

	if (dbg>5) printf(
           "@ %5.2f - %5.2f %5.2f %5.2f - %5.2f %5.2f %5.2f %s\n",
            tprc, pmin, pmax, sone,  pmnd, pmxd, sond, b_str);
/*----------------------------------------------------------------*/

       if (mva5>mvav && rmax<mva5) { /* Pick up new high /**/
/*if (dbg>-1 && rmin<900) printf("***.*\n");/**/
       		if (rmin<900) { extrm[ncrs]=rmin; 	dates[ncrs]=lnc;
				dstx[ncrs]=dstd;	rizx[ncrs]=rizd;
				pmxd=mva5; }
	        if (pmax!=pmin) dstu=(mva5-sone)/(pmax-pmin); else dstu=0.01;
	        rizu=mva5/pmin;
       		rmin=999.9;
       		rmax=mva5;
          }
       if (mva5<mvav && rmin>mva5) { /* Pick up new low /**/
/*if (dbg>-1 & rmax>0.01) printf("***'*\n");/**/
	        if (rmax>0.01) { extrm[ncrs]=rmax; 	dates[ncrs]=lnc;
				dstx[ncrs]=dstu;	rizx[ncrs]=rizu;
				pmin=mva5; }
		if (pmxd!=pmnd) dstd=(sond-mva5)/(pmxd-pmnd); else dstd=0.01;
		rizd=-pmxd/mva5;
       		rmax=0.0;
       		rmin=mva5;
          }

       if (dbg>9) printf("#  %4d   %4.1f  %4.1f - %6.3f  %6.3f   %6.3f  \n",
       		ncrs, dyab, dybl, mva5, mvav, tprc);

/*----------------------------------------------------------------*/
       if (tprc<gmin) gmin=tprc;	/* global high/low /**/
       if (tprc>gmax) gmax=tprc;

       if (mva5<pmin) {
            pmin=mva5;
            pmax=0.0;		lstp=0.0;	obv=0.0;
            vmax=0.0;		cumv=0.0;	ln=1;
            tmp=0;              sone=0.0;
/*if (dbg>-1) printf("****.\n");/**/
         }
/*if (dbg>-1 && mva5>pmax) printf(".**\n");/**/
       if (mva5>pmax) pmax=mva5;

       if (mva5>lstp) obv=obv+vol;
       if (mva5<lstp) obv=obv-vol;
       if (obv>vmax)  vmax=obv;

       if (cumv+vol>0) sone=(sone*cumv+vol*mva5)/(cumv+vol);
       if (mva5<sone) tmp=tmp+1;
       cumv=cumv+vol;
/*----------------------------------------------------------------*/

       if (mva5>pmxd) {
            pmxd=mva5;
            pmnd=999.0;		lstp=0.0;	obvd=0.0;
            vmxd=0.0;		cmvd=0.0;	sond=0.0;	
/*if (dbg>-1) printf("****'\n");/**/
         }
/*if (dbg>-1 && mva5<pmnd) printf("'**\n");/**/
       if (mva5<pmnd) pmnd=mva5;

       if (mva5>lstp) obvd=obvd+vol;
       if (mva5<lstp) obvd=obvd-vol;
       if (obvd>vmxd) vmxd=obvd;

       if (cmvd+vol>0) sond=(sond*cmvd+vol*mva5)/(cmvd+vol);
       cmvd=cmvd+vol;

/*	if (dbg>-1) printf("%4d %4d  %5.2f  %5.2f %5.2f - ",
    		lnc, ncrs, tprc, mva5, mvav);/**/

/*
if (dbg>-1 && pmax!=pmin) printf("U %5.2f %5.2f ",
        (mva5-sone)/(pmax-pmin), mva5/pmin);
if (dbg>-1 && pmxd!=pmnd) printf("D %5.2f %5.2f ",
        (sond-mva5)/(pmxd-pmnd), pmxd/mva5);/**/

/* if (dbg>-1) printf(" -  %5.2f %5.2f %2d\n", dstx[ncrs], rizx[ncrs],ncrs);/**/
/*----------------------------------------------------------------*/

/*       if (dbg>-5) printf(
           "@ %5.2f %5.2f - %8.0f %8.0f %8.0f - %5.2f %5.2f %5.2f %s\n",
            tprc, lstp,   obv, cumv, vmax,  pmin, pmax, sone, b_str);/**/

/*       if (dbg>-5) printf(
           "& %5.2f %5.2f - %8.0f %8.0f %8.0f - %5.2f %5.2f %5.2f %s\n",
            tprc, lstp,   obvd, cmvd, vmxd,  pmnd, pmxd, sond, b_str);/**/

       lstp=tprc;
     } /* vol>0 /**/
   } /* each line/**/

    if (rmin<900)  { extrm[++ncrs]=rmin; dates[ncrs]=lnc;
		        dstx[ncrs]=dstd;
		        rizx[ncrs]=rizd;
		}
	if (rmax>0.01) { extrm[++ncrs]=rmax; dates[ncrs]=lnc;
		        dstx[ncrs]=dstu;
		        rizx[ncrs]=rizu;
		}

/*
   if (dbg>-3) printf("******%4d \n",ncrs);

    if (dbg>-3) for (k=0; k<=ncrs; k++) printf("%7.2f", extrm[k]);
    if (dbg>-3) printf("\n");
    if (dbg>-3) for (k=0; k<=ncrs; k++) printf("%7d",   dates[k]);
    if (dbg>-3) printf("\n");

    if (dbg>-3) for (k=0; k<=ncrs; k++) printf("%7.2f", rizx[k]);
    if (dbg>-3) printf("\n");
    if (dbg>-3) for (k=0; k<=ncrs; k++) printf("%7.2f", dstx[k]);
    if (dbg>-3) printf("\n");/**/

	dstu=0.0;
	dstd=0.0;
	rizu=1.0;
	for (k=0; k<=ncrs; k++) {
		if (rizx[k]>0.01) {
			rizd=(dates[k]-dates[k-1]);
			dstd=dstd+rizd+1;
			dstu=dstu+dstx[k]*rizd;
			rizu=rizu+(rizx[k]-1.0)*rizd;
			}
		}

	dstu=dstu/lnc;
	if (dstd>2*lnc/3 || dstd<0.01)	rizu=1.0+rizu/lnc;
		else rizu=1.0+2.0*rizu/dstd;/**/

/*	printf("%4d %7.2f  %7.2f %7.2f  \n", lnc, dstd,
		1.0+rizu/lnc, 1.0+rizu/dstd);/**/
/* printf("%4d %7.2f %7.2f %7.2f %4d \n", k, rizu, dstd, dstu,lnc);/**/
/*----------------------------------------------------------------*/
    obvr=0.0;	    dist=0.0;    sprd=0.0;
    rize=0.0;       gain=0.0;

    if (vmax>0.01)  obvr=obv/vmax;
    if (pmax!=pmin) dist=(mva5-sone)/(pmax-pmin);
    if (pmin>0.01)  sprd=pmax/pmin;
    if (mva5>0.01)  rize=pmax/mva5;
    if (pmin>0.01)  gain=mva5/pmin;


    obrd=0.0;	    dstd=0.0;    gand=0.0;

    if (vmxd>0.01)  obrd=obvd/vmxd;
    if (pmxd!=pmnd) dstd=(sond-mva5)/(pmxd-pmnd);
    if (mva5>0.01)  gand=pmxd/mva5;

/*    if (dist<0) dist=-dist; /**/

    if (ncrs^=0) temp=lnc/ncrs;
/*  printf("%5.2f %4d %4d", temp, lnc, ncrs);/**/

/*----------------------------------------------------------------*/
/* Main output display /**/
/*----------------------------------------------------------------*/
 if(argc>2){
    if (gmin>0.01 && dybl>0.01)
     printf( "%5.2f %5.2f - %2d %3.0f * %5.2f %5.2f %s",
      gmax/gmin, dyab/dybl,  ncrs, temp, rizu, dstu, b_str);

    if (dstu<0.2 && rizu<1.8) printf(" ^");	  	/*bad*/
     else if (rizu<1.15) printf(" ^.");	

    if (dstu>0.45 && rizu>1.2) printf(" !,");		/*good*/
	else if (dstu>0.34 && rizu>1.7) printf(" !");
	else if (ncrs>5 && dstu>0.2 && rizu>1.5) printf(" !.");	
    printf("\n");
} else {
    printf( "%4d %6.2f - %5.2f %s \n", ln, dist, gain, b_str);
}
/*----------------------------------------------------------------*/
/* old display code /**/
/*----------------------------------------------------------------*/
/* Midas Evaluation /**/
/*
    printf( "%4d %4d - %5.2f %6.2f - %5.2f %s ",
              ln, tmp,  obvr, dist,   gain,  b_str);
    if (dbg>-1) printf("\n");
    if (dbg>-1) printf( "%4d %4d - %5.2f %6.2f - %5.2f %s ",
              ln, tmp,  obrd, dstd,   gand,  b_str);
/**/
/*
    printf( "%4d %4d - %4.2f %5.2f %5.2f - %4.2f %5.2f %s ",
              ln, tmp,  obvr, dist, sprd,   rize, gain,  b_str);
/**/
/*
    if (obvr>0.8 && sprd>1.5)
      	{
      		printf( " *");
    		if (dist<0.1 && dist>-0.1) printf( "*");
      		printf( "\n");
    	}else{ printf( "\n");  }
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
      dbg=0;
      rd_tc2k(argc, argv);
}
/*----------------------------------------------------------------*/
