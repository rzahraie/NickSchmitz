/*----------------------------------------------------------------*/
/*  Find Architecture from map only run
	Author: Nick Schmitz
    Date: 11/10/97
    12/3/97 : RAM detect
*/
/*----------------------------------------------------------------*/

#include <stdio.h>
#define MAX_LINE_LENGTH  120
     int j,k,m,n,p;
     int ram, clb, pin, mini, port, ary;
     int ln;
     FILE *fp;
     char a_str[256],b_str[256],c_str[256],d_str[256];
     char t_str[256],x_str[256];

/*----------------------------------------------------------------*/
rout(int argc)
{
	k=port/ary;
	j=((clb/ary)*100)/ary; /**/
	j=((mini*11/40/ary)*100)/ary; /**/

	m=16;
	if (k>=179) {
		m=25;
		if (j>=59) m=44;
	  }
	if (k>=215) m=59;

	p=mini*11/36;
	if (k>=230) if (clb<=p) ary=ary+2;/**/

	if(argc>1) switch (ary) {
		case 16: ary=18; break;
		case 22: ary=24; break;
		case 26: ary=28; break;
		case 30: ary=34; break;
		case 34: ary=36; break;
		}

printf("@\n");/**/
	switch (m) {
		case 16: printf("echo #$# Info: Easy Route\n"); break;
		case 36: printf("echo #$# Info: Medium Route\n"); break;
		case 59: printf("echo #$# Info: Difficult Route\n"); break;
		case 83: printf("echo #$# Info: Awful Route\n"); break;
	    }
printf("@\n");/**/
printf("echo #$# Info: {sdq:pcr:mpa} %3d %3d %4d : %4d %4d %4d : %4d %4d %3d %d\n",
    m, j, k,   pin, clb, ram,   mini, port, ary,p);/**/
printf("set mds3=%d\n", ary*ary-7);/**/
printf("set mds4=%d\n", m);/**/

if (pin>210) printf("set mds6=352BGA\n");
	else if (pin>165) printf("set mds6=256BGA\n");
	else printf("set mds6=208PQ\n");
}

