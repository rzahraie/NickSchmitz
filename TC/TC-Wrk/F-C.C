/*----------------------------------------------------------------*/
/*  Find Clock & Rst Nets by fanout
	Author: Nick Schmitz
    Date: 7/14/97
    f-c filename

    Fanout chaser - high fanout nets & block preplace
    Date 3/5/98
    f-c filename any_text

	Filename - design name (no extension).
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#define MAX_LINE_LENGTH  120

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     FILE *fp;
     int j,k,m,p;
     int k_fn, c_fn, r_fn;
     int net, blk, ln;

     int sig[10];
     char a_str[256],b_str[256],c_str[256],d_str[256];

     int fq[1000], fx[1000],fy[1000]; /* Fanout chaser vars /**/
     int  lx[60], ly[60]; /* location index /**/
     int  bx[60], by[60]; /* driver band /**/
     int  sx[60][30], sy[60][30]; /* signal fanout list /**/
     char cx[60][20], cy[60][20]; /* netlist names /**/
     int xp,yp,jp;

/*----------------------------------------------------------*/
     xp=0;      yp=0;	jp=0;  		/* Init vars /**/

     for (j=0; j<60; j++) {
     	for (k=0; k<25; k++) { sx[j][k]=0; sy[j][k]=0;}
     	for (k=0; k<20; k++) { cx[j][k]=0; cy[j][k]=0;}
        lx[j]=0; ly[j]=0; bx[j]=0; by[j]=0;
       }
    for (j=0; j<1000; j++) { fq[j]=0; fx[j]=0; fy[j]=0; }
    sx[xp][0]=1; sy[yp][0]=1;

     m=0;	k=0; 	   d_str[0]=0;
     k_fn=0; 	c_fn=0;	   r_fn=0;	net=0;	blk=0;

/*----------------------------------------------------------*/
     if(argc>1){  /* pick up filename from command line /**/
        *++argv;/**/    /* Skip Prgname */
/*        printf("C %s %f  \n ",*argv);/**/
     }

     strcpy(a_str,*argv);
     strcat(a_str,".nts");/**/
     printf("rem %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

     if(argc>2) printf("High Fanout Nets: {Net#,,FO, Name}\n");

/*----------------------------------------------------------*/
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
/* printf(" %s\n", a_str);/**/

        if (a_str[0]!='/' && a_str[0]!='-'){ /* end of net /**/
            sscanf(a_str,"%d %s %s",&j,&c_str,&b_str);

            if (c_str[0]=='I') m=1;
/*            printf(" %d*%s\n", j,c_str);/**/

            if (k_fn==0 && c_fn==0 && r_fn==0) {
            	net=j;
            	for (p=0; c_str[p]; ++p) d_str[p]=c_str[p];
            	d_str[p]=0;
              } else {
                if (j>blk) blk=j; /* Find highest blk # /**/
              }

/*----------------------------------------------------------*/
/* clk & rst nets - special letters /**/

            if (c_str[1]=='_' && c_str[2]=='C' &&
                c_str[3]=='L' && c_str[4]=='K') k_fn++;
            else if (c_str[1]=='_' && c_str[2]=='C' &&
                c_str[3]=='T' && c_str[4]=='L') r_fn++;
            else c_fn++;

/* printf("%d %s*%d %d %d - %d %d\n", net, d_str, k_fn,r_fn,c_fn, m,blk);/**/
/* printf("%d %s*%d %d %d - %d %d * %d %d  %d %d\n", net, d_str,
	k_fn,r_fn,c_fn, m,blk,  xp,yp, sx[xp][0], sy[yp][0]);/**/

/*----------------------------------------------------------*/
if (j>0 && jp!=j) { /* testcpu design - special nets /**/
     if(d_str[1]=='M') {sy[yp][sy[yp][0]]=j;    if (sy[yp][0]<29) sy[yp][0]++;}
     if(d_str[1]=='A') {sx[xp][sx[xp][0]]=j;    if (sx[xp][0]<29) sx[xp][0]++;}
     if (c_fn>2) jp=j;
	}
/**/
       } /* end of net /**/

/*----------------------------------------------------------*/
        if (a_str[0]=='-') {
/* printf("%d %s*%d %d %d - %d %d\n", net, d_str,k_fn,r_fn,c_fn, m,blk);/**/
     if(argc>2 && c_fn>10){ printf("%3d,,%3d,%s\n", net, c_fn-2, d_str); }

     if (c_fn>10){
        if(d_str[1]=='M') {
/*     		printf("*%3d %3d %s\n", net, c_fn-2, d_str); /**/
                for (p=0; d_str[p] && p<19; p++) cy[yp][p]=d_str[p];
            	cy[yp][p]=0;
     		yp++;
		sy[yp][0]=1;}
        if(d_str[1]=='A') {
/*     		printf(".%3d %3d %s\n", net, c_fn-2, d_str); /**/
                for (p=0; d_str[p] && p<19; p++) cx[xp][p]=d_str[p];
            	cx[xp][p]=0;

/*      printf("*%d - %s,",xp,d_str);
        for (p=0; p<cx[xp][p]; p++) { printf("%c", cx[xp][p]);}
        printf("\n"); /**/
     		xp++;
		sx[xp][0]=1;}

    } else {sx[xp][0]=1; sy[yp][0]=1;}

/*----------------------------------------------------------*/
        if (a_str[0]=='-') { /* end of net /**/
/*	    if (k_fn>blk/15 && m==1 )
	            printf("rem *c*%d %s* %d - %d\n", net, d_str,k_fn, blk);/**/
	    if (k_fn>blk/15 && m==1 ) {
	            printf("rem *c*%d %s* %d\n", net, d_str,k_fn);/**/
		    k++;
		    sig[k]=net;
		}
/*	    if (r_fn>blk/15 && m==1 )
	            printf("rem *r*%d %s* %d %d\n", net, d_str,r_fn, blk);/**/
	    if (r_fn>blk/15 && m==1 ) {
	            printf("rem *r*%d %s* %d\n", net, d_str,r_fn);/**/
		    k++;
		    sig[k]=net;
		}
            m=0;
            net=0;
    	    k_fn=0; 	c_fn=0;  	r_fn=0;
          }
       } /* end of net /**/

/*-----------------------------*/
} /* gets - each line /**/

