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
    void create_index(const std::string& bin_filename, const std::string& index_filename) const;
    void load_index(const std::string& index_filename);
    int search_for_key(int key);

    static const int TIMESTAMP_SIZE = 30; // TODO
    static const int HEADER_SIZE = TIMESTAMP_SIZE * sizeof(char) + 2 * sizeof(int);

private:
    void compute_size();

    std::vector< std::pair<std::string, std::string> > metadata;
    int size;
    std::string schema_filename;
    int id;
    std::vector<std::pair<int, int> > index_map;
};

#endif // SCHEMA_H
