# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./src
OBJDIR = obj
BINDIR = exec
SRCDIR = src
FIGDIR = figuras

# Archivos fuente
BUILDER_SRC = $(SRCDIR)/builder.cpp
MAIN_SRC = $(SRCDIR)/main.cpp $(SRCDIR)/filesystem.cpp
HEADERS = $(SRCDIR)/filesystem.hpp

# Objetos
BUILDER_OBJ = $(OBJDIR)/builder.o
MAIN_OBJ = $(OBJDIR)/main.o $(OBJDIR)/filesystem.o

# Ejecutables
BUILDER_EXE = $(BINDIR)/builder
MAIN_EXE = $(BINDIR)/simulador

# Archivo generado
DAT_FILE = filesystem.dat

# Objetivo por defecto: compila builder, ejecuta builder y compila simulador
all: $(DAT_FILE) $(MAIN_EXE)

# Crear carpetas necesarias
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Compilar builder
$(BUILDER_EXE): $(BUILDER_OBJ) | $(OBJDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/builder.o: $(BUILDER_SRC) $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar builder para generar filesystem.dat
$(DAT_FILE): $(BUILDER_EXE) $(wildcard $(FIGDIR)/*.txt)
	./$(BUILDER_EXE)

# Compilar simulador
$(MAIN_EXE): $(MAIN_OBJ) | $(OBJDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/filesystem.o: $(SRCDIR)/filesystem.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar el simulador
run: $(MAIN_EXE)
	./$(MAIN_EXE)

# Ejecutar el builder con salida
run-builder: $(BUILDER_EXE)
	./$(BUILDER_EXE)

# Limpiar todo
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DAT_FILE)

.PHONY: all clean run run builder
