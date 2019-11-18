#include <setjmp.h>
#include <stdio.h>
/*
* Testez le programme avec:
* > nomexecutable
* > nomexecutable x
* > nomexecutable x x
*/
static jmp_buf env;

static void f2(void)
{
longjmp(env,2);
}

static void f1(int val)
{
if(val == 1){
longjmp(env,1);
}
f2();
}

int main(int argc, char** argv)
{
switch(setjmp(env))
{
case 0:
/* Nous sommes ici car on vient de mettre en place la cible*/
printf("cible en place !\n");
f1(argc);
printf("Ce texte ne sera jamais affiche !");
break;
case 1:
/*Nous sommes ici car on vient d’un jump avec comme valeur 1*/
printf("saut depuis f1 ! \n");
break;
case 2:
/*Nous sommes ici car on vient d’un jump avec comme valeur 2*/
printf("saut depuis f2 ! \n");
break;
}
}