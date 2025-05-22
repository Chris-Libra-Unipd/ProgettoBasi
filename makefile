# Makefile per compilare ed eseguire prova_conn_db.c
CC = gcc
CFLAGS = -I/usr/include/postgresql
LDFLAGS = -lpq

TARGET = prova.o
SRC = prova_conn_db.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)