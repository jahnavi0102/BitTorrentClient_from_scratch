#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <bencoding.hpp>



// class ReadFile {
//     std::vector<uint8_t> getBytesFromFile(const std::string file_path) {
//         std::ifstream file(file_path, std::ios::binary, std::ios::ate);
//         if (!file.is_open()) {
//             throw std::runtime_error("Could not open file: " + file_path);
//         }
//         std::streamsize size = file.tellg();
//         file.seekg(0, std::ios::beg);

//         std::vector<char> buffer(size);
//         if (!file.read(buffer.data(), size)) {
//             throw std::runtime_error("Error while reading file");
//         }

//         for (size_t i = 0; i < buffer.size(); ++i) {
//             if (buffer[i] < 0) {
//                 throw std::runtime_error("File contains non-ASCII characters");
//             }
//         }
//         return std::vector<uint8_t>(buffer.begin(), buffer.end());
//     }
// };

int main() {
    try {
        std::cout << "Libtorrent functionality would go here." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
