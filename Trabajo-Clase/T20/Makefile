# Compiladores
CXX := g++

# Flags de compilación
CXXFLAGS := -g -c -std=c++11
LDFLAGS :=

# Directorios
SRC := .
OBJDIR := obj
BINDIR := exec

VPATH := $(SRC)

# Archivos objeto por ejecutable
OBJS_CLIENT := $(OBJDIR)/MirrorClient.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o
OBJS_SERVER := $(OBJDIR)/ThreadMirrorServer.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o $(OBJDIR)/file_system.o
OBJS_FIGURA := $(OBJDIR)/figura_discovery.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o
OBJS_TENEDOR := $(OBJDIR)/tenedor_discovery.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o

# Targets
TARGET_CLIENT := $(BINDIR)/MirrorClient.out
TARGET_SERVER := $(BINDIR)/ThreadMirrorServer.out
TARGET_FIGURA := $(BINDIR)/figura_discovery.out
TARGET_TENEDOR := $(BINDIR)/tenedor_discovery.out

TARGETS := $(TARGET_CLIENT) $(TARGET_SERVER) $(TARGET_FIGURA) $(TARGET_TENEDOR)

# Regla principal
all: $(TARGETS)

# Limpieza
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean run client server figuras tenedor

# Crear carpetas si no existen
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Reglas genéricas para .cpp y .cc -> .o
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Enlazado de ejecutables
$(TARGET_CLIENT): $(OBJS_CLIENT) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_SERVER): $(OBJS_SERVER) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_FIGURA): $(OBJS_FIGURA) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_TENEDOR): $(OBJS_TENEDOR) | $(BINDIR)
	$(CXX) $^ -o $@

# Targets específicos
client: $(TARGET_CLIENT)
server: $(TARGET_SERVER)
figuras: $(TARGET_FIGURA)
tenedor: $(TARGET_TENEDOR)

# Ejecutar con make run <nombre>
run:
	@echo "Uso: make run [client|server|figuras|tenedor]"

client: client
	./$(TARGET_CLIENT)

server: server
	./$(TARGET_SERVER)

figuras: figuras
	./$(TARGET_FIGURA)

tenedor: tenedor
	./$(TARGET_TENEDOR)

run_%: 
	@$(MAKE) run_$*
