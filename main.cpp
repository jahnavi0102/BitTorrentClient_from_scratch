#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <sstream> 
#include <cctype> 
#include <iomanip> 
#include <memory>
#include <cstdint>   
#include <openssl/sha.h> 
#include <curl/curl.h> 
#include <bencoding.hpp> 


struct Peer {
    std::string ip;
    int port;
    // Potentially add std::string id; for peer_id if parsing non-compact
};

void parseTrackerResponse (){
    
}

class Tracker {
public:
    Tracker(const std::string& announce_url)
        : announce_url_(announce_url) {}

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
        size_t total_size = size * nmemb;
        std::string* buffer_to_write_to = static_cast<std::string*>(userp);
        buffer_to_write_to->append(static_cast<char*>(contents), total_size);

        // IMPORTANT: Return the number of bytes we successfully handled.
        // If we return anything less than total_size, libcurl will think
        // something went wrong and might stop the transfer.
        return total_size;
    };

    std::string sendHttpRequest(const std::string& url){
        CURL *curl = nullptr;
        CURLcode res;
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();
        if (curl){
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); 

            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                throw std::runtime_error(std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return readBuffer;
    };

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

        try {
            std::string response_body = sendHttpRequest(url.str());
            bencode::data data = bencode::decode(response_body);
            
            std::cout << "decoded_response" << std::endl;
            auto decoded_response = data.base();

            if (!std::holds_alternative<bencode::dict>(decoded_response)) {
                throw std::runtime_error("Top-level bencode structure is not a dictionary.");
            }

            // Step 2: Parse the bencoded response
            // call parseTrackerResponse(decoded_response)

            // cout << decoded_response["peers"];
            
            // std::cout << "Tracker Response: " << response_body << std::endl;

            

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
    };
};

std::array<uint8_t, 20> compute_info_hash(const bencode::dict& info) {
    // Bencode the info dictionary and hash the resulting bytes
    std::array<uint8_t, 20> hash = {};
    std::string bencoded_info_str = bencode::encode(info);
    std::vector<uint8_t> bencoded_info(bencoded_info_str.begin(), bencoded_info_str.end());
    SHA1(bencoded_info.data(), bencoded_info.size(), hash.data());
    return hash;
};

class ReadFiles {
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
        };
};


int main() {
    try {
        std::cout << "Enter the file path: ";
        std::string file_path;
        std::getline(std::cin, file_path);
        ReadFiles reader;
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

        int length = static_cast<int>(std::get<bencode::integer>(info_dict.at("length")));

        std::string announce = std::get<bencode::string>(top_dict.at("announce"));
        

        // Compute info hash (you’ll define this function separately)
        std::array<uint8_t, 20> sha1_info_hash = compute_info_hash(info_dict);
        std::string info_hash_str;
        info_hash_str.reserve(sha1_info_hash.size());
        for (uint8_t byte : sha1_info_hash) {
            info_hash_str.push_back(static_cast<char>(byte));
        }

        // Tracker announce
        std::string peer_id = "-BC0001-123456789012";  // 20-byte peer id
        int port = 6881;
        int uploaded = 0;
        int downloaded = 0;
        int left = length;

        Tracker tracker(announce);
        tracker.announce(info_hash_str, peer_id, port, uploaded, downloaded, left);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
