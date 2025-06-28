#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <bencoding.hpp>
#include <torrentfile.cpp>

namespace bc = bencode;

class ReadFile {
    public:
        std::vector<uint8_t> getBytesFromFile(const std::string file_path) {
            std::ifstream file(file_path, std::ios::binary | std::ios::ate);
            std::cout << "Reading file: " << file_path << std::endl;
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file: " + file_path);
            }
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<char> buffer(size);
            if (!file.read(buffer.data(), size)) {
                throw std::runtime_error("Error while reading file");
            }
            file.close();
            std::cout << "File read successfully, size: " << size << " bytes" << std::endl;
            return std::vector<uint8_t>(buffer.begin(), buffer.end());
        }
};

// void parse_buffer(const bencode::data& data) {
//     auto variant = data.base();  // Get the std::variant inside

//     if (std::holds_alternative<bencode::integer>(variant)) {
//         std::cout << std::get<bencode::integer>(variant) << " ";
//     } else if (std::holds_alternative<bencode::string>(variant)) {
//         const auto& str = std::get<bencode::string>(variant);
//         bool printable = true;
//         for (char c : str) {
//             if (c < 32 || c > 126) {
//                 printable = false;
//                 break;
//             }
//         }
//         if (printable) {
//             std::cout << "\"" << str << "\" ";
//         } else {
//             std::cout << "<binary string of length " << str.size() << "> ";
//         }
//     } else if (std::holds_alternative<bencode::list>(variant)) {
//         std::cout << "[ ";
//         const auto& list = std::get<bencode::list>(variant);
//         for (const auto& item : list) {
//             parse_buffer(item);  // recursive call for each item
//         }
//         std::cout << "] ";
//     } else if (std::holds_alternative<bencode::dict>(variant)) {
//         std::cout << "{ ";
//         const auto& dict = std::get<bencode::dict>(variant);
//         for (const auto& [key, value] : dict) {
//             std::cout << key << ": ";
//             parse_buffer(value);  // recursive call for each value
//         }
//         std::cout << "} ";
//     }
// }

int main() {
    try {
        std::cout << "Enter the file path: ";
        std::string file_path;
        std::getline(std::cin, file_path);
        ReadFile reader = ReadFile();
        if (file_path.empty()) {
            throw std::runtime_error("File path cannot be empty");
        }
        std::cout << "Reading file: " << file_path << std::endl;

        auto bytes = reader.getBytesFromFile(file_path);

        std::cout << "File read successfully, size: " << bytes.size() << " bytes" << std::endl;
        bencode::data data = bencode::decode(bytes);

        auto variant = data.base();

        // Step 2: Cast it to bencode::dict
        if (!std::holds_alternative<bencode::dict>(variant)) {
            throw std::runtime_error("Top-level bencode structure is not a dictionary.");
        }

        bencode::dict top_dict = std::get<bencode::dict>(variant);

        bencode::dict info_dict = std::get<bencode::dict>(top_dict["info"]);

        std::string name = std::get<bencode::string>(info_dict["name"]);
        int piece_length = std::get<bencode::integer>(info_dict["piece length"]);
        int length = std::get<bencode::integer>(info_dict["length"]);
        std::string pieces = std::get<bencode::string>(info_dict["pieces"]);

        std::string announce = std::get<bencode::string>(top_dict["announce"]);
        std::string comment = std::get<bencode::string>(top_dict["comment"]);
        std::string created_by = std::get<bencode::string>(top_dict["created by"]);
        std::string creation_date = std::to_string(std::get<bencode::integer>(top_dict["creation date"]));  // converting to string
        std::string encoding = std::get<bencode::string>(top_dict["encoding"]);

        BencodedInfo info(name, piece_length, pieces, length);
        BencodedFile torrent_file(announce, comment, created_by, creation_date, encoding, info);

        // stroring bencoded info SSH-1 info exactly as it was stored in .torrent file
        std::cout << "Bencoded Info:\n";

        std::string bencoded_info = bencode::encode(info_dict);

        std::array<uint8_t, 20> sh1_info_hash = compute_info_hash(bencoded_info);
        


        // parse_buffer(data);
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