/*-----------------------------*/
/** now do program outputs /**/
/*----------------------------------------------------------*/
/* output clk & rst nets on router cmnd line /**/
       if (argc<3)
	if (k>0) {

/*        printf("\\vantis\\bin\\dmrouter -f 35 -P -! ");/**/
        printf("dmrouter -f 35 -P -~ -! ");
		for (j=1; j<=k; j++) printf(" %d",sig[j]);/**/
		printf(" >> %s.lgg\n",*argv);
	} else {
        printf("dmrouter -f 35 -P -~ >>%s.lgg\n",*argv);
	}

/*----------------------------------------------------------*/
     if(argc>2){  /* Fanout chaser results /**/
     printf("\n");
     printf("rem %d %d\n", xp, yp);

     printf("\n");
     printf("X entries\n");
     for (j=0; j<xp; j++) { printf("%3d %3d : ", j, sx[j][0]);
     	for (k=0; k<cx[j][k]; k++) { printf("%c", cx[j][k]);}
        printf(" ");
     	for (k=1; k<sx[j][0]; k++) { printf("%3d,", sx[j][k]);}
        printf("\n");}

     printf("\n");
     printf("Y entries\n");
     for (j=0; j<yp; j++) { printf("%3d %3d : ", j, sy[j][0]);
     	for (k=0; k<cy[j][k]; k++) { printf("%c", cy[j][k]);}
        printf(" ");
     	for (k=1; k<sy[j][0]; k++) { printf("%3d,", sy[j][k]);}
        printf("\n");}
     }/*argc*/

/*----------------------------------------------------------*/
if(argc>2){ /* Find highest unique driver for blocks /**/
    for (j=0; j<1000; j++) { fx[j]=0; fy[j]=0; }

     for (j=0; j<xp; j++) {
     for (k=0; k<yp; k++) {
     for (m=2; m<sx[j][0]; m++) {
     for (p=2; p<sy[k][0]; p++) {

/*     	if (sx[j][m]==sy[k][p])/**/
/* printf("%3d %3d  - %3d %3d - %3d %3d - %3d %3d %3d\n",
    j,k, p,m, sx[j][m],sy[k][p], fq[sx[j][m]],fx[sx[j][m]],fy[sx[j][m]]);/**/

     	if (sx[j][m]==sy[k][p]) { fq[sx[j][m]]++;
     		if (j>fx[sx[j][m]]) fx[sx[j][m]]=j;
     		if (k>fy[sx[j][m]]) fy[sx[j][m]]=k;}

/* if (p>5) p=sy[k][0];} if (m>5) m=sx[j][0];} if (k>5) k=yp;} if (j>5) j=xp;} /**/
     }} /*pm*/
     }}/*kj*/
   for (p=0; p<1000; p++) if (fq[p]>10) { lx[fx[p]]++; 	ly[fy[p]]++;}
 }/*argc*/

