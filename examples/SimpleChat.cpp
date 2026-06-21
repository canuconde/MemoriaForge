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

#include "MemoriaForge.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: "<< argv[0] << " <model.gguf> " << std::endl;
        return 1;
    }
    const std::string model_path = argv[1];

    // Load a GGUF model and create a new session.
    MemoriaForge::LLMSession llm(model_path);

    // Send a message to the model and print the response.
    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) break;

        llm.chat(input);
    }

    return 0;
}