/*----------------------------------------------------------------*/
pack(int argc)
{
     if((fp = fopen("pf.txt", "w")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {
/*	 fprintf(fp,"%s%dx%d\n", b_str,j,j);/**/

printf("rem %s \n", d_str);/**/
fprintf(fp,"%s\\Vantis1.dir\n", d_str);/**/

	fclose(fp);
	}
}

/*----------------------------------------------------------------*/
arch(int argc)
{
     if((fp = fopen("arch.fls", "w")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {
/*   fprintf(fp,"%s%dx%d\n", b_str,ary,ary);/**/

switch (ary)
{
 case 14: strcpy(x_str,"VF1012"); break;
 case 18: strcpy(x_str,"VF1020"); break;
 case 20: strcpy(x_str,"VF1025"); break;
 case 24: strcpy(x_str,"VF1036"); break;
 default: itoa (ary, &t_str, 10);
	 strcpy(x_str,t_str);
	 strcat(x_str,"x");
	 strcat(x_str,t_str);
	 break;
}

	d_str[0]=0;
	strcpy(d_str,b_str);
	 strcat(d_str,x_str);

	 printf("set mds2=%s\n", x_str);/**/
	 fprintf(fp,"%s\n", d_str);/**/
	 fprintf(fp,"%s", c_str);/**/
	fclose(fp);
	}
     if((fp = fopen("zzz.eds", "w")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {
/*  printf("rem Size chosen %d %d - %d %d %d\n", ary*ary-7, ary, pin, clb, ram);/**/

     fprintf(fp,"1,# rmap_only TRUE@Zmap_only FALSE\n");/**/
     fprintf(fp,"1,# r_goal 0@Z_goal %d\n", ary*ary-7);/**/
/*     printf("rem 1,# r_goal 0@Z_goal %d\n", ary*ary-7);/**/
	 fprintf(fp,"exit\n");/**/
	fclose(fp);
	}
}

/*----------------------------------------------------------------*/
main(int argc, char *argv[])
{
     j=0;   k=0;    m=0;    n=0;   p=0;
     pin=0; port=0; clb=0;  ram=0; ary=0; mini=0;

     if(argc>1) printf("echo #$# Info: Standard Matrix Devices \n");
	   else printf("echo #$# Info: Non-Standard Matrix Devices \n");

     if(argc>1){  /* pick up date from command line /**/
        *++argv;/**/    /* Skip stk sym */
/*        printf("C %s %f  \n ",*argv);/**/
     }

     a_str[0]=0;
     strcat(a_str,"arch.fls");/**/
/*     printf("rem %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
	  if((fp = fopen("zzz.eds", "w")) == NULL) {
/*      	  exit(1);/**/
          	} else {
		 printf("\\Vantis\\architecture\\VF1012" );/**/
		 fprintf(fp,"\\Vantis\\architecture\\VF1012" );/**/
		 fprintf(fp,"Vantis1" );/**/
		 fclose(fp);
	  }
/*        exit(1);/**/
          } else {
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {

     m=0;
     for (k=0; a_str[k]; k++) if (a_str[k]=='\\') m=k; {}
/* printf("rem %d* %s", k,a_str);/**/
        if (ln==1) {strcpy(b_str,a_str);}
        if (ln==2) {strcpy(c_str,a_str);}
        if (ln==1) {b_str[m+1]=0;}
      }

 fclose(fp);
/* printf("rem %s", c_str);
 printf("rem %s\n", b_str);/**/

/*---------------------------------------------------*/
     for (ln=1; gets(a_str); ln++) {
 printf("rem %d. %s\n", ln,a_str);/**/
/* 32 /* ram blk*/
        if (ln==1) {
            sscanf(a_str,"%d %s", &ram, &t_str);
/* printf("%d . %d\n", ram,ram/2);/**/
 }

/*---------------------------------------------------*/
/* IO: 24 PIs, 10 POs, 2 PIOs /**/
        if (ln==2999) {
            sscanf(a_str,"%s %d %s %d %s %d",&t_str, &j, &t_str, &k, &t_str, &m);
	    pin = j + k + m + 11;
/* printf("%d . %d %d %d\n", pin, j, k, m);/**/
 }

/*---------------------------------------------------*/
/* VGB/CBB usage: 195 VGBs, 653 CBBs (average 3.35 CBB/VGB) /**/
/* Map into: 28 CLBs, 103 MINIs, average 3.68 MINI per CLB /**/
        if (ln==1) {
            sscanf(a_str,"%s %s %d %s %d",&t_str, &t_str, &k, &t_str, &m);/**/
/*        if (ln==2) {
            sscanf(a_str,"%s %s %d %s %d",&t_str, &t_str, &k, &t_str, &m);/**/
	    clb = k+5;
	    mini=m;
	    clb=mini*11/40;
	    if (k>clb) clb=k;

     k=10;	     if(argc>1) k=14;

     for (j=k; clb>=j*j;  j++,j++);	/* printf("*%d %d\n", clb, j);/**/
     for (j=j; ram>=2*j;  j++,j++);	/* printf("#%d %d\n", ram, j);/**/

    ary=j;
/*    arch(argc);
    pack(argc); /**/

 printf("rem %d . %d %d %d\n", j, pin, clb, ram);/**/

 }

/*---------------------------------------------------*/
/*  Sideness analysis: 1031 feedback, 2002 same side, 1239 different side /**/
        if (ln==3) { } /* skipped no data /**/

/*---------------------------------------------------*/
/* Number of nodes:     1   12   28   24    4    0    0 /**/
        if (ln==3) {
/*        if (ln==4) {/**/
            port=(5+4+4+3)*ram;
            port=pin+mini+port;
/* printf("rem mini: %d . port: %d %s\n", mini, port, a_str);/**/
            sscanf(a_str,"%s %s %s%[^\n]", &x_str, &x_str, &x_str, &t_str);
/* printf("mini: %d . port: %d %s\n", mini, port, t_str);/**/
     	    for (j=1; j<=7; j++) {
            sscanf(t_str,"%d %[^\n]", &k, &t_str);
     		port=port+k*j;
/* printf("mini: %d . port: %d %d %d * %s\n", mini, port,k,j,t_str);/**/
	      }
/* printf("rem mini: %d . port: %d\n", mini, port);/**/
 }

/*    0 1-MINI   5 2-MINI  19 3-MINI 478 4-MINI CLBs /**/
        if (ln==5999) {
            sscanf(a_str,"%d %s", &k, &t_str);
     		port=port+k;
/* printf("rem mini: %d . port: %d\n", mini, port);/**/
 }

/*---------------------------------------------------*/
/*  859 DFFs /**/
        if (ln==5) {
            sscanf(a_str,"%d %s", &k, &t_str);
     		port=port+2*k;
 printf("rem mini: %d . port: %d\n", mini, port);/**/
/*    rout(argc);
    arch(argc);
    pack(argc);/**/
 }
        if (ln==4) {
            sscanf(a_str,"%d %s", &k, &t_str);
     		pin=pin+k;
/* printf("rem pin: %d\n", pin);/**/
 }
        if (ln==6) {
            sscanf(a_str,"%d %s", &k, &t_str);
     		pin=pin+k;
/* printf("rem pin: %d\n", pin);/**/
 }
        if (ln==7) {
            sscanf(a_str,"%d %s", &k, &t_str);
     		pin=pin+k;
/* printf("rem pin: %d\n", pin);/**/
 }
        if (ln==8) {
            sscanf(a_str,"%d %s", &k, &t_str);
            pin=pin+k+11;
/* printf("rem pin: %d\n", pin);/**/
    for (ary=ary; pin>=12*ary; ary++,ary++);  /* printf("#%d %d\n", pin, ary);/**/
    rout(argc);
    arch(argc);
    pack(argc);
 }


}
     } /* file /**/
/*----------------------------------------------------------------*/
} /* main/**/
