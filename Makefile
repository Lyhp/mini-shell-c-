# Makefile para minishell

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./include
LDFLAGS = 

# Directorios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TARGET = minishell

# Archivos fuente y objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


# Targets principales


# Target por defecto: compilar todo
all: $(BUILD_DIR) $(TARGET)
	@echo "✓ Compilación exitosa: ./$(TARGET)"

# Crear ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compilar archivos .cpp a .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorio build si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


#Utilidades

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "✓ Limpieza completa"

# Compilar y ejecutar
run: all
	./$(TARGET)

# Mostrar ayuda
help:
	@echo "Targets disponibles:"
	@echo "  make          - Compilar el proyecto"
	@echo "  make run      - Compilar y ejecutar"
	@echo "  make clean    - Limpiar archivos generados"
	@echo "  make help     - Mostrar esta ayuda"

.PHONY: all clean run help