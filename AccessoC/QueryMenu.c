#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"

#define PG_HOST "127.0.0.1" // oppure " localhost " o " postgresql "
#define PG_USER "postgres" // il vostro nome utente
#define PG_DB "primo"// il nome del database
#define PG_PASS  "12340"// la vostra password
#define PG_PORT 5432

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

void sceltaArgomento(char** argomento, PGconn * conn ){
  //recupero argomenti disponibili dal db
  PGresult *dbArgs = PQexec(conn, "SELECT * FROM argomento");
  checkResults(dbArgs,conn);

  //selezione dalla lista di argomenti recuperati
  printf("Seleziona uno dei seguenti argomenti\n");
  for(int i = 0; i < PQntuples(dbArgs);i++){
      printf("%d - %s\n",i,PQgetvalue(dbArgs,i,0));
  }
  int scelta = 0;
  scanf("%d",&scelta);

  *argomento = PQgetvalue(dbArgs,scelta,0);
  PQclear(dbArgs);
}

//---------------------------------

void sceltaArtista(char** nome, char** cognome, PGconn * conn){//double pointer necessario, single pointer non funziona!
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

  *nome = PQgetvalue(ncArtisti,scelta,0);
  *cognome = PQgetvalue(ncArtisti,scelta,1);
  PQclear(ncArtisti);
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

bool Query1(PGconn *conn){
  /*
  Trovare le esposizioni in corso che hanno almeno un certo numero di artefatti esposti appartenenti ad un determinato autore e visualizzare il numero di artefatti.
  */
  int numero;
  char* nome;
  char* cognome;

  printf("Inserire i parametri della query\n");
  printf("Numero minimo di artefatti: ");
  scanf("%d",&numero);

  sceltaArtista(&nome,&cognome,conn);
  printf("Hai scelto %s %s con almeno %d opere\n",nome,cognome,numero);


  //costruzione query parametrica
  char query[500];
  char nomeArtista[32];
  char cognomeArtista[32];
  strcpy(nomeArtista,nome);
  strcpy(cognomeArtista,cognome);

  sprintf(query, "SELECT A.Area, A.Inizio, COUNT(*) AS num_opere FROM Creazione C, (Artefatto JOIN Appartenenza ON Artefatto.codice=Appartenenza.Artefatto) A WHERE C.Codice = A.Codice  AND C.Nome = '%s' AND C.Cognome ='%s' GROUP BY A.Area, A.Inizio HAVING COUNT(*) >= %d",nome, cognomeArtista, numero);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  //printResults, libera anche res
  printResults(res,conn);

  return 1;
}

//---------------------------------

bool Query2(PGconn *conn){
  /*
  Contare quanti biglietti ad ingresso guidato per un esposizione di un certo argomento sono stati venduti indicando anche la data di acquisto.
  */
 printf("Inserire i parametri della query\n");
  char * argomento;
  sceltaArgomento(&argomento,conn);
  printf("Hai scelto l'argomento %s:\n" ,argomento);

  //costruzione query parametrica
  char query[500];
  char argStr[32];
  strcpy(argStr, argomento);

  sprintf(query, "SELECT IG.Data_Acq, COUNT(*) AS Num_biglietti FROM (Ingresso_Guidato JOIN Biglietto ON Ingresso_Guidato.id=Biglietto.id) IG, Visita_Guidata VG, Esposizione E WHERE VG.Area = E.Area AND VG.Inizio = E.Inizio AND IG.IDVG = VG.ID AND E.Argomento ='%s'GROUP BY IG.Data_Acq;",argStr);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
  return 1;

}

//---------------------------------

bool Query3(PGconn *conn){
  /*
  Contare il numero di visite guidate che si sono tenute in esposizioni di un certo argomento a partire da una certa data e calcolare la media dei loro partecipanti
  */
  printf("Inserire i parametri della query\n");
  char data[30];
  printf("Inserire la data nel formato AAAA-MM-GG:\n");
  scanf("%s",&data);

  char * argomento;
  sceltaArgomento(&argomento,conn);
  printf("Hai scelto l'argomento %s:\n" ,argomento);

  //costruzione query parametrica
  char query[500];
  char argStr[32];
  strcpy(argStr, argomento);

  sprintf(query, "SELECT AVG(A.Partecipanti) AS partecipanti_medi, COUNT(*) AS num_visite FROM (Visita_guidata VG JOIN Esposizione E ON VG.Area = E.Area AND VG.Inizio = E.inizio) A WHERE A.Data_visita >=  '%s'  AND A.Argomento = '%s';",data,argStr);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);

  return 1;

}