/*----------------------------------------------------------*/
if(argc>2){ /* number unique drivers - bands /**/
   k=1;
   printf("\n");
   printf("X Drivers\n");
   for (p=0; p<60; p++) if (lx[p]>4) {
   	printf("* ! %3d %3d %3d %3d ",  k, p, lx[p], bx[p]);/**/
     	for (j=0; j<cx[p][j]; j++) { printf("%c", cx[p][j]);}
        printf("\n");
   	lx[p]=k;
   	k++; if(k>3 && k<19) k++;} else lx[p]=0;

   k=1;
   printf("\n");
   printf("Y Drivers\n");
   for (p=0; p<60; p++) if (ly[p]>4) {
   	printf("* ! %3d %3d %3d %3d ",  k, p, ly[p], by[p]);/**/
     	for (j=0; j<cy[p][j]; j++) { printf("%c", cy[p][j]);}
        printf("\n");
   	ly[p]=k;
   	k++; if(k>1 && k<19) k++;} else ly[p]=0;

/*-------------------------------------------------------*/
   printf("\n");	/* Display block preplacements /**/
   for (p=0; p<1000; p++)
   if (fq[p]>10 && lx[fx[p]]>0 && ly[fy[p]]>0)
 	printf("BF %3d %3d %3d\n",  p, lx[fx[p]], ly[fy[p]]);/**/
     else {fx[p]=0; fy[p]=0;}
   printf("\n");
 }/*argc*/

/*----------------------------------------------------------*/
if(argc>2){ /* Put driver band numbers back on all drivers/**/
/*  for (j=0; j<1000; j++) { fx[j]=0; fy[j]=0; }
    for (j=0; j<60;   j++) { lx[j]=0; ly[j]=0; } /**/

     for (j=0; j<xp; j++) {
     for (k=0; k<yp; k++) {
     for (m=2; m<sx[j][0]; m++) {
     for (p=2; p<sy[k][0]; p++) {

     	if (sx[j][m]==sy[k][p] && fq[sx[j][m]]>10) {
     		bx[j]=lx[fx[sx[j][m]]];
     		by[k]=ly[fy[sx[j][m]]];	}
     }} /*pm*/
     }}/*kj*/
   }/*argc*/

/*----------------------------------------------------------*/
     if(argc>2){ /* Display bands of all drivers /**/
     printf("\n");

/*
     printf("\n");
     printf("X Bands\n");
     for (j=0; j<xp; j++) { printf("%3d %3d :- ", j, bx[j]);
     	for (k=0; k<cx[j][k]; k++) { printf("%c", cx[j][k]);}
        printf("\n");}

     printf("\n");
     printf("Y Bands\n");
     for (j=0; j<yp; j++) { printf("%3d %3d -: ", j, by[j]);
     	for (k=0; k<cy[j][k]; k++) { printf("%c", cy[j][k]);}
        printf("\n");} /**/

/* Display driver preplacements /**/
     printf("\n");
     for (k=1; k<21; k++) { m=4;
     for (j=0; j<xp; j++)
     	if (k==bx[j] && fx[sx[j][2]]==0 && fy[sy[j][2]]==0) {
	     	fx[sx[j][2]]=k;
		fy[sy[j][2]]=m;
/*	     	printf("%3d %3d   %3d %3d :: ", j, bx[j],fx[sx[j][2]],fy[sy[j][2]]);
     		for (p=0; p<cx[j][p]; p++) { printf("%c", cx[j][p]);} /**/
	        printf("BF %3d %3d %3d\n", sx[j][2], k,m);
	        m=m+2;}
        }
     }/*argc*/

/*-----------------------------------------------*/
  } /* file open /**/
} /* main /**/
/*----------------------------------------------------------------*/
