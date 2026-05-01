
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NB_IOS 208


static int size_vcc_gnd;
static int vcc_gnd[200];


void read_vcc_gnd()

{
FILE * fi;
int i;
char s[5];

i = 0;
fi = fopen("vcc_gnd","r");

while ( fgets(s,5,fi) != NULL)
	{
	int p;
	sscanf(s,"%d",&p);
	vcc_gnd[i] = p;
	i++;
	}
size_vcc_gnd = i;
}


	
int agree(p)
int p;
{
int i,Bad;

Bad = 0;
for (i=0;i<size_vcc_gnd;i++) if (vcc_gnd[i] == p) Bad = 1;

if (Bad) return(0); else return(1);
}



void main()
{
int List_Num[NB_IOS];
int ord[NB_IOS];

char IOType[50],name[50],IO[20],date[50],Chip[50];
char s[200];
FILE *fi,*fi2,*fi3,*fi4;
int k,i,Bon;
int tab[NB_IOS];
char   s2[200];
int PadNum;

srand( (unsigned)time( NULL ) );

read_vcc_gnd();

fi = fopen("essai.pin","r");

i=0;
fgets(s,200,fi);
fgets(s,200,fi);
fgets(s,200,fi);

for (i=0;i<NB_IOS;i++) ord[i] = 0;
i=0;
while ((fgets(s,200,fi) != NULL) && (IOType[0] != 'E'))
        {
        int numero_pin;

        if (s[0]='"') sscanf(s,"%s%s%s%s%d",IOType,name,name,IO,&numero_pin);

	if (numero_pin != 0)
		{
        	ord[i] = numero_pin;
		i++;
if (i<=10) printf(".");/**/
		}
	
/*printf("^%s\n",name);/**/
        }
 printf(".\n");/**/

fclose (fi);

/*******   lecture de la liste des entrees  *******/



fi = fopen("essai.pin","r");

fgets(s,200,fi);

sscanf(s,"%s%s%s%s%s",IO,IOType,name,name,date);
 srand( (unsigned)time( NULL ) );


while ( (name[0] != '=') && (IO[0] != 'D' ))
	{

    if ( (IO[0] == 'C' ) && ( IO[1] == 'H' )) sscanf(s,"%s%s",Chip,Chip);

 	fgets(s,200,fi);

	strcpy(s2,s);
	sscanf(s2,"%s%s%s%s",IO,name,name,name);
/* printf("@%s %s\n",name,IO);/**/
 	}

sscanf(s2,"%s%s%s%s%d",name,IOType,IOType,IO,&PadNum);


fi2 = fopen("pin_same.cst","w");
fi3 = fopen("pin_rand.cst","w");
fi4 = fopen("pin_bad.cst","w");

fprintf (fi3,"CHIP %s\nBEGIN\n",Chip);
fprintf (fi2,"CHIP %s\nBEGIN\n",Chip);
fprintf (fi4,"CHIP %s\nBEGIN\n",Chip);

/*for (i=0;i<NB_IOS;i++) ord[i] = i;*/
for (i=0;i<NB_IOS;i++) List_Num[i] = i;
for (i=0;i<NB_IOS;i++) tab[i] = -1;

/**********    boucle principale, parcours des entrees, description
		des trois fichiers					*****/
while ( (IO[0] == '=') || (name[0] == 'D') )
	{
	Bon = 1;
	
	if ( name[0] == 'D')
		{
		fprintf (fi2,"%s",s);
		fprintf (fi3,"%s",s);
		fprintf (fi4,"%s",s);
		}

if ((IOType[0] != 'L') && ( name[0] != 'D') && (name[1]!=':'))
{
	int i,j,alternance;
	int n;
	alternance = 0;

	for (i=0;i<NB_IOS;i++)
		if ((ord[i] != 0) && (ord[i] < PadNum)) alternance++;
		

	alternance = alternance % 4;

	j = 0;

    while (ord[j] != PadNum) j++;

    if (alternance == 1) j = (j + 0) % NB_IOS;
    if (alternance == 2) j = (j + 36) % NB_IOS;
    if (alternance == 3) j = (j + 72) % NB_IOS;
    if (alternance == 0) j = (j + 108) % NB_IOS;

	k = j;
		
name[0]='|';
for (n=0; name[n]; n++);
name[n-1]=0;
/* printf("%s\n",name);/**/

/*	if(!agree(PadNum))
 printf(".%s %d\n",name,PadNum);/**/

/*	if(!agree(List_Num[k]))
 printf("!%s %d %d\n",name,List_Num[k],k);/**/

	if(agree(PadNum)) {
	while ((List_Num[k] == 0 ) || (!agree(k)) )
{
/* printf(".%s %d\n",name,k);/**/
			k = (k + 1) % NB_IOS;
}
	fprintf(fi4,"     %s :	%s = %d;\n",name,IOType,List_Num[k]);
    List_Num[k] = 0;

    } else fprintf(fi4,"     %s :  %s = %d;\n",name,IOType,PadNum);
    fprintf(fi2,"     %s :  %s = %d;\n",name,IOType,PadNum);

	if(agree(PadNum)) {
    Bon = 0;
    while (Bon != 1)
		{
		j = 0;
		k = rand();
		k = ( (k)  % NB_IOS);
        while ( (tab[j] != -1) && (tab[j] != k)) j++;
		if ((tab[j] == -1) && (agree(k)) ) Bon = 1;
/* printf("*%s %d\n",name,k);/**/
		}
	tab[j] = k;	

	/*printf(" rand    %s :  %s = %d;\n",name,IOType,k);*/
    fprintf(fi3,"     %s :  %s = %d;\n",name,IOType,k);
    } else fprintf(fi3,"     %s :  %s = %d;\n",name,IOType,PadNum);

}
	if ( (fgets(s,200,fi)) == NULL) Bon =0;
	IO[0] = '.';

	if (Bon) sscanf(s,"%s%s%s%s",IO,IO,IO,IO);
    if (Bon) sscanf(s,"%s%s%s%s%d",name,IOType,IOType,IO,&PadNum);
	
	}

fprintf (fi2,"END;\n");
fprintf (fi3,"END;\n");
fprintf (fi4,"END;\n");

fclose(fi2);
fclose(fi3);
fclose(fi4);

fclose (fi);
}
