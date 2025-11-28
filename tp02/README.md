# Parser Mini-0 - Analizador Sintáctico Recursivo Descendente

**Universidad La Salle - Compiladores**  
**Trabajo Práctico 2**

## 📋 Descripción

Parser (analizador sintáctico) recursivo descendente para el lenguaje Mini-0. Implementa análisis predictivo LL(1) y valida programas según la gramática formal del lenguaje.
Aquí se está poniendo el enlace de la Tabla Principal:
https://github.com/leonardouwz/Compiladores/blob/main/tp02/TablaPrincipal.pdf
## 🎯 Características

- ✅ Análisis sintáctico LL(1) completo
- ✅ Gramática transformada (sin recursión izquierda)
- ✅ Detección de errores léxicos y sintácticos
- ✅ Mensajes de error informativos con número de línea
- ✅ Manejo de precedencia y asociatividad de operadores
- ✅ Casos de prueba exhaustivos (válidos e inválidos)
- ✅ Integración con analizador léxico Flex

## 🏗️ Estructura del Proyecto

```
mini0-parser/
├── src/
│   ├── main.c           # Programa principal
│   ├── parser.c         # Implementación del parser
│   ├── parser.h         # Interface del parser
│   ├── token.c          # Funciones de tokens
│   ├── token.h          # Definiciones de tokens
│   └── mini0.l          # Especificación léxica (Flex)
├── tests/
│   ├── test1.mini0      # Factorial (válido)
│   ├── test2.mini0      # Literales (válido)
│   ├── test3.mini0      # Comentarios (válido)
│   ├── test5.mini0      # Arrays (válido)
│   ├── test6.mini0      # Operadores (válido)
│   ├── error1.mini0     # Falta 'end'
│   ├── error2.mini0     # Falta ')'
│   ├── error3.mini0     # Tipo inválido
│   └── ...              # Más casos de error
├── Makefile             # Script de compilación
├── generate_tests.sh    # Generador de casos de prueba
├── informe_tecnico.md   # Documentación completa
└── README.md            # Este archivo
```

## 🔧 Requisitos

- GCC (compilador C)
- Flex (generador léxico)
- Make
- Sistema Linux/Unix o WSL en Windows

## 📦 Instalación

### 1. Clonar el repositorio
```bash
git clone https://github.com/tu-usuario/mini0-parser.git
cd mini0-parser
```

### 2. Instalar dependencias (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential flex
```

### 3. Compilar
```bash
make
```

Esto genera el ejecutable `mini0_parser`.

## 🚀 Uso

### Analizar un programa Mini-0
```bash
./mini0_parser <archivo.mini0>
```

**Ejemplo con programa válido:**
```bash
$ ./mini0_parser tests/test1.mini0
✓ Análisis sintáctico exitoso
Programa válido en Mini-0
```

**Ejemplo con programa inválido:**
```bash
$ ./mini0_parser tests/error1.mini0
Error sintáctico en línea 5: Se esperaba 'end'. Se encontró '$' (EOF)

✗ Análisis sintáctico fallido
Total de errores sintácticos: 1
```

### Ejecutar todos los tests
```bash
make test
```

### Generar casos de prueba
```bash
chmod +x generate_tests.sh
./generate_tests.sh
```

### Limpiar archivos compilados
```bash
make clean
```

## 📝 Gramática Mini-0

### Características del Lenguaje

- **Funciones:** Definición con parámetros y tipo de retorno
- **Tipos básicos:** `int`, `bool`, `string`, `char`
- **Arrays:** Soporte para arrays dinámicos con `new`
- **Control de flujo:** `if-else`, `while-loop`
- **Operadores:**
  - Aritméticos: `+`, `-`, `*`, `/`
  - Relacionales: `<`, `>`, `<=`, `>=`, `=`, `<>`
  - Lógicos: `and`, `or`, `not`

### Ejemplo de Programa

```mini0
fun factorial(n: int): int
    if n <= 1
        return 1
    else
        return n * factorial(n - 1)
    end
end

