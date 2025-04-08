#Makefile for Bhajan Database Project

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = bhajan-db
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

