#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>
#include <cstring>
#include <algorithm>

namespace fs = std::filesystem;

const int BLOCK_SIZE = 256;
const int ROOT_MAX_ENTRIES = 10;
const int MAX_BLOCKS = 1000;
const int DATA_SIZE = 254; // contenido real por bloque

struct EntradaRoot {
    char nombre[20];     // nombre del archivo
    uint16_t id_inicio;  // primer bloque
    uint16_t id_fin;     // último bloque
};

// ===================== UTF-8 SAFE CHUNK SPLIT =====================

bool is_utf8_lead_byte(unsigned char c) {
    // Verdadero si el byte no es un byte de continuación
    return (c >> 6) != 0b10;
}

std::vector<std::string> split_utf8_chunks(const std::string& input, size_t max_bytes) {
    std::vector<std::string> chunks;
    size_t i = 0;

    while (i < input.size()) {
        size_t start = i;
        size_t current_bytes = 0;

        while (i < input.size() && current_bytes < max_bytes) {
            size_t char_len = 1;

            unsigned char c = static_cast<unsigned char>(input[i]);
            if (c < 0x80) {
                char_len = 1;
            } else if ((c >> 5) == 0b110) {
                char_len = 2;
            } else if ((c >> 4) == 0b1110) {
                char_len = 3;
            } else if ((c >> 3) == 0b11110) {
                char_len = 4;
            }

            if (current_bytes + char_len > max_bytes)
                break;

            i += char_len;
            current_bytes += char_len;
        }

        chunks.push_back(input.substr(start, i - start));
    }

    return chunks;
}

// ========================= BUILDER MAIN ==========================

int main() {
    std::string carpeta = "figuras";
    std::string salida = "filesystem.dat";

    std::vector<EntradaRoot> root;
    std::vector<std::vector<char>> bloques;

    int bloque_actual = 1; // bloque 0 será para el Root

    for (const auto& entry : fs::directory_iterator(carpeta)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt")
            continue;

        if (root.size() >= ROOT_MAX_ENTRIES) {
            std::cerr << "Límite de figuras alcanzado (" << ROOT_MAX_ENTRIES << ").\n";
            break;
        }

        std::ifstream figura(entry.path());
        if (!figura) {
            std::cerr << "Error al abrir: " << entry.path() << "\n";
            continue;
        }

        std::stringstream buffer;
        buffer << figura.rdbuf();
        std::string contenido = buffer.str();

        EntradaRoot entrada;
        std::string nombre = entry.path().stem().string(); // quitar extensión .txt
        nombre = nombre.substr(0, 19);
        std::strncpy(entrada.nombre, nombre.c_str(), 20);

        entrada.id_inicio = bloque_actual;

        std::vector<int> bloques_ids;

        // Divide en fragmentos respetando UTF-8
        std::vector<std::string> partes = split_utf8_chunks(contenido, DATA_SIZE);

        for (const auto& parte : partes) {
            std::vector<char> bloque(BLOCK_SIZE, 0);
            std::memcpy(&bloque[0], parte.c_str(), parte.size());
            bloques_ids.push_back(bloque_actual++);
            bloques.push_back(bloque);
        }

        // Escribir punteros al final de cada bloque
        for (size_t i = 0; i < bloques_ids.size(); ++i) {
            uint16_t next = (i + 1 < bloques_ids.size()) ? bloques_ids[i + 1] : 0;
            std::memcpy(&bloques[bloques_ids[i] - 1][DATA_SIZE], &next, sizeof(uint16_t));
        }

        entrada.id_fin = bloques_ids.back();
        root.push_back(entrada);

        std::cout << "Figura '" << nombre << "' → bloques [" << entrada.id_inicio << " - " << entrada.id_fin << "]\n";
    }

    // Crear archivo .dat
    std::ofstream out(salida, std::ios::binary);
    if (!out) {
        std::cerr << "No se pudo crear " << salida << "\n";
        return 1;
    }

    // Escribir bloque 0 (Root)
    for (const auto& r : root) {
        out.write(r.nombre, 20);
        out.write(reinterpret_cast<const char*>(&r.id_inicio), sizeof(uint16_t));
        out.write(reinterpret_cast<const char*>(&r.id_fin), sizeof(uint16_t));
    }

    // Rellenar el bloque root si queda espacio
    int root_bytes = root.size() * (20 + 2 + 2);
    int padding = BLOCK_SIZE - root_bytes;
    std::vector<char> relleno(padding, 0);
    out.write(relleno.data(), padding);

    // Escribir los bloques de contenido
    for (const auto& bloque : bloques) {
        out.write(bloque.data(), BLOCK_SIZE);
    }

    out.close();
    std::cout << "filesystem.dat generado correctamente con " << bloques.size() << " bloques de contenido.\n";
    return 0;
}
