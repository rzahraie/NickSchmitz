/* Program to pad MINC DOC file lines
   for simularity comparision on RHS

    Author: Nick Schmitz
    Date: 11/7/94
*/

#include <stdio.h>

main()
{
     int j,k,m;
     int ln;
     char a_str[256], b_str[256];

     for (ln=1; gets(a_str); ln++)
       {
        for (j=0; a_str[j]!='=' && a_str[j]; j++);

/*    printf("%d %s \n ",j,a_str);/**/

         if (a_str[j]!=0)
            {
            k=0;
            for (m=0; m<j;  m++) b_str[k++]=a_str[m];
            for (m=j; m<74; m++) b_str[k++]='.';
            for (m=j; a_str[m];  m++) b_str[k++]=a_str[m];
            b_str[k]=0;
            puts(b_str);
            } else puts(a_str);
       }
     printf("\n");
}
