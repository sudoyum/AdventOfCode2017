#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
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


int get_divisor_sum(vector<int> &vals) {
  int start_index = 0;
  do {
    int focus = vals[start_index];
    for(int i = start_index + 1; i < vals.size(); i++) {
      if(focus <= vals[i]) {
        if(vals[i] % focus == 0) {
          return vals[i]/focus;
        }
      } else {
        if(focus % vals[i] == 0) {
          return focus/vals[i];
        }
      }
    }
    start_index += 1;
  } while(1);
}


void solve_p1(vector<vector<int>> &vals) {
  long long checksum = 0;

  for(int i = 0; i < vals.size(); i++) {
    checksum += *max_element(begin(vals[i]), end(vals[i])) - *min_element(begin(vals[i]), end(vals[i]));
  }
  cout << "PART1: " << checksum << endl;
}


void solve_p2(vector<vector<int>> &vals) {
  long long checksum = 0;

  for(int i = 0; i < vals.size(); i++) {
    checksum += get_divisor_sum(vals[i]);
  }
  cout << "PART2: " << checksum << endl;
}


int parse_input(char *input_file, vector<vector<int>> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    vector<int> row;
    vector<string> split_line = splitString(line, "\\s");
    for(int i = 0; i < split_line.size(); i++) {
      row.push_back(stoi(split_line[i]));
    }
    parsed_input.push_back(row);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<vector<int>> &parsed_input) {
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
  vector<vector<int>> parsed_input;
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
