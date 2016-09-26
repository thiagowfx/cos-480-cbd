#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

#include "schemadb.hpp"

void usage(const char* program) {
  // TODO usage
  std::cout << "usage: " << program << " --schemadb=<schemadb_filename> <mode> <mode options>" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  enum {
    OPERATION_CONVERT,
    OPERATION_CREATE_INDEX,
    OPERATION_SEARCH_INDEX,
  };

  int operation_flag = -1;

  static struct option long_options[] = {
    // Modes.
    {"convert", no_argument, &operation_flag, OPERATION_CONVERT},
    {"create-index", no_argument, &operation_flag, OPERATION_CREATE_INDEX},
    {"search-index", no_argument, &operation_flag, OPERATION_SEARCH_INDEX},

    // Mode options.
    {"schemadb", required_argument, NULL, 0},
    {"in", required_argument, NULL, 'i'},
    {"out", required_argument, NULL, 'o'},

    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
  };

  int option_index = 0;
  int ch;

  std::string schemadb_filename;
  std::string infile, outfile;

  // TODO parsing
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

  switch(operation_flag) {
    case -1:
      std::cout << "error: no mode specified" << std::endl;
      usage(argv[0]);
      break;
    case OPERATION_CONVERT:
      std::cout << "mode: convert" << std::endl; // TODO logging
      SchemaDb schemadb(schemadb_filename);
      schemadb.get_schema(0).convert_to_bin(infile, outfile, true); // TODO ignore_first_line flag
      break;
    // TODO more operations
  }

  return EXIT_SUCCESS;
}