#include "filesystem.hpp"
#include <iostream>
#include <string>

int main() {
    FileSystem fs;

    if (!fs.cargarFiguras("filesystem.dat")) {
        std::cerr << "Error al cargar el sistema de archivos.\n";
        return 1;
    }

    fs.imprimirTabla();

    std::string nombre;
    std::cout << "\nIngrese el nombre de la figura a mostrar (sin .txt): ";
    std::getline(std::cin, nombre);

    // Quitar ".txt" si lo incluye
    if (nombre.size() > 4 && nombre.substr(nombre.size() - 4) == ".txt")
        nombre = nombre.substr(0, nombre.size() - 4);

    std::string contenido = fs.obtenerContenido(nombre);
    std::cout << "\nContenido de '" << nombre << "':\n";
    std::cout << "--------------------------------\n";
    std::cout << contenido << "\n";
    std::cout << "--------------------------------\n";

    // Opcional: mostrar los bloques
    //fs.imprimirBloquesFiguras(nombre);

    return 0;
}
