#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

const int BLOCK_SIZE = 256;
const int DATA_SIZE = 254;
const int ROOT_MAX_ENTRIES = 10;
const int MAX_BLOCKS = 100;

struct EntradaRoot {
    char nombre[20];
    int id_inicio;
    int id_fin;
};

struct Bloque {
    char data[BLOCK_SIZE]{};
    bool ocupado = false;
    std::string figura;
};

std::vector<std::string> split_chunks(const std::string& contenido, size_t chunk_size) {
    std::vector<std::string> chunks;
    size_t i = 0;
    while (i < contenido.size()) {
        chunks.push_back(contenido.substr(i, chunk_size));
        i += chunk_size;
    }
    return chunks;
}

int buscarBloqueLibre(std::vector<Bloque>& bloques) {
    for (int i = 1; i < MAX_BLOCKS; ++i) { // bloque 0 reservado
        if (!bloques[i].ocupado) return i;
    }
    throw std::runtime_error("No hay bloques libres disponibles");
}

int main() {
    std::string salida = "filesystem.dat";
    std::ofstream out(salida, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "No se pudo crear " << salida << "\n";
        return 1;
    }

    std::vector<EntradaRoot> root;
    std::vector<Bloque> bloques(MAX_BLOCKS);

    std::vector<std::pair<std::string, std::string>> figuras = {
        {
            "circulo",
            "   ___\n"
            "  /   \\\n"
            " |     |\n"
            " |     |\n"
            "  \\___/\n"
        },
        {
            "cuadro",
            "+-------+\n"
            "|       |\n"
            "|       |\n"
            "|       |\n"
            "+-------+\n"
        },
        {
            "estrella",
            "   *\n"
            "  ***\n"
            " *****\n"
            "  ***\n"
            "   *\n"
        },
        {
            "triangulo",
            "   /\\\n"
            "  /  \\\n"
            " /____\\\n"
        }
    };

    for (const auto& [nombre_raw, contenido] : figuras) {
        EntradaRoot entrada{};
        std::string nombre = nombre_raw.substr(0, 19);
        std::strncpy(entrada.nombre, nombre.c_str(), 20);

        std::vector<int> bloques_ids;
        std::vector<std::string> partes = split_chunks(contenido, DATA_SIZE);

        // Reservar bloques
        for (const auto& parte : partes) {
            int id = buscarBloqueLibre(bloques);
            bloques[id].ocupado = true;
            bloques[id].figura = nombre;
            std::memset(bloques[id].data, 0, BLOCK_SIZE);
            std::memcpy(bloques[id].data, parte.c_str(), parte.size());
            bloques_ids.push_back(id);
        }

        // Enlazar bloques manualmente con uint8_t
        for (size_t i = 0; i < bloques_ids.size(); ++i) {
            int current = bloques_ids[i];
            int next = (i + 1 < bloques_ids.size()) ? bloques_ids[i + 1] : 0;

            bloques[current].data[254] = static_cast<char>(next & 0xFF);       // lo
            bloques[current].data[255] = static_cast<char>((next >> 8) & 0xFF); // hi

            printf("[enlace] bloque #%d → siguiente: %d (bytes: %02X %02X)\n",
                   current, next,
                   static_cast<unsigned char>(bloques[current].data[254]),
                   static_cast<unsigned char>(bloques[current].data[255]));
        }

        entrada.id_inicio = bloques_ids.front();
        entrada.id_fin = bloques_ids.back();
        root.push_back(entrada);

        std::cout << "Figura '" << nombre << "' escrita en bloques ["
                  << entrada.id_inicio << " - " << entrada.id_fin << "]\n";
    }

    // Escribir bloque 0 (root)
    out.seekp(0, std::ios::beg);
    for (const auto& r : root) {
        out.write(r.nombre, 20);

        uint8_t ini_lo = r.id_inicio & 0xFF;
        uint8_t ini_hi = (r.id_inicio >> 8) & 0xFF;
        uint8_t fin_lo = r.id_fin & 0xFF;
        uint8_t fin_hi = (r.id_fin >> 8) & 0xFF;

        out.put(ini_lo);
        out.put(ini_hi);
        out.put(fin_lo);
        out.put(fin_hi);
    }

    int root_bytes = root.size() * (20 + 4);
    int padding = BLOCK_SIZE - root_bytes;
    out.write(std::string(padding, '\0').data(), padding);

    // Escribir los bloques
    for (int i = 1; i < MAX_BLOCKS; ++i) {
        if (bloques[i].ocupado) {
            out.seekp(i * BLOCK_SIZE, std::ios::beg);
            out.write(bloques[i].data, BLOCK_SIZE);
        }
    }

    out.close();
    std::cout << "Archivo 'filesystem.dat' creado correctamente.\n";
    return 0;
}
