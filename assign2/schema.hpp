#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <utility>
#include <vector>

class Schema {
public:
    Schema();
    Schema(const std::string& filename, int id);
    int get_size() const;
    int get_id() const;
    std::string get_filename() const;
    void convert_to_bin(const std::string& csv_filename, const std::string& bin_filename, bool ignore_first_line = true) const;

    static const int TIMESTAMP_SIZE = 30; // TODO

private:
    void compute_size();

    std::vector< std::pair<std::string, std::string> > metadata;
    int size;
    std::string schema_filename;
    int id;
};

#endif // SCHEMA_H