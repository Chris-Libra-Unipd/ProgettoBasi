#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/libpq-fe.h" 
#define PG_HOST "127.0.0.1" // oppure " localhost " o " postgresql "
#define PG_USER "postgres" // il vostro nome utente
#define PG_DB "primo" // il nome del database
#define PG_PASS "12340" // la vostra password
#define PG_PORT 5432



void checkResults ( PGresult * res , const PGconn * conn );
void printResults ( PGresult * res, const PGconn * conn );


int main(){
    char conninfo [250];
    sprintf ( conninfo , "user=%s password=%s dbname=%s hostaddr=%s port=%d",PG_USER , PG_PASS , PG_DB , PG_HOST , PG_PORT ) ;
    
    printf("%s \n",conninfo);

PGconn * conn=PQconnectdb (conninfo)  ;

if ( PQstatus ( conn ) != CONNECTION_OK ) {
    printf (" Errore di connessione : %s\n", PQerrorMessage ( conn ));
    PQfinish ( conn );
    exit (1) ;
}
else {
    printf (" Connessione avvenuta correttamente \n");

    PGresult * res ;
    res = PQexec ( conn , " SELECT * FROM Artefatto ") ;
    printResults(res, conn);
/*
    checkResults(res,conn);
    printResults(res, conn);


    char* nome="Leonardo",*cogn="da Vinci";
    int nArt=2;


    PGresult * stmt = PQprepare ( conn ,"query1", "SELECT A.Area, A.Inizio, COUNT(*) FROM Creazione C, (Artefatto JOIN Appartenenza ON Artefatto.codice=Appartenenza.Artefatto) A WHERE C.Codice = A.Codice  AND C.Nome =$1::varchar AND C.Cognome =$2::varchar  GROUP BY A.Area, A.Inizio HAVING COUNT(*) >=$3::Int ", 3, NULL);
    PQexecPrepared ( conn , "query1", 1 , nome , NULL , 0, 0) ;
    PQexecPrepared ( conn , "query1", 2 , cogn , NULL , 0, 0) ;
    PQexecPrepared ( conn , "query1", 3 , &nArt , NULL , 0, 0) ;
  
  Trovare i nomi delle esposizioni che hanno o hanno avuto almeno un certo numero di artefatti esposti appartenenti a un determinato autore.
  Parametri: n = numero artefatti, a = nome autore, c = cognome autore
  */







    PQfinish ( conn );
}

  return 0;
}


void checkResults ( PGresult * res , const PGconn * conn ) {
if ( PQresultStatus ( res ) != PGRES_TUPLES_OK ) {
printf (" Risultati inconsistenti %s\n", PQerrorMessage ( conn ));
PQclear ( res );
exit (1) ;
}
}


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