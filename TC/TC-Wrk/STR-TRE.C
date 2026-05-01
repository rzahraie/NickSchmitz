/*---------------------------------------------------*/
/*---------------------------------------------------*/
#include <stdio.h>
#include <alloc.h>
#include <string.h>
#define MAX_LINE_LENGTH  120
typedef struct tnode *Tptr;
typedef struct tnode {
	char splitchar;
	Tptr lokid, eqkid, hikid;
} Tnode;

     FILE *fp;
     int j,k,m,p, ln;
     char a_str[256],b_str[256],c_str[256],d_str[256];
     Tptr tre;

/*---------------------------------------------------*/
int rsearch(Tptr p, char *s)
{	if (!p) return 0;
	if (*s < p->splitchar)
		return rsearch(p->lokid, s);
	else if (*s > p-> splitchar)
		return rsearch(p->hikid, s);
	else {
		if (*s == 0) return 1;
		return rsearch(p-> eqkid, ++s);
	}
}

/*---------------------------------------------------*/
Tptr insert (Tptr p, char *s)
{	if (p == 0) {
		p = (Tptr) malloc(sizeof(Tnode));
		p->splitchar = *s;
		p->lokid = p->eqkid = p->hikid = 0;
/* printf(".%c.\n", p->splitchar);/**/
	}
	if (*s < p->splitchar) 	p->lokid = insert(p->lokid, s);
	else if (*s == p->splitchar) {
		if (*s != 0) p->eqkid = insert(p->eqkid, ++s);/**/
	} else 	p->hikid = insert(p->hikid, s);
	return p;
}
/*		if (*s == 0) p->eqkid = (Tptr) insertstr;
		else p->eqkid = insert(p->eqkid, ++s);/**/

/*---------------------------------------------------*/
void traverse(Tptr p)
{	if (!p) return;
	traverse(p->lokid);

	if (p->splitchar) printf("%c",p->splitchar);/**/
	if (p->splitchar) traverse(p->eqkid);
/*	else printf("%s\n", (char *) p->eqkid);/**/
	else printf("%c",p->splitchar);/**/
	traverse(p->hikid);
}

/*---------------------------------------------------*/
main(int argc, char *argv[])
{
     if(argc>1){  /* pick up filename from command line /**/
        *++argv;/**/    /* Skip Prgname */
/*        printf("C %s %f  \n ",*argv);/**/
     }

     strcpy(a_str,*argv);
     strcat(a_str,".t");/**/
     printf("// %s\n",a_str);/**/

     if((fp = fopen(a_str, "r")) == NULL) {
          printf("Cannot open file %s\n",a_str);
/*        exit(1);/**/
          } else {

/*     if(argc>2) printf("High Fanout Nets: {Net#,,FO, Name}\n");/**/

/*----------------------------------------------------------*/
     for (ln=1; fgets(a_str,MAX_LINE_LENGTH,fp); ln++) {
	printf("*%s", a_str);/**/
	for (j=0; a_str[j]; j++) j=j;
	a_str[j-1]=0;
	tre=insert(tre, a_str);
} /* gets */
} /* file */
   printf("Hello\n");
   traverse(tre);
}
/*---------------------------------------------------*/
