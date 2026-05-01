/*----------------------------------------------------------------*/
/*  Generate timing paths for AGL
	Author: Nick Schmitz
    Date 3/5/98
    f-c filename any_text

	Filename - design name (no extension).
    5/16/98 - Test Vectors
*/
/*----------------------------------------------------------------*/
#include <stdio.h>
#define MAX_LINE_LENGTH  120

     FILE *fp;
     int j,k,m,p;
     int k_fn, c_fn, r_fn;
     int net, blk, ln, flg;

     int sig[10];
     char a_str[256],b_str[256],c_str[256],d_str[256];

     int  kk[1000], rk[1000], ik[1000], ok[1000];
     int  mk[1000], bk[1000]; /* Pathgen vars /**/
     char cx[25000]; /* netlist names /**/
     int cxp, mkp, bkp, ikp, okp, rkp, kkp;
     int q,r,s;
int qq[10];
/*-----------------------------------------------*/
flgt(int argc, char *argv[])
{
     if(argc>2){  /* pick up filename from command line /**/
          *++argv;/**/    /* Skip Prgname */
          *++argv;/**/    /* Skip Prgname */
/*        printf("C %s \n ",*argv);/**/
     if (*argv[0]=='T') flg=0; else flg=1;
     }
}
/*----------------------------------------------------------------*/
trc() {
    printf("\n");
    k=0;
    printf(" TRACE ["); /* first inputs /**/
    for (j=1; j<(ikp+1)/2; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ik[j]]);
       }
    printf("],\n");

    k=0;
    printf("       ["); /* second inputs /**/
    for (j=(ikp+1)/2; j<ikp; j++){
       if (j!=(ikp+1)/2) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ik[j]]);
       }
    printf("],\n");

    k=0;
    printf("        "); /* clocks /**/
    for (j=1; j<kkp; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[kk[j]]);
       }
    if (kkp>1) printf(",\n");

    k=0;
    printf("       ["); /* outputs /**/
    for (j=1; j<=okp; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ok[j]]);
       }
    printf("];\n");
}
/*----------------------------------------------------------------*/
vec(int whv, int whb) {
    k=0;
    printf("  SET ");
    if (abs(whv)==1) {q=1; r=(ikp+1)/2;}  else { q=(ikp+1)/2; r=ikp;}
    for (j=q; j<r; j++ ){
       if (j!=q) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ik[j]]);
       if (whb<0) printf("=1"); else {
	  if (whv>=0) {if (j==whb) printf("=1"); else printf("=0");}
	  if (whv<0)  {if (j==whb) printf("=0"); else printf("=1");}
        }
       }
    printf(";\n");
