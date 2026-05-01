/*----------------------------------------------------------------*/
/*
    Read & chase carry chain links
	Author: Nick Schmitz
    Date: 7/17/98
*/
/*----------------------------------------------------------------*/

#include <stdio.h>

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     int j,k,m;
     int r, c, x,y, ry, rx, bx, ary, chn, dep, lvl;
     int ln, dbg;
     char a_str[512],c_str[512];
     int ccl[149][149], cc[149], cl[149], cd[149], ct[29];

     if(argc>1){ *++argv; /**/    /* Skip Prgname */
        strcpy(a_str,*argv);
	sscanf(a_str,"%d%[^\n]",&ary,&c_str); /**/
      } else ary=14;
/*     printf("// **** %d\n",ary); /**/

/*---------------------------------------------------*/
     k=0; m=0; r=0; c=0; dbg=-1;
     for (ln=0; gets(a_str); ln++)
       {
 for (j=0; a_str[j]; j++) j=j;
 if (j>0) if (a_str[j-1]!=32) {a_str[j++]=32; a_str[j]=0;}/**/
/* printf("%2d %2d ",j,a_str[j-1]);/**/

 if (dbg>9 && a_str[0]==' ') printf(". %s\n", a_str);/**/
 if (dbg>9 && a_str[0]=='C') printf("! %s\n", a_str);/**/

/*---------------------------------------------------*/
/* printf("@%d %d %s\n", c_str[0], c_str[1], c_str);/**/
/* printf("%d-%d:%d, %s\n", k, j,c_str[1],c_str);/**/
/*---------------------------------------------------*/

	if (a_str[0]==' ') {
		sscanf(a_str,"%d%[^\n]",&j,&c_str); /**/
		for (j=0; c_str[1]!=0 && j<149; j++) {
        		sscanf(c_str,"%d%[^\n]",&ccl[k][j],&c_str); /**/
        	}
		ccl[k][j]=-1; k++;
	}

/*---------------------------------------------------*/
	if (a_str[0]=='C') {
	    cl[m]=0;
	    sscanf(a_str,"%s%d%[^\n]",&c_str,&cc[m],&c_str); /**/

	    for (j=0; c_str[1]!=0 && j<149; j++) {
        		sscanf(c_str,"%d%[^\n]",&r,&c_str);
        	}/**/
            cl[m++]=j+1;
	}
   } /* gets */

/*---------------------------------------------------*/
	cc[m]=-1;
	for (j=0; ccl[0][j]>=0; j++) ccl[k][j]=-1;

/*---------------------------------------------------*/
	if (dbg>5) {
	printf("            |");
	for (k=0; ccl[0][k]>=0 && k<149; k++) printf(" %2d",k);
	printf("\n");
	for (j=0; ccl[j][0]>=0 && j<149; j++) {
		printf(" %2d %3d %3d |",j,cc[j],cl[j]);
        	for (k=0; ccl[j][k]>=0 && k<149; k++)
			printf(" %2d",ccl[j][k]);
		printf("\n");
		}
	} /* dbg */

