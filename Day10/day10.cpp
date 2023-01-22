#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#include "knothash.h"

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


// Credit for this function:
// https://stackoverflow.com/questions/30050739/c-split-string-line-by-line-using-stdregex
vector<string> splitString(const std::string& stringToSplit, const std::string& regexPattern)
{
  vector<string> result;

  const regex rgx(regexPattern);
  sregex_token_iterator iter(stringToSplit.begin(), stringToSplit.end(), rgx, -1);

  for (sregex_token_iterator end; iter != end; ++iter)
  {
    result.push_back(iter->str());
  }

  return result;
}


void solve_p1(vector<string> vals) {
  vector<int> list;
  vector<int> lengths;

  for(auto it = vals.begin(); it != vals.end(); ++it) {
    lengths.push_back(stoi(*it));
  }

  for(int i = 0; i < SIZE; i++) {
    list.push_back(i);
  }

  int curr_pos = 0;
  int skip_size = 0;
  do_hash(list, lengths, &curr_pos, &skip_size);
  cout << "PART1: " << list[0] * list[1] << endl;
}


void solve_p2(vector<string> vals) {
  vector<int> list;
  vector<int> lengths;

  for(int i = 0; i < SIZE; i++) {
    list.push_back(i);
  }
  for(auto it = vals.begin(); it != vals.end(); ++it) {
    string entry = *it;
    for(const char &c: entry) {
      lengths.push_back(int(c));
    }
    if(it+1 != vals.end()) {
      lengths.push_back(int(','));
    }
  }

  lengths.push_back(17);
  lengths.push_back(31);
  lengths.push_back(73);
  lengths.push_back(47);
  lengths.push_back(23);

  int curr_pos = 0;
  int skip_size = 0;

  for(int i = 0; i < NUM_ROUNDS; i++) {
    do_hash(list, lengths, &curr_pos, &skip_size);
  }

  vector<int> sparse_hash;
  for(int i = 0; i < SIZE; i+=16) {
    int xored = list[i];
    for(int j = i + 1; j < (i + 16); j++) {
      xored ^= list[j];
    }
    sparse_hash.push_back(xored);
  }

  string hash;
  for(auto it = sparse_hash.begin(); it != sparse_hash.end(); ++it) {
    hash += to_hex(*it);
  }
  cout << "PART2: " << hash << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  vector<string> split_line;
  while(getline(infile, line)) {
    split_line = splitString(line, ",");
  }
  parsed_input = split_line;

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
  vector<string> parsed_input;
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
