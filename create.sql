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
    Nome VARCHAR(32) PRIMARY KEY
);

CREATE TABLE Dipendente(
    CF CHAR(32) PRIMARY KEY
);

CREATE TABLE Guida(
    CF CHAR(32) PRIMARY KEY,
    FOREIGN KEY (CF) REFERENCES Dipendente(CF)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Argomento(
    Nome VARCHAR(32) PRIMARY KEY
);

CREATE TABLE Esposizione(
    Area VARCHAR(32) PRIMARY KEY,
    Nome VARCHAR(32) NOT NULL,
    Argomento VARCHAR(32) NOT NULL,
    FOREIGN KEY (Area) REFERENCES Area(Nome)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE,
    FOREIGN KEY (Argomento) REFERENCES Argomento(Nome)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Artefatto(
    Codice CHAR(4) PRIMARY KEY,
    Nome VARCHAR(32) NOT NULL, 
    Tipologia Tipo NOT NULL, 
    Esposizione VARCHAR(32) NULL,
    FOREIGN KEY (Esposizione) REFERENCES Esposizione(Area)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);

CREATE TABLE Visita_Guidata(
    Esposizione VARCHAR(32) NOT NULL,
    Guida CHAR(32) NOT NULL,
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
    Guida CHAR(32) NOT NULL,
    Esposizione VARCHAR(32) NOT NULL,
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
    Argomento VARCHAR(32) NOT NULL,
    Guida CHAR(32) NOT NULL,
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



INSERT INTO Artista (Nome, Cognome, DN, DM) VALUES
('Leonardo', 'da Vinci', '1452-04-15', '1519-05-02'),
('Michelangelo', 'Buonarroti', '1475-03-06', '1564-02-18'),
('Vincent', 'van Gogh', '1853-03-30', '1890-07-29'),
('Pablo', 'Picasso', '1881-10-25', '1973-04-08'),
('Frida', 'Kahlo', '1907-07-06', '1954-07-13'),
('Jeff', 'Koons', '1955-01-21', NULL),
('Banksy', 'Unknown', '1974-07-28', NULL);

INSERT INTO Area (Nome) VALUES
('Area_1'),
('Area_2'),
('Area_3'),
('Area_4'),
('Area_5');

INSERT INTO Dipendente (CF) VALUES
('RSSMRA85M10H501R'),
('BNCLGU90P41G702S'),
('VRDGNN80A01D612T'),
('PLTFRN75L15H223U');

INSERT INTO Guida (CF) VALUES
('RSSMRA85M10H501R'),
('BNCLGU90P41G702S'),
('VRDGNN80A01D612T');

INSERT INTO Argomento (Nome) VALUES
('Pittura ad olio'),
('Affresco'),
('Scultura'),
('Fotografia');
('Arte digitale'),

INSERT INTO Conoscenza (Argomento, Guida) VALUES
('Pittura ad olio', 'RSSMRA85M10H501R'),
('Affresco', 'RSSMRA85M10H501R'),
('Scultura', 'BNCLGU90P41G702S'),
('Arte digitale', 'VRDGNN80A01D612T'),
('Fotografia', 'VRDGNN80A01D612T');

INSERT INTO Esposizione (Area, Nome) VALUES
('Area_1', 'Maestri Italiani','Pittura ad olio'),
('Area_2', 'Luce e Colore','Pittura ad olio'),
('Area_3', 'Rivoluzioni',''),
('Area_4', 'Oltre i Confini',''),
('Area_5', 'Forme Eterne','Scultura');

INSERT INTO Artefatto (Codice, Nome, Tipologia, Esposizione) VALUES
('A001', 'Gioconda', 'Opera', 'Area_1'),
('A002', 'David', 'Reperto', 'Area_5'),
('A003', 'Notte Stellata', 'Opera', 'Area_2'),
('A004', 'Guernica', 'Opera', 'Area_3'),
('A005', 'Balloon Dog', 'Reperto', 'Area_4'),
('A006', 'Ragazza con turbante', 'Opera', NULL);

INSERT INTO Creazione (Codice, Nome, Cognome, DN) VALUES
('A001', 'Leonardo', 'da Vinci', '1452-04-15'),
('A002', 'Michelangelo', 'Buonarroti', '1475-03-06'),
('A003', 'Vincent', 'van Gogh', '1853-03-30'),
('A004', 'Pablo', 'Picasso', '1881-10-25'),
('A005', 'Jeff', 'Koons', '1955-01-21');

INSERT INTO Visita_Guidata (Esposizione, Guida, Data_Visita, Turno, Partecipanti) VALUES
('Area_1', 'RSSMRA85M10H501R', '2023-06-15', 'Mattina', 20),
('Area_2', 'BNCLGU90P41G702S', '2023-06-15', 'Pomeriggio', 15),
('Area_3', 'VRDGNN80A01D612T', '2023-06-16', 'Mattina', 18),
('Area_5', 'BNCLGU90P41G702S', '2023-06-17', 'Pomeriggio', 12);

INSERT INTO Biglietto (ID, Data_Acq, EMail) VALUES
(1001, '2023-06-10', 'cliente1@email.com'),
(1002, '2023-06-11', 'cliente2@email.com'),
(1003, '2023-06-12', NULL),
(1004, '2023-06-13', 'cliente4@email.com'),
(1005, '2023-06-14', 'cliente5@email.com');

INSERT INTO Ingresso_Guidato (ID, Data_Visita, Turno, Guida, Esposizione) VALUES
(1001, '2023-06-15', 'Mattina', 'RSSMRA85M10H501R', 'Area_1'),
(1002, '2023-06-15', 'Pomeriggio', 'BNCLGU90P41G702S', 'Area_2'),
(1003, '2023-06-16', 'Mattina', 'VRDGNN80A01D612T', 'Area_3');


INSERT INTO Ingresso_Libero (ID, Data_Validita) VALUES
(1004, '2023-06-20'),
(1005, '2023-06-21');