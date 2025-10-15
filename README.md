# mini-shell-c-

Una mini-shell POSIX implementada en C++ que replica las funcionalidades básicas de shells Unix como bash, con soporte para pipes, redirecciones, procesos en background y comandos internos.

## Descripción

minishell es un intérprete de comandos interactivo que permite ejecutar programas externos, gestionar procesos, redirigir entrada/salida y conectar comandos mediante tuberías. Implementado utilizando llamadas al sistema POSIX (fork(), exec*(), pipe(), dup2(), wait()), demuestra los conceptos fundamentales de sistemas operativos y programación de sistemas.

## Requerimientos

### Compilador y Herramientas
- *C++17* o superior
- *g++* / *clang++*
- *Make* (para compilación automatizada)
- Sistema operativo *Unix/Linux/macOS*

### Bibliotecas
- Biblioteca estándar de C++ (<iostream>, <string>, <vector>)
- Llamadas al sistema POSIX (<unistd.h>, <sys/wait.h>, <sys/types.h>, <fcntl.h>)

## Compilación y Ejecución

### Compilar el proyecto
make


### Ejecutar el shell
./minishell


### Limpiar archivos generados
make clean


### Compilar y ejecutar directamente
make run


## Características Implementadas

### Características Base

| Característica | Estado | Descripción |
|---|:---:|---|
| *Prompt personalizado* | ✅ | Muestra minishell$ como prompt y lee comandos del usuario |
| *Resolución de rutas* | ✅ | Busca ejecutables en PATH, maneja rutas absolutas y relativas |
| *Ejecución con procesos* | ✅ | Usa fork() + execvp() + wait() para ejecutar comandos |
| *Manejo de errores* | ✅ | Mensajes claros con perror() cuando fallan comandos o redirecciones |
| *Redirección de salida >* | ✅ | Redirige stdout a archivo (crea/trunca) usando dup2() |
| *Comando de salida* | ✅ | exit o salir para terminar el shell |

### Extensiones Extra (Valor Agregado)

| Característica | Estado | Descripción |
|---|:---:|---|
| *Pipes simples \|* | ✅ | Conecta salida de un comando con entrada de otro usando pipe() |
| *Procesos en background &* | ✅ | Ejecuta comandos sin bloquear el prompt, con recolección no bloqueante |
| *Redirección de entrada <* | ✅ | Redirige stdin desde archivo |
| *Redirección append >>* | ✅ | Anexa stdout al final de un archivo existente |
| *Comandos internos (builtins)* | ✅ | cd, pwd, exit, help, history, jobs |
| *Manejo de señales* | ✅ | Captura SIGINT (Ctrl+C) sin terminar el shell |

### Comandos Internos (Builtins)

| Comando | Descripción |
|---|---|
| cd [dir] | Cambia el directorio actual (sin args → HOME) |
| pwd | Muestra el directorio de trabajo actual |
| exit [code] | Sale del shell con código de retorno opcional |
| help | Muestra ayuda sobre comandos disponibles |
| history | Muestra historial de comandos ejecutados |
| jobs | Lista procesos en ejecución en background |

## Ejemplos de Uso

### Comandos básicos

minishell$ ls -la
minishell$ pwd
minishell$ cd /tmp


### Redirecciones

minishell$ ls > archivos.txt          # Redirigir salida
minishell$ cat < entrada.txt          # Redirigir entrada
minishell$ echo "nuevo" >> log.txt    # Anexar a archivo


### Pipes

minishell$ ls -l | grep cpp           # Un pipe
minishell$ cat file.txt | grep error | wc -l    # Múltiples pipes


### Procesos en background

minishell$ sleep 10 &                 # Ejecutar en background
minishell$ jobs                        # Ver procesos activos


### Comandos internos

minishell$ help                        # Ver ayuda
minishell$ history                     # Ver historial
minishell$ exit                        # Salir del shell


## Arquitectura


src/
├── main.cpp        # Punto de entrada
├── shell.cpp       # Loop principal (REPL)
├── parser.cpp      # Parseo de comandos (tokenización, pipes)
├── executor.cpp    # Ejecución (fork, exec, pipe, dup2)
├── builtins.cpp    # Comandos internos (cd, pwd, etc.)
├── command.cpp     # Estructuras de datos
└── signals.cpp     # Manejo de señales (SIGINT)

include/            # Headers correspondientes


## Implementación Técnica

### Llamadas al Sistema Utilizadas
- *fork()*: Crea procesos hijos
- *execvp()*: Reemplaza proceso con nuevo programa
- *wait() / waitpid()*: Espera terminación de procesos
- *pipe()*: Crea tuberías para comunicación entre procesos
- *dup2()*: Duplica descriptores de archivo para redirecciones
- *open()*: Abre archivos para redirecciones
- *chdir()* / *getcwd()*: Navegación de directorios
- *sigaction()*: Configuración de manejadores de señales

### Flujo de Ejecución
1. *Lectura*: Shell lee línea de comando del usuario
2. *Parseo*: Parser tokeniza y detecta operadores (|, >, <, >>, &)
3. *Construcción*: Genera estructura Pipeline con comandos
4. *Ejecución*: 
   - Builtins se ejecutan directamente
   - Comandos externos usan fork() + exec()
   - Pipes conectan múltiples procesos
5. *Espera*: Shell espera terminación (foreground) o continúa (background)

## Notas

- Los tokens deben estar separados por espacios: ls > out.txt (correcto) vs ls>out.txt (incorrecto)
- Las comillas simples y dobles permiten argumentos con espacios: echo "hello world"
- El historial de comandos persiste durante la sesión pero no entre reinicios
- Los procesos zombie en background se recolectan automáticamente
