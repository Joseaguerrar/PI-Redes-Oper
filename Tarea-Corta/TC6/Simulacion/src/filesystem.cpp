#include <fstream>
#include <iostream>
#include <cstring>

#include "filesystem.hpp"

FileSystem::FileSystem() {
    std::memset(storage, 0, sizeof(storage));
}

bool FileSystem::cargarFiguras(const std::string& ruta) {
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo: " << ruta << std::endl;
        return false;
    }

    // Leer el bloque 0 (Root)
    for (int i = 0; i < ROOT_MAX_ENTRIES; ++i) {
        Archivo a;
        char nombre_raw[20];
        archivo.read(nombre_raw, 20);
        if (archivo.gcount() < 20 || nombre_raw[0] == '\0') break;

        std::memcpy(a.nombre, nombre_raw, 20);
        a.nombre[20] = '\0';

        archivo.read(reinterpret_cast<char*>(&a.id_inicio), sizeof(uint16_t));
        archivo.read(reinterpret_cast<char*>(&a.id_fin), sizeof(uint16_t));

        archivos.push_back(a);
    }

    // Leer el resto del archivo en bloques de 256 bytes
    int index = 1; // Bloque 0 ya fue
    while (archivo.read(storage[index], BLOCK_SIZE)) {
        index++;
        if (index >= MAX_BLOCKS) break;
    }

    return true;
}

std::string FileSystem::obtenerContenido(const std::string& nombre) {
    for (const auto& a : archivos) {
        if (nombre == a.nombre) {
            std::string resultado;
            uint16_t bloque_actual = a.id_inicio;

            while (bloque_actual != 0 && bloque_actual < MAX_BLOCKS) {
                char* bloque = storage[bloque_actual];

                // Leer 254 bytes de contenido
                resultado.append(bloque, 254);

                // Leer siguiente bloque (últimos 2 bytes del bloque actual)
                std::memcpy(&bloque_actual, bloque + 254, sizeof(uint16_t));
            }

            return resultado;
        }
    }

    return "[Figura no encontrada]";
}

void FileSystem::imprimirTabla() {
    std::cout << "== Tabla del Root (figuras registradas) ==\n";
    for (const auto& a : archivos) {
        std::cout << "Figura: " << a.nombre
                  << " | Inicio: " << a.id_inicio
                  << " | Fin: " << a.id_fin << "\n";
    }
}

void FileSystem::imprimirBloquesFiguras(const std::string& nombre) {
    for (const auto& a : archivos) {
        if (nombre == a.nombre) {
            std::cout << "== Bloques de la Figura: " << a.nombre << " ==\n";
            uint16_t bloque_actual = a.id_inicio;
            int contador = 0;

            while (bloque_actual != 0 && bloque_actual < MAX_BLOCKS) {
                char* bloque = storage[bloque_actual];

                std::cout << "Bloque # " << bloque_actual << ":\n";

                // Mostrar los primeros 20 bytes como texto legible
                std::cout << "Texto crudo: ";
                for (int i = 0; i < 20; ++i) {
                    unsigned char c = static_cast<unsigned char>(bloque[i]);
                    if (std::isprint(c))
                        std::cout << c;
                    else if (c == '\n')
                        std::cout << "\\n";
                    else if (c == '\t')
                        std::cout << "\\t";
                    else
                        std::cout << '_';  // No imprimible
                }
                std::cout << "\n";

                // Mostrar los mismos 20 bytes en hexadecimal
                std::cout << "Hex: ";
                for (int i = 0; i < 20; ++i) {
                    printf("%02X ", static_cast<unsigned char>(bloque[i]));
                }
                std::cout << "\n";

                // Mostrar los punteros al final del bloque
                uint8_t lo = static_cast<uint8_t>(bloque[254]);
                uint8_t hi = static_cast<uint8_t>(bloque[255]);
                uint16_t next = lo | (hi << 8);
                printf("Bytes crudos: %02X %02X\n", lo, hi);
                std::cout << " siguiente bloque: " << next << "\n\n";

                bloque_actual = next;

                if (contador++ > 50) break; // Seguridad ante bucles infinitos
            }
            return;
        }
    }

    std::cout << "[Figura no encontrada]: " << nombre << "\n";
}

