#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>

#include "schema.hpp"

void search_range(Schema schema, int lowlimit, int highlimit);
void search_range_bplus(Schema schema, int lowkey, int highkey);
void search_range_raw(Schema schema, int lowkey, int highkey, const std::string &filename);
void search_set(Schema schema, std::vector<int> set);
void search_set_bplus(Schema schema, std::vector<int> set);
void search_set_raw(Schema schema, std::vector<int> set, const std::string &filename);

#endif // !BENCHMARK_H

