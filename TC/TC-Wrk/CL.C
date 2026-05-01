/*----------------------------------------------------------------*/
/*
	Cleanup lines for timing paths
	Author: Nick Schmitz
	Date: 7/17/97
*/
/*----------------------------------------------------------------*/

#include <stdio.h>

/*----------------------------------------------------------------*/
main()
{
     int j,k,m;
     int ln;
     char a_str[256],c_str[256];

     for (ln=1; gets(a_str); ln++)
       {
/*            printf(" %s\n", a_str);/**/
	k=0;	m=0;
	for (j=0; a_str[j]; j++) {
	  if (m==0 && a_str[j]=='"') m=1;
	  else if (m==1 && a_str[j]=='"' || a_str[j]=='.') {
		m=0;
	  	c_str[k++]=' ';
	  }
	  else if (m==1) c_str[k++]=a_str[j];
	 }
	  c_str[k]=0;
	if (k==0) printf("%s\n", a_str);/**/
	  else printf("%s 25000000\n", c_str);/**/
       }
}
/*----------------------------------------------------------------*/
