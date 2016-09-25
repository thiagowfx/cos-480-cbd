#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <utility>
#include <vector>

class Schema {
public:
    Schema(const std::string& filename);
    int get_size() const;

private:
    void compute_size();

    std::vector< std::pair<std::string, std::string> > metadata;
    int size;
};

#endif // SCHEMA_H