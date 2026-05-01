/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/

#include <stdio.h>
/*  tot num wires = 3 tot length = 35595  loads and delay: /**/

/*----------------------------------------------------------------*/
main()
{
     int j,k,m;
     int ln;
     float q,r;
     char a_str[256],b_str[256],c_str[256],d_str[256];

     q=0.0;
     for (ln=1; gets(a_str); ln++)
       {
/*            printf(" %s\n", a_str);/**/
/*           puts(a_str);/**/
            sscanf(a_str,"%[^=]=%[^=]=%s %s",&b_str,&b_str,&c_str, &d_str);
            sscanf(c_str,"%f%s",&r,&d_str);
            sscanf(c_str,"%i%s",&j,&d_str);
            q=q+r;
/*            printf("%s*%s*%6.0f*%8i\n",b_str, c_str,q,j);/**/
/*            puts(c_str);/**/
       }
   printf("%6.0f\n",q);/**/
}
/*----------------------------------------------------------------*/
