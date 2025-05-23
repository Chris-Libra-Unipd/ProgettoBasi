#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"

#define DBNAME "sampleDB"
#define PSW "myPostGres"
#define USR "postgres"

#define PG_HOST " localhost " // oppure " localhost " o " postgresql "
#define PG_USER "postgres" // il vostro nome utente
#define PG_DB "sampleDB"// il nome del database
#define PG_PASS  "myPostGres"// la vostra password
#define PG_PORT 5432

//ATTENZIONE WINDOWS: creare la cartella dependencies con incluse libpq.dll e libpq.lib nella stessa cartella del file .c, usare il makefile

void do_exit(PGconn *conn){
  PQfinish(conn);
  exit(1);
  }

//---------------------------------

void checkResults ( PGresult * res , const PGconn * conn ) {
  if ( PQresultStatus ( res ) != PGRES_TUPLES_OK ) {
    printf (" Risultati inconsistenti %s\n", PQerrorMessage ( conn ));
    PQclear ( res );
    exit (1) ;
  }
}

//---------------------------------

void printResults ( PGresult * res, const PGconn * conn ){
    checkResults(res,conn);
    int indent=32;
    // Trovo il numero di tuple e campi selezionati
    int tuple = PQntuples ( res ) ;
    int campi = PQnfields ( res ) ;
    
    // Stampo le intestazioni delle colonne
    for ( int i = 0; i < campi ; i ++) {
        char * temp=PQfname ( res , i );
        printf ("%s", temp);
        for(int h=strlen(temp);h<indent;h++)printf(" ");
    }
    printf ("\n");
    for(int i = 0; i < 100; i++) //linea separazione intestazione
      printf("-");
    printf("\n");

    // Stampo i valori selezionati
    for ( int i = 0; i < tuple ; i ++) {
        for ( int j = 0; j < campi ; j ++) {
            char * temp=PQgetvalue ( res , i , j);
            printf ("%s", temp);
            for(int h=strlen(temp);h<indent;h++)printf(" ");

    }
    printf ("\n");
    }
    PQclear(res);
}

//---------------------------------

bool Query1(){
  /*
  Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore.
  Parametri: n = numero artefatti, a = nome autore, c = cognome autore
  */


  char conninfo [250];
  //sprintf ( conninfo , "user=%s password=%s dbname=%s hostaddr=%s port=%d",PG_USER , PG_PASS , PG_DB , PG_HOST , PG_PORT ) ;
  //host e porta non li ricordo, ma tanto non sono obbligatori
  sprintf ( conninfo , "user=%s password=%s dbname=%s",PG_USER , PG_PASS , PG_DB) ;
  PGconn *conn = PQconnectdb(conninfo); //Connessione al database
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
  checkResults(ncArtisti,conn);

  //selezione dalla lista di artisti recuperati
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

  sprintf(query, "SELECT A.Area, A.Inizio, COUNT(*) AS opere FROM Creazione C, (Artefatto JOIN Appartenenza ON Artefatto.codice=Appartenenza.Artefatto) A WHERE C.Codice = A.Codice  AND C.Nome = \'%s\' AND C.Cognome =\'%s\' GROUP BY A.Area, A.Inizio HAVING COUNT(*) >= %d",nome, cognomeArtista, numero);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
 
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
