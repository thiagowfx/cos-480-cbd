#include "schema.hpp"

#include <fstream>

Schema::Schema(){
    compute_size();
}

Schema::Schema(const std::string& filename) :
    schema_filename(filename) {
    std::ifstream input(filename);

    while(input.good()) {
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

std::string Schema::get_filename() const {
    return schema_filename;
}