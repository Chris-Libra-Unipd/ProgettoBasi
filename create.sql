DROP VIEW  IF EXISTS Visita_Guidata_Estesa;

DROP TABLE  IF EXISTS Creazione;
DROP TABLE  IF EXISTS Ingresso_Libero;
DROP TABLE  IF EXISTS Ingresso_Guidato;
DROP TABLE  IF EXISTS Biglietto;
DROP TABLE  IF EXISTS Visita_Guidata;
DROP TABLE  IF EXISTS Appartenenza;
DROP TABLE  IF EXISTS Passata;
DROP TABLE  IF EXISTS In_Corso;
DROP TABLE  IF EXISTS Artefatto;
DROP TABLE  IF EXISTS Esposizione;
DROP TABLE  IF EXISTS Guida;
DROP TABLE  IF EXISTS Argomento;
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
    PRIMARY KEY (Nome,Cognome)

);


CREATE TABLE Argomento(
    Nome VARCHAR(32) PRIMARY KEY
);

CREATE TABLE Guida(
    CF CHAR(16) PRIMARY KEY,
    Specializzazione VARCHAR(32) NOT NULL,
    FOREIGN KEY (Specializzazione) REFERENCES Argomento (Nome)
);

CREATE TABLE Esposizione(
    Area VARCHAR(32) NOT NULL,
    Inizio DATE NOT NULL,
    Fine DATE NOT NULL CHECK(Inizio <Fine),
    Nome VARCHAR(32) NOT NULL,
    Argomento VARCHAR(32) NOT NULL,
    PRIMARY KEY(Area,Inizio),

    FOREIGN KEY (Argomento) REFERENCES Argomento(Nome)
        ON DELETE NO ACTION 
        ON UPDATE CASCADE
);



CREATE TABLE Artefatto(
    Codice CHAR(4) PRIMARY KEY,
    Nome VARCHAR(32) NOT NULL, 
    Tipologia Tipo NOT NULL
);

