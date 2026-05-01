/*----------------------------------------------------------------*/
/*
	Demo program to read & convert NAS M5 Switch Matrix Format
	Author: Nick Schmitz
	Date: 3/21/95
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#include <alloc.h>/**/

#define MAX_LINE_LENGTH  120
#define NC_ROUTE           0


int l1[5][33][9];			/* Global Data */
int usl[129];
int bps, mpb, nin, sgs, sln, gln, tmc, tio;  /* Device Resources */
int tot_src, tot_dest, nwy;
int dbg;

int ** conn_tbl; /* Connection table, two dimensional array *//*s*/
/*unsigned char ** conn_tbl; /* Connection table, two dimensional array *//*p*/

/*----------------------------------------------------------------*/
/*
	Read m5 switch matrix table & build adjacency matrix.
	Matrix will contain segment line number used for connection.
*/
/*----------------------------------------------------------------*/

void rd_m5_sm(int argc, char *argv[])
{
	 int j,k,m;
	 int r,s;
	 int ln,tmp;
	 int lvl,typ,seg,cnt,blk,mc;
	 char a_str[256], b_str[256], c_str[256];
	 char xch[5];
	 FILE *fp;

/*----------------------------------------------------------------*/
/*     printf("%s \n",*argv);/**/
/*     printf("%s \n",*++argv);/**/
/*     tot_src=256;     tot_dest=128;/**/
/*     tot_src=40;     tot_dest=20;/**/

	*++argv;/**/	/* Skip Prog Name */
	xch[0]='z';	/* BLK Letter/Number Xforms */
	xch[1]='a';      xch[2]='b';
	xch[3]='c';      xch[4]='d';
	nwy=0;

/*----------------------------------------------------------------*/
/*
	File access stuff - handle errors & exit
*/
	 if(argc==1) {
		  printf("Need file name \n");
/*		  exit(1); /**/
		  }
	 if((argc>1) && (fp = fopen(*argv, "r")) == NULL) {
		  printf("Cannot open file %s\n",*argv);
/*		  exit(1);/**/
		  }

/*----------------------------------------------------------------*/
/*
	Read all lines of the sm table - structured data file
	Switch record type interpretation on first character
*/

     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++)
          {
          for (j=0; a_str[j]; j++);
	  for (m=0; a_str[m]==' '; m++);
	  if (dbg>20) printf("!%d %d %d %s \n",ln,j,m,a_str);/**/

          if (j>5) switch(a_str[m]){

		case ';' : /* Comment - Ignore */
			printf("%s",a_str);/**/
			break;

/*----------------------------------------------------------------*/
                case '0' : /* Device name & Resource Sizes */
			sscanf(a_str, "%d %s %[^\n]s", &lvl,&b_str,&c_str);
			printf("; %d %s * %s\n",lvl,b_str,c_str);/**/
            sscanf(c_str, "%d %d %d %d %d %d %d %d ",
                &bps, &mpb, &nin, &sln, &sgs, &gln, &tmc, &tio);
            if (dbg>2) printf ("%s - bps %d mpb %d nin %d sgs %d, sln %d gln %d, tmc %d tio %d \n",
                b_str, bps, mpb, nin, sgs, sln, gln, tmc, tio);/**/

/*----------------------------------------------------------------*/
    tot_src=2*bps*mpb+gln;
	tot_dest=sln;
	if (dbg>2) printf (" tot_src %d tot_dest %d \n", tot_src, tot_dest);/**/
/*	exit(1);/**/

	 /* Allocate space & init connection table */
	 conn_tbl = (int **) calloc (tot_src, sizeof(long int));/*s*/
/*	 conn_tbl = (unsigned char **) calloc (tot_src, sizeof(long int));/*p*/

	 for (j=0; j<tot_src; j++) {
		conn_tbl[j] = (int *) calloc(tot_dest, sizeof(int));/*s*/
/*		conn_tbl[j] = (unsigned char *) calloc(tot_dest, sizeof(unsigned char));/*p*/

		if (dbg>6) if ((j<10) || (j>tot_src-10))
			printf("%3d %6d - ",j,conn_tbl[j]);/**/

		for (k=0; k<tot_dest; k++){
			*(conn_tbl[j]+k)=NC_ROUTE;/**/
			if ((j==k) || (j/2==k)) {*(conn_tbl[j]+k)=j;}
				else {*(conn_tbl[j]+k)=NC_ROUTE;}/**/

			if (dbg>6) if ((j<10) || (j>tot_src-10))
				printf("%3d",*(conn_tbl[j]+k));/**/
	               }
                if (dbg>6) if ((j<10) || (j>tot_src-10))
                	printf("\n");/**/
		}
		if (dbg>6) printf("\n");/**/
		break;

/*----------------------------------------------------------------*/
                case '1':; /* Switch matrix definition Lvl 1,2,3 */
                case '2':;
                case '3': /**/

			sscanf(a_str, "%d %d %d %[^\n]s", &lvl,&seg,&cnt,&a_str);
			if (dbg>10) printf("%d %d %d",lvl,cnt,seg);/**/

			if (nwy==0) nwy=cnt;
			for (j=0; a_str[j]; j++);
			while (j>4) {
				sscanf(a_str, "%d %d %[^\n]s", &blk,&mc,&a_str);
                        printf("%dS%dB%cM%2d = ",lvl,seg,xch[blk],mc);/**/

				for(s=0; s<cnt; s++) {
                        		sscanf(a_str , " %d %[^\n]s", &tmp,&a_str);

                                   if (s==0) {printf("%3d",tmp);}
                                        else {printf(",%3d",tmp);}

				   if (lvl==1) { l1[blk][mc][s]=tmp;}
                                     else { /* Fill Adjacency Matrix */
/**/                                    for (j=1; j<=bps; j++) for (k=0; k<nin; k++)
                                          for (m=0; m<nwy; m++) if (l1[j][k][m]==tmp)
			                        {*(conn_tbl[((lvl-2)*(2*bps*mpb+2*mpb)+2*(blk-1)*mpb+mc)]+((j-1)*nin+k))=tmp+1;
					 if (dbg>8) printf(" %d %2d %d : %3d %3d   ", j,k,m,
			                        ((lvl-2)*(2*bps*mpb+2*mpb)+2*(blk-1)*mpb+mc),((j-1)*nin+k));
                                        }
                 		  }
			    	if (dbg>8) if (lvl!=1) printf("\n");/**/
/*}/**/
			    }
			    for (j=0; a_str[j]; j++);
			    printf(" \n");/**/
			}
	                break;
		 }
        }

