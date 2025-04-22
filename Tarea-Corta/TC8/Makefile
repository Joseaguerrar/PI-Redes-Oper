CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c -std=c++11
LDFLAGS := 

DEPS := Socket.h
TARGETS := ForkMirrorServer.out MirrorClient.out ThreadMirrorServer.out

# Carpetas
OBJDIR := obj
EXECDIR := exec

# Asegurar que las carpetas existan
$(shell mkdir -p $(OBJDIR) $(EXECDIR))

all: $(TARGETS)

# Regla general para objetos
$(OBJDIR)/%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

# Regla general para ejecutables
$(EXECDIR)/%.out: $(OBJDIR)/%.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o
	$(CXX) $^ $(LDFLAGS) -o $@

# Reglas específicas para cada ejecutable
ForkMirrorServer.out: $(EXECDIR)/ForkMirrorServer.out

ThreadMirrorServer.out: $(EXECDIR)/ThreadMirrorServer.out

MirrorClient.out: $(EXECDIR)/MirrorClient.out

# Limpieza
clean:
	rm -rf $(OBJDIR)/*.o $(EXECDIR)/*.out
