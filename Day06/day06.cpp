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

  for(sregex_token_iterator end; iter != end; ++iter)
  {
    result.push_back(iter->str());
  }

  return result;
}


int get_max_index(vector<int> block_counts) {
  int index = 0;
  int max = 0;
  for(int i = 0; i < block_counts.size(); i++) {
    if(block_counts[i] > max) {
      max = block_counts[i];
      index = i;
    }
  }
  return index;
}


void solve_p1(vector<int> &block_counts) {
  int index, distribute, iterations, prev_len;
  set<vector<int>> patterns;

  index = distribute = iterations = prev_len = 0;

  do {
    prev_len = patterns.size();
    patterns.insert(block_counts);
    if(prev_len == patterns.size()) {
      break;
    }

    int index = get_max_index(block_counts);
    int distribute = block_counts[index];
    block_counts[index] = 0;
    while(distribute > 0) {
      index += 1;
      index %= block_counts.size();
      block_counts[index] += 1;
      distribute -= 1;
    }
    iterations += 1;
  } while(true);

  cout << "PART1: " << iterations << endl;
}


void solve_p2(vector<int> &block_counts) {
  int index, distribute, iterations, loop_length;
  int prev_len = 0;
  bool loop_set = false;

  set<vector<int>> patterns;
  vector<int> loop_pattern;

  index = distribute = iterations = loop_length = 0;

  do {
    prev_len = patterns.size();
    patterns.insert(block_counts);

    if(loop_pattern == block_counts) {
      break;
    }

    if(prev_len == patterns.size() && !loop_set) {
      loop_set = true;
      loop_pattern = block_counts;
      loop_length = iterations;
    }

    int index = get_max_index(block_counts);
    int distribute = block_counts[index];
    block_counts[index] = 0;
    while(distribute > 0) {
      index += 1;
      index %= block_counts.size();
      block_counts[index] += 1;
      distribute -= 1;
    }
    iterations += 1;
  } while(true);

  cout << "PART2: " << iterations - loop_length << endl;
}


int parse_input(char *input_file, vector<int> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    vector<string> split_line = splitString(line, "\\s+");
    for(int i = 0; i < split_line.size(); i++) {
      parsed_input.push_back(stoi(split_line[i]));
    }
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<int> &parsed_input) {
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
  vector<int> parsed_input;
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
