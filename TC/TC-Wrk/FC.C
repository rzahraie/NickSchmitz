/*----------------------------------------------------------------*/
/*
	Author: Nick Schmitz
    Date: 7/14/97
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#define MAX_LINE_LENGTH  120


/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     int j,k,m,p;
     int k_fn, c_fn, r_fn;
     int net, blk;
     int ln;
     int sig[10];
     FILE *fp;
     char a_str[256],b_str[256],c_str[256],d_str[256];

     m=0;	k=0; 	   d_str[0]=0;
     k_fn=0; 	c_fn=0;	   r_fn=0;	net=0;	blk=0;

     if(argc>1){  /* pick up date from command line /**/
        *++argv;/**/    /* Skip stk sym */
/*        printf("C %s %f  \n ",*argv);/**/
     }

     strcpy(a_str,*argv);
     strcat(a_str,".nts");/**/
     printf("rem %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
/* printf(" %s\n", a_str);/**/

        if (a_str[0]!='/' && a_str[0]!='-'){
            sscanf(a_str,"%d %s %s",&j,&c_str,&b_str);

            if (c_str[0]=='I') m=1;
/*            printf(" %d*%s\n", j,c_str);/**/

            if (k_fn==0 && c_fn==0 && r_fn==0) {
            	net=j;
            	for (p=0; c_str[p]; ++p) d_str[p]=c_str[p];
            	d_str[p]=0;
              } else {
                if (j>blk) blk=j;
              }
            if (c_str[1]=='_' && c_str[2]=='C' &&
                c_str[3]=='L' && c_str[4]=='K') k_fn++;
            else if (c_str[1]=='_' && c_str[2]=='C' &&
                c_str[3]=='T' && c_str[4]=='L') r_fn++;
            else c_fn++;

/* printf("%d %s*%d %d %d - %d %d\n", net, d_str,k_fn,r_fn,c_fn, m,blk);/**/
           }
        if (a_str[0]=='-') {
/* printf("%d %s*%d %d %d - %d %d\n", net, d_str,k_fn,r_fn,c_fn, m,blk);/**/

/*	    if (k_fn>blk/10 && m==1 )
	            printf("rem *c*%d %s* %d\n", net, d_str,k_fn);/**/
	    if (k_fn>blk/8 && m==1 ) {
	            printf("rem *c*%d %s* %d\n", net, d_str,k_fn);/**/
		    k++;
		    sig[k]=net;
		}
/*	    if (r_fn>blk/10 && m==1 )
	            printf("rem *r*%d %s* %d\n", net, d_str,r_fn);/**/
	    if (r_fn>blk/8 && m==1 ) {
	            printf("rem *r*%d %s* %d\n", net, d_str,r_fn);/**/
		    k++;
		    sig[k]=net;
		}
            m=0;
            net=0;
    	    k_fn=0; 	c_fn=0;  	r_fn=0;
          }
       }
	if (k>0) {
		printf("\\vantis\\bin\\dmrouter -f 35 -P -! ");
		for (j=1; j<=k; j++) printf(" %d",sig[j]);/**/
		printf(" >> %s.lgg\n",*argv);
	} else {
		printf("\\vantis\\bin\\dmrouter -f 35 -P >>%s.lgg\n",*argv);
	}
 }
}
/*----------------------------------------------------------------*/
