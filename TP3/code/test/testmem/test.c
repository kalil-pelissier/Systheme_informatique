#include <age.h>
#include <datamgt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <util.h>
#include <string.h>
#include <ncurses.h>
#include <sprite.h>
#include <unistd.h>
#include <assert.h>
#include <memalloc.h>
#include <time.h>


//TODO: Objectif de la seance realiser vos propores fonctions de malloc et free. 

struct age_t* ge;

void startTest0()
{
    allocate_age( myalloc0, myfree0, &ge);
}

void endTest0(){
    age_free(ge);
}

#define TAILLE_MAX 1024

int testEtape0()
{
    startTest0();    
    age_initialize(ge);
    {
  //allocation d'un tableau
  srand (time(NULL));
  int taille = 1 + (rand() % TAILLE_MAX);
  int* buffer = (int*)myalloc0(taille*sizeof(int));
  printf("allocation de %d entiers (%d octect par entier) a: %p \n",taille,(int)sizeof(int),(void*)buffer);
  int i ;
  for( i = 0; i < taille ; i++)
    {
      buffer[i] = taille-i;
    }  
  for( i = 0; i < taille ; i++)
    {
      assert(buffer[i] == taille-i);
    }
  bloc_info0(buffer);
  myfree0(buffer);
  bloc_info0(buffer);
    }
    endTest0();    
    return OK;
}

void test10()
{
  int i = 1;
  int* table_des_pointeurs[10]; 
  for(i = 0; i< 10; i++)
    { 
      int taille = (i+1)*sizeof(int);
      table_des_pointeurs[i] = myalloc1(taille);
      printf("allocation de %d bytes à %p \n",taille,table_des_pointeurs[i]);
      int j ;
      for( j = 0; j < taille ; j++)
	{
	  table_des_pointeurs[i][j] = taille-j;
	}  
      for( j = 0; j < taille ; j++)
	{
	  assert( table_des_pointeurs[i][j] == taille-j);
	}
    }
  
  for(i = 0; i< 10; i++)
    {
      myfree1(table_des_pointeurs[i]);
    }
 
}

void test12()
{
  static char* buffer_ptr = NULL;
  if(buffer_ptr == NULL)
    {
      buffer_ptr = myalloc1(1024*sizeof(char));
      printf("allocation de 1024 bytes à %p \n",buffer_ptr);
      assert(buffer_ptr != NULL);
      bloc_info1(buffer_ptr);
    }
  else
    {
      myfree1(buffer_ptr);
    }
};
void test11()
{
  int i ;
  for(i = 10; i> 0; i--)
    { 
      int taille = i*sizeof(int);
      int* buffer = myalloc1(taille);
      printf("allocation de %d bytes à %p \n",taille,buffer);
      int j ;
      for( j = 0; j< taille ; j++)
	{
	  buffer[j] = taille-j;
	}  
      for( j = 0; j < taille ; j++)
	{
	  assert(buffer[j] == taille-j);
	}
      myfree1(buffer);
    }
}


void testEtape1(){
test10();
test11();
test12();
}


void test20()
{
  int i = 1;
  int* table_des_pointeurs[10]; 
  for(i = 0; i< 10; i++)
    { 
      int taille = (i+1)*sizeof(int);
      table_des_pointeurs[i] = myalloc2(taille);
      printf("allocation de %d bytes à %p \n",taille,table_des_pointeurs[i]);
      int j ;
      for( j = 0; j < taille ; j++)
	{
	  table_des_pointeurs[i][j] = taille-j;
	}  
      for( j = 0; j < taille ; j++)
	{
	  assert( table_des_pointeurs[i][j] == taille-j);
	}
    }
  
  for(i = 0; i< 10; i++)
    {
      myfree2(table_des_pointeurs[i]);
    }
 
}

void test22()
{
  static char* buffer_ptr = NULL;
  if(buffer_ptr == NULL)
    {
      buffer_ptr = myalloc2(1024*sizeof(char));
      printf("allocation de 1024 bytes à %p \n",buffer_ptr);
      assert(buffer_ptr != NULL);
      bloc_info2(buffer_ptr);
    }
  else
    {
      myfree2(buffer_ptr);
    }
};
void test21()
{
  int i ;
  for(i = 10; i> 0; i--)
    { 
      int taille = i*sizeof(int);
      int* buffer = myalloc2(taille);
      printf("allocation de %d bytes à %p \n",taille,buffer);
      int j ;
      for( j = 0; j< taille ; j++)
	{
	  buffer[j] = taille-j;
	}  
      for( j = 0; j < taille ; j++)
	{
	  assert(buffer[j] == taille-j);
	}
      myfree2(buffer);
    }
}

void testEtape2()
{
    test22();
    test21();
    test22();
    test20();
}
int main() {
    printf("starting...\n");
    fflush(stdout);
    testEtape0();
    // testEtape1(); //Retirer le commentaire si besoin
    // testEtape2();
    printf("end...\n");
    fflush(stdout);
}