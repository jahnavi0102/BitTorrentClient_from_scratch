#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <bencoding.hpp>
#include <iomanip>
#include <openssl/sha.h>
#include <array>


namespace bc = bencode;
// CURL *curl;


struct BencodedInfo {
    std::string name;
    int piece_length;
    int length;
    std::string pieces;

    BencodedInfo(const std::string& name, int piece_length,
                const std::string& pieces, int length = 0)
        : name(name), piece_length(piece_length), pieces(pieces), length(length) {}
    BencodedInfo() : name(""), piece_length(0), length(0), pieces("") {}
    void setName(const std::string& name) {
        this->name = name;
    };
    void setPieceLength(int piece_length) {
        this->piece_length = piece_length;
    };
    void setLength(int length) {
        this->length = length;
    };
    void setPieces(const std::string& pieces) {
        this->pieces = pieces;
    };
    void print() const {
        std::cout << "Name: " << name << "\n"
                  << "Piece Length: " << piece_length << "\n"
                  << "Length: " << length << "\n"
                  << "Pieces: " << pieces << "\n";
    };
    bool operator==(const BencodedInfo& other) const {
        return name == other.name &&
               piece_length == other.piece_length &&
               length == other.length &&
               pieces == other.pieces;
    };
};


struct BencodedFile {
    std::string announce;
    std::string comment;
    std::string created_by;
    std::string creation_date;
    std::string encoding;
    BencodedInfo info_hash;

    BencodedFile(const std::string& announce, const std::string& comment,
                 const std::string& created_by, const std::string& creation_date,
                 const std::string& encoding, const BencodedInfo& info_hash)
        : announce(announce), comment(comment), created_by(created_by),
          creation_date(creation_date), encoding(encoding), info_hash(info_hash) {}
    BencodedFile() : announce(""), comment(""), created_by(""),
                     creation_date(""), encoding(""),
                     info_hash("", 0, "") {}
    void setAnnounce(const std::string& announce) {
        this->announce = announce;
    };  
    void setComment(const std::string& comment) {
        this->comment = comment;
    };
    void setCreatedBy(const std::string& created_by) {
        this->created_by = created_by;
    };
    void setCreationDate(const std::string& creation_date) {
        this->creation_date = creation_date;
    };
    void setEncoding(const std::string& encoding) {
        this->encoding = encoding;
    };
    void setInfoHash(const BencodedInfo& info_hash) {
        this->info_hash = info_hash;
    };
    void print() const {
        std::cout << "Announce: " << announce << "\n"
                  << "Comment: " << comment << "\n"
                  << "Created by: " << created_by << "\n"
                  << "Creation date: " << creation_date << "\n"
                  << "Encoding: " << encoding << "\n"
                  << "Info Hash:\n"
                  << "  Name: " << info_hash.name << "\n"
                  << "  Piece Length: " << info_hash.piece_length << "\n"
                  << "  Pieces: " << info_hash.pieces << "\n";
    };
    bool operator==(const BencodedFile& other) const {
        return announce == other.announce &&
               comment == other.comment &&
               created_by == other.created_by &&
               creation_date == other.creation_date &&
               encoding == other.encoding &&
               info_hash == other.info_hash;
    };

};

std::array<uint8_t, 20> compute_info_hash(const bencode::dict& info) {
    // Bencode the info dictionary and hash the resulting bytes
    std::array<uint8_t, 20> hash = {};
    std::string bencoded_info_str = bencode::encode(info);
    std::vector<uint8_t> bencoded_info(bencoded_info_str.begin(), bencoded_info_str.end());
    SHA1(bencoded_info.data(), bencoded_info.size(), hash.data());
    return hash;
}

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

std::string sendHttpRequest(const std::string& url){

    // TODO: Implement HTTP request logic and return response as std::string
    return "";
}

class Tracker {
public:
    Tracker(const std::string& announce_url)
        : announce_url_(announce_url) {}

    // Example: send a simple GET request to the tracker (HTTP only)
    void announce(const std::string& info_hash, const std::string& peer_id, int port, int uploaded, int downloaded, int left) {
        // Build the announce URL with query parameters
        std::ostringstream url;
        url << announce_url_
            << "?info_hash=" << url_encode(info_hash)
            << "&peer_id=" << url_encode(peer_id)
            << "&port=" << port
            << "&uploaded=" << uploaded
            << "&downloaded=" << downloaded
            << "&left=" << left
            << "&compact=1";
        std::cout << "Tracker announce URL: " << url.str() << std::endl;

        // TODO: Use an HTTP library to send the GET request
        // This part requires adding an external library and its setup.
        // For demonstration, let's assume a hypothetical sendHttpRequest function
        // that returns the response body as a std::string.

        try {
            std::string response_body = sendHttpRequest(url.str());
            std::cout << "Tracker Response: " << response_body << std::endl;

            // Step 2: Parse the bencoded response
            // parseTrackerResponse(response_body);

        } catch (const std::exception& e) {
            std::cerr << "HTTP request failed: " << e.what() << std::endl;
        }
        
    }

    private:
    std::string announce_url_;

