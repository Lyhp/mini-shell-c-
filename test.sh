#!/bin/bash
# Script de pruebas automáticas para minishell

echo "Iniciando pruebas de minishell..."
echo ""

# Crear archivos de prueba
echo "Preparando archivos de prueba..."
echo -e "hola\nmundo\nhello\nworld" > test_input.txt
echo "Archivo de prueba creado" > test_file.txt

# Función para ejecutar comando en el shell
run_test() {
    local name="$1"
    local cmd="$2"
    local expected="$3"
    
    echo "→ Test: $name"
    echo "  Comando: $cmd"
    
    # Ejecutar comando y capturar salida
    result=$(echo "$cmd" | ./minishell 2>/dev/null | tail -n +5)
    
    echo "  ✓ Ejecutado"
    echo ""
}

# Tests
echo "=== PRUEBAS BÁSICAS ==="
run_test "Comando simple" "ls -la"
run_test "PWD" "pwd"

echo "=== PRUEBAS DE REDIRECCIÓN ==="
run_test "Redirección >" "echo 'test' > output.txt"
run_test "Redirección >>" "echo 'append' >> output.txt"
run_test "Redirección <" "cat < test_input.txt"

echo "=== PRUEBAS DE PIPES ==="
run_test "Pipe simple" "cat test_input.txt | grep hello"
run_test "Pipe doble" "cat test_input.txt | grep 'o' | wc -l"

echo "=== PRUEBAS DE BUILTINS ==="
run_test "CD" "cd /tmp"
run_test "Help" "help"
run_test "History" "history"

echo "=== LIMPIEZA ==="
rm -f test_input.txt test_file.txt output.txt

echo "Pruebas completadas"
echo ""
echo "Para pruebas interactivas, ejecuta: ./minishell"