/* Program to convert Altera EDIF to Minc EDIF

    Author: Nick Schmitz
    Date: 11/7/94
*/

#include <stdio.h>

main()
{
     int j,k,m;
     int ln;
     char a_str[256];

     for (ln=1; gets(a_str); ln++)
       {
        for (j=0; a_str[j]; j++) if (a_str[j]=='&') a_str[j]='X';

            puts(a_str);
       }
}
