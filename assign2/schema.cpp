#include "schema.hpp"

#include <fstream>

Schema::Schema(const std::string& filename) {
    std::ifstream input(filename);

    while(input) {
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