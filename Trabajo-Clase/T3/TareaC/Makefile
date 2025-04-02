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
SRCS := VSocket.cpp Socket.cpp SSLSocket.cpp tcp-cli.cc ipv6-ssl-server.cc
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(OBJS)))

# Ejecutables
SSL_CLI_EXE := $(BINDIR)/ipv6-ssl-cli.out
SSL_SERVER_EXE := $(BINDIR)/ipv6-ssl-server.out

# Regla por defecto
all: $(SSL_CLI_EXE) $(SSL_SERVER_EXE)

# Ejecutable cliente TCP/SSL IPv6
$(SSL_CLI_EXE): $(OBJDIR)/tcp-cli.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o $(OBJDIR)/SSLSocket.o | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

# Ejecutable servidor TCP/SSL IPv6
$(SSL_SERVER_EXE): $(OBJDIR)/ipv6-ssl-server.o $(OBJDIR)/VSocket.o $(OBJDIR)/Socket.o $(OBJDIR)/SSLSocket.o | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

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

# Ejecutar programas
run-client: $(SSL_CLI_EXE)
	./$(SSL_CLI_EXE)

run-server: $(SSL_SERVER_EXE)
	./$(SSL_SERVER_EXE)

# Limpiar archivos generados
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out