/*  printf(" set avec_1_=0,avec_0_=0, bvec_1_=0,bvec_0_=0; CLOCKF gclk0;\n");/**/
}
/*-----------------------------------------------*/
gen_tv()
{
    printf("MACRO CHECK SET;\n");
    printf("MACRO IMPLICIT_CLOCK CLOCKF;\n");
    printf("MACRO OUTPUT_CLEAR SET;\n");
    printf("SYSTEM_TEST; STEP 1ns;\n");

    trc();
    printf("\" ---------------\n\n");
    for (m=1; m<=ikp/2; m++) {
    	vec(1, m);
    	vec(2,-1);

/*	for (j=1; j<kkp; j++) printf("CLOCKF %s; ", &cx[kk[j]]);/**/
    	printf("CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=ikp/2; m<ikp; m++) {
    	vec(1,-1);
    	vec(2, m);
    	printf("CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=1; m<ikp/2; m++) {
    	vec( 1,m);
    	vec(-2,ikp/2+m-1);
    	printf("CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=1; m<ikp/2; m++) {
    	vec(-2,m);
    	vec( 1,ikp/2+m-1);
    	printf("CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=1; m<ikp/2; m++) {
    	vec(-1,m);
    	vec( 2,ikp+m-1);
    	printf("CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=1; m<ikp/2; m++) {
    	vec( 2,m);
    	vec(-1,ikp+m-1);
    	printf("CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    printf("END SYSTEM_TEST;\n");
}
/*-----------------------------------------------*/
void fmax()
{
     printf("// -----------------------------\n");
     printf("// All Reg x Reg\n// .\n");/**/

     for (j=1; j<rkp; j++) {
     for (k=1; k<rkp; k++) {
        printf("__FMAX %s to %s > 50.0\n", &cx[rk[j]], &cx[rk[k]]);
/* if (j>5) j=rkp;/**/
     } printf("\n"); } /**/
	printf("\n");
}

/*-----------------------------------------------*/
void fmxc()
{
     printf("// -----------------------------\n");
     printf("// All clks\n// .\n");/**/

     for (k=1; k<kkp; k++) {
        printf("// FMAX %s 50.0\n", &cx[kk[k]]);
/*        printf("__FMAXCLOCK %s 50.0\n", &cx[kk[k]]);/**/
	}
	printf("\n");
}
/*-----------------------------------------------*/
void setup()
{
     printf("// -----------------------------\n");
     printf("// All INPUT x Reg\n// . \n");/**/
     for (k=1; k<rkp; k++) {
        printf("__SETUP INPUT to %s <10.1\n", &cx[rk[k]]);
      }
	printf("\n");
}

/*-----------------------------------------------*/
void clko()
{
     printf("// -----------------------------\n");
     printf("// All Reg x OUTPUT\n// .\n");/**/

     for (k=1; k<rkp; k++) {
        printf("__CLKOUT %s to OUTPUT < 10.2\n", &cx[rk[k]]);
	}
	printf("\n");
}

/*-----------------------------------------------*/
void arr()
{
     printf("// -----------------------------\n");
     printf("// All Inp x OUTPUT\n// .\n");/**/

     for (k=1; k<ikp; k++) {
        printf("__ARR %s to OUTPUT < 10.3\n", &cx[ik[k]]);
	}
	printf("\n");
}

/*-----------------------------------------------*/
void dpp()
{
     printf("// -----------------------------\n");/**/
     printf("// All INPUT x Out\n");/**/
     printf("// .\n");/**/
    for (k=1; k<okp; k++) {
        printf("__DEP INPUT to %s < 10.4\n", &cx[ok[k]]);
      } /**/
	printf("\n");
}

/*-----------------------------------------------*/
void clkclk()
{
     printf("// -----------------------------\n");/**/
     printf("// All Reg x Reg\n");/**/
     printf("// .\n");/**/

     for (j=1; j<rkp; j++) {
     for (k=1; k<rkp; k++) {
        printf("__CLK2CLK %s to %s > 50.0\n", &cx[rk[j]], &cx[rk[k]]);
/* if (j>5) j=rkp;/**/
     } printf("\n"); } /**/
	printf("\n");
}

/*-----------------------------------------------*/
void onchip()
{
     printf("// -----------------------------\n");/**/
     printf("// All Reg x Reg\n");/**/
     printf("// .\n");/**/
     printf("__ON_CHIP\n");
    for (j=1; j<rkp; j++) {
    for (k=1; k<rkp; k++) {
        printf(" %s %s 9.1\n", &cx[rk[j]], &cx[rk[k]]);

     } printf("\n"); } /**/
    printf("__END\n\n");
}

/*-----------------------------------------------*/
void tpd()
{
     printf("// -----------------------------\n");/**/
     printf("// All Ipin x Opin\n");/**/
     printf("// .\n");/**/
     printf("__PAD_PAD\n");
    for (j=1; j<ikp; j++) {
    for (k=1; k<okp; k++) {
        printf(" %s %s 9.2\n", &cx[ik[j]], &cx[ok[k]]);

/*    if (k>5) k=ikp;} printf("\n"); if (j>5) j=ikp;} /**/
     } printf("\n"); } /**/
    printf("__END\n\n");
}

/*-----------------------------------------------*/
void tpc()
{
     printf("// -----------------------------\n");/**/
     printf("// All Clk x Reg\n");/**/
     printf("// .\n");/**/
     printf("__PAD_CLOCK\n");
    for (j=1; j<kkp; j++) {
    for (k=1; k<rkp; k++) {
        printf("%s %s 9.3\n", &cx[kk[j]], &cx[rk[k]]);

     } printf("\n"); } /**/
    printf("__END\n\n");
}

/*-----------------------------------------------*/
void tco()
{
     printf("// -----------------------------\n");/**/
     printf("// All Reg x Opin\n");/**/
     printf("// .\n");/**/
     printf("__CLOCK_OUT\n");
    for (j=1; j<rkp; j++) {
    for (k=1; k<okp; k++) {
        printf("%s %s 9.4\n", &cx[rk[j]], &cx[ok[k]]);

     } printf("\n"); } /**/
	printf("\n");
    printf("__END\n\n");
}

/*-----------------------------------------------*/
void rd_nts(int argc, char *argv[])
{
     if(argc>1){  /* pick up filename from command line /**/
          *++argv;/**/    /* Skip Prgname */
/*        printf("C %s %f  \n ",*argv);/**/
     }

     strcpy(a_str,*argv);
     strcat(a_str,".nts");/**/
     if (flg) printf("// %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

/*     if(argc>2) printf("High Fanout Nets: {Net#,,FO, Name}\n");/**/

/*----------------------------------------------------------*/
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
/* printf("*%s", a_str);/**/

        if (a_str[0]!='/' && a_str[0]!='-'){ /* end of net /**/
            sscanf(a_str,"%d %s %s",&j,&c_str,&b_str);

            q=-1;
            for (p=0; c_str[p]; ++p) if (c_str[p]=='|') q=p;
            if (q>0) {q++;
                for (p=0; c_str[q]; ++p) c_str[p]=c_str[q++];
                c_str[p]=0;
                }
/* printf("*%s*%s\n", a_str, c_str);/**/

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

            if (((c_str[1]=='_' && c_str[2]=='C' &&
                c_str[3]=='L' && c_str[4]=='K')) ||
               ((c_str[1]=='_' && c_str[2]=='I' &&
                c_str[3]=='C' && c_str[4]=='x')) ||
               ((c_str[1]=='_' && c_str[2]=='I' &&
                c_str[3]=='G' && c_str[4]=='K'))) {
                	k_fn++;

                	r=0; s=0;
                	for (q=0; q<bkp; q++) {
                		if (bk[q]==j) r=1;
                		if (j>bk[q]) s=q;}
/* printf("*%s*%s\n", a_str, c_str);/**/
/* printf("%3d %3d %3d %3d - %3d %3d\n", bkp,j,r,s, k_fn, m); /**/

                	if (r==0) {
                	  	if (s<bkp-1) for (r=bkp; r>s; r--) bk[r]=bk[r-1];
                		s++; bk[s]=j; bkp++;}

			if (k_fn==1 && m==1) { /* get clk name */
				kk[kkp++]=cxp;
				for (q=0; d_str[q]; q++) cx[cxp++]=d_str[q];
/* printf(" %d\n",cxp);/**/
				cx[cxp++]=0;}

/* for (q=0; q<5; q++) printf(" %3d",bk[q]); printf("\n"); /**/
                 }
            else if (c_str[1]=='_' && c_str[2]=='C' &&
                c_str[3]=='T' && c_str[4]=='L') r_fn++;
            else c_fn++;

/* printf("%d %s*%d %d %d - %d %d\n", net, d_str, k_fn,r_fn,c_fn, m,blk);/**/
/* printf("%d %s %s*%d %d %d - %d %d\n", net, d_str,b_str, k_fn,r_fn,c_fn, m,blk);/**/
}
/*----------------------------------------------------------*/
/* add signals to Ouput list /**/
            if (c_str[0]=='I' && c_str[1]=='_' &&
                c_str[2]=='O' ){
                /* && b_str[0]!='R') {/**/
                	r=0; s=0;
                    for (q=0; q<okp; q++) {
                        if (ok[q]==j) r=1;
                        if (j>ok[q]) s=q;}
/* printf("%3d %3d %3d %3d\n", okp,j,r,s); /**/

                	if (r==0) {
                        if (s<okp-1) for (r=okp; r>s; r--) ok[r]=ok[r-1];
                        s++; ok[s]=j; okp++;}
/* printf("*%s", a_str);/**/
/* for (q=0; q<okp; q++) printf("%d,",ok[q]); printf(",o\n");/**/
/* printf("%3d %3d %3d %3d\n", okp,j,r,s); /**/
	}  /**/

/*----------------------------------------------------------*/
/* add signals to Input list, if not on output /**/
            if (c_str[0]=='I' && c_str[1]=='_' &&
                c_str[2]=='I' && c_str[3]=='G' && b_str[0]!='R') {
		if (m==1 && k_fn==0) {
                	r=0; s=0;
                    for (q=0; q<okp; q++) if (ok[q]==j) r=1;
                    if (r==0) for (q=0; q<ikp; q++) {
                        if (ik[q]==j) r=1;
                        if (j>ik[q]) s=q;}
/* printf("%3d %3d %3d %3d\n", ikp,j,r,s); /**/

                	if (r==0) {
                        if (s<ikp-1) for (r=ikp; r>s; r--) ik[r]=ik[r-1];
                        s++; ik[s]=j; ikp++;}
/*            ik[ikp++]=j;/**/
/* for (q=0; q<ikp; q++) printf("%d,",ik[q]); printf(",i\n");/**/
/* printf("%3d %3d %3d %3d\n", ikp,j,r,s); /**/
			}
/* if (m==1 && k_fn==0)/**/
/* printf("@%3d %3d %3d %3d %3d *%s*%s\n", net, j, jp, k_fn, m, d_str, c_str);/**/
}
/*----------------------------------------------------------*/
        if (a_str[0]=='-') { /* end of net /**/
/*	    if (k_fn>blk/15 && m==1 )
	            printf("rem *c*%d %s* %d - %d\n", net, d_str,k_fn, blk);/**/
	    if (k_fn>blk/15 && m==1 ) {
/*	            printf("rem *c*%d %s* %d\n", net, d_str,k_fn);/**/
		    k++;
		    sig[k]=net;
		}
/*	    if (r_fn>blk/15 && m==1 )
	            printf("rem *r*%d %s* %d %d\n", net, d_str,r_fn, blk);/**/
	    if (r_fn>blk/15 && m==1 ) {
/*	            printf("rem *r*%d %s* %d\n", net, d_str,r_fn);/**/
		    k++;
		    sig[k]=net;
		}
            m=0;
            net=0;
    	    k_fn=0; 	c_fn=0;  	r_fn=0;
       } /* end of net /**/
} /* gets */
} /* file */
}

/*-----------------------------------------------*/
void rd_blk(int argc, char *argv[])
{
     if(argc>1){ *++argv; }/**/    /* Skip Prgname */
     strcpy(a_str,*argv);
     strcat(a_str,".blk");/**/
     if (flg) printf("// %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

/*----------------------------------------------------------*/
	k=1; m=1;
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
/* printf("*%s", a_str);/**/

        if (a_str[0]!='/'){ /* real blocks /**/
            sscanf(a_str,"%d %s %s %s",&j,&c_str,&b_str,&d_str);
/*        printf("%3d *%s*%s*%s*\n",j,c_str, b_str, d_str);/**/

        if (d_str[0]=='I'){ /* pins /**/
/*        printf("j:%3d m:%3d o:%3d ik:%3d *%s*\n",j,m,okp, ik[m],c_str);/**/
       	 	if (ik[m]==j) {
	        	ik[m++]=cxp;
			for (q=0; c_str[q]; q++) cx[cxp++]=c_str[q];
/* printf(" %d\n",cxp);/**/
			cx[cxp++]=0;
/*	for (q=ik[m-1]; cx[q]; q++) printf ("%c",cx[q]); printf("\n");/**/
}
	           else {
			ok[okp++]=cxp;
			for (q=0; c_str[q]; q++) cx[cxp++]=c_str[q];
/* printf(" %d\n",cxp);/**/
			cx[cxp++]=0;}

	  }

	if (bk[k]==j) {
/*        printf("%3d %3d %s ",j,k,c_str);/**/
        mk[mkp++]=cxp;
		for (q=0; c_str[q]; q++) cx[cxp++]=c_str[q];
/* printf(" %d\n",cxp);/**/
		cx[cxp++]=0;
		k++;
/* printf(" %d\n",cxp);/**/
	   } /* j */
	} /* blks */
     } /* gets */
   } /* file open */

}

/*----------------------------------------------------------*/
void rd_cfg(int argc, char *argv[])
{
     if(argc>1){ *++argv; }/**/    /* Skip Prgname */
     strcpy(a_str,*argv);
     strcat(a_str,".cfg");/**/
     if (flg) printf("// %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

/*----------------------------------------------------------*/
	k=1;
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
/* printf("*%s", a_str);/**/

        if (a_str[0]=='F' || a_str[2]=='F' || a_str[3]=='F'){ /* FFDATA & INFF & OUTFFOUT /**/
            sscanf(a_str,"%[^\"]%[^)]%s",&c_str,&b_str,&d_str);
/*        printf("%3d *%s*%s*%s*\n",j,c_str, b_str, d_str);/**/

        if (a_str[0]==9 || a_str[0]=='F') /* repeated reg bug /**/
        if ((a_str[1]=='F' && a_str[2]=='O') /* FFOUT /**/
           || (a_str[3]=='F' && a_str[4]=='O') /* INFFOUT /**/
           || (a_str[4]=='F' && a_str[5]=='O')){ /* OUTFFOUT /**/
/*        if (a_str[1]=='F' && a_str[2]=='D'){ /* FFDATA /**/
		rk[rkp++]=cxp;

            q=-1;
            for (p=0; b_str[p]; ++p) if (b_str[p]=='|' && q<0) q=p;
            if (q>0) {q++;
                b_str[0]='"';
                for (p=1; b_str[q] && b_str[q]!='|'; ++p) b_str[p]=b_str[q++];
                b_str[p]='"';
                }
/* printf("*%s*%s%d\n", a_str, b_str,a_str[0]);/**/


		for (q=1; b_str[q]!='"'; q++) cx[cxp++]=b_str[q];
/* printf(" %d\n",cxp);/**/
		cx[cxp++]=0;/**/
	  }
	} /* blks */
     } /* gets */
   } /* file open */
}
/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
/*----------------------------------------------------------*/
     for (j=0; j<1000; j++)  { bk[j]=0; mk[j]=0; kk[j]=0; rk[j]=0;
     		ik[j]=0; ok[j]=0;}
     for (j=0; j<25000; j++) cx[j]=0;

     m=0;	k=0; 	   d_str[0]=0;
     k_fn=0; 	c_fn=0;	   r_fn=0;	net=0;	blk=0;

     cxp=0;    bkp=1;  mkp=1;   ikp=1;  okp=1;
     rkp=1;  kkp=1;    flg=1;   /* Init vars /**/

/*-----------------------------------------------*/
    flgt(argc, argv);
    if (flg){
     printf("// __FMAX\n// \n");
     printf("// Timing Path Generation A: Nick Schmitz 3/5/98\n");
     printf("// \n");
    }
	rd_nts(argc, argv);

/* printf("// \nPin\n// "); for (j=0; j<ikp; j++) printf ("%3d,",ik[j]); printf("\n");/**/
/* printf("// \nBk \n// "); for (j=0; j<bkp; j++) printf ("%3d,",bk[j]); printf("\n");/**/

	okp=1;
	rd_blk(argc, argv);
	rd_cfg(argc, argv);
/*-----------------------------------------------*/

if (!flg) gen_tv();
if (flg) {
	printf("\n");
	printf("// b:%3d n:%3d - i:%3d o:%3d -  r:%3d k:%3d c:%6d %3d\n",
		bkp,mkp, ikp,okp, rkp,kkp, cxp, argc);/**/
/*-----------------------------------------------*/
/*
	printf("%3d - ", bkp);
	for (j=1; j<bkp; j++) printf(" %3d",bk[j]);
	printf("\n"); /**/

/*-----------------------------------------------*/

printf("\n// Inp\n//    "); for (j=1; j<ikp && j<20; j++) printf("%s,", &cx[ik[j]]);
printf("\n// Out\n//    "); for (j=1; j<okp && j<20; j++) printf("%s,", &cx[ok[j]]);
printf("\n// Clk\n//    "); for (j=1; j<kkp && j<20; j++) printf("%s,", &cx[kk[j]]);
printf("\n// Ins\n//    "); for (j=1; j<mkp && j<20; j++) printf("%s,", &cx[mk[j]]);
printf("\n// Reg\n//    "); for (j=1; j<rkp && j<20; j++) printf("%s,", &cx[rk[j]]);
printf("\n\n");/**/
/*-----------------------------------------------*/

     if(argc==2)	onchip();
     if(argc==4)	fmax();

     if(argc==3) {
     	onchip();
	tpd();
	tpc();
	tco();}

     if(argc>4){
    printf("//Generated\n");
    printf("//Endgenerated\n");
    fmxc();
	fmax();
	setup();
	clko();
	arr();
	dpp();
	clkclk();
    printf("//Generated\n");
    printf("//Endgenerated\n");
	}

     printf("// -----------------------------\n");/**/
}
} /* main /**/
/*----------------------------------------------------------------*/
