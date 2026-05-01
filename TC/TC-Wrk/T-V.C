/*----------------------------------------------------------------*/
/*  Generate test vectors
	Author: Nick Schmitz
    Date: 5/13/98
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
     int j,k,m,n,p;
     int q[5];
     char a_str[256],b_str[256],c_str[256],d_str[256];

/*----------------------------------------------------------------*/
trc() {
    k=0;
    printf(" TRACE [");
    for (j=q[2]; j>=0; j-- ){
       if (j!=q[2]) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
       printf("avec_%d_",j);
       }
    printf("],\n");

    k=0;
    printf("     [");
    for (j=q[1]; j>=0; j-- ){
       if (j!=q[1]) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
       printf("bvec_%d_",j);
       }
    printf("],\n");

    k=0;
    printf("     [");
    for (j=q[0]; j>=0; j-- ){
       if (j!=q[0]) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}
       printf("cvec_%d_",j);
       }
    printf("];\n");
}
/*----------------------------------------------------------------*/
vec(int whv, int whb) {
    k=0;
    printf("  SET ");
    for (j=q[abs(whv)]; j>=0; j-- ){
       if (j!=q[abs(whv)]) printf(",");
       k++; if (k>6 && j!=0) { printf("\n       "); k=0;}

       if (abs(whv)==1) printf("a"); else printf("b");
       if (whv>=0) printf("vec_%d_",j); else printf("vec_%d_",q[abs(whv)]-j);

       if (whb<0) printf("=1"); else {
	  if (whv>=0) {if (j==whb) printf("=1"); else printf("=0");}
	  if (whv<0)  {if (j==whb) printf("=0"); else printf("=1");}
       }
       }
    printf(";\n");
/*  printf(" set avec_1_=0,avec_0_=0, bvec_1_=0,bvec_0_=0; CLOCKF gclk0;\n");/**/
}
/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     j=0;   k=0;    m=0;    n=0;   p=0;

    printf("MACRO CHECK SET;\n");
    printf("MACRO IMPLICIT_CLOCK CLOCKF;\n");
    printf("MACRO OUTPUT_CLEAR SET;\n");
    printf("SYSTEM_TEST; STEP 1ns;\n");

    if(argc==4){  /* only valid # /**/

/*     printf("rem .%d.\n",argc);/**/

     k=2;
     for (j=argc; j>1; j--) {*++argv;/**/    /* Skip Prgname */
     	strcpy(a_str,*argv);
     	sscanf(a_str,"%d^[\n]",&q[k--],&c_str);
/*     	printf("rem %s.%d.\n",a_str,k);/**/
	}

/*     for (j=0; j<argc; j++ ){
     	printf("rem .%d.\n",q[j]);
	}/**/

    trc();
    printf("\" ---------------\n\n");
    for (m=0; m<=q[1]; m++) {
    	vec(1, m);
    	vec(2,-1);
    	printf("   CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=0; m<=q[2]; m++) {
    	vec(1,-1);
    	vec(2, m);
    	printf("   CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=0; m<=q[2]; m++) {
    	vec( 1,m);
    	vec(-2,m);
    	printf("   CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
    for (m=0; m<=q[1]; m++) {
    	vec(-1,m);
    	vec( 2,m);
    	printf("   CLOCKF gclk0;\n\n");
    }

    printf("\" ---------------\n\n");
/*  printf(" set avec_1_=0,avec_0_=0, bvec_1_=0,bvec_0_=0; CLOCKF gclk0;\n");/**/
    printf("END SYSTEM_TEST;\n");
    }
/*----------------------------------------------------------------*/
} /* main/**/
