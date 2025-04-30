.PHONY: all clean

# Directorios
OBJDIR := obj
BINDIR := exec

# Compiladores y banderas
CC    := gcc
CXX   := g++
CFLAGS  := -g -c
CXXFLAGS:= -g -c
LDFLAGS := -lssl -lcrypto

# Crear directorios si no existen
$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(BINDIR))

# Archivos fuente
SRC_CXX := VSocket.cc Socket.cc SSLSocket.cc SSLServer.cc SSLClient.cc MirrorClient.cc
OBJS := $(OBJDIR)/VSocket.o \
        $(OBJDIR)/Socket.o  \
        $(OBJDIR)/SSLSocket.o

# Ejecutables
EXECUTABLES := $(BINDIR)/SSLServer.out \
               $(BINDIR)/SSLClient.out \

# Objetivo por defecto
all: $(EXECUTABLES)

# Reglas de compilación para objetos
$(OBJDIR)/VSocket.o: VSocket.cc VSocket.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJDIR)/Socket.o: Socket.cc Socket.h VSocket.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJDIR)/SSLSocket.o: SSLSocket.cc SSLSocket.h Socket.h VSocket.h
	$(CXX) $(CXXFLAGS) -o $@ $<

# Reglas para generar los ejecutables
$(BINDIR)/SSLServer.out: SSLServer.cc $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/SSLClient.out: SSLClient.cc $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Limpiar archivos generados
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out
