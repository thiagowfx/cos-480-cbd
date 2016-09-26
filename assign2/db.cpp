#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <string>

#include "schemadb.hpp"

void usage(const char* program) {
  // TODO usage
  std::cout << "usage: " << program << " <mode> <mode options>" << std::endl;
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
    {"convert", no_argument, &operation_flag, OPERATION_CONVERT},
    {"create-index", no_argument, &operation_flag, OPERATION_CREATE_INDEX},
    {"search-index", no_argument, &operation_flag, OPERATION_SEARCH_INDEX},
    {"in", required_argument, NULL, 'i'},
    {"out", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
  };

  int option_index = 0;
  int ch;

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
        break;
      case 'h':
      case '?':
      case ':':
      default:
        usage(argv[0]);
    }
  }

  if(operation_flag == - 1) {
    usage(argv[0]);
  }

  return EXIT_SUCCESS;
}