fun main(): int
    x: int
    x = factorial(5)
    return x
end
```

## 🔍 Transformaciones Aplicadas

### 1. Eliminación de Recursión por la Izquierda

**Antes:**
```
Expression → Expression 'or' AndExpr | AndExpr
```

**Después:**
```
Expression  → AndExpr ExprPrime
ExprPrime   → 'or' AndExpr ExprPrime | ε
```

### 2. Precedencia de Operadores

De menor a mayor precedencia:
1. `or`
2. `and`
3. `<`, `>`, `<=`, `>=`, `=`, `<>`
4. `+`, `-`
5. `*`, `/`
6. `not`, `-` (unarios)

### 3. Tabla LL(1)

Consulta el archivo `informe_tecnico.md` para ver la tabla completa de análisis sintáctico.

## 🐛 Tipos de Errores Detectados

| Categoría | Ejemplo |
|-----------|---------|
| **Léxicos** | Caracteres inválidos (`@`, `#`) |
| **Falta de tokens** | Falta `)`, `end`, `:` |
| **Tipo inválido** | `x: invalid_type` |
| **Expresión incompleta** | `x = 5 +` (sin operando) |
| **Estructura incorrecta** | `while ... end` (debe ser `loop`) |
| **EOF inesperado** | Función sin `end` |

## 📊 Casos de Prueba

### Válidos
- ✅ **test1.mini0:** Función recursiva (factorial)
- ✅ **test2.mini0:** Literales (decimales, hexadecimales, strings)
- ✅ **test3.mini0:** Comentarios de línea y bloque
- ✅ **test5.mini0:** Arrays y loops
- ✅ **test6.mini0:** Operadores lógicos y relacionales

### Con Errores
- ❌ **error1.mini0:** Falta `end`
- ❌ **error2.mini0:** Falta `)`
- ❌ **error3.mini0:** Tipo inválido
- ❌ **error4.mini0:** Falta `:`
- ❌ **error5.mini0:** Expresión incompleta
- ❌ Y más... (ver carpeta `tests/`)

## 🔬 Detalles de Implementación

### Estructura del Parser

```c
typedef struct {
    Token* tokens;      // Array de tokens del léxico
    int token_count;    // Cantidad total de tokens
    int current;        // Índice del token actual
    bool has_error;     // Flag de error
    int error_count;    // Contador de errores
} Parser;
```

### Funciones Principales

- `parse()`: Punto de entrada del análisis
- `parse_program()`: Regla inicial de la gramática
- `parse_function()`, `parse_statement()`, etc.: Reglas de producción
- `match()`: Verifica tipo de token
- `consume()`: Consume token o reporta error

### Manejo de Saltos de Línea

Los tokens `TK_NL` (nueva línea) se saltan automáticamente para no interferir con la gramática:

```c
Token* current_token(Parser* p) {
    while (p->current < p->token_count && 
           p->tokens[p->current].type == TK_NL) {
        p->current++;
    }
    return &p->tokens[p->current];
}
```

## 📖 Documentación

- **Informe Técnico Completo:** Ver `informe_tecnico.md`
  - Gramática original y transformada
  - Tabla LL(1) detallada
  - Diseño del parser
  - Manejo de errores
  - Casos de prueba explicados

## 🎓 Evaluación

Este proyecto se evalúa según la rúbrica oficial (20 puntos):

1. **Modificaciones a la gramática:** 4 pts
2. **Implementación del parser:** 5 pts
3. **Manejo de errores:** 2 pts
4. **Casos de prueba:** 2 pts
5. **Informe técnico:** 4 pts
6. **Presentación oral:** 3 pts (obligatoria)

## 👥 Autores

Leonardo Raphaeel Pachari Gomez
Angela Milagros Quispe Huanca  
Universidad La Salle - Compiladores  
Fecha: Noviembre 2025

## 📄 Licencia

Este proyecto es material académico para el curso de Compiladores.

## 🤝 Contribuciones

Este es un proyecto académico grupal. No se aceptan contribuciones externas.
