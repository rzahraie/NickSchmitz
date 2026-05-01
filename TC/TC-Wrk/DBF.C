/* Program to read/write DBF-3 file

    DBF  fname.dbf >out.txt  {read}
    DBF  data.txt out.dbf    {write}

    Author: Nick Schmitz
    Date: 9/5/96
*/

#include <stdio.h>

#define MAX_LINE_LENGTH  120
int   dbg;

/*----------------------------------------------------------------*/
void rd_dbf(int argc, char *argv[])
{
     int k,q,ln;
     FILE *fp;
     char  cbuf[31], a_str[256];
     int   lbuf[16];
     char  jbuf[2];

     *++argv;/**/    /* Skip Prog Name */
     strcpy(a_str,*argv);
     if((argc>1) && (fp = fopen(a_str, "rb")) == NULL) {

          strcpy(a_str,"c:\\tc2000\\watch\\");/**/
          strcat(a_str,*argv);
          strcat(a_str,".dbf");
          if((argc>1) && (fp = fopen(a_str, "rb")) == NULL) {

                strcpy(a_str,"szz.dbf");/**/
                if((argc>1) && (fp = fopen(a_str, "rb")) == NULL) {
                    printf("Cannot open file %s\n",*argv);
/*                  exit(1);/**/
                 }
            }
       }

    fread(&lbuf,sizeof(lbuf),1,fp);   /* */
/*  for (q=0; q<=7; q++) printf("%8d ",lbuf[q]);
    printf("\n");/**/

    ln=lbuf[2];
/*  printf("%8d \n",ln);/**/
    fread(&lbuf,sizeof(lbuf),1,fp);   /* */
    fread(&lbuf,sizeof(lbuf),1,fp);   /* */
    fread(&lbuf,sizeof(lbuf),1,fp);   /* */

    fread(&jbuf,sizeof(jbuf),1,fp);   /* */

    for (k=0; k<ln; k++) {
    	fread(&cbuf,sizeof(cbuf[1]),1,fp);   /* */
/*	printf("%8d \n",cbuf[0]);/**/
    	fread(&cbuf,sizeof(cbuf),1,fp);   /* */
/*   for (q=0; q<=7; q++) printf("%8d ",cbuf[q]);    printf("\n");/**/
        cbuf[6]=0;
        for (q=5; q>1; q--) if (cbuf[q]==32) cbuf[q]=0;
        printf("%s,0.0\n",cbuf);
     }
}

/*----------------------------------------------------------------*/
void wr_dbf(int argc, char *argv[])
{
     int j,m,q,ln,lnc;
     FILE *fo, *fi, *fp;
     char  cbuf[25], a_str[256], b_str[256], d_str[256];
     int   lbuf[16];
     char  jbuf[2],xbuf[32];

/*----------------------------------------------------------------*/
     *++argv;/**/    /* Skip Prog Name */
     strcpy(a_str,*argv);
     if (dbg>5) printf("!read %s\n", a_str);/**/

     if ((argc>1) && (fi = fopen(a_str, "r")) != NULL) {

     for (ln=1; fgets(b_str,MAX_LINE_LENGTH,fi); ln++){
     	     if (b_str[0]==';') ln--;
	     if (dbg>15) printf("! %s\n", b_str);/**/
         }
     lnc=ln-1;
     fclose(fi);  /**/
     if (dbg>0) printf("!read %s - %d\n", a_str,lnc);/**/

     strcpy(d_str,"\tc\szz.dbf");/**/
     strcpy(d_str,"szz.dbf");/**/
     if((argc>1) && (fp = fopen(d_str, "rb")) != NULL) {
            *++argv;/**/    /* Skip Prog Name */
            strcpy(d_str,*argv);
            if((argc>1) && (fo = fopen(d_str, "wb")) != NULL) {

               if (dbg>0) printf("!write %s - %d\n", *argv,lnc);/**/
                fread(&lbuf,sizeof(lbuf),1,fp);   /* */
                lbuf[2]=lnc;
                fwrite(&lbuf,sizeof(lbuf),1,fo);   /* */

                fread(&lbuf,sizeof(lbuf),1,fp);   /* */
                fwrite(&lbuf,sizeof(lbuf),1,fo);   /* */
                fread(&lbuf,sizeof(lbuf),1,fp);   /* */
                fwrite(&lbuf,sizeof(lbuf),1,fo);   /* */
                fread(&lbuf,sizeof(lbuf),1,fp);   /* */
                fwrite(&lbuf,sizeof(lbuf),1,fo);   /* */

                fread(&jbuf,sizeof(jbuf),1,fp);   /* */
                fwrite(&jbuf,sizeof(jbuf),1,fo);   /* */

                }
         }

/*----------------------------------------------------------------*/
     fi = fopen(a_str, "r");
     for (ln=1; fgets(b_str,MAX_LINE_LENGTH,fi); ln++)
 	  if (b_str[0]!=';')
          {
          for (j=0; b_str[j]; j++);
          for (m=0; b_str[m]!=','; m++);
          if (dbg>15) printf("!%d %d %d %s ",ln,j,m,b_str);/**/

          cbuf[0]=b_str[0];  cbuf[1]=0;  b_str[m]=0;
/*          strcpy(d_str,"c:\\tc2000\\data\\");/**/
          strcpy(d_str,"e:\\tc2000cd\\");/**/
          strcat(d_str,cbuf);/**/
          strcat(d_str,"\\");
          strcat(d_str,b_str);
          strcat(d_str,".ndx");
          if (dbg>5) printf("%s\"",d_str);

          if((argc>1) && (fp = fopen(d_str, "rb")) != NULL) {
	          fread(&cbuf,sizeof(cbuf)-1,1,fp);   /* read stock name */
		  cbuf[24]=0;
		  if (dbg >5) printf("%s\"%s\"",b_str,cbuf);/**/
		  fclose(fp);  /**/
		
		  for (j=0; j<32; j++) xbuf[j]=' ';
		  m=1;
		  for (j=0; b_str[j]; j++,m++) xbuf[m]=b_str[j];
		  m=7;
		  for (j=0; cbuf[j]; j++,m++) xbuf[m]=cbuf[j];
	          fwrite(&xbuf,sizeof(xbuf),1,fo);   /* */

          if (dbg>0) printf(".");/**/

              }
	  if (dbg>5) printf("\n");/**/
        }
/*----------------------------------------------------------------*/
      xbuf[0]=26;
      fwrite(&xbuf[0],sizeof(xbuf[0]),1,fo);   /* */
      fclose(fo);  /**/
      if (dbg>0) printf(".\n");/**/
  }
}

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     dbg=6;
     if(argc==1) { printf("Need file name \n"); }
     if(argc==3) { wr_dbf(argc, argv); }
     if(argc==2) { rd_dbf(argc, argv); }
}
/*----------------------------------------------------------------*/
