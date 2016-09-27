#include "schema.hpp"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>

// Format Www Mmm dd hh:mm:ss yyyy
std::string get_current_timestamp() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

Schema::Schema(){
    compute_size();
}

Schema::~Schema() {
    if(!bplus) {
        delete bplus;
    }
}

Schema::Schema(const std::string& filename, int id) :
    schema_filename(filename),
    id(id) {
    std::ifstream input(filename);

    while(input.good() && input.peek() != EOF) {
        std::string datatype, column;
        std::getline(input, datatype, ',');
        std::getline(input, column);

        metadata.push_back(std::make_pair(datatype, column));
    }

    input.close();

    compute_size();
}

int Schema::get_size() const {
    return size;
}

void Schema::compute_size() {
    size = 0;

    for(const auto& data: metadata) {
        if (data.first[0] == 'i') {
            size += sizeof(int);
        }
        else {
            // ASSUMES: string.
            size += atoi(data.first.c_str() + 1);
        }
    }
}

int Schema::get_id() const {
    return id;
}

std::string Schema::get_filename() const {
    return schema_filename;
}

void Schema::convert_to_bin(const std::string& csv_filename, const std::string& bin_filename, bool ignore_first_line) const {
    std::ifstream csv_file(csv_filename);
    FILE* bin_file = fopen(bin_filename.c_str(), "wb");

    if(ignore_first_line) {
        std::string line;
        std::getline(csv_file, line);
    }

    int next_key = 0;

    while(csv_file.good() && csv_file.peek() != EOF) {
        std::string timestamp = get_current_timestamp();

        // Write header.
        fwrite(&next_key, sizeof(int), 1, bin_file);
        fwrite(timestamp.c_str(), sizeof(char), TIMESTAMP_SIZE, bin_file);
        fwrite(&id, sizeof(int), 1, bin_file);

        ++next_key;

        // Write data.
        for(unsigned i = 0; i < metadata.size(); ++i) {
            const auto& data = metadata[i];
            const char delimiter = (i == (metadata.size() - 1)) ? '\n' : ',';

            std::string token;
            std::getline(csv_file, token, delimiter);

            if (data.first[0] == 'i') {
                const int int_token = std::stoi(token);
                fwrite(&int_token, sizeof(int), 1, bin_file);
            }
            else {
                // ASSUMES: string.
                fwrite(token.c_str(), sizeof(char), atoi(data.first.c_str() + 1), bin_file);
            }
        }
    }

    fclose(bin_file);
    csv_file.close();
}

void Schema::create_index(const std::string& bin_filename, const std::string& index_filename) const {
    FILE* bin_file = fopen(bin_filename.c_str(), "rb");
    FILE* index_file = fopen(index_filename.c_str(), "wb");

    int offset = 0;
    int pace = HEADER_SIZE + size - sizeof(int);
    int key;

    while(fread(&key, sizeof(int), 1, bin_file)) {
        fwrite(&key, sizeof(int), 1, index_file);
        fwrite(&offset, sizeof(int), 1, index_file);

        offset += pace;
        fseek(bin_file, pace, SEEK_CUR);
    }

    fclose(index_file);
    fclose(bin_file);
}

void Schema::create_index_bplus(const std::string& bin_filename, const std::string& index_filename) const {
    FILE* bin_file = fopen(bin_filename.c_str(), "rb");
    bpt::bplus_tree bplus(index_filename.c_str(), true);

    int offset = 0;
    int pace = HEADER_SIZE + size - sizeof(int);
    int key;

    while(fread(&key, sizeof(int), 1, bin_file)) {
        bplus.insert(bpt::key_t(std::to_string(key).c_str()), offset);
        offset += pace;
    }

    fclose(bin_file);
}

void Schema::load_index(const std::string& index_filename) {
    index_map.clear();

    FILE* index_file = fopen(index_filename.c_str(), "rb");
    int key, offset;

    while(fread(&key, sizeof(int), 1, index_file)) {
        fread(&offset, sizeof(int), 1, index_file);
        index_map.push_back(std::make_pair(key, offset));
    }

    fclose(index_file);
}

void Schema::load_index_bplus(const std::string& index_filename) {
    bplus = new bpt::bplus_tree(index_filename.c_str());
}

int Schema::search_for_key(int key){
    auto it = std::lower_bound(index_map.begin(), index_map.end(), std::make_pair(key, 0), [](const std::pair<int, int>& op1, const std::pair<int, int>& op2) {
        return op1.first < op2.first;
    });
    std::cout << it->second << std::endl;
    return it->second;
}

int Schema::search_for_key_bplus(int key) {
    bpt::value_t value;
    // NOTE: if the return value is (-1), then such key hasn't been found.
    bplus->search(bpt::key_t(std::to_string(key).c_str()), &value);
    return value;
}

int Schema::search_for_key_raw(int key, const std::string& bin_filename) {

    FILE* binfile = fopen(bin_filename.c_str(), "rb");

    int offset = 0;
    int pace = HEADER_SIZE + size - sizeof(int);
    int k;

    while(fread(&k, sizeof(int), 1, binfile)) {

        if (k == key) {
            fclose(binfile);
            return offset;
        }
        offset += pace;
        fseek(binfile, pace, SEEK_CUR);
    }

    fclose(binfile);
    return -1;
}