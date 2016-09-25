#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <utility>
#include <vector>

class Schema {
public:
    Schema();
    Schema(const std::string& filename);
    int get_size() const;
    std::string get_filename() const;

private:
    void compute_size();

    std::vector< std::pair<std::string, std::string> > metadata;
    int size;
    std::string schema_filename;
};

#endif // SCHEMA_H