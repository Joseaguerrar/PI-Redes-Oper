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

# Ejecutables
MAIN_EXE := $(BINDIR)/ipv4-ssl-cli.out
SSL_CLI  := $(BINDIR)/ssl-cli.out

# Regla por defecto
all: $(MAIN_EXE) $(SSL_CLI)

# Ejecutable cliente SSL con clases (orientado a objetos)
$(MAIN_EXE): $(OBJS) | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

# Ejecutable cliente SSL procedimental
$(SSL_CLI): ssl-cli.c | $(BINDIR)
	$(CC) ssl-cli.c -o $@ $(LDFLAGS)

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

# Ejecutar programa orientado a objetos con SSL
run: $(MAIN_EXE)
	./$(MAIN_EXE) ssl

# Ejecutar cliente con clases (sin SSL, TCP simple)
run-no-ssl: $(MAIN_EXE)
	./$(MAIN_EXE)
	
# Ejecutar cliente SSL procedimental sin argumentos
ssl-run: $(SSL_CLI)
	./$(SSL_CLI)

# Ejecutar cliente SSL procedimental con host y puerto como argumentos
ssl-run-args: $(SSL_CLI)
	./$(SSL_CLI) redes.ecci.ucr.ac.cr https

	
# Limpiar archivos generados
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out
