# Compiladores
CXX := g++
CXXFLAGS := -g -c -std=c++11
LDFLAGS :=

# Directorios
SRC := .
OBJDIR := obj
BINDIR := exec

VPATH := $(SRC)

# Objetos por ejecutable
OBJS_FIGURA := $(OBJDIR)/figura_discovery.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o
OBJS_TENEDOR := $(OBJDIR)/tenedor_discovery.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o

# Targets finales
TARGET_FIGURA := $(BINDIR)/figura_discovery.out
TARGET_TENEDOR := $(BINDIR)/tenedor_discovery.out

# Target general
all: figuras tenedor

# Compilar figuras
figuras: $(TARGET_FIGURA)

# Compilar tenedor
tenedor: $(TARGET_TENEDOR)

# Crear carpetas si no existen
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Reglas genéricas
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Enlazado
$(TARGET_FIGURA): $(OBJS_FIGURA) | $(BINDIR)
	$(CXX) $^ -o $@

$(TARGET_TENEDOR): $(OBJS_TENEDOR) | $(BINDIR)
	$(CXX) $^ -o $@

# Limpieza
clean:
	rm -rf $(OBJDIR) $(BINDIR)
