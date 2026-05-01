/* Program to count eq's RHS for simularity
    Author: Nick Schmitz
    Date: 11/7/94
*/
#include <stdio.h>
main()
{
     int j,k,m;
     int ln;
     char a_str[255], b_str[255];
     int sme;

     gets(b_str);
     for (m=0; b_str[m]; m++)
     	{ a_str[m]=b_str[m]; }
     a_str[m]=0;

     j=1;
     for (ln=1; gets(b_str); ln++)
       {
        for (k=0; b_str[k]; k++)
        sme=1;
        if (k>75)
          for (m=75; b_str[m]; m++)
            if (a_str[m]!=b_str[m]) sme=0;
        if (sme!=0)  ++j;
           else {
              printf("%u \t %s\n",j,a_str);

              for (m=0; b_str[m]; m++)
         	{  a_str[m]=b_str[m]; }
              a_str[m]=0;

              j=1;
            }
       }
       printf("%u \t %s\n",j,a_str);
}
