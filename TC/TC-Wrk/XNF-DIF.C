/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <alloc.h>/**/


/*----------------------------------------------------------------*/
main()
{
     int j,k,m;
     int ln;
     char a_str[256],b_str[256],c_str[256],d_str[256],e_str[256];

     d_str[0]=0;
     e_str[0]=0;
     m=1;
     for (ln=1; gets(a_str); ln++)
       {
/*            printf(" %s\n", a_str);/**/
/*           puts(a_str);/**/
            sscanf(a_str,"%s %s %s",&b_str,&c_str,&d_str);
/*            printf(" %s\n", d_str);/**/
            k=0;
            if (b_str[0]=='S') {
            	for (j=0; d_str[j]; j++) if (d_str[j]!=e_str[j]) k=1;
            	if (k==0) m++;
            	if (k>0 && e_str[0]) {printf("S x %s %d\n",e_str,m);}
            	if (k>0) {
            		for (j=0; d_str[j]; j++) e_str[j]=d_str[j];
              		e_str[j]=0; m=1;
                    }
              }

/*            puts(c_str);/**/
       }
            printf("S x %s %d\n",e_str,m);
}
/*----------------------------------------------------------------*/
