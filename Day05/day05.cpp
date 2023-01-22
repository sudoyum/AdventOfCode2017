#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


void solve_p1(vector<int> jump_offsets) {
  int num_steps, index_ptr, prev_ptr = 0;
  num_steps = index_ptr = prev_ptr = 0;

  while(true) {
    prev_ptr = index_ptr;
    index_ptr += jump_offsets[index_ptr];
    if(index_ptr >= jump_offsets.size()) {
      num_steps += 1;
      break;
    } else {
      jump_offsets[prev_ptr] += 1;
      num_steps += 1;
    }
  }

  cout << "PART1: " << num_steps << endl;
}


void solve_p2(vector<int> jump_offsets) {
  int num_steps, index_ptr, prev_ptr = 0;
  num_steps = index_ptr = prev_ptr = 0;

  while(true) {
    prev_ptr = index_ptr;
    index_ptr += jump_offsets[index_ptr];
    if(index_ptr >= jump_offsets.size()) {
      num_steps += 1;
      break;
    } else {
      if(jump_offsets[prev_ptr] >= 3) {
        jump_offsets[prev_ptr] -= 1;
      } else {
        jump_offsets[prev_ptr] += 1;
      }
      num_steps += 1;
    }
  }

  cout << "PART2: " << num_steps << endl;
}


int parse_input(char *input_file, vector<int> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    parsed_input.push_back(stoi(line));
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
  vector <int> parsed_input;
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