CREATE TABLE In_Corso(
    Area VARCHAR(32) NULL,
    Inizio DATE NOT NULL,
    Curatore CHAR(16) NOT NULL UNIQUE, 
    PRIMARY KEY(Area,Inizio),
    FOREIGN KEY (Area,Inizio) REFERENCES Esposizione(Area,Inizio)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY (Curatore) REFERENCES Guida(CF)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

CREATE TABLE Passata(
    Area VARCHAR(32) NULL,
    Inizio DATE NOT NULL,
    PRIMARY KEY(Area,Inizio),

    FOREIGN KEY (Area,Inizio) REFERENCES Esposizione(Area,Inizio)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

CREATE TABLE Appartenenza(
    Area VARCHAR(32) NULL,
    Inizio DATE NOT NULL,
    Artefatto CHAR(4) NOT NULL,
    PRIMARY KEY(Area, Inizio, Artefatto),

    FOREIGN KEY (Area,Inizio) REFERENCES Esposizione(Area,Inizio)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY (Artefatto) REFERENCES Artefatto(Codice)
        ON DELETE NO ACTION
        ON UPDATE CASCADE   
);


CREATE TABLE Visita_Guidata(
    ID INT NOT NULL CHECK (ID>=0) PRIMARY KEY,
    Area VARCHAR(32) NOT NULL,
    Inizio DATE NOT NULL,
    Guida CHAR(32) NOT NULL,
    Data_Visita DATE NOT NULL CHECK(Data_Visita>=Inizio),
    Turno Turni_Giornata NOT NULL,
    Partecipanti INT NOT NULL CHECK (Partecipanti>=0),
    CONSTRAINT unique_visits UNIQUE (Area, Inizio, Data_Visita, Turno, Guida),

    FOREIGN KEY (Guida) REFERENCES Guida(CF)
        ON DELETE NO ACTION
        ON UPDATE CASCADE ,
    
    FOREIGN KEY (Area,Inizio) REFERENCES Esposizione(Area,Inizio)
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
    IDvg INT NOT NULL,
    FOREIGN KEY (IDvg) REFERENCES Visita_Guidata(ID)
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



CREATE TABLE Creazione(
    Codice CHAR(4) NOT NULL,
    Nome VARCHAR(32) NOT NULL, 
    Cognome VARCHAR(32) NOT NULL,
    PRIMARY Key(Codice, Nome, Cognome),
    
    FOREIGN KEY (Codice) REFERENCES Artefatto(Codice)
    ON DELETE NO ACTION 
    ON UPDATE CASCADE,
    
    FOREIGN KEY (Nome, Cognome) REFERENCES Artista(Nome, Cognome)
    ON DELETE NO ACTION 
    ON UPDATE CASCADE

);

--Popolamento

INSERT INTO Artista (Nome, Cognome, DN, DM) VALUES
('Leonardo', 'da Vinci', '1452-04-15', '1519-05-02'),
('Michelangelo', 'Buonarroti', '1475-03-06', '1564-02-18'),
('Vincent', 'van Gogh', '1853-03-30', '1890-07-29'),
('Pablo', 'Picasso', '1881-10-25', '1973-04-08'),
('Frida', 'Kahlo', '1907-07-06', '1954-07-13'),
('Claude', 'Monet', '1840-11-14', '1926-12-05');

INSERT INTO Argomento (Nome) VALUES
('Rinascimento'),
('Impressionismo'),
('Cubismo'),
('Arte Moderna'),
('Arte Contemporanea'),
('Surrealismo');

INSERT INTO Guida (CF, Specializzazione) VALUES
('RSSMRA85M10H501R', 'Rinascimento'),
('BNCLSN90A41H501X', 'Impressionismo'),
('VRDGNN75P55H501Y', 'Cubismo'),
('PLTFRN60D12H501Z', 'Arte Moderna'),
('RSSGPP82E30H501W', 'Arte Contemporanea');

INSERT INTO Esposizione (Area, Nome, Inizio, Fine, Argomento) VALUES
('Area1', 'Maestri Rinascimentali', '2023-01-15', '2023-06-15', 'Rinascimento'),
('Area2', 'Luce e Colore', '2023-02-01', '2023-07-31', 'Impressionismo'),
('Area3', 'Forme Spezzate', '2023-03-10', '2024-08-20', 'Cubismo'),
('Area1', 'Modernità in Mostra', '2023-09-01', '2024-01-31', 'Arte Moderna'),
('Area2', 'Contemporanei a Confronto', '2023-10-15', '2024-03-15', 'Arte Contemporanea');

INSERT INTO Artefatto (Codice, Nome, Tipologia) VALUES
('A001', 'Monna Lisa', 'Opera'),
('A002', 'Gioconda', 'Opera'),
('A003', 'David', 'Opera'),
('A004', 'Notte Stellata', 'Opera'),
('A005', 'Guernica', 'Opera'),
('A006', 'Le due Frida', 'Opera'),
('A007', 'Ninfee', 'Opera'),
('A008', 'Vaso Antico', 'Reperto'),
('A009', 'Maschera Tribale', 'Reperto');

-- Esposizioni passate
INSERT INTO Passata (Area, Inizio) VALUES
('Area1', '2023-01-15'),
('Area2', '2023-02-01');

-- Esposizioni in corso
INSERT INTO In_Corso (Area, Inizio, Curatore) VALUES
('Area1', '2023-09-01', 'PLTFRN60D12H501Z'),
('Area2', '2023-10-15', 'RSSGPP82E30H501W'),
('Area3', '2023-03-10','BNCLSN90A41H501X');

INSERT INTO Appartenenza (Area, Inizio, Artefatto) VALUES
('Area1', '2023-01-15', 'A001'),
('Area1', '2023-01-15', 'A002'),
('Area1', '2023-01-15', 'A003'),
('Area2', '2023-02-01', 'A004'),
('Area2', '2023-02-01', 'A007'),
('Area3', '2023-03-10', 'A005'),
('Area3', '2023-03-10', 'A008');


INSERT INTO Visita_Guidata (ID, Area, Inizio, Guida, Data_Visita, Turno, Partecipanti) VALUES
(1, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-02-10', 'Mattina', 2),
(2, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-02-10', 'Pomeriggio', 0),
(3, 'Area2', '2023-02-01', 'BNCLSN90A41H501X', '2023-03-05', 'Mattina', 1),
(4, 'Area3', '2023-03-10', 'VRDGNN75P55H501Y', '2023-04-15', 'Pomeriggio', 1),
(5, 'Area1', '2023-09-01', 'PLTFRN60D12H501Z', '2023-10-10', 'Mattina', 1),
(6, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-02-24', 'Mattina', 2),
(7, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-02-24', 'Pomeriggio', 1),
(8, 'Area2', '2023-02-01', 'BNCLSN90A41H501X', '2023-03-29', 'Mattina', 1),
(9, 'Area3', '2023-03-10', 'VRDGNN75P55H501Y', '2023-04-29', 'Pomeriggio', 1),
(10, 'Area1', '2023-09-01', 'PLTFRN60D12H501Z', '2023-10-24', 'Mattina', 1),
(11, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-03-10', 'Mattina', 1),
(12, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-03-10', 'Pomeriggio', 1),
(13, 'Area2', '2023-02-01', 'BNCLSN90A41H501X', '2023-04-05', 'Mattina', 1),
(14, 'Area3', '2023-03-10', 'VRDGNN75P55H501Y', '2023-05-15', 'Pomeriggio', 1),
(15, 'Area1', '2023-09-01', 'PLTFRN60D12H501Z', '2023-11-10', 'Mattina', 1),
(16, 'Area1', '2023-09-01', 'PLTFRN60D12H501Z', '2023-12-01', 'Mattina', 1),
(17, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-04-10', 'Mattina', 1),
(18, 'Area1', '2023-01-15', 'RSSMRA85M10H501R', '2023-05-10', 'Pomeriggio', 2),
(19, 'Area2', '2023-02-01', 'BNCLSN90A41H501X', '2023-06-05', 'Mattina',0),
(20, 'Area3', '2023-03-10', 'VRDGNN75P55H501Y', '2023-07-15', 'Pomeriggio',0),
(21, 'Area1', '2023-09-01', 'PLTFRN60D12H501Z', '2023-12-10', 'Mattina',0);


INSERT INTO Biglietto (ID, Data_Acq, EMail) VALUES
(1, '2023-02-08', 'mario.rossi@email.com'),
(2, '2023-02-08', 'luigi.verdi@email.com'),
(3, '2023-03-01', 'anna.bianchi@email.com'),
(4, '2023-04-10', 'paolo.neri@email.com'),
(5, '2023-10-05', 'laura.gialli@email.com'),
(6, '2023-10-05', 'giovanni.blu@email.com'),
(7, '2023-10-06', 'maria.rosa@email.com'),
(8, '2023-10-06', 'maria.biachi@email.com'),
(9, '2023-10-06', 'paolo.neri@email.com'),
(10, '2023-10-06', 'maria.rosa@email.com'),
(11, '2023-10-06', 'maria.rosa@email.com'),
(12, '2023-10-06', 'maria.rosa@email.com'),
(13, '2023-10-06', 'maria.rosa@email.com'),
(14, '2023-10-06', 'paolo.neri@email.com'),
(15, '2023-10-06', 'maria.rosa@email.com'),
(16, '2023-10-06', 'laura.gialli@email.com'),
(17, '2023-10-06', 'maria.rosa@email.com'),
(18, '2023-10-06', 'maria.rosa@email.com'),
(19, '2023-10-06', 'maria.rosa@email.com'),
(20, '2023-10-06', 'paolo.neri@email.com'),
(21, '2023-10-06', 'laura.gialli@email.com'),
(22, '2023-10-06', 'maria.rosa@email.com');


-- Ingressi guidati
INSERT INTO Ingresso_Guidato (ID, IDvg) VALUES
(1, 1),
(2, 1),
(3, 3),
(4, 4),
(5, 5),
(8, 6),
(9, 6),
(10, 7),
(11, 8),
(12, 9),
(13, 10),
(14, 11),
(15, 13),
(16, 12),
(17, 14),
(18, 15),
(19, 16),
(20, 17),
(21, 18),
(22, 18);

-- Ingressi liberi
INSERT INTO Ingresso_Libero (ID, Data_Validita) VALUES
(6, '2023-10-05'),
(7, '2023-10-06');



INSERT INTO Creazione (Codice, Nome, Cognome) VALUES
('A001', 'Leonardo', 'da Vinci'),
('A002', 'Leonardo', 'da Vinci'),
('A003', 'Michelangelo', 'Buonarroti'),
('A004', 'Vincent', 'van Gogh'),
('A005', 'Pablo', 'Picasso'),
('A006', 'Frida', 'Kahlo'),
('A007', 'Claude', 'Monet');

-- Query
-- query 1
SELECT A.Area, A.Inizio, COUNT(*)
FROM Creazione C, (Artefatto JOIN Appartenenza ON Artefatto.codice=Appartenenza.Artefatto) A
WHERE C.Codice = A.Codice  AND C.Nome = 'Leonardo' AND C.Cognome ='da Vinci'
GROUP BY A.Area, A.Inizio
HAVING COUNT(*) >= 1

--query 2
SELECT COUNT(*)
FROM I_Guidato IG, Visita_Guidata VG, Esposizione E
WHERE VG.Area = E.Area AND VG.Inizio = E.Inizio AND IG.IDVG = VG.ID AND E.Argomento = 'Rinascimento'
GROUP BY IG.Data_Acq

--query 3
SELECT AVG(A.Partecipanti), COUNT(*)
FROM (Visita_guidata VG JOIN Esposizione E ON VG.Area = E.Area AND VG.Inizio = E.inizio) A
WHERE A.Data_visita >= '2020-01-10' AND A.Argomento = 'Rinascimento'

--query 4
SELECT CF, NumVisite
FROM (
    SELECT VG.Guida AS CF, COUNT(*) AS NumVisite
    FROM Visita_Guidata_Estesa VG
    WHERE Argomento = 'Rinascimento'
    GROUP BY VG.Guida
) AS A
ORDER BY NumVisite DESC
LIMIT 1;

--
--query 5
SELECT AVG(Num)
FROM	(SELECT COUNT(*) AS Num
FROM Creazione C, Artefatto A
WHERE A.Codice = C.Codice AND C.Nome = 'Leonardo' AND C.Cognome = 'da Vinci' 
GROUP BY A.Esposizione)



-- Inidci


CREATE INDEX index_visits ON Visita_Guidata ( Data_Visita );