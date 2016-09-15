#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Company {
public:
  Company(int key, const std::string& name, const std::string& slogan):
    key(key)
  {
    strncpy(this->name, name.c_str(), 100);
    strncpy(this->slogan, slogan.c_str(), 200);
  }

  void write(FILE* outfile) {
    fwrite(&key, sizeof(unsigned), 1, outfile);
    fwrite(name, sizeof(char), 100, outfile);
    fwrite(slogan, sizeof(char), 200, outfile);
  }

  void read(FILE* infile) {
    fread(&key, sizeof(unsigned), 1, infile);
    fread(name, sizeof(char), 100, infile);
    fread(slogan, sizeof(char), 200, infile);
  }

  // private:
  unsigned key;
  char name[100];
  char slogan[200];
};

void read_csv_file(char* infilename, char* outfilename) {
  std::ifstream input(infilename);
  FILE* output;

  if((output = fopen(outfilename, "wb")) == NULL) {
    std::cerr << "error: cannot open output file for writing: " << outfilename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;

  // ignore header
  std::getline(input, line);

  unsigned key = 0;
  
  while(input) {
    std::getline(input, line);

    std::stringstream ss;
    ss << line;

    std::string name, slogan;

    std::getline(ss, name, ',');
    std::getline(ss, slogan, '\n');

    Company company(key, name, slogan);
    ++key;
		
    company.write(output);
  }

  fclose(output);
  input.close();
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << " " << "<input file (text)> <output file (binary)>" << std::endl;
    return EXIT_FAILURE;
  }

  read_csv_file(argv[1], argv[2]);

  return EXIT_SUCCESS;
}
