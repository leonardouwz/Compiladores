# Analizador Léxico para Mini-0

**Laboratorio 10 - Compiladores**  
**Universidad La Salle**  
**Escuela Profesional de Ingeniería de Software**

## 📋 Descripción

Este proyecto implementa un analizador léxico completo para el lenguaje Mini-0 utilizando Flex. El analizador es capaz de reconocer todos los tipos de tokens definidos en la especificación del lenguaje y clasificarlos correctamente.

## 🎯 Características Implementadas

### ✅ Reconocimiento de Tokens
- **Palabras reservadas**: `if`, `else`, `end`, `while`, `loop`, `fun`, `return`, `new`, `string`, `int`, `char`, `bool`, `true`, `false`, `and`, `or`, `not`
- **Identificadores**: Letra o underscore seguido de letras, números o underscores
- **Números literales**: 
  - Decimales: `15`, `42`, `100`
  - Hexadecimales: `0x0f`, `0xFF`, `0x1A2B`
  - ⚠️ Los valores `15` y `0x0f` se reconocen como el mismo valor (15)
- **Strings**: Entre comillas dobles con soporte para escapes `\\`, `\n`, `\t`, `\"`
- **Operadores**: `+`, `-`, `*`, `/`, `>`, `<`, `>=`, `<=`, `=`, `<>`
- **Delimitadores**: `(`, `)`, `[`, `]`, `,`, `:`
- **Saltos de línea**: Relevantes para la gramática (NL)

### ✅ Comentarios
- **Comentarios de línea**: `// hasta el fin de línea`
- **Comentarios de bloque**: `/* ... */` sin anidamiento

### ✅ Estructura de Datos
```c
typedef struct {
    TokenType type;        // Tipo de token
    char* lexeme;          // Cadena reconocida
    int line;              // Número de línea (primera línea = 1)
    int has_numeric_value; // Indica si tiene valor numérico
    long numeric_value;    // Valor numérico procesado
} Token;
```

### ✅ Manejo de Errores
- Caracteres no reconocidos generan tokens de tipo `TK_ERROR`
- Se reporta la línea donde ocurre el error

## 📁 Estructura del Repositorio

```
mini0-lexer/
├── src/
│   ├── mini0.l          # Especificación Flex del analizador léxico
│   ├── token.h          # Definición de tipos y estructuras de tokens
│   └── token.c          # Implementación de funciones auxiliares
├── tests/
│   ├── test1.mini0      # Programa básico con funciones
│   ├── test2.mini0      # Números y strings
│   ├── test3.mini0      # Comentarios
│   ├── test4.mini0      # Casos de error
│   ├── test5.mini0      # Arrays y loops
│   └── test6.mini0      # Operadores booleanos
├── Makefile             # Archivo de compilación
├── README.md            # Documentación
└── .gitignore           # Archivos a ignorar en Git
```

## 🔧 Compilación y Uso

### Requisitos
- `flex` (Fast Lexical Analyzer)
- `gcc` (GNU Compiler Collection)
- `make`

### Instalación de dependencias (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install flex gcc make
```

### Compilación
```bash
make
```

### Uso
```bash
./mini0_lexer <archivo.mini0>
```

Ejemplo:
```bash
./mini0_lexer tests/test1.mini0
```

### Ejecutar todas las pruebas
```bash
make test
```

### Limpiar archivos generados
```bash
make clean
```

## 📊 Salida del Programa

El programa genera una tabla con los tokens reconocidos:

```
========================================
ANÁLISIS LÉXICO - Mini-0
========================================
Total de tokens: 45

LÍNEA  TIPO            LEXEMA               VALOR          
--------------------------------------------------------------
1      NL              \n                  
2      FUN             fun                 
2      ID              factorial           
2      LPAREN          (                   
2      ID              n                   
2      COLON           :                   
2      INT_TYPE        int                 
2      RPAREN          )                   
2      COLON           :                   
2      INT_TYPE        int                 
2      NL              \n                  
3      IF              if                  
3      ID              n                   
3      LE              <=                  
3      NUMERAL         1                   1              
...
========================================
```

## 🧪 Casos de Prueba

### Test 1: Programa Básico
- Función recursiva `factorial`
- Función `main`
- Declaraciones de variables
- Estructuras de control `if/else/end`

### Test 2: Números y Strings
- Números decimales y hexadecimales
- Strings con caracteres de escape
- Verificación de equivalencia (15 = 0x0f)

### Test 3: Comentarios
- Comentarios de línea (`//`)
- Comentarios de bloque (`/* */`)
- Comentarios inline

### Test 4: Errores
- Caracteres inválidos (`@`, `#`)
- Tokens de error correctamente identificados

### Test 5: Arrays
- Declaración de arrays con `[]`
- Operador `new`
- Acceso a elementos con `[i]`
- Bucle `while/loop`

### Test 6: Operadores
- Operadores relacionales y booleanos
- Expresiones complejas
- Precedencia de operadores

## 🔍 Detalles de Implementación

### Procesamiento de Números
```c
// Los hexadecimales se convierten a decimal
0x0f → valor numérico: 15
15   → valor numérico: 15
```

### Procesamiento de Strings
```c
// Los escapes se traducen al carácter correspondiente
"Hola\nmundo" → "Hola
mundo"
"Ruta: C:\\Users" → "Ruta: C:\Users"
```

### Contador de Líneas
- La primera línea del programa es la línea 1
- Se incrementa con cada `\n` encontrado
- Se mantiene correctamente en comentarios multilínea

## 📝 Notas Importantes

1. **Variable `yyin`**: Se utiliza para redirigir la entrada de Flex desde un archivo
2. **Memoria dinámica**: Los tokens se almacenan en un array dinámico que crece según sea necesario
3. **Liberación de memoria**: Se liberan todos los recursos al finalizar el programa
4. **Compatibilidad**: El código es compatible con C99 y superiores

## 👥 Autores

- [Tu Nombre]
- [Nombre del compañero] (si aplica)

## 📅 Fecha de Entrega

03/11/2025

## 🔗 Repositorio

[Incluir link a GitHub aquí]

## 📧 Contacto

Para consultas sobre la implementación, contactar al profesor:
**Phd. Marcela Quispe Cruz**
