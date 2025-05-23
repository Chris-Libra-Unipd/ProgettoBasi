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
  Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore.
  Parametri: n = numero artefatti, a = nome autore, c = cognome autore
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

  sprintf(query, "SELECT A.Area, A.Inizio, COUNT(*) AS opere FROM Creazione C, (Artefatto JOIN Appartenenza ON Artefatto.codice=Appartenenza.Artefatto) A WHERE C.Codice = A.Codice  AND C.Nome = \'%s\' AND C.Cognome =\'%s\' GROUP BY A.Area, A.Inizio HAVING COUNT(*) >= %d",nome, cognomeArtista, numero);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
 
  //libera memoria e chiudi connessione
  
  PQfinish(conn);
  return 1;
}

//---------------------------------

bool Query2(PGconn *conn){
  /*
  Contare quanti biglietti ad ingresso guidato per un esposizione di un certo argomento sono stati venduti ogni giorno
  Parametri: arg = argomento
  */
 printf("Inserire i parametri della query\n");
  char * argomento;
  sceltaArgomento(&argomento,conn);
  printf("Hai scelto l'argomento %s:\n" ,argomento);

  //costruzione query parametrica
  char query[500];
  char argStr[32];
  strcpy(argStr, argomento);

  sprintf(query, "SELECT IG.Data_Acq, COUNT(*) FROM (Ingresso_Guidato  join Biglietto On Ingresso_Guidato.id=Biglietto.id) IG, Visita_Guidata VG, Esposizione E WHERE VG.Area = e.Area AND VG.Inizio = E.Inizio AND IG.IDVG = VG.ID AND E.Argomento = \'%s\' GROUP BY IG.Data_Acq",argStr);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);

  PQfinish(conn);
  return 1;

}

//---------------------------------

bool Query3(PGconn *conn){
  /*
  Contare il numero di visite guidate che hanno trattato un certo argomento a partire da una certa data e calcolare la media dei loro partecipanti
  Parametri: Argomento arg, Data d
  */
  printf("Inserire i parametri della query\n");
  char data[12];
  printf("Inserire la data nel formato AAAA-MM-GG:\n");
  scanf("%s",&data);

  char * argomento;
  sceltaArgomento(&argomento,conn);
  printf("Hai scelto l'argomento %s:\n" ,argomento);

  //costruzione query parametrica
  char query[500];
  char argStr[32];
  strcpy(argStr, argomento);

  sprintf(query, "SELECT AVG(A.Partecipanti), COUNT(*) FROM (Visita_guidata VG JOIN Esposizione E ON VG.Area = E.Area AND VG.Inizio = E.inizio) A WHERE A.Data_visita >=\'%s\' AND A.Argomento =\'%s\'",data,argStr);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
  
  PQfinish(conn);
  return 1;

}

//---------------------------------

bool Query4(PGconn *conn){
  /*
  Trovare il Codice Fiscale della guida che ha tenuto più visite guidate su un certo argomento
  Parametri: a = argomento

  */
  printf("Inserire i parametri della query\n");
  char * argomento;
  sceltaArgomento(&argomento,conn);
  printf("Hai scelto l'argomento %s:\n" ,argomento);

  //costruzione query parametrica
  char query[500];
  char argStr[32];
  strcpy(argStr, argomento);

  sprintf(query, "SELECT CF, NumVisite FROM (SELECT VG.Guida AS CF, COUNT(*) AS NumVisite FROM Visita_Guidata_Estesa VG WHERE Argomento = \'%s\' GROUP BY VG.Guida ) AS A ORDER BY NumVisite DESC LIMIT 1; ",argStr);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
  
  PQfinish(conn);
  return 1;

}

//---------------------------------

bool Query5(PGconn *conn){
  /*
  Calcolare la media del numero di artefatti di un determinato artista presenti in ogni area
  Parametri: Nome n, Cognome c

  */

  char* nome;
  char* cognome;

  printf("Inserire i parametri della query\n");

  sceltaArtista(&nome,&cognome,conn);
  printf("Hai scelto l\'artista: %s %s\n",nome,cognome);

  //costruzione query parametrica
  char query[500];
  char nomeArtista[32];
  char cognomeArtista[32];
  strcpy(nomeArtista,nome);
  strcpy(cognomeArtista,cognome);

  sprintf(query, "SELECT AVG(Num) FROM	(SELECT COUNT(*) AS Num FROM Creazione C, (Artefatto Ar JOIN Appartenenza Ap ON Ar.Codice=Ap.Artefatto) A WHERE A.Codice = C.Codice AND C.Nome = \'%s\' AND C.Cognome = \'%s\'  GROUP BY A.Area) ",nome, cognomeArtista);

  //Esecuzione query
  PGresult *res = PQexec(conn, query);

  printResults(res,conn);
 
  PQfinish(conn);
  return 1;

}


//---------------------------------


int main(){

  int num = -1;

  PGconn *conn;
  char conninfo [250];
  //sprintf ( conninfo , "user=%s password=%s dbname=%s hostaddr=%s port=%d",PG_USER , PG_PASS , PG_DB , PG_HOST , PG_PORT ) ;
  //host e porta non li ricordo, ma tanto non sono obbligatori
  sprintf ( conninfo , "user=%s password=%s dbname=%s",PG_USER , PG_PASS , PG_DB) ;
  conn = PQconnectdb(conninfo); //Connessione al database
  if (PQstatus(conn) == CONNECTION_BAD) //Se non è possibile connettersi
  {
    fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
    do_exit(conn);
  }
  
  while(num != 9){
    printf("Query menu:\n");
    printf("1- Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore \n");
    printf("2- Contare quanti biglietti ad ingresso guidato per un esposizione di un certo argomento sono stati venduti ogni giorno \n");
    printf("3- Contare la visite guidate che includono nell\'itinerario una determinata opera \n");
    printf("4- Trovare il Codice Fiscale della guida che ha tenuto piu\' visite guidate su un certo argomento \n");
    printf("5- Calcolare la media del numero di artefatti di un determinato artista presenti in ogni area\n");
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

  return 0;
}
