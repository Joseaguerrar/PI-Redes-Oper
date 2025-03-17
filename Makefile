# Linux Makefile
# Socket classes
#

# Compilador
CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c -Wall
DEPS := Socket.h VSocket.h

# Carpetas
OBJ_DIR := obj
SRC_DIR := .
BIN_DIR := .

# Crear carpeta obj si no existe
$(shell mkdir -p $(OBJ_DIR))

# Archivos objeto
OBJ_IPV4 := $(OBJ_DIR)/ipv4-test.o $(OBJ_DIR)/Socket.o $(OBJ_DIR)/VSocket.o
OBJ_IPV6 := $(OBJ_DIR)/ipv6-test.o $(OBJ_DIR)/Socket.o $(OBJ_DIR)/VSocket.o

# Ejecutables
TARGET_IPV4 := $(BIN_DIR)/ipv4-test.out
TARGET_IPV6 := $(BIN_DIR)/ipv6-test.out

# Reglas principales
all: $(TARGET_IPV4) $(TARGET_IPV6)

4: $(TARGET_IPV4)
6: $(TARGET_IPV6)

# Compilación de los ejecutables
$(TARGET_IPV4): $(OBJ_IPV4)
	$(CXX) -g $^ -o $@

$(TARGET_IPV6): $(OBJ_IPV6)
	$(CXX) -g $^ -o $@

# Compilación de archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar después de compilar
run4: 4
	./$(TARGET_IPV4)

run6: 6
	./$(TARGET_IPV6)

# Compilar y ejecutar con número de versión
run: 
	@if [ "$(NUM)" = "4" ]; then make run4; \
	elif [ "$(NUM)" = "6" ]; then make run6; \
	else echo "Error: Debes especificar NUM=4 o NUM=6"; exit 1; fi

# Limpiar archivos objeto y ejecutables
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET_IPV4) $(TARGET_IPV6)
