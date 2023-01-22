#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


void solve_p1(vector<string> &vals) {
  long long total = 0;
  string captcha = vals[0];

  for(int i = 0; i < captcha.size() - 1; i++) {
    if(captcha[i] == captcha[i + 1]) {
      total += (int)(captcha[i] - 48);
    }
  }

  if(captcha[captcha.size() - 1] == captcha[0]) {
    total += (int)captcha[0] - 48;
  }

  cout << "PART1: " << total << endl;
}


void solve_p2(vector<string> &vals) {
   string captcha = vals[0];

   long long total = 0;
   long long size = captcha.size();
   long long offset = size/2;

   for(int i = 0; i < size; i++) {
      long long new_offset = i + offset;
      if(new_offset >= size) {
         new_offset -= size;
      }

      if(captcha[i] == captcha[new_offset]) {
          total += (int)(captcha[i] - 48);
      }
   }

   cout << "PART2: " << total << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    parsed_input.push_back(line);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<string> &parsed_input) {
  if(p_arg == "p1") {
    solve_p1(parsed_input);
  } else if(p_arg == "p2") {
    solve_p2(parsed_input);
  } else if(p_arg == "both") {
    solve_p1(parsed_input);
    solve_p2(parsed_input);
  }
}


int main(int argc, char *argv[]) {
  int rc = EXIT_SUCCESS;
  vector <string> parsed_input;
  std::vector<std::string> cl_args = {"both", "p1", "p2"};

  if(argc != 3 || count(cl_args.begin(), cl_args.end(), argv[2]) <= 0) {
    cout << "Usage: ./day input.txt p1|p2|both" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  if(parse_input(argv[1], parsed_input) == PARSE_FAILURE) {
    cout << "Issue parsing input, exiting" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  run_solutions(argv[2], parsed_input);

cleanup:
  return rc;
}
