/*----------------------------------------------------------------*/
/*
	Demo program to read & convert NAS M5 Switch Matrix Format
	Author: Nick Schmitz
	Date: 3/21/95
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <alloc.h>/**/


/*----------------------------------------------------------------*/
main()
{
     int j,k,m;
     int ln;
     char a_str[256],c_str[256];

     for (ln=1; gets(a_str); ln++)
       {
/*            printf(" %s\n", a_str);/**/
/*           puts(a_str);/**/
            sscanf(a_str,"%s %s %s",&c_str,&c_str,&c_str);
/*            printf(" %s\n", c_str);/**/
/*            puts(c_str);/**/
	    m=0;
	    if (c_str[0]=='0') m=1;
	    _exit(m);/**/
       }
}
/*----------------------------------------------------------------*/
