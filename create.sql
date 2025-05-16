DROP TABLE Argomenti IF EXISTS;
DROP TABLE Guide IF EXISTS;
DROP TABLE Dipendenti IF EXISTS;
DROP TABLE Aree IF EXISTS;
DROP TABLE Autori IF EXISTS;

DROP TYPE turno IF EXISTS;


CREATE TYPE turno AS ENUM (
    'Mattina',
    'Pomeriggio'
);


CREATE TABLE Autori(
    Nome VARCHAR(32) NOT NULL, 
    Cognome VARCHAR(32) NOT NULL,
    Data_Nascita DATE NOT NULL,
    Data_Morte DATE NULL,
    PRIMARY KEY (Nome,Cognome,Data_Nascita)

);

CREATE TABLE Aree(
    Nome VARCHAR(16) PRIMARY KEY
);

CREATE TABLE Dipendenti(
    CF CHAR(16) PRIMARY KEY
);

CREATE TABLE Guide(
    CF CHAR(16) PRIMARY KEY,
    conteggio INT UNSIGNED NOT NULL,
    FOREIGN KEY (CF) REFERENCES Dipendenti(CF)
);

CREATE TABLE Argomenti(
    Nome VARCHAR(16) PRIMARY KEY
);

CREATE TABLE Esposizioni(
    Data_Inizio DATE NOT NULL,
    Data_Inizio DATE NULL,
    Area VARCHAR(16) NOT NULL,
    FOREIGN KEY (Area) REFERENCES Aree(Nome)
);
