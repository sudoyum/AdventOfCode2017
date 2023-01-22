#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <vector>


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


bool check_anagram(string str1, string str2) {
  if(str1.size() != str2.size()) {
    return false;
  }
  auto str2_copy = str2;

  for(int i = 0; i < str1.size(); i++) {
    int index = -1;
    for(int j = 0; j < str2.size(); j++) {
      if(str1[i] == str2[j]) {
        index = j;
        break;
      }
    }
    if(index == -1){
      return false;
    } else {
      str2.erase(str2.begin() + index);
    }
  }
  return true;
}


bool is_anagram(int index, vector<string> possibles) {
  string selected = possibles[index];

  for(int i = 0; i < possibles.size(); i++) {
    if(i != index) {
      if(check_anagram(selected, possibles[i])) {
        return true;
      }
    }
  }
  return false;
}


void solve_p1(vector<vector<string>> &vals) {
  int valid = 0;
  for(int i = 0; i < vals.size(); i++) {
    set<string> word_set(vals[i].begin(), vals[i].end());
    if(vals[i].size() == word_set.size()) {
      valid++;
    }
  }

  cout << "PART1: " << valid << endl;
}


void solve_p2(vector<vector<string>> &vals) {
  int invalid = 0;
  for(int i = 0; i < vals.size(); i++) {
    //don't need to check last
    for(int j = 0; j < vals[i].size() - 1; j++) {
      if(is_anagram(j, vals[i])) {
        invalid++;
        break;
      }
    }
  }

  cout << "PART2: " << vals.size() - invalid << endl;
}


int parse_input(char *input_file, vector<vector<string>> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    vector<string> row;
    vector<string> split_line = splitString(line, "\\s");
    for(int i = 0; i < split_line.size(); i++) {
      row.push_back(split_line[i]);
    }
    parsed_input.push_back(row);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<vector<string>> &parsed_input) {
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
  vector<vector<string>> parsed_input;
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
