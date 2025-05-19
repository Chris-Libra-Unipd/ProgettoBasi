DROP TABLE  IF EXISTS Ingresso_Libero;
DROP TABLE  IF EXISTS Ingresso_Guidato;
DROP TABLE  IF EXISTS Biglietti;
DROP TABLE  IF EXISTS Visite_Guidate;
DROP TABLE  IF EXISTS Esposizioni;
DROP TABLE  IF EXISTS Argomenti;
DROP TABLE  IF EXISTS Guide;
DROP TABLE  IF EXISTS Dipendenti;
DROP TABLE  IF EXISTS Aree;
DROP TABLE  IF EXISTS Autori;

DROP TYPE  IF EXISTS Turno;
DROP TYPE  IF EXISTS Tipo;


CREATE TYPE Turno AS ENUM (
    'Mattina',
    'Pomeriggio'
);
CREATE TYPE tipo AS ENUM (
    'Opera',
    'Reperto'
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
    FOREIGN KEY (CF) REFERENCES Dipendenti(CF)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Argomenti(
    Nome VARCHAR(16) PRIMARY KEY
);

CREATE TABLE Esposizioni(
    Area VARCHAR(16) NOT NULL,
    Nome VARCHAR(16) NOT NULL,
    FOREIGN KEY (Area) REFERENCES Aree(Nome)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE,
    PRIMARY KEY (Area)
);

CREATE TABLE Visite_Guidate(
    Numero_Partecipanti INT NOT NULL CHECK (Numero_Partecipanti>=0),
    Data_Visita DATE NOT NULL,
    Turno_Visita Turno NOT NULL,
    Guida CHAR(16) NOT NULL,
    Area VARCHAR(16) NOT NULL,
    FOREIGN KEY (Guida) REFERENCES Guide(CF)
        ON DELETE NO ACTION
        ON UPDATE CASCADE ,
    FOREIGN KEY (Area) REFERENCES Esposizioni(Area)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    PRIMARY KEY (Data_Visita,Turno_Visita,Area,Guida)
);

CREATE TABLE Biglietti (
    ID SERIAL PRIMARY KEY,
    Data_Acquisto DATE NOT NULL,
    Mail VARCHAR(32)
);

CREATE TABLE Ingresso_Libero(
    ID SERIAL PRIMARY KEY,
    FOREIGN KEY (ID) REFERENCES Biglietti(ID)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    Data_Validita DATE NOT NULL
);


CREATE TABLE Ingresso_Guidato(
    ID SERIAL PRIMARY KEY,
    Data_Visita DATE NOT NULL,
    Turno_Visita Turno  NOT NULL,
    Guida CHAR(16) NOT NULL,
    Area VARCHAR(16) NOT NULL,
    Data_Inizio DATE NOT NULL,
    FOREIGN KEY (Data_Visita,Turno_Visita,Area,Guida)REFERENCES Visite_Guidate(Data_Visita,Turno_Visita,Area,Guida)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY (ID) REFERENCES Biglietti(ID)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);