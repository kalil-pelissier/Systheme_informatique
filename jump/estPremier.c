#include <stdlib.h>
/* atol */
#include <stdio.h> /*fp
rintf*/
static int recEstPremier(long valeur, long diviseur)
{
if(diviseur==1)
{
return 1;
}
if(valeur%diviseur == 0)
{
return 0;
}
else
{
int reponse = recEstPremier(valeur, diviseur-1);
return reponse;
}
}

static int estPremier(long valeur)
{
if(valeur == 1)
{
return 1;
}
else
{
return recEstPremier(valeur, valeur-1);
}
}


int main(int argc, char** argv)
{
if(argc!=2)
{
printf("Usage %s <entier>",argv[0]);
exit(-1);
}
long valeur = atol(argv[1]);
if(valeur==0)
{
printf("Usage %s <entier !=0>",argv[0]);
}
valeur = (valeur<0)?-valeur:valeur;
fprintf(stdout,"estPremier ?: %d\n",estPremier(valeur));
}