/*----------------------------------------------------------------*/
/*	 fclose(fp);  /**/

/* Display Level 1 SM data - debug function */

    r=nin;	if (r>10) r=10;

    if (dbg>4) printf("\n");
    if (dbg>4) for (j=1; j<=bps; j++){
    	for (k=0; k<r; k++) {
		printf(" %c %3d - ",xch[j],k);
		for (m=0; m<nwy; m++) printf(" %3d",l1[j][k][m]);
		printf("\n");
		}
	printf("\n");
	}
     if (dbg>4) printf("\n");/**/


/*----------------------------------------------------------------*/

/*    Display Adjacency table - debug function */
     r=tot_src;		if (r>40) r=40;
     s=tot_dest;	if (s>20) s=20;

     if (dbg>6) printf("\n");
     if (dbg>6) for (j=0; j<r; /* tot_src;/**/ j++) {
     	printf("%3d - ",j);
     	for (k=0; k<s; /* tot_dest;/**/ k++)
     		if (*(conn_tbl[j]+k)==NC_ROUTE) {printf("  .");}
			else { printf("%3d",*(conn_tbl[j]+k));}
     	printf("\n");
        }

     r=tot_src-40;	if (r<0) r=0;
     s=tot_dest-20;	if (s<0) s=0;
     if ((s==0) && (r==0)) r=tot_src;

     if (dbg>6) printf("\n%d %d\n",s,r);
     if (dbg>6) for (j=r; j<tot_src; j++) {
     	printf("%3d - ",j);
     	for (k=s; k<tot_dest; k++)
     		if (*(conn_tbl[j]+k)==NC_ROUTE) {printf("  .");}
     			else { printf("%3d",*(conn_tbl[j]+k));}
     	printf("\n");
        }
     if (dbg>6) printf("\n");/**/
}

/*----------------------------------------------------------------*/
/*   for (m=m; m<j;  m++) if (a_str[m]==',') {b_str[k++]=' ';}/**/
/*   b_str[k++]=0;/**/
/*   printf("%s",b_str);/**/
/*   for (k=0; c_str[k]; k++) a_str[k]=c_str[k];
     a_str[k++]='\n'; a_str[k++]=0;/**/

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
      int mc, lc;
      dbg=10;
      rd_m5_sm(argc, argv);
}
/*----------------------------------------------------------------*/
