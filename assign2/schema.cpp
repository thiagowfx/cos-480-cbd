#include "schema.hpp"

#include <cstdio>
#include <fstream>

Schema::Schema(){
    compute_size();
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
        std::string timestamp = "2016-06-25 04:48:00"; // TODO

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