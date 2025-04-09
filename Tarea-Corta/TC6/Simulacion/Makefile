# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./src
OBJDIR = obj
BINDIR = exec
SRCDIR = src

# Archivos fuente
GEN_SRC = crear_filesystem.cpp
SIM_SRC = $(SRCDIR)/main.cpp $(SRCDIR)/filesystem.cpp
HEADERS = $(SRCDIR)/filesystem.hpp

# Objetos
SIM_OBJS = $(OBJDIR)/main.o $(OBJDIR)/filesystem.o

# Ejecutables
GEN_EXE = $(BINDIR)/crear_filesystem
SIM_EXE = $(BINDIR)/simulador

# Archivo de datos
DAT_FILE = filesystem.dat

# Objetivo por defecto
all: $(GEN_EXE) $(DAT_FILE) $(SIM_EXE)

# Crear carpetas necesarias
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Ejecutable del generador
$(GEN_EXE): $(GEN_SRC) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Generar filesystem.dat
$(DAT_FILE): $(GEN_EXE)
	./$(GEN_EXE)

# Ejecutable del simulador
$(SIM_EXE): $(SIM_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/filesystem.o: $(SRCDIR)/filesystem.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar el simulador
run: $(SIM_EXE)
	./$(SIM_EXE)

# Ejecutar solo el generador
gen: $(GEN_EXE)
	./$(GEN_EXE)

# Limpiar todo
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DAT_FILE)

.PHONY: all run gen clean
