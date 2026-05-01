/*----------------------------------------------------------------*/
/*
    Write out BF locs
	Author: Nick Schmitz
    Date: 7/24/98
*/
/*----------------------------------------------------------------*/

#include <stdio.h>

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     int j,k,m, x,y,r,c;
     int ary, dep, chn, lnk;
     int ln, dbg;
     char a_str[512],c_str[512];
     int  ct[29], cl[999], lc[99];

     if(argc>1){ *++argv; /**/    /* Skip Prgname */
        strcpy(a_str,*argv);
	sscanf(a_str,"%d%[^\n]",&ary,&c_str); /**/
      } else ary=14;
/*     printf("// **** %d\n",ary); /**/

/*---------------------------------------------------*/
     k=0; m=0;  dbg=-1; lnk=0;
     x=1; y=1; r=1; c=1; dep=0; chn=0; ary=0;
     for (ln=0; gets(a_str); ln++)
       {
 for (j=0; a_str[j]; j++) j=j;
 if (j>0) if (a_str[j-1]!=32) {a_str[j++]=32; a_str[j]=0;}/**/
/* printf("%2d %2d ",j,a_str[j-1]);/**/

 if (dbg>9 && a_str[0]==':') printf("// . %s\n", a_str);/**/
 if (dbg>9 && a_str[0]=='/') printf("// ! %s\n", a_str);/**/

/*---------------------------------------------------*/
/* printf("@%d %d %s\n", c_str[0], c_str[1], c_str);/**/
/* printf("%d-%d:%d, %s\n", k, j,c_str[1],c_str);/**/
/*---------------------------------------------------*/

    if (a_str[0]==':') {k=1;
		sscanf(a_str,"%c%c%[^\n]",&j,&j ,&c_str); /**/
		for (j=0; c_str[1]!=0 && j<149; j++) {
	                sscanf(c_str,"%d%[^\n]",&ct[k++],&c_str); /**/
        	}
        	ct[0]=k-1;
        	for (j=1; j<ct[0]; j++) { m=-1;
        		for (k=0; k<lnk; k++) if (ct[j]==cl[k]) m=k;
        		if (m<0) cl[lnk++]=ct[j];
        		  else ct[j]=-abs(ct[j]);
        		}
		printf("// : %3d - ",ct[0]);
		for (j=ct[0]; j>0; j--) printf(" %4d",ct[j]);
		printf("\n");/**/

        x=r;
        for (j=ct[0]; j>0; j--) {m=-1;
	    for (k=0; k<=lc[0];	k++) if (ct[j]==lc[k]) m=k;
            if (m>=0) if (r==ary) x--; else x++;
            if (ct[j]>0) printf("BF %4d  %2d %2d \n",ct[j],x,y);
            if (r==ary) x--; else x++;
            }
        y=y+chn; if (y>ary) {r=ary; y=1;}
		printf("\n");/**/

/*		printf("* %3d - ",lnk);
		for (j=0; j<lnk; j++) printf(" %4d",cl[j]);
		printf("\n");/**/
	}

    if (a_str[0]=='/') {k=1;
		sscanf(a_str,"%c%c%[^\n]",&j,&j ,&c_str); /**/
		for (j=0; c_str[1]!=0 && j<149; j++) {
	                sscanf(c_str,"%d%[^\n]",&ct[k++],&c_str); /**/
        	}
        	ct[0]=k-1;
            	if (a_str[1]=='/') { dep=ct[1]; ary=ct[2]; chn=ct[3];}
		   else for (j=0; j<ct[0]; j++) lc[j]=ct[j];

		printf("// ! %3d - ",ct[0]);
		for (j=ct[0]; j>0; j--) printf(" %3d",ct[j]);
		printf("\n");/**/
	}

/*---------------------------------------------------*/
   } /* gets */

/*---------------------------------------------------*/
printf("// !!%3d %3d %3d\n", ary, dep, chn);/**/

/*---------------------------------------------------*/
}
/*----------------------------------------------------------------*/
