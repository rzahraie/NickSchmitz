/* nas changes to makt pgm compatible with tc 1.0, keep pins the same /**/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define NB_IOS 160


void main()
{
char * List_Pin[NB_IOS];
char * List_Pad[NB_IOS];
int List_Num[NB_IOS];
int ord[NB_IOS];


char name[50],pin[10],IO[30],date[50],Pad[10];
char s[200];
FILE *fi,*fi2,*fi3,*fi4;
int k,i,m;
int bad[NB_IOS];
int tab[NB_IOS];
char c;
int PadNum;
int numero_pin;
int pkg_pin;

printf("Pin Placement program for Xilinx 9/25/97\n");/**/
/******  lecture du package ******/

for (i=0;i<NB_IOS;i++) ord[i] = NULL;
for (i=0;i<NB_IOS;i++) List_Num[i] = NULL;
for (i=0;i<NB_IOS;i++) List_Pad[i] = NULL;
for (i=0;i<NB_IOS;i++) List_Pin[i] = NULL;
for (i=0;i<NB_IOS;i++) tab[i] = -1; /**/

fi = fopen("4010p208.pkg","r");

i=0;
pkg_pin=0;
fgets(s,200,fi);

while ((fgets(s,200,fi)) != NULL)
	{
	char   name_loc[10];

 	sscanf(s,"%s%s%s",Pad,Pad,name_loc);

	List_Pin[i] = (char *) calloc(10,sizeof(char));
	List_Pad[i] = (char *) calloc(10,sizeof(char));
        PadNum=-1;
	strcpy(List_Pin[i],name_loc);
	strcpy(List_Pad[i],Pad);
	sscanf(s,"%s%s%1s%d",Pad,Pad,Pad,&PadNum);
	List_Num [i] = PadNum;

/* printf("%d %d %s %s   - ", i,PadNum, List_Pin[i], List_Pad[i]); /**/
	if (PadNum<0) {
	  List_Pin[i][0]='p';
	  for (k=2; name_loc[k]; k++) List_Pin[i][k-1]=name_loc[k];
	  List_Pin[i][k-1]=0;
	  List_Pad[i]=NULL;
	  tab[i]=0;
	 }
/* printf("%d %d %s %s\n", i,PadNum, List_Pin[i], List_Pad[i]); /**/
	i++;
	pkg_pin++;	
	}
fclose (fi);
 printf("Package file read: %d Pins found.\n", pkg_pin); /**/

/*******   lecture de la liste des entrees  *******/

fi = fopen("essai.pin","r");

i=0;
fgets(s,200,fi);
fgets(s,200,fi);
fgets(s,200,fi);

for (i=0;i<NB_IOS;i++) ord[i] = 0;
i=0;
while ( fgets(s,200,fi) != NULL)
	{

  	sscanf(s,"%s%s%s%s%1s%d",IO,IO,IO,IO,IO,&numero_pin);

	ord[i] = numero_pin;
/* printf("** %s %d %d \n", s, i, numero_pin); /**/

	i++;
	}
 printf("Design file read: %d Pins used.\n", i); /**/

fclose (fi);


m=0;
fi = fopen("essai.pin","r");

fgets(s,200,fi);
sscanf(s,"%s%s",name,date);
m++;

fi2 = fopen("pin_same.cst","w");
fi3 = fopen("pin_rand.cst","w");
fi4 = fopen("pin_bad.cst","w");

sscanf(s,"%s%s%s%s%c%c%d",IO,IO,IO,pin,&c,&c,&PadNum);

sscanf(s,"%s%s%s%s%s",IO,IO,IO,pin,pin);

srand( (unsigned)time( NULL ) );

/* for (i=0;i<NB_IOS;i++) tab[i] = -1; /**/

/**********    boucle principale, parcours des entrees, description
		des trois fichiers					*****/

while ( (name[0] == 'p') && (name[1] =='l'))
	{
	int n,i,Bon,j,alternance;

	alternance = 0;

	for (i=0;i<NB_IOS;i++)
		if ((ord[i] != 0) && (ord[i] < PadNum)) alternance++;
		
	alternance = alternance % 4;

	j = 0;

	while (abs(List_Num[j]) != PadNum)
			j++;
        n=j;
	if (List_Num[n]>0) {
/******* Distribution des pins sur les 4 cotes du chip *******/

	if (alternance == 1)
        j = (j + 0) % NB_IOS;
	if (alternance == 2)
        j = (j + 40) % NB_IOS;
	if (alternance == 3)
        j = (j + 80) % NB_IOS;
	if (alternance == 0)
        j = (j + 120) % NB_IOS;

	k = j;
		
	while (List_Pad[k] == NULL )
            k = (k + 1) % NB_IOS;

	List_Pad[k] = NULL;
}	
	
	fprintf(fi2,"    place instance %s 	: %s ;\n",IO,pin);

	if ((IO[0]!='$') && (List_Num[n]<0)) {
	  fprintf(fi3,"     place instance %s 	: %s ;\n",IO,pin);
  	  fprintf(fi4,"     place instance %s 	: %s ;\n",IO,pin);
        } else {
        fprintf(fi4,"    place instance %s	: %s ;\n",IO,List_Pin[k]);

 	Bon = 0;
 n=0;

/****** generation du pin aleatoire et verification de sa disponibilite ****/

	while ((Bon != 1) && (n<100))
		{


		j = 0;
		k = rand();
		k = (k + rand());
		if (k<0) k = -k;
/*		k = (k % NB_IOS); /**/
		k = (k % pkg_pin);
		while ( (tab[j] != -1) && (tab[j] != k))
			j++;
/*		if (tab[j] == -1) Bon = 1; /**/
		if ((tab[j] == -1) && (List_Num[j]>0)) Bon = 1;
 printf("       %d %d   \n",j,k);/**/
n++;
		}

	if (Bon==0) {
		k=0;
		while ((tab[k] != -1) || (List_Num[k]<0)) k++;
		j=k;
printf("*");
/* printf("*** %d %d   \n",j,k);/**/
            }

	tab[j] = k;
/* printf("%d %d   \n",j,k);/**/
	
	if ((List_Num[n]>0) && (IO[0]!='$'))
 	fprintf(fi3,"    place instance %s	: %s ;\n",IO,List_Pin[k]);
}
	if ( fgets(s,200,fi) == NULL) Bon =0;
		name[0] = '.';
m++;
if (m>pkg_pin) Bon=0;

	if (Bon) sscanf(s,"%s",name);

	sscanf(s,"%s%s%s%s%c%c%d",IO,IO,IO,pin,&c,&c,&PadNum);

	sscanf(s,"%s%s%s%s%s",IO,IO,IO,pin,pin);
		k=0;
		while (tab[k] != -1) k++;
		j=0;
		while (List_Pad[j] == NULL) j++;
printf(".");
 printf("%s %d %d   %d %d %d\n", s, PadNum,m,  j,k,pkg_pin); /**/
	}

printf("\n");
fclose(fi2);
fclose(fi3);
fclose(fi4);


fclose (fi);
}
