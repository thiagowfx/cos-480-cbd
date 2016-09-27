#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <utility>
#include <vector>

#include "BPlusTree/bpt.h"

std::string get_current_timestamp();

class Schema {
public:
    Schema();
    virtual ~Schema();
    Schema(const std::string& filename, int id);
    int get_size() const;
    int get_id() const;
    std::string get_filename() const;
    void convert_to_bin(const std::string& csv_filename, const std::string& bin_filename, bool ignore_first_line = true) const;
    void create_index(const std::string& bin_filename, const std::string& index_filename) const;
    void create_index_bplus(const std::string& bin_filename, const std::string& index_filename) const;
    void load_index(const std::string& index_filename);
    void load_index_bplus(const std::string& index_filename);
    int search_for_key(int key) const;
    int search_for_key_bplus(int key) const;
    int search_for_key_raw(int key, const std::string& bin_filename) const;
    
    static const int TIMESTAMP_SIZE = 25;
    static const int HEADER_SIZE = TIMESTAMP_SIZE * sizeof(char) + 2 * sizeof(int);
    bpt::bplus_tree *bplus = NULL;
    
private:
    void compute_size();

    std::vector< std::pair<std::string, std::string> > metadata;
    int size;
    std::string schema_filename;
    int id;
    std::vector<std::pair<int, int> > index_map;

};

#endif // SCHEMA_H
