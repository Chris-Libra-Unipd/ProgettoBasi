#include <stdio.h>
#include <stdlib.h>
#include "path/to/libpq-fe.h" //file .h che si trova in PostgreSQL/17/unclude

bool Query1(){
  /*
  Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore.
  Parametri: n = numero artefatti, a = nome autore, c = cognome autore
  */
  int numero;
  std::string nome;
  std::string cognome;

  printf("Inserire i parametri della query\n:");
  scanf("Numero minimo di artefatti: %d",&numero);

  //nome e cognome autori vanno scelti dal db

}

//---------------------------------

bool Query2(){
  /*
  Contare quanti biglietti ad ingresso guidato per un esposizione di un certo argomento sono stati venduti ogni giorno
  Parametri: arg = argomento
  */

  std::string argomento;

  printf("Inserire i parametri della query\n:");
  //argomento va scelto dal db

}

//---------------------------------

bool Query3(){
  /*
  Contare la visite guidate che includono nell’itinerario una determinata opera
  Parametri: c = codice opera

  */

  int codice;
  printf("Inserire i parametri della query\n:");
  //codice va scelto dal db

}

//---------------------------------

bool Query4(){
  /*
  Trovare il Codice Fiscale della guida che ha tenuto più visite guidate su un certo argomento
  Parametri: a = argomento

  */

  std::string argomento;

  printf("Inserire i parametri della query\n:");
  //argomento va scelto dal db

}

//---------------------------------

bool Query5(){
  /*
  Calcolare la media del numero di artefatti di un determinato artista presenti in ogni area
  Parametri: Nome n, Cognome c

  */

  std::string nome;
  std::string cognome;

  printf("Inserire i parametri della query\n:");
  //nome e cognome autori vanno scelti dal db

}


//---------------------------------


int main(){

  printf("Query menu:\n");
  printf("1- Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore \n");
  printf("2- Contare quanti biglietti ad ingresso guidato per un esposizione di un certo argomento sono stati venduti ogni giorno \n");
  printf("3- Contare la visite guidate che includono nell’itinerario una determinata opera \n");
  printf("4- Trovare il Codice Fiscale della guida che ha tenuto più visite guidate su un certo argomento \n");
  printf("5- Calcolare la media del numero di artefatti di un determinato artista presenti in ogni area\n");

  printf("Digita il numero della query da eseguire:\n");
  int num;
  scanf("%d",&num);

  if(num == 1){
    Query1();
  }
  if(num == 2){
    Query2();
  }
  if(num == 3){
    Query3();
  }
  if(num == 4){
    Query4();
  }
  if(num == 5){
    Query5();
  }






  return 0;
}
