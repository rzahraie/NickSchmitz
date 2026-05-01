/*----------------------------------------------------------------*/
/*  Create STM File
	Author: Nick Schmitz
    Date 5/28/98
    f-v filename
*/
/*----------------------------------------------------------------*/
#include <stdio.h>
#define MAX_LINE_LENGTH  1999

     FILE *fp;
     int j,k,m,p;
     int k_fn, c_fn, r_fn;
     int net, blk, ln, flg;
     int sig[10];
     char a_str[1999],d_str[1999];

     int  ik[2000], ok[2000];
     char cx[25000]; /* netlist names /**/
     int cxp, ikp, okp;
     int q,r,s;

void hdr()
{
/*        0,0,0,.x.,.x.,.x.,0;/**/

    printf("SIMULATION;\n");
    printf("STEP 1ns;\n");
    printf("TRACE ");
/*      sel_1_,sel_0_,i0,i1,i2,i3,Y0; /**/
    k=0;
    for (j=1; j<ikp; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ik[j]]);
/*	printf("%d %d %s\n",ik[j], j, &cx[ik[j]]);/**/
       }
    printf(",\n");
    k=0;
    printf("      ");
    for (j=1; j<okp; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ok[j]]);
       }
    printf(";\n");

    printf("TEST_VECTORS\n");
    k=0;
    printf("      ",ikp);
    for (j=1; j<ikp; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ik[j]]);
       }
    printf(",\n");
    k=0;
    printf("      ");
    for (j=1; j<okp; j++ ){
       if (j!=1) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
	printf(" %s", &cx[ok[j]]);
       }
    printf(";\n");
}
main(int argc, char *argv[])
{
/*----------------------------------------------------------*/
     for (j=0; j<2000; j++)  { ik[j]=0; ok[j]=0;}
     for (j=0; j<25000; j++) cx[j]=0;
     for (j=0; j<2000; j++) d_str[j]=0;

     m=0;	k=0; 	   d_str[0]=0;
     k_fn=0; 	c_fn=0;	   r_fn=0;	net=0;	blk=0;

     cxp=0;  ikp=1;  okp=1;

/*-----------------------------------------------*/
     if(argc>1){ *++argv; }/**/    /* Skip Prgname */
     strcpy(a_str,*argv);
     strcat(a_str,".sd");/**/
     if (flg) printf("// %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

/*----------------------------------------------------------*/
     p=0; m=0;
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
/* printf("*%s~%c%c", a_str, a_str[4], a_str[9]);/**/
 	if (a_str[4]==':' ) {
/*	 	printf(",%s", a_str);/**/
 		if (a_str[6]=='I' ) {
			d_str[p]=1;
                	ik[ikp++]=cxp;
			for (q=8; a_str[q]!='\n'; q++) cx[cxp++]=a_str[q];

/* printf("*%s~%c", a_str, a_str[q-1]);/**/
if (a_str[q-1]>='0' && a_str[q-1]<='9') {
 if (a_str[q-2]=='_' && a_str[q-1]!='_') cx[cxp++]='_';
 if (a_str[q-3]=='_' && a_str[q-1]!='_') cx[cxp++]='_';
}
if (a_str[8]=='g' && a_str[9]=='c' && a_str[10]=='l' && a_str[11]=='k')
	{d_str[p]=0; ikp--;}
if (a_str[8]=='G' && a_str[9]=='C' && a_str[10]=='L' && a_str[11]=='K')
	{d_str[p]=0; ikp--;}

			cx[cxp++]=0;
/* printf("%d %d %s\n",ik[ikp-1], ikp-1, &cx[ik[ikp-1]]);/**/
}
 		if (a_str[6]=='O' ) {
			d_str[p]=2;
                	ok[okp++]=cxp;
			for (q=8; a_str[q]!='\n'; q++) cx[cxp++]=a_str[q];
if (a_str[q-1]>='0' && a_str[q-1]<='9') {
			if (a_str[q-2]=='_' && a_str[q-1]!='_') cx[cxp++]='_';
			if (a_str[q-3]=='_' && a_str[q-1]!='_') cx[cxp++]='_';
}
			cx[cxp++]=0;
/* printf("%d %d %s\n",ok[okp-1], okp-1, &cx[ok[okp-1]]);/**/
}
 		p++;
	 }
 	if (a_str[9]==':') {
/* 		printf(".%s", a_str);/**/
 		if (m==0) hdr();
		m++;

		p=0;
		for (q=11; a_str[q]!='\n' && q<250; q++) if (d_str[p++]==1){
		if (p!=1) printf(","); else printf("   ");/**/
/* 		printf(".%d %d %d",q,p,d_str[p-1]);/**/
        	switch (a_str[q]) {
	            case '0': printf("0"); break;
	            case '1': printf("1"); break;
	            case 'X': printf(".x."); break;
	            case 'C': printf(".c."); break;
		}}

		p=0;
		for (q=11; a_str[q]!='\n'; q++)if (d_str[p++]==2){
        	switch (a_str[q]) {
	            case '0': printf(",0"); break;
	            case '1': printf(",1"); break;
	            case 'L': printf(",0"); break;
	            case 'H': printf(",1"); break;
	            case 'X': printf(",.x."); break;
	            case 'Z': printf(",.x."); break;
/*	            case 'Z': printf(",.z."); break;/**/
		}}
        printf(";\n");
	}
a_str[9]=0;
 }
    printf("END TEST_VECTORS;\n");
    printf("END SIMULATION;\n");
}
} /* main /**/
/*----------------------------------------------------------------*/

