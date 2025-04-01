# Makefile para UDP IPv6 Client-Server
# Universidad de Costa Rica - CI0123
# Proyecto Integrador

# Compilador
CXX := g++
CXXFLAGS := -g -Wall -c

# Carpetas
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := exec

# Crear carpetas necesarias si no existen
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# Archivos fuente
CLIENT_SRC := $(SRC_DIR)/ipv6-udp-client.cc
SERVER_SRC := $(SRC_DIR)/ipv6-udp-server.cc
SOCKET_SRC := $(SRC_DIR)/Socket.cc
VSOCKET_SRC := $(SRC_DIR)/VSocket.cc
DEPS := Socket.h VSocket.h

# Objetos
OBJS_CLIENTE := $(OBJ_DIR)/ipv6-udp-client.o $(OBJ_DIR)/Socket.o $(OBJ_DIR)/VSocket.o
OBJS_SERVIDOR := $(OBJ_DIR)/ipv6-udp-server.o $(OBJ_DIR)/Socket.o $(OBJ_DIR)/VSocket.o

# Ejecutables
TARGET_CLIENTE := $(BIN_DIR)/cliente
TARGET_SERVIDOR := $(BIN_DIR)/servidor

# Regla por defecto: compilar ambos
all: $(TARGET_CLIENTE) $(TARGET_SERVIDOR)

# Compilar ejecutables
$(TARGET_CLIENTE): $(OBJS_CLIENTE)
	$(CXX) -g $^ -o $@

$(TARGET_SERVIDOR): $(OBJS_SERVIDOR)
	$(CXX) -g $^ -o $@

# Compilar objetos individuales
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

# Ejecutar cliente o servidor
run-client: $(TARGET_CLIENTE)
	./$(TARGET_CLIENTE)

run-server: $(TARGET_SERVIDOR)
	./$(TARGET_SERVIDOR)

# Limpiar archivos objeto y ejecutables
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/cliente $(BIN_DIR)/servidor
