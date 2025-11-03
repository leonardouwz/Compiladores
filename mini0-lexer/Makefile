CC = gcc
CFLAGS = -Wall -g
LEX = flex

TARGET = mini0_lexer
SOURCES = lex.yy.c token.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): lex.yy.c token.o
	$(CC) $(CFLAGS) -o $(TARGET) lex.yy.c token.o -lfl

lex.yy.c: mini0.l
	$(LEX) mini0.l

token.o: token.c token.h
	$(CC) $(CFLAGS) -c token.c

clean:
	rm -f $(TARGET) lex.yy.c *.o

test: $(TARGET)
	@echo "=== Prueba 1: Programa básico ==="
	./$(TARGET) tests/test1.mini0
	@echo "\n=== Prueba 2: Números y strings ==="
	./$(TARGET) tests/test2.mini0
	@echo "\n=== Prueba 3: Comentarios ==="
	./$(TARGET) tests/test3.mini0
	@echo "\n=== Prueba 4: Errores ==="
	./$(TARGET) tests/test4.mini0

.PHONY: all clean test