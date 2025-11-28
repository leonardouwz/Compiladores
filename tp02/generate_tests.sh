#!/bin/bash
# Script para generar casos de prueba automáticamente

mkdir -p tests

# =============================================================================
# CASOS VÁLIDOS (ya los tienes, solo verificamos)
# =============================================================================

echo "Verificando casos de prueba válidos..."
if [ ! -f "tests/test1.mini0" ]; then
    echo "⚠️  test1.mini0 no encontrado - debe existir"
fi

# =============================================================================
# CASOS CON ERRORES
# =============================================================================

echo "Generando casos de prueba con errores..."

# Error 1: Falta 'end' al final de función
cat > tests/error1.mini0 << 'EOF'
fun test(): int
    x: int
    x = 42
    return x
EOF

# Error 2: Falta ')' en llamada a función
cat > tests/error2.mini0 << 'EOF'
fun factorial(n: int): int
    return n
end

fun main(): int
    x: int
    x = factorial(5
    return x
end
EOF

# Error 3: Tipo inválido
cat > tests/error3.mini0 << 'EOF'
fun test(a: invalid_type): int
    return a
end
EOF

# Error 4: Falta ':' en declaración
cat > tests/error4.mini0 << 'EOF'
fun test(): int
    x int
    x = 10
    return x
end
EOF

# Error 5: Expresión incompleta
cat > tests/error5.mini0 << 'EOF'
fun test(): int
    x: int
    x = 5 +
    return x
end
EOF

# Error 6: Falta 'loop' después de while
cat > tests/error6.mini0 << 'EOF'
fun test(): int
    i: int
    i = 0
    while i < 10
        i = i + 1
    end
    return i
end
EOF

# Error 7: Asignación sin '='
cat > tests/error7.mini0 << 'EOF'
fun test(): int
    x: int
    x 42
    return x
end
EOF

# Error 8: Array sin especificar tipo
cat > tests/error8.mini0 << 'EOF'
fun test(): int
    arr: []
    return 0
end
EOF

# Error 9: Return sin expresión (comentado para evitar conflicto)
cat > tests/error9.mini0 << 'EOF'
fun test(): int
    x: int
    x = 10
end
EOF

# Error 10: Paréntesis no balanceados
cat > tests/error10.mini0 << 'EOF'
fun test(): int
    x: int
    x = ((5 + 3) * 2
    return x
end
EOF

echo "✓ Casos de prueba generados en ./tests/"
echo ""
echo "Archivos creados:"
ls -1 tests/error*.mini0
echo ""
echo "Para ejecutar las pruebas: make test"