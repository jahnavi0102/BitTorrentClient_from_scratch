#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>  // For std::hex

std::vector<char> readFileToVector(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Error while reading file");
    }
    return buffer;
}

int main() {
    try {
        std::vector<char> fileContent = readFileToVector("C:\\Users\\jjain\\Downloads\\debian-edu-12.10.0-amd64-netinst.iso.torrent");
        
        // Print first 100 bytes in hex to verify it's being read correctly
        std::cout << "First 100 bytes (hex):\n";
        for (int i = 0; i < 100 && i < fileContent.size(); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << (static_cast<int>(fileContent[i]) & 0xff) << " ";
            if ((i + 1) % 16 == 0) std::cout << "\n";
        }
        std::cout << "\n";
        
    } catch (const std::runtime_error& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }
    return 0;
}