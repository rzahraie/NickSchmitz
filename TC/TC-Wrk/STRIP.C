/* Program to strip commented lines from minc source files
    Author: Nick Schmitz
    Date: 4/30/97
*/

#include <stdio.h>

main()
{
     int j,k,m;
     int ln;
     char a_str[256];

     k=-1;
     for (ln=1; gets(a_str); ln++)
       {
        for (j=0; a_str[j]; j++) if (a_str[j]=='"') a_str[j]=0;
        if ((j==1) || (a_str[0]==0)) k=k+1; else k=-1;
        if (k<=0) puts(a_str);
       }
/* puts(" "" %done%");/**/
}
