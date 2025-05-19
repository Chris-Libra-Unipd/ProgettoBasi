DROP TABLE  IF EXISTS Creazione;
DROP TABLE  IF EXISTS Conoscenza;
DROP TABLE  IF EXISTS Ingresso_Libero;
DROP TABLE  IF EXISTS Ingresso_Guidato;
DROP TABLE  IF EXISTS Biglietto;
DROP TABLE  IF EXISTS Visita_Guidata;
DROP TABLE  IF EXISTS Artefatto;
DROP TABLE  IF EXISTS Esposizione;
DROP TABLE  IF EXISTS Argomento;
DROP TABLE  IF EXISTS Guida;
DROP TABLE  IF EXISTS Dipendente;
DROP TABLE  IF EXISTS Area;
DROP TABLE  IF EXISTS Artista;

DROP TYPE  IF EXISTS Turni_Giornata;
DROP TYPE  IF EXISTS Tipo;


CREATE TYPE Turni_Giornata AS ENUM (
    'Mattina',
    'Pomeriggio'
);
CREATE TYPE Tipo AS ENUM (
    'Opera',
    'Reperto'
);


CREATE TABLE Artista(
    Nome VARCHAR(32) NOT NULL, 
    Cognome VARCHAR(32) NOT NULL,
    DN DATE NOT NULL,
    DM DATE NULL,
    PRIMARY KEY (Nome,Cognome,DN)

);

CREATE TABLE Area(
    Nome VARCHAR(16) PRIMARY KEY
);

CREATE TABLE Dipendente(
    CF CHAR(16) PRIMARY KEY
);

CREATE TABLE Guida(
    CF CHAR(16) PRIMARY KEY,
    FOREIGN KEY (CF) REFERENCES Dipendente(CF)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Argomento(
    Nome VARCHAR(16) PRIMARY KEY
);

CREATE TABLE Esposizione(
    Area VARCHAR(16) PRIMARY KEY,
    Nome VARCHAR(16) NOT NULL,
    FOREIGN KEY (Area) REFERENCES Area(Nome)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Artefatto(
    Codice CHAR(4) PRIMARY KEY,
    Nome VARCHAR(32) NOT NULL, 
    Tipologia Tipo NOT NULL, 
    Esposizione VARCHAR(16) NULL,
    FOREIGN KEY (Esposizione) REFERENCES Esposizione(Area)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Visita_Guidata(
    Esposizione VARCHAR(16) NOT NULL,
    Guida CHAR(16) NOT NULL,
    Data_Visita DATE NOT NULL,
    Turno Turni_Giornata NOT NULL,
    Partecipanti INT NOT NULL CHECK (Partecipanti>=0),
    PRIMARY KEY (Data_Visita,Turno,Esposizione,Guida),

    FOREIGN KEY (Guida) REFERENCES Guida(CF)
        ON DELETE NO ACTION
        ON UPDATE CASCADE ,
    
    FOREIGN KEY (Esposizione) REFERENCES Esposizione(Area)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

CREATE TABLE Biglietto (
    ID INT NOT NULL CHECK (ID>=0) PRIMARY KEY,
    Data_Acq DATE NOT NULL,
    EMail VARCHAR(32)
);



CREATE TABLE Ingresso_Guidato(
    ID INT NOT NULL CHECK (ID>=0) PRIMARY KEY,
    Data_Visita DATE NOT NULL,
    Turno Turni_Giornata  NOT NULL,
    Guida CHAR(16) NOT NULL,
    Esposizione VARCHAR(16) NOT NULL,
    Data_Inizio DATE NOT NULL,
    FOREIGN KEY (Data_Visita,Turno,Esposizione,Guida)REFERENCES Visita_Guidata(Data_Visita,Turno,Esposizione,Guida)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY (ID) REFERENCES Biglietto(ID)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);
CREATE TABLE Ingresso_Libero(
    ID INT NOT NULL CHECK (ID>=0) PRIMARY KEY,
    FOREIGN KEY (ID) REFERENCES Biglietto(ID)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    Data_Validita DATE NOT NULL
);



CREATE TABLE Conoscenza(
    Argomento VARCHAR(16) NOT NULL,
    Guida CHAR(16) NOT NULL,
    PRIMARY KEY (Argomento,Guida),
    FOREIGN KEY (Argomento) REFERENCES Argomento(Nome)
        ON DELETE NO ACTION
        ON UPDATE CASCADE ,    
    FOREIGN KEY (Guida) REFERENCES Guida(CF)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);




CREATE TABLE Creazione(
    Codice CHAR(4) NOT NULL,
    Nome VARCHAR(32) NOT NULL, 
    Cognome VARCHAR(32) NOT NULL,
    DN DATE NOT NULL,
    PRIMARY Key(Codice, Nome, Cognome, DN),
    
    FOREIGN KEY (Codice) REFERENCES Artefatto(Codice)
    ON DELETE NO ACTION 
    ON UPDATE CASCADE,
    
    FOREIGN KEY (Nome, Cognome, DN) REFERENCES Artista(Nome, Cognome, DN)
    ON DELETE NO ACTION 
    ON UPDATE CASCADE

);