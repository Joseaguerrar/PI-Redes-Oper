# Compiladores y flags
CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c 
LDFLAGS := -lssl -lcrypto

# Carpetas
OBJDIR := obj
BINDIR := exec

# Archivos fuente requeridos
SRCS := ipv4-ssl-cli.cc VSocket.cpp Socket.cpp SSLSocket.cpp
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(OBJS)))

# Ejecutable principal (cliente SSL)
MAIN_EXE := $(BINDIR)/ipv4-ssl-cli.out

# Regla por defecto
all: $(MAIN_EXE)

# Compilar ejecutable
$(MAIN_EXE): $(OBJS) | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

# Compilar cada archivo fuente
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Crear carpetas si no existen
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Ejecutar programa
run: $(MAIN_EXE)
	./$(MAIN_EXE) ssl

# Limpiar
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out
