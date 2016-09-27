#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>
#include <time.h>

#include "schemadb.hpp"

void usage(const char* program) {
  std::cout << "usage: " << program << " --schemadb=<schemadb_filename> --schema=<schema_id> <mode> <mode options>" << std::endl;
  std::cout << "\t" << "mode: --convert --in <.csv file> --out <.bin file>" << std::endl;
  std::cout << "\t" << "mode: --create-index --in <.bin file> --out <.index file>" << std::endl;
  std::cout << "\t" << "mode: --create-index-bplus --in <.bin file> --out <.index file>" << std::endl;
  std::cout << "\t" << "mode: --search-index --in <.index file> --key <key>" << std::endl;
  std::cout << "\t" << "mode: --search-index-bplus --in <.index file> --key <key>" << std::endl;
  exit(EXIT_FAILURE);
}

#define BENCHMARK(f) \
do { \
  clock_t start = clock(); \
  f; \
  int msec = (clock() - start) * 1000 / CLOCKS_PER_SEC; \
  printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000); \
} while(false)

int main(int argc, char *argv[]) {
  // BENCHMARK(sum(a,b));

  enum {
    OPERATION_CONVERT,
    OPERATION_CREATE_INDEX,
    OPERATION_CREATE_INDEX_BPLUS,
    OPERATION_SEARCH_INDEX,
    OPERATION_SEARCH_INDEX_BPLUS,
  };

  int operation_flag = -1;

  static struct option long_options[] = {
    // Modes.
    {"convert", no_argument, &operation_flag, OPERATION_CONVERT},
    {"create-index", no_argument, &operation_flag, OPERATION_CREATE_INDEX},
    {"create-index-bplus", no_argument, &operation_flag, OPERATION_CREATE_INDEX_BPLUS},
    {"search-index", no_argument, &operation_flag, OPERATION_SEARCH_INDEX},
    {"search-index-bplus", no_argument, &operation_flag, OPERATION_SEARCH_INDEX_BPLUS},

    // Mode options.
    {"schema", required_argument, NULL, 0},
    {"schemadb", required_argument, NULL, 0},
    {"in", required_argument, NULL, 'i'},
    {"out", required_argument, NULL, 'o'},
    {"key", required_argument, NULL, 0},

    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
  };

  int option_index = 0;
  int ch;

  std::string schemadb_filename;
  int schema_id = 0;
  int key = 0;
  std::string infile, outfile;

  while((ch = getopt_long(argc, argv, "hi:o:", long_options, &option_index)) != -1) {
    switch(ch) {
      case 'i':
        infile = std::string(optarg);
        break;
      case 'o':
        outfile = std::string(optarg);
        break;
      case 0:
        if(!strcmp(long_options[option_index].name, "schemadb")) {
          schemadb_filename = std::string(optarg);
        }
        else if(!strcmp(long_options[option_index].name, "schema")) {
          schema_id = std::stoi(std::string(optarg));
        }
        else if(!strcmp(long_options[option_index].name, "key")) {
          key = std::stoi(std::string(optarg));
        }
        break;
      case 'h':
      case '?':
      case ':':
      default:
        usage(argv[0]);
    }
  }

  // Validate arguments.
  if(schemadb_filename.empty()) {
    std::cout << "error: schemadb_filename not specified" << std::endl;
    usage(argv[0]);
  }

  SchemaDb schemadb(schemadb_filename);
  Schema schema;

  switch(operation_flag) {
    case -1:
      std::cout << "error: no mode specified" << std::endl;
      usage(argv[0]);
      break;
    case OPERATION_CONVERT:
      std::cout << "mode: convert" << std::endl;
      schemadb.get_schema(schema_id).convert_to_bin(infile, outfile, true);
      break;
    case OPERATION_CREATE_INDEX:
      std::cout << "mode: create index" << std::endl;
      schemadb.get_schema(schema_id).create_index(infile, outfile);
      break;
    case OPERATION_CREATE_INDEX_BPLUS:
      std::cout << "mode: create index w/ B+ tree" << std::endl;
      schemadb.get_schema(schema_id).create_index_bplus(infile, outfile);
      break;
    case OPERATION_SEARCH_INDEX:
      std::cout << "mode: search index" << std::endl;
      schema = schemadb.get_schema(schema_id);
      schema.load_index(infile);
      schema.search_for_key(key);
      break;
    case OPERATION_SEARCH_INDEX_BPLUS:
      std::cout << "mode: search index w/ B+ tree" << std::endl;
      schema = schemadb.get_schema(schema_id);
      schema.load_index_bplus(infile);
      schema.search_for_key_bplus(key);
      break;
  }

  return EXIT_SUCCESS;
}