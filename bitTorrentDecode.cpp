#include <iostream>
#include <vector>
#include <string>


namespace BitTorrent {
    class  BEncoding {
        private:
            uint8_t const dictionary_start = static_cast<uint8_t>('d');
            uint8_t const dictionary_end = static_cast<uint8_t>('e');
            uint8_t const list_start = static_cast<uint8_t>('l');
            uint8_t const list_end = static_cast<uint8_t>('e');
            uint8_t const number_tart = static_cast<uint8_t>('i');
            uint8_t const number_end = static_cast<uint8_t>('e');
            uint8_t const byte_array_divider = static_cast<uint8_t>(':');

    };
}

// class ReadFile {
//     std::vector<uint8_t> getBytesFromFile(const std::string file_path) {
//         // Open the file in binary mode
//         file_.open(file_path, std::ios::binary);
//         if (!file_.is_open()) {
//             throw std::runtime_error("Failed to open file: " + file_path_);
//         }
//     }
// }

