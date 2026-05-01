/*----------------------------------------------------------------*/
/*
    Program to read TC2000 data (csv) & Calculate Midas Info
    Author: Nick Schmitz
    Date: 5/4/95
    mod: 2/27/96 - Osc detection
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
float extrm[41], prchst[41];
float dstx[41],  rizx[41];
int   dates[41];

/*----------------------------------------------------------------*/
/*
	Read stock data & compute volume & price data
*/
/*----------------------------------------------------------------*/

void rd_tc2k(int argc, char *argv[])
{
	 int j,k,m;
	 int r,s;
	 int ln, lnc, tmp;
	 char a_str[256], b_str[256], c_str[256];
	 FILE *fp;

/*----------------------------------------------------------------*/
/*     printf("%s \n",*argv);/**/
/*     printf("%s \n",*++argv);/**/

    *++argv;/**/	/* Skip Prog Name */

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
	 if((argc>1) && (fp = fopen(*argv, "r")) == NULL) {
		  printf("Cannot open file %s\n",*argv);
/*		  exit(1);/**/
		  }


/*----------------------------------------------------------------*/
/*
*/
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++)
          {
          for (j=0; a_str[j]; j++);		/* find eol /**/
	  for (m=0; a_str[m]==' '; m++);        /* leading spaces /**/
          if (dbg>7) printf(" \n");
	  if (dbg>5) printf("!%d %d %d %s ",ln,j,m,a_str);/**/
/*----------------------------------------------------------------*/
/* CSV data */
          sscanf(a_str, "%f%*c%f%[^\n]s", &high, &low, &b_str);
          sscanf(b_str, "%*c%f%*c%f%*c%[^\,]s", &cls, &vol, &b_str);/**/

   if (dbg>9) printf(";  %4.2f * %4.2f * %4.2f * %4.2f * %s\n",
                high, low, cls, vol, b_str);/**/
/*----------------------------------------------------------------*/

/* Spaced Data */ /* unused/**/
/* sscanf(a_str, "%f %f %[^\n]s", &high, &low, &b_str);
   sscanf(b_str, "%f %f %[^\n]s", &cls, &vol, &b_str); /**/

/* sscanf(b_str, "%*c%f%*c%f%*c%[^\n]s", &cls, &vol, &b_str);/**/
/* for (k=1; b_str[k]^=','; k++)  k=k;  b_str[k]=0; /**/

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
  } /* each data line in file /**/

/*----------------------------------------------------------------*/
/*	 fclose(fp);  /**/

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
