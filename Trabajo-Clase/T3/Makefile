# Linux Makefile
# Socket class with SSL
#

# Compiladores y flags
CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c 
LDFLAGS := -lssl -lcrypto

# Carpetas
OBJDIR := obj
BINDIR := exec

# Archivos fuente
SRCS := ipv4-ssl-cli.cc VSocket.cpp Socket.cpp SSLSocket.cpp
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(OBJS)))

# Ejecutables
TARGET := $(BINDIR)/ipv4-ssl-cli.out

# Regla por defecto
all: $(TARGET)

# Compilar y enlazar ejecutable
$(TARGET): $(OBJS) | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

# Compilar cada archivo fuente en obj/
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Crear carpeta obj si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Crear carpeta exec si no existe
$(BINDIR):
	mkdir -p $(BINDIR)

# Ejecutar el programa
run: all
	./$(TARGET)

# Limpiar
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out