//---------------------------------

bool Query4(PGconn *conn){
  /*
  Trovare il Codice Fiscale delle guide che hanno tenuto più visite guidate su un certo argomento, indicare anche il numero di visite, ritorna una tabella vuota se nessuna guida ha tenuto visite guidate sull’argomento.
  */

  printf("Inserire i parametri della query\n");

  char * argomento;
  sceltaArgomento(&argomento,conn);
  printf("Hai scelto l'argomento %s:\n" ,argomento);

  //costruzione query parametrica
  char query[500];
  char argStr[32];
  strcpy(argStr, argomento);

  sprintf(query, "SELECT Guida AS CF, NumVisite FROM Conteggio_Visite VG WHERE Argomento = '%s' AND NumVisite=(SELECT MAX(NumVisite) FROM Conteggio_Visite WHERE Argomento='%s'); ",argStr,argStr);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
  return 1;

}

//---------------------------------

bool Query5(PGconn *conn){
  /*
  Trovare il CF del curatore dell’esposizione con esattamente un certo numero di opere di un determinato artista.
  */
  printf("Inserire i parametri della query\n");

  int numero;
  printf("Numero di artefatti: ");
  scanf("%d",&numero);

  char* nome;
  char* cognome;
  sceltaArtista(&nome,&cognome,conn);
  printf("Hai scelto l\'artista: %s %s\n",nome,cognome);

  //costruzione query parametrica
  char query[600];
  char nomeArtista[32];
  char cognomeArtista[32];
  strcpy(nomeArtista,nome);
  strcpy(cognomeArtista,cognome);

  sprintf(query, "SELECT IC.Curatore FROM In_corso IC, (SELECT  A.Area, A.Inizio,COUNT(*) AS Num FROM Creazione C, (Artefatto Ar JOIN Appartenenza Ap ON Ar.Codice=Ap.Artefatto) A WHERE A.Codice = C.Codice AND C.Nome =  '%s'  AND C.Cognome = '%s' GROUP BY A.Area, A.Inizio) E WHERE IC.Area = E.Area AND IC.Inizio = E.Inizio AND E.Num = %d;",nome, cognomeArtista, numero);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);

  return 1;

}


//---------------------------------


int main(){

  int num = -1;

  PGconn *conn;
  char conninfo [250];

  //sprintf ( conninfo , "user=%s password=%s dbname=%s",PG_USER , PG_PASS , PG_DB) ;
  sprintf ( conninfo , "user=%s password=%s dbname=%s hostaddr=%s port=%d",PG_USER , PG_PASS , PG_DB , PG_HOST , PG_PORT ) ;
  conn = PQconnectdb(conninfo); //Connessione al database
  if (PQstatus(conn) == CONNECTION_BAD) //Se non è possibile connettersi
  {
    fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
    do_exit(conn);
  }

  while(num != 9){
    printf("Query menu:\n");
    printf("1- Trovare le esposizioni in corso che hanno almeno un certo numero di artefatti esposti appartenenti ad un determinato autore e visualizzare il numero di artefatti.\n");
    printf("2- Contare quanti biglietti ad ingresso guidato per le esposizioni di un certo argomento sono stati venduti, mostrando per ciascuna data di acquisto il conteggio totale dei biglietti emessi.\n");
    printf("3- Contare il numero di visite guidate che si sono tenute in esposizioni di un certo argomento a partire da una certa data e calcolare la media dei loro partecipanti\n");
    printf("4- Trovare il Codice Fiscale delle guide che hanno tenuto piu' visite guidate su un certo argomento, indicare anche il numero di visite, ritorna una tabella vuota se nessuna guida ha tenuto visite guidate sull’argomento.\n");
    printf("5- Trovare il CF del curatore dell'esposizione con esattamente un certo numero di opere di un determinato artista.\n");
    printf("9- Esci\n");

    printf("Digita il numero della query da eseguire:\n");
    scanf("%d",&num);

    if(num == 1){
      Query1(conn);
    }
    else if(num == 2){
      Query2(conn);
    }
    else if(num == 3){
      Query3(conn);
    }
    else if(num == 4){
      Query4(conn);
    }
    else if(num == 5){
      Query5(conn);
    }

    for(int i = 0; i < 100; i++)
      printf("-");
    printf("\n\n");
  }

  PQfinish(conn);

  return 0;
}
