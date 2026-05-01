     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main()

{
char name [10];
char c;
FILE *fi,*fi2;
int num1,num2,num3;
int Tab_Trans[500][3];
char s[100];
int i,j,nb,Pin_Num,Side,Pos,part1,part2,side_length,Side_Trans,Pos_Trans;


for (i=0;i<3;i++)
	for (j=0;j<500;j++)
		Tab_Trans[j][i] = 0;

fi = fopen("pack","r");
fgets(s,100,fi);
sscanf(s,"%s%s%c%c%d%c%d%c%c%d%c%d%c%c%d%c%d",name,name,&c,&c,&num1,&c,&num1,&c,&c,&num2,&c,&num2,&c,&c,&num2,&c,&num2);

i = 0;

while (fgets(s,100,fi) != NULL)
	{

	sscanf(s,"%d%c%c",&nb,&c,&c);

	if ( (c != 'T') && (c != 'C') && (c != 'V') && (c != 'G') && (c != '/') && (c != 'D') && (c != 'I') )
		{
		int n1,n2,n3;
		sscanf (s,"%d%c%d%c%d%c%d",&nb,&c,&n1,&c,&n2,&c,&n3);
		Tab_Trans[i][0] = n1;
		Tab_Trans[i][1] = n2;
		Tab_Trans[i][2] = n3;
		i++;
		}
	}

fclose (fi);

fi = fopen("param_trans","r");
fgets(s,100,fi);
sscanf(s,"%d%d",&part1,&part2);
fclose(fi);


fi = fopen("ent","r");
fi2 = fopen("ext","w");

while (fgets(s,100,fi) != NULL)
	{
	sscanf(s,"%s%d%d%d",name,&Pin_Num,&Side,&Pos);
/*printf ("%s",s);*/

	if ( (name[0] == 'I') && (name[1] == 'F') )
	{
	int a;
	
	if (part1 == 0) side_length = 60;
	if (part1 == 1) side_length = 54;
	if (part1 == 2) side_length = 42;

	if ((Side == 1) || (Side == 4))
		Pos = side_length - Pos + 1;

	a = (side_length * (Side - 1)) + Pos;

	i = 0;
	while ((Tab_Trans[i][part1] != a) && (i<500)) i++;
/*printf ("a = %d; i= %d\n",a,i);*/

if (i == 500) printf("INCORRECT PIN ASSIGNMENT : %s\n",s);

	if (part2 == 0) side_length = 60;
	if (part2 == 1) side_length = 54;
	if (part2 == 2) side_length = 42;

	Side_Trans = div(Tab_Trans[i][part2],side_length).quot + 1;
	Pos_Trans = div(Tab_Trans[i][part2],side_length).rem;

	if ((Side == 1) || (Side == 4))
		Pos_Trans = side_length - Pos_Trans +1;

	/*printf("IF %d %d %d\n",Pin_Num,Side_Trans,Pos_Trans);*/

	if (i != 500) fprintf(fi2,"IF %d %d %d\n",Pin_Num,Side_Trans,Pos_Trans);
	
	}

	else fprintf(fi2,"%s",s);
	}

fclose (fi);
fclose(fi2);

}


	