/*---------------------------------------------------*/
/* for (j=0; cc[j]>0 && j<149; j++) printf("%d %d\n",j,cc[j]); /**/
/* if (m>=0) printf("%3d %3d\n",j,m); else printf("BF %d ***\n",j);/**/
/* printf("!! %2d %2d %2d %2d \n",x,y, ry, bx);/**/
/*		printf(" %2d %4d |",j,cc[j]);/**/
/*---------------------------------------------------*/
	if (dbg>5) {
	k=0; m=0; r=0; c=0; x=1; y=1; rx=1; ry=0; bx=0;
        chn=1; lvl=0; dep=0;
	for (j=0; ccl[j][0]>=0 && j<149; j++) {
		m=-1;
	        for (k=0; ccl[k][j]>=0 && k<149; k++) if (ccl[k][j]>5) m=k;

		if (m<0) {
		printf("\n// %2d\n",chn++);/**/
		lvl=0;

		if(bx<x) bx=x;
		y=y+ry; ry=0;

		if (y+cl[j]>ary) {y=1; rx=bx-1;}
		x=rx;

		if (cc[j]>0) if (x<=ary && y<=ary && y+cl[j]<=ary)
			   printf("BF %3d  %2d %2d\n",cc[j], x,y);
		  else
			   printf("// BF %3d  %2d %2d %d\n",cc[j], x,y, cl[j]);
		if(ry<cl[j]) ry=cl[j];
		x++;
		cc[j]=-abs(cc[j]);

		for (r=j; r>=0; r=r) {
			m=-1; lvl++;
		for (c=0; ccl[r][c]>=0 && c<149; c++) if (ccl[r][c]>5) m=c;
			if (m>=0) {
				if (cc[m]>0) {
				if (x<=ary && y<=ary && y+cl[m]<=ary)
				   printf("BF %3d  %2d %2d\n",cc[m], x,y);
				  else
				   printf("// BF %3d  %2d %2d %d\n",cc[m], x,y, cl[m]);
				if(ry<cl[m]) ry=cl[m];/**/
				x++;
				cc[m]=-abs(cc[m]);
				}
				r=m; } else { r=-1;}
				
			} /* if m */
		  } /* for r */
		if (dep<lvl) dep=lvl;
		cd[j]=lvl;
		} /* for j */
	} /* dbg */
/*---------------------------------------------------*/
/* printf(" %2d %4d |",j,cc[j]);/**/
/*---------------------------------------------------*/
	if (dbg>-5) {
	k=0; m=0; r=0; c=0;
        chn=1; lvl=0; dep=0;
	for (j=0; ccl[j][0]>=0 && j<149; j++) {
		m=-1;
	        for (k=0; ccl[k][j]>=0 && k<149; k++) if (ccl[k][j]>5) m=k;

		if (m<0) {chn++; lvl=0;
		ct[lvl]=cc[j];
		for (r=j; r>=0; r=r) {
		    m=-1; lvl++;
	            for (c=0; ccl[r][c]>=0 && c<149; c++) if (ccl[r][c]>5) m=c;
		    if (m>=0) {  r=m; } else { r=-1; }
		    ct[lvl]=cc[r];
 		  }
		if (dep<lvl) dep=lvl;
		cd[j]=lvl;
		printf("::");
		for (r=lvl-1; r>0; r--) printf(" %3d",ct[r]);
		printf("\n");
	     } /* if m */
	  } /* for j */
	} /* dbg */
/*---------------------------------------------------*/
	printf("\n// %3d %3d %3d \n",dep, ary, cl[1]);
	printf("/! ");
	for (j=0; ccl[j][0]>=0; j++) if (cl[j] >cl[1]+1) printf (" %3d", cc[j]);

/*	for (j=0; ccl[j][0]>0; j++)  printf (" %3d %3d,", ccl[j][1],cl[j]);/**/
	printf("\n");
/*---------------------------------------------------*/
	if (dbg>5) {
	k=0; m=0; r=0; c=0; chn=1;
	for (j=0; ccl[j][0]>=0 && j<149; j++) {
		m=-1;
	        for (k=0; ccl[k][j]>=0 && k<149; k++) if (ccl[k][j]>5) m=k;

		if (m<0) {
		printf("\n// %2d: %3d ",chn++,abs(cc[j]));
		for (r=cd[j]; r<dep; r++) printf("    ");

		for (r=j; r>=0; r=r) {
		    m=-1;
	            for (c=0; ccl[r][c]>=0 && c<149; c++) if (ccl[r][c]>5) m=c;
		    if (m>=0) { printf(" %3d",abs(cc[m])); r=m; } else { r=-1; }
 		  }
		 } /* if m */
	  } /* for j */
	printf("\n\n");/**/
	} /* dbg */

/*---------------------------------------------------*/
}
/*----------------------------------------------------------------*/
