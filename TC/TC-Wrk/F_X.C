/*----------------------------------------------------------------*/
/*
	Demo program to read & convert NAS M5 Switch Matrix Format
	Author: Nick Schmitz
	Date: 3/21/95
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <alloc.h>/**/
#define MAX_LINE_LENGTH  120


/*----------------------------------------------------------------*/
main()
{
     int j,k,m;
     int ln,lx,lnc,lq;
     char a_str[256],c_str[256];
     FILE *fx;

     strcpy(a_str,"f-x.p");/**/
     lnc=1;
     lq=0;

     if ((fx = fopen(a_str, "r")) == NULL) {
        printf("Cannot open file %s\n",a_str);
/*      exit(1);/**/
       } else {

     printf("-----------------------------\n");/**/

    fgets(c_str,MAX_LINE_LENGTH,fx);
/*    printf("*!*%d\n", c_str[0]);/**/
    if (c_str[0]==10)
       for (j=1; fgets(c_str,MAX_LINE_LENGTH,fx) && c_str[0]==10; j++) m=1;

    for (ln=1; gets(a_str) && lnc; ln++)
       {
           m=1;
           for (j=0; a_str[j] && m; j++) if (a_str[j]!=c_str[j]) m=0;
           if (j==0) m=0;
/*           printf("%d %d\n",j,m);/**/

           if (m>0) {
               if (lq>0) printf(".\n");
               puts(a_str);/**/
               for (ln=1; gets(a_str) && ln<13; ln++) {
               	    for (k=1; a_str[k]; k++) m=1;
                    if (a_str[k-1]=='s') {
                    	ln=100;
	                puts(a_str);/**/
		      }
/*                    printf("%s,%d,%c",a_str,k,a_str[k-1]);/**/
                  }
/*               puts(a_str);/**/
       	       if (fgets(c_str,MAX_LINE_LENGTH,fx)) lnc=1; else lnc=0;
               if (c_str[0]=='.') lnc=0;
               lq++;
               if (lq>2) lnc=0;
              }
       }
     printf("-----------------------------\n");/**/
  }
}
/*----------------------------------------------------------------*/
