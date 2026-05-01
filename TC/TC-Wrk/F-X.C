/*----------------------------------------------------------------*/
/*
    Program to scan SDL file for timing paths.
	Author: Nick Schmitz
    Date: 7/18/97

	2 output formats, switched by cmd line args.
	Used by tpath.bat / processing xdelay SDL timing paths delays.

	Program reads 2 sources - mathches one against another.
	If match found, then look ahead for timing numbers of path.

	first output is just whole lines - path & delay.
	2nd output is subset of first line btw "" & delay time
		calculated from logic level of second line.
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#define MAX_LINE_LENGTH  120


/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     int j,k,m;
     int ln,lx,lnc,lq;
     float time;
     char a_str[256],b_str[256],c_str[256], d_str[256];
     FILE *fx;

     strcpy(a_str,"f-x.p");/**/
     lnc=1;
     lq=0;

     if ((fx = fopen(a_str, "r")) == NULL) {
        printf("Cannot open file %s\n",a_str);
	printf("%d %s \n", argc,*argv);/**/
/*      exit(1);/**/
       } else {

     if(argc==1) printf("-----------------------------\n");/**/

    fgets(c_str,MAX_LINE_LENGTH,fx);

/*    printf("*!*%d\n", c_str[0]);/**/ /* skip lf lines /**/
    if (c_str[0]==10)
       for (j=1; fgets(c_str,MAX_LINE_LENGTH,fx) && c_str[0]==10; j++) m=1;

/*----------------------------------------------------------------*/
    for (ln=1; gets(a_str) && lnc; ln++)
       {
           m=1; /* check if matched /**/
           for (j=0; a_str[j] && m; j++) if (a_str[j]!=c_str[j]) m=0;
           if (j==0) m=0;
/*           printf("%d %d\n",j,m);/**/

           if (m>0) {
               if (lq>0 && argc==1) printf(".\n");
               if(argc==1) puts(a_str);/**/
               for (ln=1; gets(a_str) && ln<13; ln++) {
               	    for (k=1; a_str[k]; k++) m=1; /* find eol /**/
                    if (a_str[k-1]=='s') { /* ns found /**/
   		     ln=100;

/*----------------------------------------------------------------*/
                     if(argc==1) puts(a_str);/**/ /* output text /**/
                      else {

               	    	for (k=1; c_str[k]; k++) m=1; /* find eol /**/
                    	c_str[k-1]=' ';

			k=0;	m=0;
			for (j=0; c_str[j]; j++) { /* find stuff btw "" /**/
			  if (m==0 && c_str[j]=='"') m=1;
			    else if (m==1 && c_str[j]=='"' || c_str[j]=='.') {
				m=0;
				b_str[k++]=' ';
			      }
			    else if (m==1) b_str[k++]=c_str[j]; /* got it /**/
			  }
			b_str[k]=0;

/*----------------------------------------------------------------*/
			sscanf(a_str, "%d %s", &m,&d_str);/**/
/*			printf("%d * %s * %s\n", m,a_str,d_str);/**/
			if (m==0) time=3.5; else time=2.3*m+1.5;
			time=time*800000;

			if (k==0) printf("%s\n", c_str);/**/
			   else printf("%s %9.0f\n", b_str, time);/**/
			}
                     }
/*----------------------------------------------------------------*/
/*                    printf("%s,%d,%c",a_str,k,a_str[k-1]);/**/
                  }
/*               puts(a_str);/**/
       	       if (fgets(c_str,MAX_LINE_LENGTH,fx)) lnc=1; else lnc=0;
               if (c_str[0]=='.') lnc=0;
               lq++;
               if (lq>2) lnc=0;
              }
       }
     if(argc==1) printf("-----------------------------\n");/**/
  }
}
/*----------------------------------------------------------------*/
