@echo off
echo ========================================
echo   LABORATORIO 11 - COMPILADOR AST
echo   Conversion Automatica Int to Bool
echo ========================================
echo.

echo [1/3] Compilando proyecto...
g++ -o traductor.exe traductor.cpp parser.cpp lexer.cpp ast.cpp symtable.cpp error.cpp checker.cpp -std=c++17

if %ERRORLEVEL% EQU 0 (
    echo [OK] Compilacion exitosa!
    echo.
    echo [2/3] Ejecutando pruebas...
    echo.
    
    echo ========================================
    echo TESTE 1: Programa basico
    echo ========================================
    traductor.exe Testes\teste1.cpp
    echo.
    echo.
    
    echo ========================================
    echo TESTE 2: Programa con arrays y bucles
    echo ========================================
    traductor.exe Testes\teste2.cpp
    echo.
    echo.
    
    echo ========================================
    echo TESTE 3: Conversiones int to bool
    echo ========================================
    traductor.exe Testes\teste3.cpp
    echo.
    echo.
    
    echo ========================================
    echo TESTE 4: Conversiones avanzadas
    echo ========================================
    traductor.exe Testes\teste4.cpp
    echo.
    echo.
    
    echo [3/3] Pruebas completadas!
    echo ========================================
) else (
    echo [ERROR] Falla en la compilacion.
    echo Verifica que g++ este instalado y en el PATH.
    echo Comando: g++ --version
)

echo.
pause