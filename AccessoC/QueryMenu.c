#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h" //file .h che si trova in PostgreSQL/17/unclude 

//ATTENZIONE WINDOWS: creare la cartella dependencies con incluse libpq.dll e libpq.lib nella stessa cartella del file .c, usare il makefile

void do_exit(PGconn *conn){
  PQfinish(conn);
  exit(1);
  }

bool Query1(){
  /*
  Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore.
  Parametri: n = numero artefatti, a = nome autore, c = cognome autore
  */


  PGconn *conn = PQconnectdb("dbname=primo password=12340 user=postgres"); //Connessione al database
  if (PQstatus(conn) == CONNECTION_BAD) //Se non è possibile connettersi
  {
    fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
    do_exit(conn);
  }

  int numero;
  char* nome;
  char* cognome;

  printf("Inserire i parametri della query\n");
  printf("Numero minimo di artefatti: ");
  scanf("%d",&numero);

  //recupero artisti disponibili dal db
  PGresult *ncArtisti = PQexec(conn, "SELECT nome,cognome FROM artista");
  if (PQresultStatus(ncArtisti) != PGRES_TUPLES_OK) 
  {
    fprintf(stderr, "Non è stato restituito un risultato per il seguente errore: %s", PQerrorMessage(conn));
    PQclear(ncArtisti);
    do_exit(conn);
    return 0;
  }


  printf("Seleziona uno dei seguenti artisti\n");
  for(int i = 0; i < PQntuples(ncArtisti);i++){
      printf("%d - %s %s\n",i,PQgetvalue(ncArtisti,i,0),PQgetvalue(ncArtisti,i,1));
  }
  int scelta = 0;
  scanf("%d",&scelta);

  nome = PQgetvalue(ncArtisti,scelta,0);
  cognome = PQgetvalue(ncArtisti,scelta,1);

  printf("Hai scelto %s %s con almeno %d opere\n",nome,cognome,numero);


  //costruzione query parametrica
  char query[500];
  char nomeArtista[32];
  char cognomeArtista[32];
  strcpy(nomeArtista,nome);
  strcpy(cognomeArtista,cognome);

  sprintf(query, "SELECT A.Area, A.Inizio, COUNT(*) FROM Creazione C, (Artefatto JOIN Appartenenza ON Artefatto.codice=Appartenenza.Artefatto) A WHERE C.Codice = A.Codice  AND C.Nome = \'%s\' AND C.Cognome =\'%s\' GROUP BY A.Area, A.Inizio HAVING COUNT(*) >= %d",nome, cognomeArtista, numero);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);
  if (PQresultStatus(res) != PGRES_TUPLES_OK) 
  {
    fprintf(stderr, "Non è stato restituito un risultato per il seguente errore: %s", PQerrorMessage(conn));
    PQclear(res);
    do_exit(conn);
    return 0;
  }

  //stampa risultato
  if(PQntuples(res) == 0)
    printf("Non e stato prodotto alcun risultato\n");
  else
    printf("Risultato: %s %s %s\n",PQgetvalue(res,0,0),PQgetvalue(res,0,1),PQgetvalue(res,0,2));

  //libera memoria e chiudi connessione
  PQclear(ncArtisti); 
  PQfinish(conn);
  return 1;
}

//---------------------------------

bool Query2(){
  /*
  Contare quanti biglietti ad ingresso guidato per un esposizione di un certo argomento sono stati venduti ogni giorno
  Parametri: arg = argomento
  */

  char* argomento;

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

  char* argomento;

  printf("Inserire i parametri della query\n:");
  //argomento va scelto dal db

}

//---------------------------------

bool Query5(){
  /*
  Calcolare la media del numero di artefatti di un determinato artista presenti in ogni area
  Parametri: Nome n, Cognome c

  */

  char* nome;
  char* cognome;

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
