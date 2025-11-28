CC = gcc
CFLAGS = -Wall -Wextra -g
LEX = flex

TARGET = mini0_parser

SRC_DIR = src
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/parser.c \
          $(SRC_DIR)/token.c \
          $(SRC_DIR)/lex.yy.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lfl

# Generar lex.yy.c dentro de src/
$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/mini0.l
	$(LEX) -o $(SRC_DIR)/lex.yy.c $(SRC_DIR)/mini0.l

$(SRC_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/parser.h $(SRC_DIR)/token.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(SRC_DIR)/main.o

$(SRC_DIR)/parser.o: $(SRC_DIR)/parser.c $(SRC_DIR)/parser.h $(SRC_DIR)/token.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/parser.c -o $(SRC_DIR)/parser.o

$(SRC_DIR)/token.o: $(SRC_DIR)/token.c $(SRC_DIR)/token.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/token.c -o $(SRC_DIR)/token.o

$(SRC_DIR)/lex.yy.o: $(SRC_DIR)/lex.yy.c $(SRC_DIR)/token.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/lex.yy.c -o $(SRC_DIR)/lex.yy.o

clean:
	rm -f $(TARGET) $(SRC_DIR)/lex.yy.c $(SRC_DIR)/*.o

test: $(TARGET)
	@echo "=== Test 1: Factorial (válido) ==="
	./$(TARGET) tests/test1.mini0
	@echo ""
	@echo "=== Test 2: Literales (válido) ==="
	./$(TARGET) tests/test2.mini0
	@echo ""
	@echo "=== Test 3: Comentarios (válido) ==="
	./$(TARGET) tests/test3.mini0
	@echo ""
	@echo "=== Test 5: Arrays (válido) ==="
	./$(TARGET) tests/test5.mini0
	@echo ""
	@echo "=== Test 6: Operadores (válido) ==="
	./$(TARGET) tests/test6.mini0
	@echo ""
	@echo "=== Test Error 1: Falta 'end' ==="
	-./$(TARGET) tests/error1.mini0
	@echo ""
	@echo "=== Test Error 2: Falta ')' ==="
	-./$(TARGET) tests/error2.mini0
	@echo ""
	@echo "=== Test Error 3: Tipo inválido ==="
	-./$(TARGET) tests/error3.mini0

.PHONY: all clean test