    // Simple URL encoding function for info_hash and peer_id
    std::string url_encode(const std::string& value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;
        for (unsigned char c : value) {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
            } else {
                escaped << '%' << std::setw(2) << int(c);
            }
        }
        return escaped.str();
    }
};


int main() {
    try {
        std::cout << "Enter the file path: ";
        std::string file_path;
        std::getline(std::cin, file_path);
        ReadFile reader;
        if (file_path.empty()) {
            throw std::runtime_error("File path cannot be empty");
        }
        auto bytes = reader.getBytesFromFile(file_path);
        bencode::data data = bencode::decode(bytes);
        auto variant = data.base();

        if (!std::holds_alternative<bencode::dict>(variant)) {
            throw std::runtime_error("Top-level bencode structure is not a dictionary.");
        }

        bencode::dict top_dict = std::get<bencode::dict>(variant);

        // Get "info" dictionary
        auto info_it = top_dict.find(bencode::string("info"));
        if (info_it == top_dict.end() || !std::holds_alternative<bencode::dict>(info_it->second)) {
            throw std::runtime_error("'info' key not found or not a dictionary");
        }
        bencode::dict info_dict = std::get<bencode::dict>(info_it->second);

        
        // Extract fields from "info" dictionary
        // std::string name = std::get<bencode::string>(info_dict.at("name"));
        // int piece_length = static_cast<int>(std::get<bencode::integer>(info_dict.at("piece length")));
        int length = static_cast<int>(std::get<bencode::integer>(info_dict.at("length")));
        // std::string pieces = std::get<bencode::string>(info_dict.at("pieces"));

        // // Extract top-level metadata
        std::string announce = std::get<bencode::string>(top_dict.at("announce"));
        // std::string comment = std::get<bencode::string>(top_dict.at("comment"));
        // std::string created_by = std::get<bencode::string>(top_dict.at("created by"));
        // std::string creation_date = std::to_string(std::get<bencode::integer>(top_dict.at("creation date")));
        // std::string encoding = std::get<bencode::string>(top_dict.at("encoding"));

        // std::cout << "\nBencoded Info:\n";
        // std::cout << "Torrent Name: " << name << "\n";
        // std::cout << "Piece Length: " << piece_length << "\n";
        // std::cout << "Length: " << length << "\n";
        // std::cout << "Announce URL: " << announce << "\n";

        // Compute info hash (you’ll define this function separately)
        std::array<uint8_t, 20> sha1_info_hash = compute_info_hash(info_dict);
        std::string info_hash_str(reinterpret_cast<const char*>(sha1_info_hash.data()), sha1_info_hash.size());

        // Tracker announce
        std::string peer_id = "-BC0001-123456789012";  // 20-byte peer id
        int port = 6881;
        int uploaded = 0;
        int downloaded = 0;
        int left = length;

        Tracker tracker(announce);
        tracker.announce(info_hash_str, peer_id, port, uploaded, downloaded, left);

            

        // auto variant = data.base();

        // // Step 2: Cast it to bencode::dict
        // if (!std::holds_alternative<bencode::dict>(variant)) {
        //     throw std::runtime_error("Top-level bencode structure is not a dictionary.");
        // }


        // std::cout << "start processing";
        // bencode::dict top_dict = std::get<bencode::dict>(variant);
        // bencode::dict info_dict = std::get<bencode::dict>(top_dict["info"]);
        // std::string name = std::get<bencode::string>(info_dict["name"]);
        // int piece_length = std::get<bencode::integer>(info_dict["piece length"]);
        // int length = std::get<bencode::integer>(info_dict["length"]);
        // std::string pieces = std::get<bencode::string>(info_dict["pieces"]);
        // std::string announce = std::get<bencode::string>(top_dict["announce"]);
        // std::string comment = std::get<bencode::string>(top_dict["comment"]);
        // std::string created_by = std::get<bencode::string>(top_dict["created by"]);
        // std::string creation_date = std::to_string(std::get<bencode::integer>(top_dict["creation date"]));  // converting to string
        // std::string encoding = std::get<bencode::string>(top_dict["encoding"]);
        // BencodedInfo info(name, piece_length, pieces, length);
        // BencodedFile torrent_file(announce, comment, created_by, creation_date, encoding, info);

        // // stroring bencoded info SSH-1 info exactly as it was stored in .torrent file
        // std::cout << "Bencoded Info:\n";

        // // std::string bencoded_info = bencode::encode(info_dict);

        // std::array<uint8_t, 20> sh1_info_hash = compute_info_hash(info);
        
        // std::string info_hash_str(reinterpret_cast<const char*>(sh1_info_hash.data()), sh1_info_hash.size());

        // // Generate a random peer_id (20 bytes, here just an example)
        // std::string peer_id = "-BC0001-123456789012";

        // int port = 6881;      // Your listening port
        // int uploaded = 0;     // Bytes uploaded so far
        // int downloaded = 0;   // Bytes downloaded so far
        // int left = length;    // Bytes left to download

        // // Announce to tracker
        // Tracker tracker(announce);
        // tracker.announce(info_hash_str, peer_id, port, uploaded, downloaded, left);

        // // parse_buffer(data);
        // std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
