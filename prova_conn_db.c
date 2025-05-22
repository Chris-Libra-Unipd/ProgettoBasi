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
void printResults ( PGresult * res);


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

    checkResults(res,conn);
    printResults(res);



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


void printResults ( PGresult * res){
    // Trovo il numero di tuple e campi selezionati
    int tuple = PQntuples ( res ) ;
    int campi = PQnfields ( res ) ;
    
    // Stampo le intestazioni delle colonne
    for ( int i = 0; i < campi ; i ++) {
    printf ("%s\t\t", PQfname ( res , i ));
    }
    printf ("\n");

    // Stampo i valori selezionati
    for ( int i = 0; i < tuple ; i ++) {
    for ( int j = 0; j < campi ; j ++) {
    printf ("%s\t\t", PQgetvalue ( res , i , j)) ;
    }
    printf ("\n");
    }
}