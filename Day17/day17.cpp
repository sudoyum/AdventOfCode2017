#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;


#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define P1_NUM_ITERATIONS 2017
#define P2_NUM_ITERATIONS 50000000


void solve_p1(vector<string> vals) {
  vector <int> buffer;
  int index = 0;
  int num_steps = stoi(vals[0]);

  buffer.push_back(0);
  for(int i = 1; i <= P1_NUM_ITERATIONS; i++) {
    for(int j = 0; j < num_steps; j++) {
      index += 1;
      index %= buffer.size();
    }
    index += 1;
    buffer.insert(buffer.begin() + index, i);
  }

  cout << "PART1: " << buffer[index + 1] << endl;
}


void solve_p2(vector<string> vals) {
  vector <int> buffer;
  int index = 0;
  int size = 1;
  int num_steps = stoi(vals[0]);

  buffer.push_back(0);
  buffer.push_back(0);
  for(int i = 1; i <= P2_NUM_ITERATIONS; i++) {
    for(int j = 0; j < num_steps; j++) {
      index += 1;
      index %= size;
    }
    index += 1;
    size += 1;
    if(index == 1) {
      buffer[1] = i;
    }
  }

  for (auto it = buffer.begin(); it<buffer.end(); it++) {
    if(*it == 0) {
      index = it - buffer.begin() + 1;
      index %= buffer.size();
    }
  }

  cout << "PART2: " << buffer[1] << endl;
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
