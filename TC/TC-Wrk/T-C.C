/*----------------------------------------------------------------*/
/*
	Author: Nick Schmitz
    Date: 7/14/97
*/
/*----------------------------------------------------------------*/

#include <stdio.h>


/*----------------------------------------------------------------*/
main()
{
     int j,k,m,n,p;
     int ln;
     char a_str[256],b_str[256];

     a_str[0]=0;    b_str[0]=0;  m=0;

     for (ln=1; gets(a_str); ln++) {
/* printf(".%s.\n", a_str);/**/

	for (p=0; a_str[p] && a_str[p]==' '; p++) p=p;

	k=p;	j=0;
	for (n=p; a_str[n] && b_str[j] && a_str[n]!='.'; n++)
                if (b_str[j++]!=a_str[n]) k=-1;
        if (j==0) k=-1;
/* printf("*%c %d %d %d \n", a_str[p],p,k,n);/**/

        if (a_str[p]==',' || k>0){ /* merge onto line /**/
            if (a_str[p]==','){
                for (j=p; a_str[j]; j++)
                	if (a_str[j]!='.') b_str[m++]=a_str[j];
                		else if (b_str[m-1]!=',') b_str[m++]=',';
/*                	if (a_str[j]=='.') b_str[m++]=',';
                		else b_str[m++]=a_str[j];/**/
                b_str[m]=0;
/* printf("m#%s#\n", b_str);/**/
              }
         } else if (a_str[p]>0) { /* Just xfer to b/**.

/* printf("%s\n", b_str);/**/

	     if (m>0) printf("%s\n", b_str);/**/
              m=0;
              for (j=p; a_str[j]; j++)
              	if (a_str[j]=='.') b_str[m++]=',';
              		else b_str[m++]=a_str[j];
              b_str[m]=0;
/* printf("t*%s*\n", b_str);/**/
	  }
       }
}
/*----------------------------------------------------------------*/
