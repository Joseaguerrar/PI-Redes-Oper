#pragma once
#include <vector>
#include <string>

const int BLOCK_SIZE = 256;
const int ROOT_MAX_ENTRIES = 10;  // Máximo de figuras indexadas
const int MAX_BLOCKS = 100;

struct Archivo {
    char nombre[21];  // nombre de hasta 20 caracteres
    int id_inicio;
    int id_fin;
};

class FileSystem {
private:
    char storage[MAX_BLOCKS][BLOCK_SIZE];      // almacenamiento simulado
    std::vector<Archivo> archivos;             // Estructura en memoria del root
    int siguiente_bloque_libre = 1;            // Bloque 0 reservado para el root

public:
    FileSystem();

    // Carga desde filesystem.dat
    void cargarFiguras(const std::string& ruta);

    //Muestra el contenido del root
    void imprimirTabla();

    // Devuelve el contenido completo de una figura por nombre
    std::string obtenerContenido(const std::string& nombre);
};
