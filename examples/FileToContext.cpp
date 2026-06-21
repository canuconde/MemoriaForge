//                     MemoriaForge
//     Copyright (C) 2024 Sebastián 'Canu' Bergara Conde <canu.conde@gmail.com>
//
// This file is part of MemoriaForge.
//
// MemoriaForge is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// MemoriaForge is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with MemoriaForge.
// If not, see <https://www.gnu.org/licenses/>.

#include <iostream>
#include <fstream>
#include <sstream>
#include "MemoriaForge.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: "<< argv[0] << " <model.gguf> <file_name>" << std::endl;
        return 1;
    }

    // Obtener parámetros de línea de comandos
    const std::string model_path = argv[1];
    const std::string file_name  = argv[2];

    // Abrir archivo de entrada
    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << file_name << std::endl;
        return 1;
    }

    // Leer el archivo completo preservando
    // espacios, tabs y saltos de línea
    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string content = buffer.str();

    // Mostrar información básica
    std::cout << "Loaded file: "<< file_name<< std::endl;

    std::cout << "Size: "<< content.size()<< " bytes"<< std::endl;

    // Crear una sesión LLM
    //
    // Parámetros:
    // - Ruta del modelo
    // - Tamaño de contexto
    // - Seed para generación reproducible
    MemoriaForge::LLMSession llm(model_path,8192,99);

    // Inyectar el contenido del archivo
    // dentro del contexto del modelo
    llm.context_injector(content);

    // Guardar el estado completo de la sesión
    //
    // state.bin:
    //     Estado interno del contexto LLM
    //
    // messages_meta.txt:
    //     Metadatos de mensajes almacenados
    llm.save_state("state.bin");

    std::cout << "Context saved successfully."<< std::endl;

    return 0;

}
