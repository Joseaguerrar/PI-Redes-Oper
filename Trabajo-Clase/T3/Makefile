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
SRCS := ipv4-ssl-cli.cc VSocket.cpp Socket.cpp SSLSocket.cpp tcp-cli.cc ipv6-udp-server.cc
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(OBJS)))

# Ejecutables
MAIN_EXE := $(BINDIR)/ipv4-ssl-cli.out
UDP_EXE := $(BINDIR)/tcp-cli.out
UDP_SERVER_EXE := $(BINDIR)/ipv6-udp-server.out

# Regla por defecto
all: $(MAIN_EXE) $(UDP_EXE) $(UDP_SERVER_EXE)

# Ejecutable TCP/SSL
$(MAIN_EXE): $(OBJDIR)/ipv4-ssl-cli.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o $(OBJDIR)/SSLSocket.o | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

# Ejecutable cliente UDP IPv6
$(UDP_EXE): $(OBJDIR)/tcp-cli.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o | $(BINDIR)
	$(CXX) $^ -o $@

# Ejecutable servidor UDP IPv6
$(UDP_SERVER_EXE): $(OBJDIR)/ipv6-udp-server.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o | $(BINDIR)
	$(CXX) $^ -o $@

# Compilar cada archivo fuente a obj/
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Crear carpetas si no existen
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Ejecutar programa TCP/SSL
run: $(MAIN_EXE)
	./$(MAIN_EXE)

# Ejecutar cliente UDP IPv6
udp-run: $(UDP_EXE)
	./$(UDP_EXE)

# Ejecutar servidor UDP IPv6
udp-server: $(UDP_SERVER_EXE)

run-server: udp-server
	./$(UDP_SERVER_EXE)

# Limpiar archivos generados
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out