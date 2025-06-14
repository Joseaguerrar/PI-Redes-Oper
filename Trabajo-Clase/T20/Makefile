# Compilador
CXX := g++
CXXFLAGS := -g -c -std=c++11
LDFLAGS :=

# Directorios
SRC := .
OBJDIR := obj
BINDIR := exec

VPATH := $(SRC)

# Objetos por ejecutable
OBJS_CLIENT := $(OBJDIR)/MirrorClient.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o
OBJS_SERVER := $(OBJDIR)/ThreadMirrorServer.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o $(OBJDIR)/file_system.o
OBJS_FIGURA := $(OBJDIR)/figura_discovery.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o $(OBJDIR)/file_system.o
OBJS_TENEDOR := $(OBJDIR)/tenedor_discovery.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o

# Ejecutables
TARGET_CLIENT := $(BINDIR)/MirrorClient.out
TARGET_SERVER := $(BINDIR)/ThreadMirrorServer.out
TARGET_FIGURA := $(BINDIR)/figura_discovery.out
TARGET_TENEDOR := $(BINDIR)/tenedor_discovery.out

TARGETS := $(TARGET_CLIENT) $(TARGET_SERVER) $(TARGET_FIGURA) $(TARGET_TENEDOR)

.PHONY: all clean run client server figuras tenedor

# Regla por defecto
all: $(TARGETS)

# Crear directorios si no existen
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Compilación genérica
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Enlace de ejecutables
$(TARGET_CLIENT): $(OBJS_CLIENT) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_SERVER): $(OBJS_SERVER) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_FIGURA): $(OBJS_FIGURA) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_TENEDOR): $(OBJS_TENEDOR) | $(BINDIR)
	$(CXX) $^ -o $@

# Targets individuales
client: $(TARGET_CLIENT)
server: $(TARGET_SERVER)
figuras: $(TARGET_FIGURA)
tenedor: $(TARGET_TENEDOR)

# Limpieza
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Ejecución simple
run:
	@echo "Uso: make run [client|server|figuras|tenedor]"

run_client: client
	./$(TARGET_CLIENT)

run_server: server
	./$(TARGET_SERVER)

run_figuras: figuras
	./$(TARGET_FIGURA)

run_tenedor: tenedor
	./$(TARGET_TENEDOR)
