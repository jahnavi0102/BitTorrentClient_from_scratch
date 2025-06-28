#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <openssl/sha.h>

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

std::array<uint8_t, 20> compute_info_hash(const BencodedInfo& info) {
    // Placeholder for actual hash computation logic
    std::array<uint8_t, 20> hash = {};
    SHA1(reinterpret_cast<const unsigned char*>(&info.name[0]),
         info.name.size(), hash.data());
    return hash;
}
