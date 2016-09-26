#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

#include "schemadb.hpp"

void usage(const char* program) {
  std::cout << "usage: " << program << " --schemadb=<schemadb_filename> <mode> <mode options>" << std::endl;
  std::cout << "\t" << "mode: --convert --in <.csv file> --out <.bin file>" << std::endl;
  std::cout << "\t" << "mode: --create-index --in <.bin file> --out <.index file>" << std::endl;
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
  SchemaDb schemadb(schemadb_filename);

  switch(operation_flag) {
    case -1:
      std::cout << "error: no mode specified" << std::endl;
      usage(argv[0]);
      break;
    case OPERATION_CONVERT:
      std::cout << "mode: convert" << std::endl;
      schemadb.get_schema(0).convert_to_bin(infile, outfile, true); // TODO ignore_first_line flag
      break;
    case OPERATION_CREATE_INDEX:
      std::cout << "mode: create index" << std::endl;
      schemadb.get_schema(0).create_index(infile, outfile);
      break;
    case OPERATION_SEARCH_INDEX:
      std::cout << "mode: search index" << std::endl;
      schemadb.get_schema(0).load_index(infile);
      schemadb.get_schema(0).search_for_index(28); // TODO complete search_for_index
      break;
  }

  return EXIT_SUCCESS;
}

// TODO logging
